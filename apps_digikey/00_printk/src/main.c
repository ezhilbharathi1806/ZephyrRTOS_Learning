//physical UART0 pins (GP0 = TX, GP1 = RX)

#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

void main(void)
{
    printk("Hello from Zephyr on Raspberry Pi Pico!\n");
    printk("Built: %s %s\n", __DATE__, __TIME__);

    while (1) {
        printk("Hello again... (%u)\n", k_uptime_get_32() / 1000);
        k_sleep(K_SECONDS(2));
    }
}