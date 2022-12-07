#include <linux/module.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>
#include <linux/string.h>
#include <linux/gpio.h>
#include <linux/delay.h>
#include <linux/interrupt.h>

#define LED_NAME leds
#define RIGHT_PIN 24
#define LEFT_PIN 25
#define BUTTON_PIN 20

enum led_state {
    LED_LEFT = 0,
    LED_RIGHT = 1,
    OFF = 2,
};

static int led_state = OFF;

static irq_handler_t handler(unsigned int irq, void *dev_id, struct pt_regs *regs) {
	printk("Interrupt triggerd!\n");
    if(led_state == LED_LEFT || led_state == OFF){
        led_state = LED_RIGHT;
        gpio_set_value(LEFT_PIN, 0);
        gpio_set_value(RIGHT_PIN, 1);
    }
    else if(led_state == LED_RIGHT){
        led_state = LED_LEFT;
        gpio_set_value(LEFT_PIN, 1);
        gpio_set_value(RIGHT_PIN, 0);
    }
	return (irq_handler_t) IRQ_HANDLED; 
}

static ssize_t led_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf){
    if (led_state == LED_LEFT){
        return sprintf(buf, "left");
    }
    if (led_state == LED_RIGHT){
        return sprintf(buf, "right");
    }
    if (led_state == OFF){
        return sprintf(buf, "off");
    }
    return 0;
}

static ssize_t led_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count){
    //check size of buf is smaller than 6
    if (count < 7 ){
        if (strncmp(buf, "left", 4) == 0){
            led_state = LED_LEFT;
            gpio_set_value(LEFT_PIN, 1);
        }
        if (strncmp(buf, "right", 5) == 0){
            led_state = LED_RIGHT;  
            gpio_set_value(RIGHT_PIN, 1);
        }
        if (strncmp(buf, "off", 3) == 0){
            led_state = OFF;
            gpio_set_value(LEFT_PIN, 0);
            gpio_set_value(RIGHT_PIN, 0);
        }
        return count;
    }
    else{
        return -EINVAL;
    }
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

    //set up gpios with request
    gpio_request(RIGHT_PIN, "right");
    gpio_request(LEFT_PIN, "left");  
    gpio_request(BUTTON_PIN, "button");   
    gpio_direction_output(RIGHT_PIN, 0);
    gpio_direction_output(LEFT_PIN, 0);
    gpio_direction_input(BUTTON_PIN);

    //set up intterupt for button
    unsigned int irq = gpio_to_irq(BUTTON_PIN);
    int result =  request_irq(irq, (irq_handler_t) handler, IRQF_TRIGGER_RISING, "irq_button_handler", NULL);
    return error;
}

static void __exit embedded_minor_exit(void){
    kobject_put(&embedded_minor_kobj);
    gpio_free(LEFT_PIN);
    gpio_free(RIGHT_PIN);
    gpio_free(BUTTON_PIN);
}

module_init(embedded_minor_init);
module_exit(embedded_minor_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Bas van Driel");
MODULE_DESCRIPTION("A simple kernel module with a kobject and sysfs file");