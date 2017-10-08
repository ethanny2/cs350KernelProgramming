#include<stdio.h>
#include<errno.h>
#include<unistd.h>
#include<linux/unistd.h>
//Define new sty call number (from table file)
#define _NR_hello_call 326

int main(void){
	while(1){
		//make the sys call
		int ret;
		int dummy =10;
		ret= syscall(326);
		/*Check if any sys call errors */
		printf("ret = %d errno = %d\n",ret,errno);
		sleep(1);
	}
	return 0;
}
