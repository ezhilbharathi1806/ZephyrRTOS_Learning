#include <zephyr.h>
#include <sys/printk.h>

// Define a queue statically
K_QUEUE_DEFINE(my_queue);

struct data_item {
    int value;
};

void producer(void)
{
    struct data_item item;
    item.value = 42;

    printk("Producer: Putting item %d into queue\n", item.value);
    k_queue_append(&my_queue, &item);
}

void consumer(void)
{
    struct data_item *item;

    item = k_queue_get(&my_queue, K_FOREVER); // Wait forever until item arrives
    printk("Consumer: Got item %d from queue\n", item->value);
}

void main(void)
{
    printk("Queue Example - Static\n");

    producer();
    consumer();
}
