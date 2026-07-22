/*
Pico        I2C Device
-------------------------
GP4  ------ SDA
GP5  ------ SCL
3V3  ------ VCC
GND  ------ GND
*/

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/sys/printk.h>

/* Get I2C device from devicetree (use your specific label, e.g., i2c0) */
static const struct device *i2c_dev = DEVICE_DT_GET(DT_NODELABEL(i2c0));

void main(void) {
    if (device_is_ready(i2c_dev)) {
        printk("I2C device ready: %s\n", i2c_dev->name);

        /* Optional: force 100 kHz if not already set */
        int ret_cfg = i2c_configure(i2c_dev, I2C_SPEED_SET(I2C_SPEED_STANDARD));
        if (ret_cfg < 0) {
            printk("Cannot configure I2C speed: %d\n", ret_cfg);
        }
    } else {
        printk("I2C NOT ready!\n");
        return;
    }
    
    while (1) {
        printk("I2C device: %s\n", i2c_dev->name);
        printk("Scanning I2C bus...\n");

        for (uint8_t addr = 0x08; addr <= 0x77; addr++) {
            int ret = i2c_write(i2c_dev, NULL, 0, addr);
            printk("ret = %d\n", ret);
            if (ret == 0) {
                printk("Found device at address: 0x%02x\n", addr);
            }
            k_msleep(5); // Short delay between address checks
        }
        printk("Scan complete.\n");
        k_msleep(100); // Wait before next scan
    }
}
