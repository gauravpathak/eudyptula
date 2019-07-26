#include <linux/module.h>
#include <linux/init.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/err.h>
#include <linux/errno.h>
#include <linux/string.h>

struct identity {
	char name[20];
	int id;
	bool busy;
	struct list_head list;
};

static struct identity *ID;
static struct kmem_cache *slab_kmem;
/* Declare and init the head of the linked list. */
LIST_HEAD(id_list);

void identity_destroy(int id)
{

	struct identity *getId = NULL;
	bool found = false;

	list_for_each_entry(getId, &id_list, list) {
		if (getId->id == id) {
			pr_debug("Found %s with Id %d for destruction",
					getId->name,
					getId->id);
			found = true;
			break;
		}
	}

	if (found) {
		list_del_init(&getId->list);
		kmem_cache_free(slab_kmem, getId);

		getId = NULL;

		pr_debug("After deletion: ");
		list_for_each_entry(getId, &id_list, list) {
			pr_debug("Name: %s Id: %d", getId->name, getId->id);
		}
	} else
		pr_debug("Id %d not found for destruction", id);
}

struct identity *identity_find(int id)
{
	struct identity *getId = NULL;
	bool found = false;

	list_for_each_entry(getId, &id_list, list) {
		if (getId->id == id) {
			found = true;
			break;
		}
	}

	if (!found)
		getId = NULL;
	return getId;
}

int identity_create(char *name, int id)
{

	int ret_val = 0;
	ssize_t name_len = 0;

	ID = kmem_cache_alloc(slab_kmem, GFP_ATOMIC);
	if (ID == NULL)
		ret_val = -ENOMEM;
	else {
		if (name == NULL) {
			ret_val = -ENODATA;
		} else {
			ID->id = id;
			ID->busy = false;
			name_len = strlen(name);
			strncpy(ID->name, name, name_len);
			ID->name[name_len] = '\0';
			list_add_tail(&ID->list, &id_list);
		}
	}

	return ret_val;
}

static int create_list_init(void)
{

	int error_val = 0;
	struct identity *temp = NULL;

	pr_debug("Hello World!\n");

	INIT_LIST_HEAD(&id_list);

	slab_kmem = kmem_cache_create("slab_823daa4ac90a",
			sizeof(struct identity), 0, SLAB_HWCACHE_ALIGN,
			NULL);

	if (slab_kmem == NULL) {
		pr_err("Failed to make kmem cache\n");
		error_val = -ENOMEM;
		goto err_exit;
	}

	error_val = identity_create("Alice", 1);
	if (error_val < 0) {
		pr_err("Cannot create identity %d", error_val);
		goto err_exit;
	}
	error_val = identity_create("Bob", 2);
	if (error_val < 0) {
		pr_err("Cannot create identity %d", error_val);
		goto err_exit;
	}
	error_val = identity_create("Dave", 3);
	if (error_val < 0) {
		pr_err("Cannot create identity %d", error_val);
		goto err_exit;
	}
	error_val = identity_create("Gena", 10);
	if (error_val < 0) {
		pr_err("Cannot create identity %d", error_val);
		goto err_exit;
	}

	temp = identity_find(10);
	pr_debug("Found id: %d Name: %s\n", temp->id, temp->name);

	temp = identity_find(42);
	if (temp == NULL)
		pr_debug("id 42 not found\n");

	identity_destroy(2);
	identity_destroy(1);
	identity_destroy(10);
	identity_destroy(42);
	identity_destroy(3);

	return error_val;

err_exit:
	kmem_cache_destroy(slab_kmem);
	return error_val;
}

static void create_list_exit(void)
{
	kmem_cache_destroy(slab_kmem);
	pr_debug("Module Exiting\n");

}

module_init(create_list_init);
module_exit(create_list_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Task 13");
MODULE_AUTHOR("823daa4ac90a Gaurav Pathak");
