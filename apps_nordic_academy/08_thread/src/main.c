#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

/* STEP 2 - Define stack size and scheduling priority used by each thread */
#define STACKSIZE 1024
#define PRIORITY 5  


void thread0(void)
{
	while (1) {
		/* STEP 3 - Call printk() to display a simple string "Hello, I am thread0" */
		printk("Hello, I am thread0\n");
        
        /* STEP 6 - Make the thread yield */
        k_yield();   // Yield to allow other threads of the same priority to run
		
        /* STEP 10 - Put the thread to sleep */
		//k_msleep(5);  // Sleep for 5 milliseconds to allow other threads to run
	}
}

void thread1(void)
{
	while (1) {
		/* STEP 3 - Call printk() to display a simple string "Hello, I am thread1" */
		printk("Hello, I am thread1\n");

        /* STEP 8 - Make the thread yield */
        k_yield();   // Yield to allow other threads of the same priority to run
		
        /* STEP 10 - Put the thread to sleep */
		//k_msleep(5);  // Sleep for 5 milliseconds to allow other threads to run
	}
}

/* STEP 4 - Define and initialize the two threads */
K_THREAD_DEFINE(thread0_name, STACKSIZE, thread0, NULL, NULL, NULL, PRIORITY, 0, 0);
K_THREAD_DEFINE(thread1_name, STACKSIZE, thread1, NULL, NULL, NULL, PRIORITY, 0, 0);