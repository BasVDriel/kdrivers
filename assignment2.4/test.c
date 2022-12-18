#include <string.h>
#include <stdio.h>
#include <unistd.h>


int set_frequence_of_thing(int);
int read_frequence_of_thing();

char *path = "/sys/embedded_minor/leds";
FILE *fp;

int main() {
    int freq=1;
    while(freq != 0) {
        printf("what frequence would you like?\n");
        scanf("%d",&freq);
        printf("The current frequence is: %d\n",read_frequence_of_thing());
        set_frequence_of_thing(freq); 
    }
    return 0;
}

int set_frequence_of_thing(int f) {
    // Open the sysfs file in write mode
    fp = fopen(path, "w");
    if (fp == NULL) {
        // Error opening file
        perror("Error opening file");
        return -1;
    }

    // Write the frequency value to the sysfs file
    fprintf(fp, "%d", f);

    // Close the file
    fclose(fp);

    return 0;
}

int read_frequence_of_thing() {
    int f;

    // Open the sysfs file in read mode
    fp = fopen(path, "r");
    if (fp == NULL) {
        // Error opening file
        perror("Error opening file");
        return -1;
    }

    // Read the frequency value from the sysfs file
    fscanf(fp, "%d", &f);

    // Close the file
    fclose(fp);

    return f;
}
