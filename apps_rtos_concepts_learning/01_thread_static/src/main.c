#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

/* Stack size and thread priorities */
#define STACK_SIZE 1024
#define THREAD1_PRIORITY 5
#define THREAD2_PRIORITY 7

/* Thread stack definitions */
K_THREAD_STACK_DEFINE(thread1_stack, STACK_SIZE);
K_THREAD_STACK_DEFINE(thread2_stack, STACK_SIZE);

/* Thread control blocks */
struct k_thread thread1_data;
struct k_thread thread2_data;

/* Thread entry functions */
void thread1_entry(void *arg1, void *arg2, void *arg3)
{
    while (1) {
        printk("Thread 1 running (priority %d)\n", THREAD1_PRIORITY);
        k_msleep(1000);
    }
}

void thread2_entry(void *arg1, void *arg2, void *arg3)
{
    while (1) {
        printk("Thread 2 running (priority %d)\n", THREAD2_PRIORITY);
        k_msleep(2000);
    }
}

int main(void)
{
    printk("Zephyr Threads Example\n");

    k_thread_create(
        &thread1_data,
        thread1_stack,
        STACK_SIZE,
        thread1_entry,
        NULL, NULL, NULL,
        THREAD1_PRIORITY,
        0,
        K_NO_WAIT
    );

    k_thread_create(
        &thread2_data,
        thread2_stack,
        STACK_SIZE,
        thread2_entry,
        NULL, NULL, NULL,
        THREAD2_PRIORITY,
        0,
        K_NO_WAIT
    );

    return 0;
}
