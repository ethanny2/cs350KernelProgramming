#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>

int main(){
  struct timeval utime;
    char xtime[500];
    int mytimeDevice = open("/dev/mytime", O_RDONLY);
    if (mytimeDevice== -1) {
            perror ("open");
            return 2;
    }

    int bytes_read = read(mytimeDevice, &xtime, 500);
    close(mytimeDevice);
    printf("%s\n", xtime);
  return 0;
}
