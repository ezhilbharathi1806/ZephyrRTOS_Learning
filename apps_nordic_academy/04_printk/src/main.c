#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

int main(void){
    uint8_t a = 10;
    printk("hello from main.c  %d\n", a);
    while(1){
        printk("hello from while loop\n");
        k_msleep(1000);
    }
}