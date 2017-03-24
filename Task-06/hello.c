#include <linux/module.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/string.h>
#include <linux/vmalloc.h>

#define MY_ID "823daa4ac90a"

static ssize_t write_node(struct file *fp, const char __user *buf, size_t len,
		loff_t *offset)
{
	ssize_t ret_val;
	char *loc_buf = vmalloc(len+1);

	if (*offset > len)
		ret_val = -EINVAL;

	ret_val = simple_write_to_buffer(loc_buf, len, offset, buf, len);
	if (ret_val < 0) {
		vfree(loc_buf);
		return ret_val;
	}
	if (strncmp(loc_buf, MY_ID, strlen(MY_ID)) != 0) {
		pr_info("Error Writing: %d Bytes\nExpected: %d Bytes\nWritten %s",
				ret_val, len, loc_buf);
		ret_val = -EINVAL;
	}

	vfree(loc_buf);
	return ret_val;
}
static ssize_t read_node(struct file *fp, const char __user *buf, size_t len,
		loff_t *offset)
{
	ssize_t ret_val;

	ret_val = simple_read_from_buffer(buf, len, offset, MY_ID,
			sizeof(MY_ID));
	pr_info("%d Bytes read", ret_val);

	return ret_val;
}

static const struct file_operations fileops = {
		.write = write_node,
		.read = read_node
};

static const struct miscdevice sample_device = {
		.minor = MISC_DYNAMIC_MINOR,
		.name = "eudyptula",
		.fops = &fileops,
};

static int hello_init(void)
{
	int error;

	error = misc_register(&sample_device);
	if (error) {
		pr_err("can't misc_register :(\n");
		return error;
	}
	pr_info("I'm in\n");
	return 0;
}

static void hello_exit(void)
{
	misc_deregister(&sample_device);
	pr_info("I'm out\n");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Task 06");
MODULE_AUTHOR("823daa4ac90a Gaurav Pathak");
