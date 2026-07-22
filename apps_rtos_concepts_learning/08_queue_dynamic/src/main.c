#include <zephyr.h>
#include <sys/printk.h>

// Define a queue object dynamically
struct k_queue my_dynamic_queue;

struct data_item {
    int value;
};

void producer(void)
{
    struct data_item *item = k_malloc(sizeof(struct data_item));
    item->value = 99;

    printk("Producer: Putting item %d into queue\n", item->value);
    k_queue_append(&my_dynamic_queue, item);
}

void consumer(void)
{
    struct data_item *item;

    item = k_queue_get(&my_dynamic_queue, K_FOREVER); // Wait forever until item arrives
    printk("Consumer: Got item %d from queue\n", item->value);

    k_free(item); // Don't forget to free memory!
}

void main(void)
{
    printk("Queue Example - Dynamic\n");

    k_queue_init(&my_dynamic_queue); // Initialize dynamic queue

    producer();
    consumer();
}
