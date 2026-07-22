/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>

/* 1000 msec = 1 sec */
#define SLEEP_TIME_MS   1000

/* The devicetree node identifier for the "led0" and "button0" alias. */
#define LED0_NODE DT_ALIAS(my_led)
#define BUTTON0_NODE DT_ALIAS(my_button)

/*
 * A build error on this line means your board is unsupported.
 * See the sample documentation for information on how to fix this.
 */
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);
static const struct gpio_dt_spec button = GPIO_DT_SPEC_GET(BUTTON0_NODE, gpios);

/* Callback structure for button interrupt */
static struct gpio_callback button_cb_data;

/* Button interrupt callback handler */
static void button_pressed(const struct device *dev, struct gpio_callback *cb,
			   uint32_t pins)
{
	/* Toggle LED when button is pressed */
	gpio_pin_toggle_dt(&led);
	printf("Button pressed! LED toggled.\n");
}

int main(void)
{
	int ret;

	if (!gpio_is_ready_dt(&led)) {
		return 0;
	}

	/* Configure led pin */
	ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		return 0;
	}
	/* Configure button pin */
	ret = gpio_pin_configure_dt(&button, GPIO_INPUT);
	if (ret < 0) {
		return 0;
	}

	/* Configure interrupt on rising edge */
	ret = gpio_pin_interrupt_configure_dt(&button, GPIO_INT_EDGE_TO_ACTIVE);
	if (ret < 0) {
		return 0;
	}

	/* Initialize and register button callback */
	gpio_init_callback(&button_cb_data, button_pressed, BIT(button.pin));
	gpio_add_callback_dt(&button, &button_cb_data);

	printf("GPIO interrupt example started. Press button to toggle LED.\n");

	/* Main loop: wait for button interrupts */
	while (1) {
		k_msleep(SLEEP_TIME_MS);
	}
	return 0;
}
