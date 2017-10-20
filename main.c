#include<stdio.h>
#include<errno.h>
#include<unistd.h>
#include<linux/unistd.h>
#include<sys/time.h>
#include <sys/syscall.h>
#include<linux/kernel.h>
#include<sys/unistd.h>


//Define new sty call number (from table file)
#define _NR_curtime 326

int main(void){
		//make the sys call
		struct timespec test;
		int ret= syscall(326,&test);
		/*Check if any sys call errors print the time again in */
		unsigned long conversionVal = 1000000000;
		unsigned long totalNanosecs = (test.tv_nsec) + ( (test.tv_sec)*conversionVal);
		printf("Total nano secs from user space program: %lu\n" , totalNanosecs);
	return 0;
}
