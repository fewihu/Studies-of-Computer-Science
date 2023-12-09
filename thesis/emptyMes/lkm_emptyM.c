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
	register unsigned long testvalue;	// variable to make gcc think we use the read value
	// 'register' is used to force load from memory to register: gcc uses 'register' as hint,
	// check asm-file to be sure no other then the desired operation is measured
	
	register unsigned long start;	// measure cycles
	// 'register' is used to avoid occurance of another unwanted cache miss while storing measurement values
	unsigned long dur;
	
	printk(KERN_INFO "Starting lkm_crpi!\n");
	
	start = read_pmccntr();
	// ===
	dur = read_pmccntr() - start;
	
	printk(KERN_INFO "empty measurement: %ld (%ld)\n",dur,start);
	
	return 0;
}

static void __exit lkm_example_exit(void)
{
	printk(KERN_INFO "Stopping lkm_crpi!\n");
}

module_init(lkm_example_init);
module_exit(lkm_example_exit);
