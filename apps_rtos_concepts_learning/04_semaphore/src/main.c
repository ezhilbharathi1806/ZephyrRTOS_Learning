#include <zephyr/kernel.h>

/* Define a semaphore
 * Initial count = 0
 * Max count = 1 (binary semaphore)
 */
K_SEM_DEFINE(my_sem, 0, 1);

/* Thread stack size and priority */
#define STACK_SIZE 1024
#define PRIORITY 5

void thread_wait(void *arg1, void *arg2, void *arg3)
{
    while (1) {
        printk("Thread WAIT: waiting for semaphore...\n");

        /* Take semaphore (wait forever) */
        k_sem_take(&my_sem, K_FOREVER);

        printk("Thread WAIT: semaphore acquired!\n");
        k_sleep(K_SECONDS(1));
    }
}

void thread_give(void *arg1, void *arg2, void *arg3)
{
    while (1) {
        k_sleep(K_SECONDS(3));

        printk("Thread GIVE: giving semaphore\n");
        k_sem_give(&my_sem);
    }
}

/* Define threads */
K_THREAD_DEFINE(wait_tid, STACK_SIZE,
                thread_wait, NULL, NULL, NULL,
                PRIORITY, 0, 0);

K_THREAD_DEFINE(give_tid, STACK_SIZE,
                thread_give, NULL, NULL, NULL,
                PRIORITY, 0, 0);
