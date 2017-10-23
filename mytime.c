#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/module.h>

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

static  ssize_t my_read (struct file *, char *, size_t, loff_t *){


}




static const struct file_operations sample_fops = {
    .owner			= THIS_MODULE, /* This is a predefined macro for convience*/
    .open			= my_open,
    .read = 			my_read,
    .release		= my_close
};

struct miscdevice mytime_device = {
    .minor = MISC_DYNAMIC_MINOR, /* Dyanmic minor assigned with a macro*/
    .name = "mytime",  /*Name of the misc device */
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
