#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/sched.h>
#include<linux/syscalls.h>




asmlinkage long sys_call_test(void){

	printk("Hello world!");

}

EXPORT_SYMBOL(sys_call_test);
