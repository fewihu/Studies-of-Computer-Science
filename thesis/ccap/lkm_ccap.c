#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Felix Mueller");
MODULE_DESCRIPTION("lkm_crpi");
MODULE_VERSION("0.01");

#define TAU_PAGE_SIZE 4096
#define TAU_NUM_PAGES 8
#define TAU_CACHELINE_SIZE 64

static inline uint64_t read_pmccntr(void);
int determineL1Miss(unsigned long accesTime);
static int __init lkm_example_init(void);
static void __exit lkm_example_exit(void);

static inline uint64_t read_pmccntr(void)
{
	unsigned long val;
	asm volatile("mrs %0, pmccntr_el0" : "=r"(val));
	return val;
}

int determineL1Miss(unsigned long accesTime){
	return (accesTime > 15) ? 1 : 0;
}

static int __init lkm_example_init(void)
{
// 'register' is used to avoid occurance of another unwanted cache miss while storing measurement values
	unsigned long* ptr;	// holds actual accesed cache line
	int i;
	int j;
	register unsigned long testvalue;
	register unsigned long* ptr_m;
	unsigned long ptr_l;
	
	register unsigned long start;	// measure cycles
	unsigned long dur;
	
	// get pages	(2^4 = 16 -> 16 * 4096 = 64 KiB)
	ptr	= (unsigned long*) __get_free_pages(GFP_KERNEL, 4);
	ptr_m = ptr;
		
	// check allocation worked
	if(ptr == NULL)
	{
		printk(KERN_INFO "Allocation failed");
		return 0;
	}

	// Cache sizes up to 2^16
	i = 1;
	j = 0;
	while(i < 64)
	{
		ptr_l = (unsigned long) ptr;
		while(j < i*1024)
		{
			ptr = (unsigned long*) ptr_l + j;
			testvalue = *ptr;
			printk(KERN_INFO "%lu\n", testvalue);	
			j += 64;
		}
		i++;
	}
	
	// determine first cacheline
	start = read_pmccntr();
	testvalue = *ptr_m;
	dur = read_pmccntr() - start;
	
	printk(KERN_INFO "acces value: %lu %d (%ld)\n", testvalue, determineL1Miss(dur), dur);
	
	//free memory
	free_pages((unsigned long)ptr_s, 3);
	
	return 0;
}

static void __exit lkm_example_exit(void)
{
	printk(KERN_INFO "Stopping lkm_crpi!\n");
}

module_init(lkm_example_init);
module_exit(lkm_example_exit);
