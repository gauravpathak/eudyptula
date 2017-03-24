#include <linux/module.h>
#include <linux/init.h>

static int hello_init(void)
{
	pr_debug("Hello World!\n");
	return 0;
}

static void hello_exit(void)
{
	pr_debug("Module Exiting\n");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("First Task 01");
MODULE_AUTHOR("823daa4ac90a Gaurav Pathak");
