#include <linux/module.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>
#include <linux/string.h>
#include <linux/gpio.h>
#include <linux/delay.h>
#include <linux/interrupt.h>
#include <linux/kthread.h>

#define LED_NAME leds
#define RIGHT_PIN 24
#define LEFT_PIN 25
#define BUTTON_PIN_UP 20
#define BUTTON_PIN_DWN 21
#define MIN_FREQ 1
#define MAX_FREQ 1000

unsigned int frequency = 1;
unsigned int irq_up;
unsigned int irq_dwn;
static struct task_struct *kthread;

//blink function for kthread
static int blink(void *data){
    int i = 0;
    while(!kthread_should_stop()){
        gpio_set_value(RIGHT_PIN, 1);
        gpio_set_value(LEFT_PIN, 0);
        msleep(1000/frequency);
        gpio_set_value(RIGHT_PIN, 0);
        gpio_set_value(LEFT_PIN, 1);
        msleep(1000/frequency);
    }
    return 0;
}

void changeFrequency(int freq){
    frequency+=freq;
    if (frequency < MIN_FREQ){
        frequency = MIN_FREQ;
    }
    if (frequency > MAX_FREQ){
        frequency = MAX_FREQ;
    }
}

static irq_handler_t handler(unsigned int irq, void *dev_id, struct pt_regs *regs) {
    printk(KERN_ALERT "Interrupt triggerd!\n");
    if(irq == irq_up){
        changeFrequency(1);
    }
    if (irq == irq_dwn){
        changeFrequency(-1);
    }
	return (irq_handler_t) IRQ_HANDLED; 
}

static ssize_t led_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf){
    printk(KERN_ALERT "Show called!\n");
    return sprintf(buf, "%d", frequency);
}

static ssize_t led_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count){
    //check size of buf is smaller than 6
    int tempFreq;
    printk(KERN_ALERT "Store called!\n");
    if (count > 5){
        return -EINVAL;
    }
   
    sscanf(buf, "%d", &tempFreq);
    changeFrequency(frequency-tempFreq);
    printk(KERN_ALERT "%s", buf, frequency);
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

    //set up gpios with request
    gpio_request(RIGHT_PIN, "right");
    gpio_request(LEFT_PIN, "left");  
    gpio_request(BUTTON_PIN_UP, "button_up");
    gpio_request(BUTTON_PIN_DWN, "button_dwn");
    gpio_direction_output(RIGHT_PIN, 0);
    gpio_direction_output(LEFT_PIN, 0);
    gpio_direction_input(BUTTON_PIN_UP);
    gpio_direction_input(BUTTON_PIN_DWN);


    //set up intterupt for button
    irq_up = gpio_to_irq(BUTTON_PIN_UP);
    irq_dwn = gpio_to_irq(BUTTON_PIN_DWN);
    
    request_irq(irq_up, (irq_handler_t) handler, IRQF_TRIGGER_RISING, "irq_button_handler", NULL);
    request_irq(irq_dwn, (irq_handler_t) handler, IRQF_TRIGGER_RISING, "irq_button_handler", NULL);

	kthread = kthread_create(blink, NULL, "kthread");
	if(kthread != NULL){
		wake_up_process(kthread);
		printk("kthread - Thread 1 was created and is running now!\n");
	}

    return error;
}

static void __exit embedded_minor_exit(void){
    kobject_put(&embedded_minor_kobj);
    free_irq(irq_up, NULL);
    free_irq(irq_dwn, NULL);
    gpio_set_value(LEFT_PIN, 0);
    gpio_set_value(RIGHT_PIN, 0);
    gpio_free(LEFT_PIN);
    gpio_free(RIGHT_PIN);
    gpio_free(BUTTON_PIN_UP);
    kthread_stop(kthread);
}

module_init(embedded_minor_init);
module_exit(embedded_minor_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Bas van Driel");
MODULE_DESCRIPTION("A simple kernel module with a kobject and sysfs file");