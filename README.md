Ethan Soo Hon
CS 375


Makefile- Make file needed to build the misc character device

log.log - Log version of git commit messages.

main_module_call.c - The userspace program where the misc character device module read() function was tested. Accepts command line arg for loop value.

mytime.c - Misc character device main module code. 

  
Q:Now, change the module so that init_module() returns -1, recompile, 
and retest. What happens? Why?
A: The init_module() function is not allowed to return a value other than 0.
If this functions returns a nunmber that is not 0 such as -1, then it signals
when you are trying to install the module to the Kernel that your module
cannot be loaded.



Q:Run the user-level program for various values of N (both very small and very large). In your journal, explain the reason for any differences that you observe between the three timing mechanisms. 

Run Value: N=4,5,6
Thoughts: The gettimeofday() and getnstimeofday() seem very close in terms of
calculating time. The tv_sec value of the timespecs returned from both these functions are idential. Their
tv_nsec fields vary by very little. gettimeofday() tv_sec field will always be larger because internally
this function adds extra nanoseconds via the timespec_add_ns() function.The current_kernel_time differs greatly.

Run Value: N=100,200,500
Thoughts: As the N input size gets exponetially larger the differece between the tv_sec fields of 
all three functions starts to go away. Normally the values in getnstimeofday() and gettimeofday() vary 
greatly from current_kernel_time(). This is because current_kernel_time returns a value of time from a 
timer value that is interrupted by a timer interrupt. This means if the value in here is too drastically 
different your timer interrupt period is not fast enough to get the the interrupt to read the data when the
clock is refreshed. This is how it loses precision. When called with a high enough value N, the timer 
interrupt period is quicker and you get more accurate readings.


