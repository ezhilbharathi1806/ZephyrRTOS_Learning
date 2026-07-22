#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

/* Define FIFO */
K_FIFO_DEFINE(my_fifo);

/* Data structure for FIFO items */
struct fifo_item {
    void *fifo_reserved;   /* 1st word reserved for kernel */
    int value;
};

/* Producer thread */
void producer_thread(void)
{
    static int counter = 0;

    while (1) {
        struct fifo_item *item = k_malloc(sizeof(struct fifo_item));
        if (!item) {
            printk("Allocation failed\n");
            return;
        }

        item->value = counter++;
        printk("Produced: %d\n", item->value);

        k_fifo_put(&my_fifo, item);
        k_sleep(K_SECONDS(1));
    }
}

/* Consumer thread */
void consumer_thread(void)
{
    while (1) {
        struct fifo_item *item =
            k_fifo_get(&my_fifo, K_FOREVER);

        printk("Consumed: %d\n", item->value);
        k_free(item);
    }
}

/* Thread definitions */
K_THREAD_DEFINE(producer_tid, 1024,
                producer_thread, NULL, NULL, NULL,
                5, 0, 0);

K_THREAD_DEFINE(consumer_tid, 1024,
                consumer_thread, NULL, NULL, NULL,
                5, 0, 0);
