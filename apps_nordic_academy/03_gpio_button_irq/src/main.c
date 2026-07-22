// Include Zephyr kernel and GPIO driver headers
#include<zephyr/kernel.h>
#include<zephyr/drivers/gpio.h>

// Poll interval in milliseconds
#define SLEEP_TIME_MS 10

// Get LED GPIO spec from device tree alias
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(DT_ALIAS(my_led), gpios);

// Get button GPIO spec from device tree alias
static const struct gpio_dt_spec button = GPIO_DT_SPEC_GET(DT_ALIAS(my_button), gpios);

// button interrupt callback handler
void button_pressed(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
    gpio_pin_toggle_dt(&led);
}

//Callback structure for button interrupt
static struct gpio_callback button_cb_data;

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

    //configure interrupt on raising edge
    ret = gpio_pin_interrupt_configure_dt(&button, GPIO_INT_EDGE_TO_ACTIVE);
    if(ret != 0){
        return 0;
    }

    //Initialize and register button callback
    gpio_init_callback(&button_cb_data, button_pressed, BIT(button.pin)); 	
    gpio_add_callback_dt(&button, &button_cb_data);

    // Main loop: wait for button interrupts 
    while(1){
        k_msleep(SLEEP_TIME_MS);
    }

    return 0;
}