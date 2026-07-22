/*Polling API (Blocking / Simple)

The simplest UART API.
The CPU actively checks (polls) the UART hardware for data.

No interrupts. No callbacks.

uart_poll_out(dev, char);       // TX: waits until TX register is ready, then write
uart_poll_in(dev, &char);       // RX: checks if data is available. Returns 0 if data received and Returns -1 if no data

| Feature     | Polling                |
| ----------- | ---------------------- |
| Blocking    | Yes                    |
| CPU usage   | High                   |
| Easy to use | Very                   |
| Good for    | Simple apps, debugging |
| Scalable    | No                     |

*/

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/uart.h>
#include <string.h>

#define UART_NODE DT_NODELABEL(uart0)

int main(void)
{
    const struct device *uart_dev = DEVICE_DT_GET(UART_NODE);

    if (!device_is_ready(uart_dev)) {
        return;
    }

    const char *startup_msg = "Simple UART polling example on RPi Pico\r\n";

    /* --- Transmit startup message --- */
    for (int i = 0; i < strlen(startup_msg); i++) {
        uart_poll_out(uart_dev, startup_msg[i]);
    }

    /* --- Main loop --- */
    while (1) {
        unsigned char c;

        /* Wait until a character is received */
        while (uart_poll_in(uart_dev, &c) != 0) {
            k_sleep(K_MSEC(1));
        }

        /* Echo it back */
        uart_poll_out(uart_dev, c);
    }

    return 0;
}