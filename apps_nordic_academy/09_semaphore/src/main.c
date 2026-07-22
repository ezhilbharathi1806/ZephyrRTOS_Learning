/*
 * Simple semaphore example for rpi_pico
 */

#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

#define STACK_SIZE 512
#define PRODUCER_PRIORITY 5
#define CONSUMER_PRIORITY 5

/* Define semaphore with initial count of 5 */
K_SEM_DEFINE(resource_sem, 5, 5);

void producer(void)
{
	printk("Producer started\n");
	for (int i = 0; i < 3; i++) {
		k_msleep(1000);
		k_sem_give(&resource_sem);
		printk("Resource released | Available: %d\n", k_sem_count_get(&resource_sem));
	}
}

void consumer(void)
{
	printk("Consumer started\n");
	for (int i = 0; i < 3; i++) {
		k_sem_take(&resource_sem, K_FOREVER);
		printk("Resource acquired | Available: %d\n", k_sem_count_get(&resource_sem));
		k_msleep(500);
	}
}

K_THREAD_DEFINE(producer_id, STACK_SIZE, producer, NULL, NULL, NULL, PRODUCER_PRIORITY, 0, 0);
K_THREAD_DEFINE(consumer_id, STACK_SIZE, consumer, NULL, NULL, NULL, CONSUMER_PRIORITY, 0, 0);