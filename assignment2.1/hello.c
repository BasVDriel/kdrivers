/**
 * @file hello.c
 * @author John
 * @date 9 Sep 2017
 * @brief An introduction in the linux kernel module.
*/

#include <linux/init.h>
#include <linux/module.h>
MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("John");


static int hello_init(void){
    printk(KERN_ALERT "Hello, minor\n");
    return 0;
}

static void hello_exit(void){

    printk(KERN_ALERT "Goodbye, embedded minor\n");
}

module_init(hello_init);
module_exit(hello_exit);