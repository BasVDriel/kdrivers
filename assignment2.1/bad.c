/**
 * @file hello.c
 * @author John
 * @date 9 Sep 2017
 * @brief An introduction in the linux kernel module.
*/

//sudo insmod bad.ko fault_nr=1
//sudo rmmod bad.ko

#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/slab.h>

static unsigned int fault_nr = 0;

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("John");

module_param(fault_nr, uint, S_IRUGO);
MODULE_PARM_DESC(fault_nr, "Each number represents a different fault");

static int hello_init(void){
    printk(KERN_ALERT "Hello, minor\n");
    //fault selection
    switch(fault_nr){
        case 1:
            printk(KERN_ALERT "Fault 1: devide by zero\n");
            int number = 100/0;
            break;
        case 2:
            printk(KERN_ALERT "Fault 2: dereference null pointer\n");
            int *ptr;
            int num;
            ptr = NULL;
            num = *ptr;
            break;
        case 3:
            printk(KERN_ALERT "Fault 3: return value other than 0\n");
            return 1;
            break;
        case 4:
            printk(KERN_ALERT "Fault 4: access after free \n");
            int *ptr2;
            ptr2 = (int *)kmalloc(sizeof(int), GFP_KERNEL);
            kfree(ptr2);
            *ptr2 = 100;
            break;
        default :
            printk(KERN_ALERT "Select a valid lower number\n");
            break;
    }
    return 0;
}

static void hello_exit(void){
    printk(KERN_ALERT "Goodbye, embedded minor\n");
}

module_init(hello_init);
module_exit(hello_exit);