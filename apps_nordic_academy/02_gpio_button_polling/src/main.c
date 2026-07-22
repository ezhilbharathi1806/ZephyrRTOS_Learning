// Include Zephyr kernel and GPIO driver headers
#include<zephyr/kernel.h>
#include<zephyr/drivers/gpio.h>

// Poll interval in milliseconds
#define SLEEP_TIME_MS 10

// Get LED GPIO spec from device tree alias
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(DT_ALIAS(my_led), gpios);

// Get button GPIO spec from device tree alias
static const struct gpio_dt_spec button = GPIO_DT_SPEC_GET(DT_ALIAS(my_button), gpios);


int main(void){
    int ret;
    
    // Check if LED device is ready for use
    if(!gpio_is_ready_dt(&led)){
        return 0;
    }
    // Check if button device is ready for use
    if(!gpio_is_ready_dt(&button)){
        return 0;
    }

    // Configure LED as output (initially active)
    ret = gpio_pin_configure_dt(&led,GPIO_OUTPUT_ACTIVE);
    if(ret != 0){
        return 0;
    }

    // Configure button as input
    ret = gpio_pin_configure_dt(&button,GPIO_INPUT);
    if(ret != 0){
        return 0;
    }

    // Main loop: poll button and mirror state to LED
    while(1){
        // Read button state (pressed or not)
        bool val = gpio_pin_get_dt(&button);
        // Set LED to match button state
        gpio_pin_set_dt(&led, val);
        // Sleep before next poll
        k_msleep(SLEEP_TIME_MS);
    }

    return 0;
}