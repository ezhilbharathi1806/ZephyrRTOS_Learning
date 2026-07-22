/*main() submits a work item

System workqueue thread (k_sys_work_q) executes my_work_handler()

main() keeps running independently

Output repeats every 2 seconds*/

#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

/* Work handler function */
void my_work_handler(struct k_work *work)
{
    printk("Workqueue handler running!\n");

    /* Simulate some work */
    k_msleep(500);

    printk("Workqueue handler done.\n");
}

/* Define a work item */
K_WORK_DEFINE(my_work, my_work_handler);

int main(void)
{
    printk("Zephyr Workqueue Example\n");

    while (1) {
        printk("Submitting work\n");

        /* Submit work to system workqueue */
        k_work_submit(&my_work);

        /* Main thread continues */
        k_msleep(2000);
    }

    return 0;
}
