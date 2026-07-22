/*Interrupt-Driven AP

Uses hardware interrupts.The UART peripheral interrupts the CPU when:
    Data arrives (RX)
    TX buffer is empty
You provide a callback function.

uart_irq_callback_user_data_set()
uart_irq_rx_enable()
uart_irq_tx_enable()
uart_fifo_read()
uart_fifo_fill()

| Feature            | Interrupt          |
| ------------------ | ------------------ |
| Blocking           | No                 |
| CPU usage          | Efficient          |
| Complexity         | Medium             |
| Good for           | Most embedded apps |
| Requires ISR logic | Yes                |
*/

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/drivers/gpio.h>
#include <string.h>

/* Get LED0 device node from device tree */
#define LED_NODE DT_ALIAS(led0)

/* GPIO configuration for LED0 obtained from device tree */
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED_NODE, gpios);

/* UART device pointer obtained from device tree (UART0) */
static const struct device *uart_dev = DEVICE_DT_GET(DT_NODELABEL(uart0));

/*UART Interrupt Callback Handler
 *
 * Functionality:
 * - Reads available data from UART FIFO
 * - Interprets '1' as LED ON command
 * - Interprets '0' as LED OFF command
 */
void uart_cb(const struct device *dev, void *user_data)
{
    uint8_t c;

    /* Check for pending interrupts and if RX data is ready */
    while (uart_irq_update(dev) && uart_irq_rx_ready(dev)) {

        /* Read one byte from UART FIFO */
        if (uart_fifo_read(dev, &c, 1)) {

            /* Control LED based on received character */
            if (c == '1') {
                gpio_pin_set_dt(&led, 1);   // LED ON
            }
            else if (c == '0') {
                gpio_pin_set_dt(&led, 0);   // LED OFF
            }
        }
    }
}

int main(void)
{
    /* Verify LED GPIO port is ready before use */
    if (!device_is_ready(led.port)) {
        return 0;
    }

    /* Configure LED pin as output, initially inactive (OFF) */
    gpio_pin_configure_dt(&led, GPIO_OUTPUT_INACTIVE);

    /* Verify UART device is ready before use */
    if (!device_is_ready(uart_dev)) {
        return 0;
    }

    /* Register the UART interrupt callback function */
    uart_irq_callback_user_data_set(uart_dev, uart_cb, NULL);

    /* Enable RX interrupt so uart_cb is called when data arrives */
    uart_irq_rx_enable(uart_dev);

    /* Idle loop: CPU sleeps indefinitely until interrupt wakes it */
    /* The uart_cb handler will execute whenever UART data is received */
    while (1) {
        k_sleep(K_FOREVER);
    }

    return 0;
}