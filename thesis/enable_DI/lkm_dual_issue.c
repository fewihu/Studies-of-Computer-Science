#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/smp.h>
#include <linux/miscdevice.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <linux/platform_device.h>
#include <linux/version.h>

static void toggle_dual_issuing(void* data)
{
	//MSR S3_1_C15_C2_0, <Xt>
	asm volatile("msr S3_1_C15_C2_0, %0" :: "r" BIT(29));
}

static int __init dual_issue_init(void)
{
	on_each_cpu(toggle_dual_issuing, NULL, 1);
	return 0;
}

static void __exit dual_issue_exit(void)
{
	on_each_cpu(toggle_dual_issuing, NULL, 1);
}

MODULE_DESCRIPTION("Enables / disables cpu dual issue processing");
MODULE_LICENSE("GPL");
module_init(dual_issue_init);
module_exit(dual_issue_exit);
