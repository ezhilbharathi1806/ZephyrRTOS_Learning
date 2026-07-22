#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

/* Mutex and condition variable */
K_MUTEX_DEFINE(lock);
K_CONDVAR_DEFINE(condvar);

/* Shared condition */
bool data_ready = false;

/* Thread configuration */
#define STACK_SIZE 1024
#define PRIORITY   5

void consumer_thread(void *, void *, void *)
{
    k_mutex_lock(&lock, K_FOREVER);

    /* Always wait in a while loop */
    while (!data_ready) {
        printk("Consumer: waiting for data...\n");
        k_condvar_wait(&condvar, &lock, K_FOREVER);
    }

    printk("Consumer: data received!\n");
    data_ready = false;

    k_mutex_unlock(&lock);
}

void producer_thread(void *, void *, void *)
{
    k_sleep(K_SECONDS(2));  /* Simulate work */

    k_mutex_lock(&lock, K_FOREVER);

    data_ready = true;
    printk("Producer: data ready\n");

    k_condvar_signal(&condvar);

    k_mutex_unlock(&lock);
}

/* Thread definitions */
K_THREAD_DEFINE(consumer_id, STACK_SIZE,
                consumer_thread, NULL, NULL, NULL,
                PRIORITY, 0, 0);

K_THREAD_DEFINE(producer_id, STACK_SIZE,
                producer_thread, NULL, NULL, NULL,
                PRIORITY, 0, 0);
