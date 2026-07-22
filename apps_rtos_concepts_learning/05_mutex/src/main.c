#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

/* Define mutex */
K_MUTEX_DEFINE(my_mutex);

/* Thread stack size and priority */
#define STACK_SIZE 1024
#define THREAD_PRIORITY 5

void thread_a(void *, void *, void *)
{
    while (1) {
        k_mutex_lock(&my_mutex, K_FOREVER);

        printk("Thread A: entered critical section\n");
        k_sleep(K_MSEC(1000));
        printk("Thread A: leaving critical section\n");

        k_mutex_unlock(&my_mutex);

        k_sleep(K_MSEC(500));
    }
}

void thread_b(void *, void *, void *)
{
    while (1) {
        k_mutex_lock(&my_mutex, K_FOREVER);

        printk("Thread B: entered critical section\n");
        k_sleep(K_MSEC(1000));
        printk("Thread B: leaving critical section\n");

        k_mutex_unlock(&my_mutex);

        k_sleep(K_MSEC(500));
    }
}

/* Define threads */
K_THREAD_DEFINE(thread_a_id, STACK_SIZE,
                thread_a, NULL, NULL, NULL,
                THREAD_PRIORITY, 0, 0);

K_THREAD_DEFINE(thread_b_id, STACK_SIZE,
                thread_b, NULL, NULL, NULL,
                THREAD_PRIORITY, 0, 0);
