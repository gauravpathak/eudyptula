#include <linux/kobject.h>
#include <linux/string.h>
#include <linux/sysfs.h>
#include <linux/module.h>
#include <linux/jiffies.h>
#include <linux/init.h>

const char MY_ID[] = "823daa4ac90a";

static char foo[PAGE_SIZE];
static char id[12];

struct rw_semaphore foo_sem;

static ssize_t foo_show(struct kobject *kobj, struct kobj_attribute *attr,
		char *buf)
{
	ssize_t ret_val;

	down_read(&foo_sem);
	ret_val = sprintf(buf, "%s\n", foo);
	up_read(&foo_sem);

	return ret_val;
}

static ssize_t foo_store(struct kobject *kobj, struct kobj_attribute *attr,
		const char *buf, size_t count)
{
	down_read(&foo_sem);
	strncpy(foo, buf, PAGE_SIZE);
	up_read(&foo_sem);

	return count;
}

static struct kobj_attribute foo_attribute =
__ATTR_RW(foo);


static ssize_t b_show(struct kobject *kobj, struct kobj_attribute
		*attr,
		char *buf)
{
	int ret;

	if (strcmp(attr->attr.name, "id") == 0)
		ret = sprintf(buf, "%s\n", MY_ID);

	else
		ret = sprintf(buf, "%ld\n", jiffies);

	return ret;
}

static ssize_t b_store(struct kobject *kobj, struct kobj_attribute *attr,
		const char *buf, size_t count)
{
	ssize_t ret = 0;

	if (strcmp(attr->attr.name, "id") == 0) {
		pr_debug("ID: %s\n", buf);
		if (strncmp(buf, MY_ID, strlen(MY_ID)) != 0)
			ret = -EINVAL;

		else {
			strncpy(id, buf, strlen(MY_ID));
			ret = count;
		}
	}

	return ret;
}

static struct kobj_attribute id_attribute = __ATTR_RW(b);
static struct kobj_attribute jiffies_attribute = __ATTR_RO(b);


static struct attribute *attrs[] = {
	&foo_attribute.attr,
	&id_attribute.attr,
	&jiffies_attribute.attr,
	NULL,	/* need to NULL terminate the list of attributes */
};

static struct attribute_group attr_group = {
	.attrs = attrs,
};

static struct kobject *example_kobj;

static int __init example_init(void)
{
	int retval;

	pr_debug("Creting Kobject\n");

	example_kobj = kobject_create_and_add("eudyptula", kernel_kobj);

	if (!example_kobj)
		return -ENOMEM;

	init_rwsem(&foo_sem);

	pr_debug("Creting Sysfs Groups\n");

	retval = sysfs_create_group(example_kobj, &attr_group);

	if (retval)
		kobject_put(example_kobj);

	return retval;
}

static void __exit example_exit(void)
{
	pr_debug("Cleaning and Exiting\n");
	kobject_put(example_kobj);
}

module_init(example_init);
module_exit(example_exit);
MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("823daa4ac90a");

