#include<zephyr/kernel.h>
#include<zephyr/drivers/gpio.h>

#define SLEEP_TIME_MS 1000

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(DT_ALIAS(my_led), gpios);

int main(void){
    if(!gpio_is_ready_dt(&led)){
        return 0;
    }

    int ret = gpio_pin_configure_dt(&led,GPIO_OUTPUT_ACTIVE);
    if(ret != 0){
        return 0;
    }

    while(1){
        gpio_pin_toggle_dt(&led);
        k_msleep(SLEEP_TIME_MS);
    }

    return 0;
}