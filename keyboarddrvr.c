#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/kernel.h>

//These preprocessors have to be the last ones
#include <linux/fs.h>
#include <linux/uaccess.h>


#define SUCCESS 0 				//anything other than 0 is a problem
#define DEVICE_NAME "keyboarddrvr"		// have to get another keyboard to test on
#define BUF_LEN 256 				//max length of messages
#define MINOR_NUMBER 71	//defined when plugged in. dynamically alloc if not.

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Daivd Gistand");
MODULE_VERSION("0.1"); 			//its not done yet.
MODULE_DESCRIPTION("Just for fun. Please dont run this on a real machine");

static int major;			//number given to device driver by kernel
//static int device_open = 0;
static char message[BUF_LEN] = {0};

///////////////////////function prototypes///////////////////////

//callbacks executed when name happens. source: <linux/fs.h>
static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
//returns ssize_t in case err
static ssize_t device_read(struct file *, char __user *buffer, size_t, loff_t *);
static ssize_t device_write(struct file *, const char __user *buffer, size_t, loff_t *);

//connect internal events to user functions
static struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = device_open,
	.read = device_read,
	.write = device_write,
	.release = device_release,
};


//START HERE
static int __init chardev_init(void){
	printk(KERN_ALERT "Adding dgistando Driver\n");
	
	//might have to unregister before I can register. Like its new
	//unregister_chrdev(13, DEVICE_NAME);	

	major = register_chrdev(0, DEVICE_NAME, &fops);
	if(major < 0){
		printk(KERN_ALERT"Device register failed. reason: %d\n", major);
		return major;
	}
	
	return SUCCESS;	
}


static void __exit chardev_exit(void){
//	int retVal;
	unregister_chrdev(major, DEVICE_NAME);
//	if(retVal)
//		printk(KERN_ALERT "Error unregistering device. might have to restart.\n");
}

void cleanup(void){

}

///////////////////////fill in callbacks///////////////////////

/**
 *  This is the first operation performed on the device file.
 *  The function that calls this one will always follow through
 *  even if this is NULL. Invoked when the user calls open
 *  
 *  @param inodeptr	Pointer to the inode associated with the device file
 *  @param fileptr	Pointer to the device file  
 */
static int device_open(struct inode *inodeptr, struct file *fileptr){
	printk(KERN_ALERT "The device was opened\n");
	return 0;
}

/**
 *  Called when the user closes the device.
 *
 *  @param inodeptr	Pointer to the inode associated with the device file
 *  @param fileptr	Pointer to the device file 
 */
static int device_release(struct inode *inodeptr, struct file *fileptr){
	printk(KERN_ALERT "Device was released\n");
	return 0;
}


/*

//Theese functions should be used
//They both just copy chunks of bytes.
//The memcpy of KernelC
// ref. Ch.3,pg64

copy_to_user(void __user *to, const void *from, ulint count)
copy_from_user(void *to, const void *from, ulint count )

*/

/**
 *  Used to read data from device. If NULL is unsed err -EINVAL
 *  is thrown and returned to the user.
 *
 *  @param fileptr	Pointer to he file
 *  @param buffer	The buffer you are writing to
 *  @param size		The size of the buffer
 *  @param offset	The offset
 *
 *  @return The number of bytes succefully read 
 */
static ssize_t device_read(struct file *fileptr, char __user *buffer, size_t size, loff_t *offset){
	
	long amountRead = 0;
	long resp;
	
	resp = copy_to_user(buffer, message, amountRead);

	if(resp){//nonzero value
		printk(KERN_ALERT "Couldnt send chars to the user\n");
		return resp;
	}else{
		printk(KERN_INFO "[%ld] characters were sent to the user\n", amountRead);
		return amountRead;
	}
}

/**
 *  Used to write data to device from user. For example Anytime a scanf()
 *  is called this is invoked.
 *
 *  @param fileptr	A pointer to the device file
 *  @param buffer	Holds the string that you will write to the device
 *  @param size		Size of the buffer being taken
 *  @param offset	The offset
 *
 *  @return The number of bytes succefully read 
 */
static ssize_t device_write(struct file *fileptr, const char __user *buffer, size_t size, loff_t *offset){
	//Not sure if this will work, or should
	printk(KERN_ALERT "Device Write \n");

	return 0;
}


//lets the kernel know where to start
module_init(chardev_init);
module_exit(chardev_exit);
