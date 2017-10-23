#include <linux/cdev.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/module.h>
#include <asm/uaccess.h>
#include <linux/cdev.h>
#include <asm/uaccess.h>
#include <linux/time.h>
#include <linux/proc_fs.h>
#include <linux/init.h>
#include <linux/device.h>
#include <asm/uaccess.h>

/*
 *  Prototypes - this would normally go in a .h file
 */
int init_module(void);
void cleanup_module(void);
static int device_open(struct inode *, struct file *);
//static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);

#define SUCCESS 0
#define DEVICE_NAME "mytime"	/* Dev name as it appears in /proc/devices   */
#define BUF_LEN 200		/* Max length of the message from the device say 200 for safety*/

/* 
 * Global variables are declared as static, so are global within the file. 
 */

static int Major;		/* Major number assigned to our device driver */
static int Device_Open = 0;	/*Counter to keep track of the number of times the device is opened*/
				 /* Used to prevent multiple access to device */
static  char kernelTimeBuf[BUF_LEN];	/* The buffer to put kernel data into */
static  ssize_t kernelTimeBufSize = sizeof(kernelTimeBuf); /* Size of the buffer for the read funtion*/

/* If I only have 1 char device, do I really need it to have a device class?*/
static struct class*  timeClass  = NULL; ///< The device-driver class struct pointer
static struct device* timeDevice = NULL; ///< The device-driver device struct pointer



static struct file_operations fops = {
	.read = device_read,
	.open = device_open,
};

/*
 * This function is called when the module is loaded
 */
static int __init time_init(void)
{
	
	int result = 0;
	printk( KERN_NOTICE "Most recent mytime: register_device() is called." );
	Major = register_chrdev( 0, DEVICE_NAME, &fops );
	if( Major < 0 )
	{
		printk( KERN_WARNING "Most recent mytime:  can\'t register character device with errorcode = %i", result );
		return result;
	}
	printk( KERN_NOTICE "Most recent mytime: registered character device with major number = %i and minor numbers 0...255", Major );
	return 0;

	/*Char device module seems get a major number successfully (can see through dmesg) but does not build into the dev file as a module dynamically. */

	/* Start by using UDEV helper functions to see if appropriate space can be allocated for the module, if not (when it returns 0) we clean up the 
	modules/classes and exit*/

	//int successfully_created = 0;

	/*Check if there is room for new devices which is located in the /proc/devices dir
	alloc_chrdev_region(majornum ptr, first of requested minor)
	
	if(alloc_chrdev_region(&Major, 0, 1
	*/


}


/**
	Helper function for the __exit macro function. Also used if init fails.
	@param Boolean to indicate if device was successfully created or not
 */

//static void clean(int dev_created){
//
//}






/*
 * This function is called when the module is unloaded,turning this into a helper function for the office definition.
  The difference for this helper functions is that an boolean (int =0  or something else) to indicate if device was successfully created
 */
static void __exit cleanup_time(int de){

	printk( KERN_NOTICE "Simple-driver: unregister_device() is called" );
	if(Major != 0){
		unregister_chrdev(Major, DEVICE_NAME);
	}
	
	/* Implementation with classes as well, need to unregister those too*/

}




/*
 * Methods
 */

/* 
 * Innode is a reference to the file on disk
  struct file represents an abstract open file
 */
static int device_open(struct inode *inode, struct file *file)
{
	if (Device_Open){return -EBUSY;}
	Device_Open++;
	try_module_get(THIS_MODULE);
	return SUCCESS;
}



/* 
 * Called when a process, which already opened the dev file, attempts to
 * read from it. In this function I should printout my time from both the 
   current_kerenel_time and getnstimeofday().
   More generally called when the user wants to get information from the device
 */
static ssize_t device_read(struct file *filp,char *buffer,size_t count,loff_t * position){
	printk(KERN_NOTICE "Using read() function of character device in kernel space\n");

	/*Getting time values from both */
	int len = 0;
	struct timespec time = current_kernel_time();
	struct timespec time2 ;
	getnstimeofday(&time2);
	/* Send the input to the created string buffer*/

	/*Concatenating strings in c style */
	len+=	sprintf(kernelTimeBuf+len, "current_kernel_time: %ld %ld\n", time.tv_sec, time.tv_nsec);
	len+= 	sprintf(kernelTimeBuf+len, "getnstimeofday: %ld %ld\n", time2.tv_sec, time2.tv_nsec);
	/* If position is further than  */
    	if( *position >= kernelTimeBufSize ){
		return 0;
	}
    	/* If current position of pointer + how many bytes the user wants to read is greater than the size of the buffer we created in the kernel
	then we set the number bytes to be read as bufferSize-position (to get the rest of the remaining bytes) */
    	if( *position + count > kernelTimeBufSize )
        	count = kernelTimeBufSize - *position;

	/* If copying the kernel data to user buffer is 0 the assignment failed
	Cannot derefernce user-space point "buffer" because it has different data in the kernel space
	Count is how many bytes we are copying into the user buffer*/
    	if( copy_to_user(buffer, kernelTimeBuf + *position, count) != 0 ){
		return -EFAULT;
	}
    	/* Move reading position */
    	*position += count;
    	return count;

}



/* Register the functions for module init and module exit*/
module_init(time_init);
module_exit(cleanup_time);

