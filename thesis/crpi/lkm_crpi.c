#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>
#include <linux/seq_file.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/spinlock.h>

#include <asm/io.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Felix Mueller");
MODULE_DESCRIPTION("lkm_crpi");
MODULE_VERSION("0.01");

#define CRPI_ORDER_PAGES 5

#define CRPI_M0_OFFSET 8
#define CRPI_M1_OFFSET 8192
#define CRPI_M2_OFFSET 16384
#define CRPI_M3_OFFSET 24576
#define CRPI_M4_OFFSET 32768

#define CRPI_NUM_MEASUREMENTS 100

#define CRPI_MEASURE_LATENCY				\
isb();							\
asm volatile("mrs %0, pmccntr_el0" : "=r"(start));	\
testvalue = *ptr_m; 					\
asm volatile("dmb ld"); 				\
asm volatile("mrs %0, pmccntr_el0" : "=r"(dur));	\
dur -= start;

static spinlock_t mylock;

// number of misses for specific access sequences
static int o1_misses 	= 0;
static int o2_misses 	= 0;
static int o3_misses[4]= {0};

// make gcc asume loaded data is needed
static unsigned long trickValue;		

// physical address of used memory chunk
phys_addr_t phyAddr;

// prototypes
static int	determineL1Miss(unsigned long);
static void	layoutPointerChasingO1(unsigned long*);
static void	layoutPointerChasingO2(unsigned long*);
static void	layoutPointerChasingO3(unsigned long*);
static int	doMeasurements(void);
static int	sf_show(struct seq_file*, void*);
static int	call_seq_open(struct inode*, struct file*);
static int 	__init mod_init(void);
static void 	__exit mod_exit(void);

int determineL1Miss(unsigned long accesTime){ return (accesTime > 8) ? 1 : 0; }

void layoutPointerChasingO1(unsigned long* ptr_s){
	unsigned long* actPtr;
	unsigned long  actValue;
	
	actPtr = ptr_s;
	actValue = (unsigned long) ptr_s;
	
	//Adr M01 -> M1
	*actPtr = actValue + CRPI_M1_OFFSET;
	
	//adr M1 -> M2
	actPtr= ptr_s + (CRPI_M1_OFFSET / sizeof(unsigned long));
	*actPtr = actValue + CRPI_M2_OFFSET;
	
	//adr M2 -> M3
	actPtr= ptr_s + (CRPI_M2_OFFSET / sizeof(unsigned long));
	*actPtr = actValue + CRPI_M3_OFFSET;
	
	//adr M3 -> M0,2
	actPtr= ptr_s + (CRPI_M3_OFFSET / sizeof(unsigned long));
	*actPtr = actValue + CRPI_M0_OFFSET;
	
	//adr M0,2 -> M4
	actPtr= ptr_s + (CRPI_M0_OFFSET / sizeof(unsigned long));
	*actPtr = actValue + CRPI_M4_OFFSET;

	//adr M4 -> NULL
	actPtr= ptr_s + (CRPI_M4_OFFSET / sizeof(unsigned long));
	*actPtr = 0;
}

void layoutPointerChasingO2(unsigned long* ptr_s){
	unsigned long* actPtr;
	unsigned long  actValue;
	
	actPtr = ptr_s;
	actValue = (unsigned long) ptr_s;
	
	//Adr M0_1 -> M1_1
	*actPtr = actValue + CRPI_M1_OFFSET;
	
	//adr M1 -> M2_1
	actPtr= ptr_s + (CRPI_M1_OFFSET / sizeof(unsigned long));
	*actPtr = actValue + CRPI_M2_OFFSET;
	
	//adr M2 -> M3
	actPtr= ptr_s + (CRPI_M2_OFFSET / sizeof(unsigned long));
	*actPtr = actValue + CRPI_M3_OFFSET;
	
	//adr M3 -> M0_2
	actPtr= ptr_s + (CRPI_M3_OFFSET / sizeof(unsigned long));
	*actPtr = actValue + CRPI_M0_OFFSET;
		
	//adr M0_2 -> M1_2
	actPtr= ptr_s + (CRPI_M0_OFFSET / sizeof(unsigned long));
	*actPtr = actValue + CRPI_M1_OFFSET + CRPI_M0_OFFSET;
	
	//adr M1_2 -> M2_2
	actPtr= ptr_s + ((CRPI_M1_OFFSET + CRPI_M0_OFFSET) / sizeof(unsigned long));
	*actPtr = actValue + CRPI_M2_OFFSET + CRPI_M0_OFFSET;
	
	//adr M2_2 -> M4
	actPtr= ptr_s + ((CRPI_M2_OFFSET + CRPI_M0_OFFSET) / sizeof(unsigned long));
	*actPtr = actValue + CRPI_M4_OFFSET;
	
	//adr M4 -> NULL
	actPtr= ptr_s + (CRPI_M4_OFFSET / sizeof(unsigned long));
	*actPtr = 0;
}

void layoutPointerChasingO3(unsigned long* ptr_s){
	unsigned long* actPtr;
	unsigned long  actValue;
	
	actPtr = ptr_s;
	actValue = (unsigned long) ptr_s;
	
	//Adr M0 -> M1_1
	*actPtr = actValue + CRPI_M1_OFFSET;
	
	//adr M1 -> M2
	actPtr= ptr_s + (CRPI_M1_OFFSET / sizeof(unsigned long));
	*actPtr = actValue + CRPI_M2_OFFSET;
	
	//adr M2 -> M3
	actPtr= ptr_s + (CRPI_M2_OFFSET / sizeof(unsigned long));
	*actPtr = actValue + CRPI_M3_OFFSET;
	
	//adr M3 -> M4
	actPtr= ptr_s + (CRPI_M3_OFFSET / sizeof(unsigned long));
	*actPtr = actValue + CRPI_M4_OFFSET;
	
	//adr M4 -> NULL
	actPtr= ptr_s + (CRPI_M4_OFFSET / sizeof(unsigned long));
	*actPtr = 0;
}

static int doMeasurements(void){
	register unsigned long* ptr;		// holds actual accesed cache line
	unsigned long* ptr_s;			// holds start of allocated memory area 
	register unsigned long* ptr_m;		// address of cache line to measure

	register unsigned long testvalue;	// register variable to take data from ld
	
	register unsigned long start;		// measure cycles
	unsigned long dur;

	int i;					// number of measurements
		
	printk(KERN_INFO "Starting lkm_crpi!\n");
	
	ptr	= (unsigned long*) __get_free_pages(GFP_KERNEL, CRPI_ORDER_PAGES);
	ptr_s	= ptr;
	
	if(ptr == NULL)
	{
		printk(KERN_INFO "memory allocation failed\n");
		return -1;
	}
	
/* =================================================================
   do access sequence O1 and determine state of cache line 0      */
	
	ptr_m	= ptr_s;
	layoutPointerChasingO1(ptr_s);
		
	spin_lock_irq(&mylock);
	while(ptr != 0) ptr = (unsigned long*) *ptr;
	
	i=0;
	while(i<CRPI_NUM_MEASUREMENTS){
		
		ptr=ptr_s;
		while(ptr != 0) ptr = (unsigned long*) *ptr;
		CRPI_MEASURE_LATENCY
		trickValue %= testvalue;
		if(determineL1Miss(dur)) o1_misses++;	
		i++;
	}
	spin_unlock_irq(&mylock);

/* =================================================================	
   do access sequence O2 and determine state of cache line 3      */
	
	layoutPointerChasingO2(ptr_s);
	ptr_m	= ptr_s + (CRPI_M3_OFFSET / sizeof(unsigned long));
	
	spin_lock_irq(&mylock);
	while(ptr != 0)	ptr = (unsigned long*) *ptr;
	
	i=0;
	while(i<CRPI_NUM_MEASUREMENTS){

		ptr=ptr_s;
		while(ptr != 0) ptr = (unsigned long*) *ptr;
		CRPI_MEASURE_LATENCY
		trickValue %= testvalue;
		if(determineL1Miss(dur)) o2_misses++;	
		i++;
	}
	spin_unlock_irq(&mylock);

/* =================================================================
   layout memory for sequence O3                                  */
	
	layoutPointerChasingO3(ptr_s);
	
/* =================================================================
   do access sequence O3 and determine state of cache line 0      */
	
	ptr_m = ptr_s;
	spin_lock_irq(&mylock);
	while(ptr != 0)	ptr = (unsigned long*) *ptr;
	
	i=0;
	while(i<CRPI_NUM_MEASUREMENTS){

		ptr=ptr_s;
		while(ptr != 0) ptr = (unsigned long*) *ptr;
		CRPI_MEASURE_LATENCY
		printk(KERN_INFO "%d\n", determineL1Miss(dur));
		trickValue %= testvalue;
		if(determineL1Miss(dur)) o3_misses[0]++;	
		i++;
	}
	spin_unlock_irq(&mylock);
	
/* ================================================================
   do access sequence O3 and determine state of cache line 1     */
	
	ptr_m	= ptr_s + (CRPI_M1_OFFSET / sizeof(unsigned long));
	spin_lock_irq(&mylock);
	while(ptr != 0)	ptr = (unsigned long*) *ptr;
	
	i=0;
	while(i<CRPI_NUM_MEASUREMENTS){
		
		ptr=ptr_s;
		while(ptr != 0)	ptr = (unsigned long*) *ptr;
		CRPI_MEASURE_LATENCY
		trickValue %= testvalue;
		if(determineL1Miss(dur)) o3_misses[1]++;	
		i++;
	}
	spin_unlock_irq(&mylock);
	
/* ================================================================
   do access sequence O3 and determine state of cache line 2     */
	
	ptr_m	= ptr_s + (CRPI_M2_OFFSET / sizeof(unsigned long));
	spin_lock_irq(&mylock);
	while(ptr != 0)	ptr = (unsigned long*) *ptr;
	
	i=0;
	while(i<CRPI_NUM_MEASUREMENTS){
		
		ptr=ptr_s;
		while(ptr != 0)	ptr = (unsigned long*) *ptr;
		CRPI_MEASURE_LATENCY
		trickValue %= testvalue;
		if(determineL1Miss(dur)) o3_misses[2]++;	
		i++;
	}
	spin_unlock_irq(&mylock);
	
/* ================================================================
   do access sequence O3 and determine state of cache line 3     */
	
	ptr_m	= ptr_s + (CRPI_M3_OFFSET / sizeof(unsigned long));
	spin_lock_irq(&mylock);
	while(ptr != 0)	ptr = (unsigned long*) *ptr;

	i=0;
	while(i<CRPI_NUM_MEASUREMENTS){
		
		ptr=ptr_s;
		while(ptr != 0) ptr = (unsigned long*) *ptr;
		CRPI_MEASURE_LATENCY
		trickValue %= testvalue;
		if(determineL1Miss(dur)) o3_misses[3]++;	
		i++;
	}
	spin_unlock_irq(&mylock);

	// get physical address
	phyAddr = virt_to_phys(ptr_s);

	//free memory
	free_pages((unsigned long)ptr_s, CRPI_ORDER_PAGES);
	return 0;
}


/* ================================================================
   reach out recovered information to user space                 */
static int sf_show(struct seq_file* m, void* v){

	o1_misses	= 0;
	o2_misses	= 0; 
	o3_misses[0]	= 0;
	o3_misses[1]	= 0;
	o3_misses[2]	= 0;
	o3_misses[3]	= 0;

	if(doMeasurements() != 0){
		seq_printf(m, "error ocurred, check dmesg for more information\n");
		return 0;
	}

	seq_printf(m, "O1  : %d\n", o1_misses);
	seq_printf(m, "O2  : %d\n", o2_misses);
	seq_printf(m, "O3_0: %d\n", o3_misses[0]);
	seq_printf(m, "O3_1: %d\n", o3_misses[1]);
	seq_printf(m, "O3_2: %d\n", o3_misses[2]);
	seq_printf(m, "O3_3: %d\n", o3_misses[3]);
	seq_printf(m, "%llu\n", phyAddr);
	seq_printf(m, "trick value: %lu\n",trickValue);

	return 0;
}

static int call_seq_open(struct inode* devFile, struct file* instanz){ 
	return single_open(instanz, sf_show, NULL);
}

static struct proc_ops proc_fops = {
	.proc_open	= call_seq_open,
	.proc_read	= seq_read,
	.proc_release   = single_release,
};

static int __init mod_init(void){
	static struct proc_dir_entry * procdirentry;
	
	procdirentry = proc_create_data(
		"CRPI_DATA",
		S_IRUGO,
		NULL,
		&proc_fops,
		NULL
	);
	
	return 0;
}

static void __exit mod_exit(void){
	remove_proc_entry("CRPI_DATA", NULL);
}

module_init(mod_init);
module_exit(mod_exit);
MODULE_LICENSE("GPL");
