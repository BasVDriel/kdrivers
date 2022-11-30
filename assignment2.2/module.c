/**
 * @file hello.c
 * @author John
 * @date 9 Sep 2017
 * @brief An introduction in the linux kernel module.
*/

//sudo insmod bad.ko fault_nr=1
//sudo rmmod bad.ko

#include <linux/module.h>
#include <linux/init.h>
#include <linux/kobject.h>
#include <linux/string.h>
#include <linux/sysfs.h>
#include <linux/slab.h>

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("Bas_van_Driel");

static struct kobject *kobj;

struct data {
    struct attribute attr;
    int value;
};

static struct data *led_data;

static ssize_t show(struct kobject *kobj, struct attribute *attr, char *buffer){
    int value = led_data->value;
    int num_chars = sprintf(buffer, "%d\n", value);
    printk(KERN_ALERT "show function value_ptr contained %d\n", value);
    return num_chars;
}

static ssize_t store(struct kobject *kobj, struct attribute *attr, const char *buffer, size_t count){
    sscanf(buffer, "%d", &led_data->value);
    printk("sysfs_test - You wrote '%s' to /sys/kernel/%s/%s\n", buffer, kobj->name, attr->name);
    return count;
}

static int hello_init(void){
    printk(KERN_ALERT "Hello, minor\n");

    //initialize sysfs_ops
    struct sysfs_ops ops = {
        .show = show,
        .store = store,
    };

    printk(KERN_ALERT "All good 1\n");
    //initialize a single data struct with an attribute
    led_data->attr.name = "led";
    led_data->attr.mode = 0666;
    led_data->value = 0;

    printk(KERN_ALERT "All good 2\n");
    //pointer to array of my data strucs
    struct attribute * attrs[] = {
        &led_data->attr,
        NULL
    };
    printk(KERN_ALERT "All good 3\n");
    //initialize kobj_type
    struct kobj_type ktype = {
        .sysfs_ops = &ops,
        .default_attrs = attrs,
    };

    printk(KERN_ALERT "All good 4\n");
    int err = -1;
    kobj = kmalloc(sizeof(*kobj), GFP_KERNEL);
    if (kobj) {
        kobject_init(kobj, &ktype);
        if (kobject_add(kobj, NULL, "%s","embedded_minor")) {
            err = -1;
            printk("Sysfs creation failed\n");
            kobject_put(kobj);
            kobj = NULL;
        }
        err = 0;
    }

    return err;
}

static void hello_exit(void){
    printk(KERN_ALERT "Goodbye, embedded minor\n");
    kobject_put(kobj);
}

module_init(hello_init);
module_exit(hello_exit);