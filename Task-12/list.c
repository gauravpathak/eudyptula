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
/* Declare and init the head of the linked list. */
LIST_HEAD(id_list);

void identity_destroy(int id) {

  struct identity *getId = NULL;
//  struct list_head *iter_list;
  bool found = false;

  list_for_each_entry(getId, &id_list, list)
  {
    if (getId->id == id) {
      pr_debug("Found %s with Id %d for destruction", getId->name, getId->id);
      found = true;
      break;
    }
  }

  if (found) {
    list_del_init(&getId->list);
    kfree(getId);

    getId = NULL;

    pr_debug("After deletion: ");
    list_for_each_entry(getId, &id_list, list)
    {
      pr_debug("Name: %s Id: %d", getId->name, getId->id);
    }
  }
  else {
    pr_debug("Id %d not found for destruction", id);
  }
}

struct identity *identity_find(int id) {

  struct identity *getId = NULL;
  struct list_head *iter_list;
  bool found = false;

  list_for_each(iter_list, &id_list)
  {
    getId = list_entry(iter_list, struct identity, list);
    if (getId->id == id) {
      found = true;
    }
  }
  if (!found) {
    getId = NULL;
  }
  return getId;
}

int identity_create(char *name, int id) {

  int ret_val = 0;
  ssize_t name_len = 0;

  ID = kmalloc(sizeof(struct identity), GFP_KERNEL);

  if (ID == NULL) {
    pr_err("Cannot Allocate Memory");
    ret_val = -ENOMEM;
  }
  else {
    if (name == NULL) {
      ret_val = -ENODATA;
    }
    else {
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

static int create_list_init(void) {

  int error_val = 0;
  struct identity *temp;

  pr_debug("Hello World!\n");

  INIT_LIST_HEAD(&id_list);

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

  err_exit: return error_val;
}

static void create_list_exit(void) {
  pr_debug("Module Exiting\n");
}

module_init(create_list_init);
module_exit(create_list_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Task 12");
MODULE_AUTHOR("823daa4ac90a Gaurav Pathak");
