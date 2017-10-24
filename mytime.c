#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <asm/uaccess.h>
#include <linux/slab.h>


#define BUF_LEN 500		/* Max length of the message from the device say 200 for safety*/
#define STR_LEN1 50 /* Why 25? Because I counted the output. current_time... needs ~40 chars. */
#define STR_LEN2 50
static  char kernelTimeBuf[BUF_LEN];	/* The buffer to put kernel data into */
static char  str1[STR_LEN1];
static char  str2[STR_LEN2];

static  ssize_t kernelTimeBufSize = sizeof(kernelTimeBuf); /* Size of the buffer for the read funtion*/
static  ssize_t str1Size = sizeof(str1);
static  ssize_t str2Size= sizeof(str2);


static int my_open(struct inode *inode, struct file *file)
{
    printk(KERN_NOTICE "Opened mytime misc char device \n");
    return 0;
}

static int my_close(struct inode *inodep, struct file *filp)
{
    printk(KERN_NOTICE "Closed mytime misc char device\n");
    return 0;
}

static ssize_t my_read(struct file *filp,char *buffer,size_t count,loff_t * position){
	printk(KERN_NOTICE "Using read() function of character device in kernel space\n");
	/*Getting time values from both */
	//int len = 0;
	struct timespec time = current_kernel_time();
	struct timespec time2;
	getnstimeofday(&time2);
	/* Send the input to the created string buffer*/
	sprintf(kernelTimeBuf,"current_kernel_time: %lld %9ld\n getnstimeofday: %lld %9ld" ,(long long)time.tv_sec,time.tv_nsec,(long long)time2.tv_sec,time2.tv_nsec);
	//sprintf(str2, "getnstimeofday: %lld %9ld" ,(long long)time2.tv_sec,time2.tv_nsec);
	/* +2 for the terminating chars in both*/
//	char * dynamicKernelBuff = kmalloc(str1Size+str2Size+2 ,GFP_KERNEL);
	//memcpy(dynamicKernelBuff, str1, str1Size);
	/* Separate by a newline*/
//	sprintf(dynamicKernelBuff,"%s , %s",str1,str2);

	//dynamicKernelBuff[str1Size] = '\n';
//	memcpy(dynamicKernelBuff + str1Size + 1, str2,str2Size  + 1); // includes terminating null
	/*Debugging */
	//printk(KERN_NOTICE "current_kernel_time: %lld %9ld", (long long)time.tv_sec , time.tv_nsec);
	//printk(KERN_NOTICE "getnstimeofday: %lld %9ld", (long long)time2.tv_sec , time2.tv_nsec);
	printk(KERN_NOTICE "TEST : %s\n", kernelTimeBuf);
	/*Concatenating strings in c style */
	//len+=	sprintf(kernelTimeBuf+len, "current_kernel_time: %ld %ld\n", time.tv_sec, time.tv_nsec);
	//len+= 	sprintf(kernelTimeBuf+len, "getnstimeofday: %ld %ld\n", time2.tv_sec, time2.tv_nsec); 
	/* If position is further than  */
//    	if( *position >= kernelTimeBufSize ){
//		return 0;
//	}
    	/* If current position of pointer + how many bytes the user wants to read is greater than the size of the buffer we created in the kernel
	then we set the number bytes to be read as bufferSize-position (to get the rest of the remaining bytes) */
  //  	if( *position + count > kernelTimeBufSize )
    //    	count = kernelTimeBufSize - *position;

	/* If copying the kernel data to user buffer is 0 the assignment failed
	Cannot derefernce user-space point "buffer" because it has different data in the kernel space
	Count is how many bytes we are copying into the user buffer*/
    	if( copy_to_user(buffer, kernelTimeBuf , count) != 0 ){
		printk(KERN_NOTICE "ERROR DID NOT COPY ALL BYTES TO USER!\n");
		return -EFAULT;
	}
    	/* Move reading position */
  //  	*position += count;
    	return count;

}




static const struct file_operations sample_fops = {
    .owner			= THIS_MODULE, /* This is a predefined macro for convience*/
    .open			= my_open,
    .read = 			my_read,
    .release		= my_close
};

struct miscdevice mytime_device = {
    .minor = MISC_DYNAMIC_MINOR, /* Dyanmic minor assigned with a macro*/
    .name = "mytime",  /*Name of the misc device has to be same name as the .c file */
    .fops = &sample_fops, /*Point to struct that has the file definitions */
};

static int __init misc_init(void)
{
    int error;
    /*Attempting to register misc device. Pass in struct */
    error = misc_register(&mytime_device);
    if (error){
        printk(KERN_NOTICE "ERROR TRYING TO REGISTER mytime \n");
        return error;
    }
    printk( KERN_NOTICE "SUCCESS Registering mytime misc char device\n");
    return 0;
}

static void __exit misc_exit(void)
{
    misc_deregister(&mytime_device);
    printk(KERN_NOTICE "SUCCESSFULLY DEREGISTERED mytime misc device\n");
}

module_init(misc_init);
module_exit(misc_exit);
