#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>

#define BUTTON0_NODE DT_ALIAS(btn_0)
#define BUTTON1_NODE DT_ALIAS(btn_1)

#if !DT_NODE_HAS_STATUS(BUTTON0_NODE, okay) || \
    !DT_NODE_HAS_STATUS(BUTTON1_NODE, okay)
#error "Buttons not defined in device tree"
#endif

static const struct gpio_dt_spec button0 =
    GPIO_DT_SPEC_GET(BUTTON0_NODE, gpios);
static const struct gpio_dt_spec button1 =
    GPIO_DT_SPEC_GET(BUTTON1_NODE, gpios);

static struct gpio_callback button0_cb;
static struct gpio_callback button1_cb;

/* ISR for Button 0 */
void button0_pressed(const struct device *dev,
                     struct gpio_callback *cb,
                     uint32_t pins)
{
    printk("0\n");
}

/* ISR for Button 1 */
void button1_pressed(const struct device *dev,
                     struct gpio_callback *cb,
                     uint32_t pins)
{
    printk("1\n");
}

int main(void)
{
    if (!device_is_ready(button0.port) ||
        !device_is_ready(button1.port)) {
        printk("GPIO device not ready\n");
        return 0;
    }

    /* Configure buttons as input with pull-up */
    gpio_pin_configure_dt(&button0, GPIO_INPUT | GPIO_PULL_UP);
    gpio_pin_configure_dt(&button1, GPIO_INPUT | GPIO_PULL_UP);

    /* Configure interrupts */
    gpio_pin_interrupt_configure_dt(&button0,
        GPIO_INT_EDGE_TO_ACTIVE);
    gpio_pin_interrupt_configure_dt(&button1,
        GPIO_INT_EDGE_TO_ACTIVE);

    /* Setup callbacks */
    gpio_init_callback(&button0_cb, button0_pressed, BIT(button0.pin));
    gpio_add_callback(button0.port, &button0_cb);

    gpio_init_callback(&button1_cb, button1_pressed, BIT(button1.pin));
    gpio_add_callback(button1.port, &button1_cb);

    printk("RP2040 interrupt example started\n");

    while (1) {
        k_sleep(K_FOREVER);
    }
}
