#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/kdev_t.h>
#include <linux/cdev.h>
#include <linux/gpio.h>

#define RIGHT_PIN 24
#define LEFT_PIN 25
#define BUTTON_PIN_UP 20
#define BUTTON_PIN_DWN 21

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Bas van Driel");
MODULE_DESCRIPTION("A simple device driver to turn leds on and off");

dev_t dev = 0;
struct cdev my_cdev;
struct device *my_device = NULL;
static struct class *my_class = NULL;
int minor, major;
unsigned int dev_alloc, cdev_added, class_created, device_created;
char selector;


ssize_t write_to_driver(struct file *file_obj, const char *buf, size_t count, loff_t *offset){
    //remember copy from user
    char command;
    copy_from_user(&command, buf, sizeof(char));
    if(command == 'C'){
        selector = 'C';
    }
    if(selector == 'C'){
        if(command == '0'){
            gpio_set_value(RIGHT_PIN, 0);
            gpio_set_value(LEFT_PIN, 0);
        }
        if(command == '1'){
            gpio_set_value(LEFT_PIN, 1);
        }
        if(command == '2'){
            gpio_set_value(RIGHT_PIN, 1);
        }
    }
    else{
        printk(KERN_ALERT "Invalid command");
    }
    return count;
}

struct file_operations fops = {
    .owner = THIS_MODULE,
    .write = write_to_driver
};

int init_chardev(void) {
    //dynamic allocation of major and minor numbers
    dev_alloc = alloc_chrdev_region(&dev, 0, 1, "device_drv");
    if(dev_alloc != 0){
        return -1;
    }
    major = MAJOR(dev);
    minor = MINOR(dev);
    printk(KERN_ALERT "major: %d, minor: %d", major, minor);

    cdev_init(&my_cdev, &fops);
    my_cdev.owner = THIS_MODULE;
    my_cdev.ops = &fops;
    cdev_added = cdev_add(&my_cdev, dev, 1);
    if(cdev_added != 0){
        return -1;
    }

    my_class = class_create(THIS_MODULE, "device_class");
    if(my_class == NULL){
        class_created = 0;
        return -1;
    }

    my_device = device_create(my_class, NULL, dev, NULL, "device_drv");
    if(my_device == NULL){
        device_created = 0;
        return -1;
    }
    return 0;
}

int exit_chardev(void) {
    if(cdev_added == 0){
        cdev_del(&my_cdev);
    }
    if(device_created == 0){
        device_destroy(my_class, dev);
    }
    if(class_created == 0){
        class_destroy(my_class);
    }
    if(dev_alloc == 0){
        unregister_chrdev(major, "device_drv");
    }
    return 0;
}

static int hello_init(void){
    int retv; 
    printk(KERN_ALERT "Hello, minor\n");
    //set up gpios with request
    gpio_request(RIGHT_PIN, "right");
    gpio_request(LEFT_PIN, "left");  
    gpio_request(BUTTON_PIN_UP, "button_up");
    gpio_request(BUTTON_PIN_DWN, "button_dwn");
    gpio_direction_output(RIGHT_PIN, 0);
    gpio_direction_output(LEFT_PIN, 0);
    gpio_direction_input(BUTTON_PIN_UP);
    gpio_direction_input(BUTTON_PIN_DWN);
    retv=init_chardev();
    return retv;
}

static void hello_exit(void){
    int retv;
    printk(KERN_ALERT "Goodbye, embedded minor\n");
    gpio_set_value(LEFT_PIN, 0);
    gpio_set_value(RIGHT_PIN, 0);
    gpio_free(LEFT_PIN);
    gpio_free(RIGHT_PIN);
    gpio_free(BUTTON_PIN_UP);
    retv=exit_chardev();
}

module_init(hello_init);
module_exit(hello_exit);