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

//export gpio pins function
void export_gpio(int pin)
{
   char buffer[256];
   sprintf(buffer, "%d", pin);
   fp = fopen("/sys/class/gpio/export", "w");
   fwrite(buffer, sizeof(char), strlen(buffer), fp);
   fclose(fp);
}



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

    //export the pins
    export_gpio(24);
    export_gpio(25);
    export_gpio(20);
    gpio_set_dir(1, gpio24);
    gpio_set_dir(1, gpio25);
    gpio_set_dir(0, gpio20);
    gpio_set_val(0, gpio24);
    gpio_set_val(0, gpio25);
    while(1){
        //debounce button press with usleep
        int button = gpio_get_val(gpio20); 
        if(button == 1){
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
