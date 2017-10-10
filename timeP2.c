#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/syscalls.h>

asmlinkage long my_xtime(struct timespec *current_time){
	if(!current_time || !access_ok(VERIFY_WRITE, current_time, sizeof(struct timespec))){
		/* Return the error*/
		printk(KERN_ALERT "Problem with memory address\n");
		return -EFAULT;
	}else{
		/*Place current time values in the object */
		struct timespec cur = current_kernel_time();
		current_time->tv_sec = cur.tv_sec;
		current_time->tv_nsec = cur.tv_nsec;
		/* Convert current time to nanoseconds*/
		unsigned long conversionVal = 1000000000;
		unsigned long totalNanosecs = (current_time->tv_nsec) + 
		((current_time->tv_sec)*conversionVal);

		printk(KERN_ALERT "Current time in Nanonseconds is: %lu", totalNanosecs);
		return 0;
	} 
}

EXPORT_SYMBOL(my_xtime);
