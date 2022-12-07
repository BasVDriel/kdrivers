#include <linux/module.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>
#include <linux/string.h>

#define LED_NAME led

static int led_state = 0;

static ssize_t led_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf){
    led_state += 1;
    return sprintf(buf, "%d\n", led_state);
}

static ssize_t led_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count){
    sscanf(buf, "%du", &led_state);
    return count;
}

static struct kobj_attribute led_attribute = __ATTR(LED_NAME, 0664, led_show, led_store);

static struct kobject embedded_minor_kobj;

static struct kobj_type ktype = {
    .sysfs_ops = &kobj_sysfs_ops,
    .release = NULL,
};

static int __init embedded_minor_init(void){
    int error = 0;

    kobject_init_and_add(&embedded_minor_kobj, &ktype, NULL, "embedded_minor");

    error = sysfs_create_file(&embedded_minor_kobj, &led_attribute.attr);
    if (error)
        kobject_put(&embedded_minor_kobj);

    return error;
}

static void __exit embedded_minor_exit(void){
    kobject_put(&embedded_minor_kobj);
}

module_init(embedded_minor_init);
module_exit(embedded_minor_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Bas van Driel");
MODULE_DESCRIPTION("A simple kernel module with a kobject and sysfs file");