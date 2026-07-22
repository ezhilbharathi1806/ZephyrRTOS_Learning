#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/sys/printk.h>

#include <stdio.h>

LOG_MODULE_REGISTER(my_module, LOG_LEVEL_DBG);

int main(void) {
    k_msleep(2000);  // Wait 2 seconds for USB/host to settle

    printk("Hello from printk\n");

    printf("Hello from printf\n");

    LOG_INF("Hello from logger");
    LOG_DBG("Debug detail: %d", 42);
    LOG_ERR("Something bad: %p", (void *)NULL);  // Example pointer

    while (1) {
        LOG_INF("Alive: %u", k_uptime_get_32());
        k_msleep(2000);
    }
    return 0;
}
