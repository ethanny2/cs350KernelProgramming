#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>



int main(int argc, char** argv){
 if(argc != 2){ 
	fprintf(stderr, "Format: [Number of iterations to compare]\n"); 
	return 1;
  }
   int n=atoi(argv[1]);
    struct timeval utime[500];
    for(int i=0;i<n;i++){
	    	int bytes_read;
		int mytimeDevice;
		char xtime[500];
		mytimeDevice = open("/dev/mytime", O_RDONLY);
		if (mytimeDevice== -1) {
		    perror ("open");
	            return 2;
    		}
	    	bytes_read = read(mytimeDevice, xtime, 500);
		if(bytes_read<0){
			printf("Error reading number of bytes from char device\n");
			exit(1);
		}
	    	gettimeofday(&utime[i], 0);
		printf("i#: %d\nDEVICE TIMES:\n %s\n gettimeofday:%ld %.06ld\n", i , xtime, (long long)utime[i].tv_sec, utime[i].tv_usec );
		//printf("Testing %lld  %.06ld\n", (long long)utime[i].tv_sec , utime[i].tv_usec);
		//printf("Testing read file %s\n", xtime);
		close(mytimeDevice); 

   }
  return 0;
}
