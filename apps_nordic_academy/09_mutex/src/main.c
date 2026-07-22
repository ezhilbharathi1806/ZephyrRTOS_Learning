/*
 * Simple mutex example for rpi_pico
 */

#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

#define STACK_SIZE 512
#define THREAD0_PRIORITY 4
#define THREAD1_PRIORITY 4

/* Define mutex to protect shared resource */
K_MUTEX_DEFINE(shared_mutex);

static int32_t counter = 0;

void critical_section(int thread_id)
{
	/* Lock mutex before accessing shared resource */
	k_mutex_lock(&shared_mutex, K_FOREVER);
	
	counter++;
	printk("Thread %d: counter = %d\n", thread_id, counter);
	
	k_msleep(100);
	
	/* Unlock mutex */
	k_mutex_unlock(&shared_mutex);
}

void thread0(void)
{
	printk("Thread 0 started\n");
	for (int i = 0; i < 5; i++) {
		critical_section(0);
		k_msleep(500);
	}
}

void thread1(void)
{
	printk("Thread 1 started\n");
	for (int i = 0; i < 5; i++) {
		critical_section(1);
		k_msleep(500);
	}
}

K_THREAD_DEFINE(thread0_id, STACK_SIZE, thread0, NULL, NULL, NULL, THREAD0_PRIORITY, 0, 0);
K_THREAD_DEFINE(thread1_id, STACK_SIZE, thread1, NULL, NULL, NULL, THREAD1_PRIORITY, 0, 0);