#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>

int write2drv(char);

int main(){
    write2drv('C'); //select the LEDs
    write2drv('2');//set LD2 on
    sleep(3);

    write2drv('1');// set LD1 on
    sleep(3);

    write2drv('0'); //both LEDs are off.

    return 0;
}


int write2drv(char c) {
    FILE *fp;

    fp=fopen("/dev/device_drv","w+");
    if(!fp){
        printf("fail to open /dev/device_drv");
        return -1;
    }

    fputc(c,fp);
    fclose(fp);
    return 0;
}