#include<stdio.h>
#include<errno.h>
#include<unistd.h>
#include<linux/unistd.h>
#include<sys/time.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/unistd.h>


//Define new sty call number (from table file)
#define _NR_curtime 326

int main(void){
	while(1){
		//make the sys call
		struct timespec test;
		int ret= syscall(_NR_curtime, &test);
		/*Check if any sys call errors */
		printf("nano sec: %ld , secs: %ld\n" , test.tv_nsec, test.tv_sec);
		sleep(1);
	}
	return 0;
}
