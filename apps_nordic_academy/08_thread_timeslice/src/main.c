#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

#define STACKSIZE 1024

#define THREAD0_PRIORITY 6
#define THREAD1_PRIORITY 7

void thread0(void)
{
	while (1) {
		printk("Hello, I am thread0\n");
		k_busy_wait(1000000);
	}
}

void thread1(void)
{
	while (1) {
		printk("Hello, I am thread1\n");
		k_busy_wait(1000000);
	}
}

/* STEP 4 - Define and initialize the two threads */
K_THREAD_DEFINE(thread0_name, STACKSIZE, thread0, NULL, NULL, NULL, THREAD0_PRIORITY, 0, 0);
K_THREAD_DEFINE(thread1_name, STACKSIZE, thread1, NULL, NULL, NULL, THREAD1_PRIORITY, 0, 0);