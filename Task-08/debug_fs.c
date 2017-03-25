#include <linux/module.h>
#include <linux/init.h>
#include <linux/debugfs.h>
#include <linux/string.h>
#include <linux/jiffies.h>
#include <linux/uaccess.h>
#include <linux/rwsem.h>

#define len 200
#define MY_ID "823daa4ac90a"
#define dir_name "eudyptula"

int file_value;
char ker_buf[len];
char foo_buf[PAGE_SIZE];

struct rw_semaphore foo_sem;
struct dentry *dirret, *fileret;

static ssize_t jiffies_reader(struct file *fp, char __user *user_buffer,
		size_t count, loff_t *position)
{
	unsigned long j_start = jiffies;
	char loc_buf[65];

	if (*position == 0) {
		pr_debug("Jiffies: %lu\n", j_start);
		snprintf(loc_buf, count, "%lu", j_start);
	}

	return simple_read_from_buffer(user_buffer, count, position, loc_buf,
			(strlen(loc_buf)+1));
}

static const struct file_operations fops_jiffies = {.read = jiffies_reader, };

static ssize_t myreader(struct file *fp, char __user *user_buffer,
		size_t count, loff_t *position)
{
	strncpy(ker_buf, MY_ID, strlen(MY_ID));

	return simple_read_from_buffer(user_buffer, count, position,
			ker_buf, len);
}

/* write file operation */
static ssize_t mywriter(struct file *fp, const char __user *user_buffer,
		size_t count, loff_t *position)
{
	ssize_t ret_val;

	if (*position > count)
		ret_val = -EINVAL;

	ret_val = simple_write_to_buffer(ker_buf, count, position,
			user_buffer, count);

	if (strncmp(ker_buf, MY_ID, strlen(MY_ID)) != 0)
		ret_val = -EINVAL;

	return ret_val;
}

static const struct file_operations fops_debug = {.read = myreader, .write =
		mywriter, };

static ssize_t foo_reader(struct file *fp, char __user *user_buffer,
		size_t count, loff_t *position)
{
	size_t ret_val;

	down_read(&foo_sem);

	strncpy(ker_buf, MY_ID, strlen(MY_ID));
	ret_val = simple_read_from_buffer(user_buffer, count, position,
				foo_buf, PAGE_SIZE);
	up_read(&foo_sem);

	return ret_val;
}

/* write file operation */
static ssize_t foo_writer(struct file *fp, const char __user *user_buffer,
		size_t count, loff_t *position)
{
	ssize_t ret_val;

	if (*position > count)
		ret_val = -EINVAL;
	else {
		down_write(&foo_sem);
		ret_val = simple_write_to_buffer(foo_buf, PAGE_SIZE, position,
				user_buffer, count);
		up_write(&foo_sem);
	}

	return ret_val;
}

static const struct file_operations fops_foo = {.read = foo_reader, .write =
		foo_writer, };

static int debug_init(void)
{
	pr_debug("Initilizing Debugfs Entry\n");

	dirret = debugfs_create_dir(dir_name, NULL);

	if (!dirret) {
		pr_err("Cannot create directory in debugfs\n");
		return -ENOENT;
	}

	fileret = debugfs_create_file("id", 0666, dirret, &file_value,
			&fops_debug);
	if (!fileret) {
		pr_err("Cannot create file in debugfs\n");
		return -ENOENT;
	}

	fileret = debugfs_create_file("jiffies", 0444, dirret, &file_value,
			&fops_jiffies);
	if (!fileret) {
		pr_err("Cannot create jiffies file in debugfs\n");
		return -ENOENT;
	}

	fileret = debugfs_create_file("foo", 0244, dirret, &file_value,
			&fops_foo);
	if (!fileret) {
		pr_err("Cannot create foo file in debugfs\n");
		return -ENOENT;
	}

	init_rwsem(&foo_sem);

	return 0;
}

static void debug_exit(void)
{
	pr_debug("Module Exiting\n");
	debugfs_remove_recursive(dirret);
}

module_init(debug_init);
module_exit(debug_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Task 08");
MODULE_AUTHOR("823daa4ac90a Gaurav Pathak");
