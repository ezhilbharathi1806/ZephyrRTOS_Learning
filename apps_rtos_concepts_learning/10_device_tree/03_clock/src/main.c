#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/clock_control.h>
#include <zephyr/drivers/clock_control/rp2040_clock_control.h>
#include <zephyr/sys/printk.h>

int main(void)
{
    const struct device *clk_dev;
    uint32_t rate;

    clk_dev = DEVICE_DT_GET(DT_NODELABEL(clocks));

    if (!device_is_ready(clk_dev)) {
        printk("Clock device not ready!\n");
        return 0;
    }

    printk("RP2040 Clock Information:\n\n");

    // System clock (CPU)
    clock_control_get_rate(clk_dev,
        (clock_control_subsys_t)RP2040_CLOCK_SYS,
        &rate);
    printk("System Clock (SYS): %u Hz\n", rate);

    // Reference clock
    clock_control_get_rate(clk_dev,
        (clock_control_subsys_t)RP2040_CLOCK_REF,
        &rate);
    printk("Reference Clock (REF): %u Hz\n", rate);

    // Peripheral clock
    clock_control_get_rate(clk_dev,
        (clock_control_subsys_t)RP2040_CLOCK_PERI,
        &rate);
    printk("Peripheral Clock (PERI): %u Hz\n", rate);

    // USB clock
    clock_control_get_rate(clk_dev,
        (clock_control_subsys_t)RP2040_CLOCK_USB,
        &rate);
    printk("USB Clock: %u Hz\n", rate);

    // ADC clock
    clock_control_get_rate(clk_dev,
        (clock_control_subsys_t)RP2040_CLOCK_ADC,
        &rate);
    printk("ADC Clock: %u Hz\n", rate);

    while (1) {
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
