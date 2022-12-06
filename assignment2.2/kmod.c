#include <linux/module.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>
#include <linux/string.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Bas van Driel GNU/Linux");
MODULE_DESCRIPTION("Creating a folder and a file in sysfs");

static struct kobject *embedded_minor_kobj;
static int value = 0;

static ssize_t value_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf){
    return sprintf(buf, "%d\n", value);
}

static ssize_t value_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count){
    sscanf(buf, "%du", &value);
    return count;
}

static struct kobj_attribute led_attr = __ATTR(led, 0644, value_show, value_store);

static int __init embedded_minor_init(void){
    printk(KERN_INFO "initializing the embedded_minor module\n");
    int retval = 0;

    embedded_minor_kobj = kobject_create_and_add("embedded_minor", kernel_kobj);
    if (!embedded_minor_kobj){
        retval = -ENOMEM;
    }
    printk(KERN_INFO "Created the embedded_minor folder in sysfs, value: %d", retval);

    retval = sysfs_create_file(embedded_minor_kobj, &led_attr.attr);
    if (retval){
        printk(KERN_INFO "Failt to create the fs. return value: %d", retval);
        kobject_put(embedded_minor_kobj);
    }
    return retval;
}

static void __exit embedded_minor_exit(void){
    printk(KERN_INFO "Removing the embedded_minor module");
    kobject_put(embedded_minor_kobj);
}

module_init(embedded_minor_init);
module_exit(embedded_minor_exit);