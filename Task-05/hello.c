#include <linux/module.h>
#include <linux/init.h>
#include <linux/usb.h>
#include <linux/hid.h>

static const struct usb_device_id usb_id_table[] = {
	{ USB_INTERFACE_INFO(USB_INTERFACE_CLASS_HID,
		USB_INTERFACE_SUBCLASS_BOOT,
			USB_INTERFACE_PROTOCOL_MOUSE) },
	{}
};

MODULE_DEVICE_TABLE(usb, usb_id_table);

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
