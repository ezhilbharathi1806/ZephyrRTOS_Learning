/*Asynchronous API (DMA / Event-Based)

The most advanced UART API.
Uses:
    Interrupts
    DMA (if supported by hardware)
    Event-driven callbacks
Designed for high-performance, non-blocking communication.

uart_callback_set()
uart_rx_enable()
uart_tx()
uart_rx_buf_rsp()

| Feature     | Async                   |
| ----------- | ----------------------- |
| Blocking    | No                      |
| CPU usage   | Very low                |
| DMA support | Yes                     |
| Complexity  | High                    |
| Best for    | High throughput systems |
*/

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/drivers/gpio.h>

/* LED configuration from device tree */
#define LED_NODE DT_ALIAS(led0)

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED_NODE, gpios);
static const struct device *uart_dev = DEVICE_DT_GET(DT_NODELABEL(uart0));

/* RX buffer */
#define RX_BUF_SIZE 16
static uint8_t rx_buf[RX_BUF_SIZE];

/**
 * UART Event Callback Handler
 *
 * Handles async UART events: data ready (RX_RDY) and reception disabled (RX_DISABLED)
 * Processes received characters and controls LED
 */
static void uart_cb(const struct device *dev,
                    struct uart_event *evt,
                    void *user_data)
{
    switch (evt->type) {

    case UART_RX_RDY:
        /* Data received - process all bytes in buffer */
        for (int i = 0; i < evt->data.rx.len; i++) {

            uint8_t c = evt->data.rx.buf[evt->data.rx.offset + i];

            /* Control LED based on received character */
            if (c == '1') {
                gpio_pin_set_dt(&led, 1);   // LED ON
            }
            else if (c == '0') {
                gpio_pin_set_dt(&led, 0);   // LED OFF
            }
        }
        break;

    case UART_RX_DISABLED:
        /* Re-enable reception after disabled event */
        uart_rx_enable(dev, rx_buf, sizeof(rx_buf), 50);
        break;

    default:
        break;
    }
}

/**
 * Main function - Initialize and run async UART application
 */
int main(void)
{
    /* Check LED GPIO is available */
    if (!device_is_ready(led.port)) {
        return 0;
    }

    /* Configure LED pin as output, initially inactive (OFF) */
    gpio_pin_configure_dt(&led, GPIO_OUTPUT_INACTIVE);

    /* Verify UART device is ready before use */
    if (!device_is_ready(uart_dev)) {
        return 0;
    }

    /* Register async callback handler */
    uart_callback_set(uart_dev, uart_cb, NULL);

    /* Enable RX with DMA/async transfer (50ms timeout) */
    uart_rx_enable(uart_dev, rx_buf, sizeof(rx_buf), 50);

    /* Sleep indefinitely - all work done in callbacks */
    while (1) {
        k_sleep(K_FOREVER);
    }

    return 0;
}