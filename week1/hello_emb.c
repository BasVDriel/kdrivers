/*
 ============================================================================
 Name        : Hello_virtual_machine.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

FILE * fp;

struct Pin {
   char loc[30];
   int len;
};  


// set the dircetion of the pins 
void gpio_set_dir(int dir, struct Pin pin){
    char* in = "in";
    char* out = "out";
    char full_path[100];
    strcpy(full_path, pin.loc);
    strcat(full_path, "/direction");
    fp = fopen (full_path, "w");
    if(dir == 1){
        fprintf(fp, "%s", out);
    } else {
        fprintf(fp, "%s", in);
    }
    fclose(fp);
}

void gpio_set_val(int val, struct Pin pin){
    char full_path[100];
    strcpy(full_path, pin.loc);
    strcat(full_path, "/value");
    fp = fopen (full_path, "w");
    fprintf(fp, "%d", val);
    fclose(fp);
}

int gpio_get_val(struct Pin pin){
    char full_path[100];
    strcpy(full_path, pin.loc);
    strcat(full_path, "/value");
    fp = fopen (full_path, "r");
    int val;
    fscanf(fp, "%d", &val);
    fclose(fp);
    return val;
}

int main(void) {
    //make the 3 gpio structs
    struct Pin gpio24;
    struct Pin gpio25;
    struct Pin gpio20;

    //fill the struct with a path to the gpio pin and the length
    strcpy(gpio24.loc, "/sys/class/gpio/gpio24");
    gpio24.len = 22;
    strcpy(gpio25.loc, "/sys/class/gpio/gpio25");
    gpio25.len = 22;
    strcpy(gpio20.loc, "/sys/class/gpio/gpio20");
    gpio20.len = 22;


    puts("Hello World!!!"); /* prints Hello World!!! */
    gpio_set_dir(1, gpio24);
    gpio_set_dir(1, gpio25);
    gpio_set_dir(0, gpio20);
    gpio_set_val(0, gpio24);
    gpio_set_val(0, gpio25);
    while(1){
        int button1 = gpio_get_val(gpio20);
        if(button1 == 1){
            gpio_set_val(1, gpio24);
            gpio_set_val(0, gpio25);
            usleep(1000000);
            gpio_set_val(0, gpio24);
            gpio_set_val(1, gpio25);
            usleep(1000000);
        }
    }

	puts("!!!Hello Embedded Minor!!!"); /* prints !!!Hello Embedded minor!!! */
	return EXIT_SUCCESS;
}
