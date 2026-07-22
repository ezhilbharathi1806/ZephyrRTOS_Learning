/*
k_sleep(K_SECONDS(10));
Now the watchdog will expire (5s timeout), and the RP2040 will reset.
*/

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/watchdog.h>
#include <zephyr/sys/printk.h>

#define WDT_TIMEOUT_MS 5000

int main(void)
{
    const struct device *wdt;
    int wdt_channel_id;
    struct wdt_timeout_cfg wdt_config;

    printk("Starting RP2040 Watchdog example\n");

    wdt = DEVICE_DT_GET(DT_ALIAS(watchdog));
    if (!device_is_ready(wdt)) {
        printk("Watchdog device not ready\n");
        return 0;
    }

    wdt_config.flags = WDT_FLAG_RESET_SOC;
    wdt_config.window.min = 0U;
    wdt_config.window.max = WDT_TIMEOUT_MS;
    wdt_config.callback = NULL;

    wdt_channel_id = wdt_install_timeout(wdt, &wdt_config);
    if (wdt_channel_id < 0) {
        printk("Cannot install watchdog timeout\n");
        return 0;
    }

    if (wdt_setup(wdt, 0) < 0) {
        printk("Cannot setup watchdog\n");
        return 0;
    }

    printk("Watchdog started (timeout %d ms)\n", WDT_TIMEOUT_MS);

    while (1) {
        printk("Feeding watchdog...\n");
        wdt_feed(wdt, wdt_channel_id);
        k_sleep(K_SECONDS(2)); // for watchdog timer to expire, change this to K_SECONDS(10)
    }
}
