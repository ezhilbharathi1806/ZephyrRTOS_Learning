#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

/* Dynamic event object */
struct k_event app_event;

/* Event bit */
#define EVT_DATA_READY  BIT(0)

#define STACK_SIZE 1024
#define PRIORITY   5

void consumer_thread(void *, void *, void *)
{
    printk("Consumer: waiting for event...\n");

    k_event_wait(&app_event,
                 EVT_DATA_READY,
                 false,
                 K_FOREVER);

    printk("Consumer: event received!\n");
}

void producer_thread(void *, void *, void *)
{
    k_sleep(K_SECONDS(2));

    printk("Producer: setting event\n");
    k_event_set(&app_event, EVT_DATA_READY);
}

/* Create threads */
K_THREAD_DEFINE(consumer_id, STACK_SIZE,
                consumer_thread, NULL, NULL, NULL,
                PRIORITY, 0, 0);

K_THREAD_DEFINE(producer_id, STACK_SIZE,
                producer_thread, NULL, NULL, NULL,
                PRIORITY, 0, 0);

void main(void)
{
    /* Runtime initialization */
    k_event_init(&app_event);
}
