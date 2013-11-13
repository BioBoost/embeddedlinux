#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>		// file_operations structure - open, read, write, close
#include <linux/cdev.h>		// This is char driver; makes cdev available
#include <linux/semaphore.h>	// synchronizations
#include <linux/uaccess.h>	// copy_to_user; copy_from_user

// (1) Create a structure for our fake device
struct fake_device
{
	char data[100];
	struct semaphore sem;
} virtual_device;

// (2) To later register our device we need a cdev instance and some other variables
struct cdev *mcdev;
int major_number;	// Extracted from dev_t using macro - mknod /dev/<file> c major minor
int ret;			// Used to hold return values of functions; Safe on kernel stack space !

dev_t dev_num;		// Will hold major number that the kernel gives us
					
// name -> appears in /proc/devices
#define DEVICE_NAME		"bbt_charmod"


// (7) Called on device_file open
//		inode: reference to the file on disk and contains info about that file
//		struct file: represents and abstract open file
int device_open(struct inode *inode, struct file *filep)
{
	// Only allow one process to open this device by using a semaphore as mutual exclusive lock - mutex
	if (down_interruptible(&virtual_device.sem) != 0)
	{
		printk(KERN_ALERT "charmod: could not lock device during open");
		return -1;
	}

	printk(KERN_INFO "charmod: opened device");
	return 0;
}

// (8) Called when the user wants to get information from the device
//	char* buffStoreData: destination in user space here to store the data
//	size_t buffCount: space available in buffStoreData
//	loff_t* curOffset: something about current open file offset (??)
ssize_t device_read(struct file* filep, char* buffStoreData, size_t buffCount, loff_t* curOffset)
{
	// Take data from kernel space (device) to user space (process accessing our device)
	// copy_to_user(destination, source, sizeToTransfer)

	printk(KERN_INFO "charmod: reading data from the device");
	ret = copy_to_user(buffStoreData, virtual_device.data, buffCount);
	return ret;
}

// (9) Called when the user want to write information to the device
// char* buffSourceData: source in userspace from where to get the data
// size_t buffCount: number of bytes to fetch and store in our device
//	loff_t* curOffset: something about current open file offset (??)
ssize_t device_write(struct file* filep, const char* buffSourceData, size_t buffCount, loff_t* currOffset)
{
	// Fetch data from user space to kernel space
	// copy_from_user(destination, source, sizeToTransfer)

	printk(KERN_INFO "charmod: writing to device");
	ret = copy_from_user(virtual_device.data, buffSourceData, buffCount);
	return ret;
}

// (10) Called when the user closes the device
int device_close(struct inode* inode, struct file* filep)
{
	// By calling up (opposite of down for semaphore) we release the mutex that we optained when
	// we opened the device. This has the effect of allowing another process to use the device now
	up(&virtual_device.sem);
	printk(KERN_INFO "charmod: closed device");
	return 0;
}

// (6) Tell the kernel which functions to call when the user operator on our device file
struct file_operations fops = {
	// List of initializers
	.owner = THIS_MODULE,		// prevent unloading of module when operations are in use
	.open = device_open,		// method to call when opening the device
	.release = device_close,	// method to call when closing the device
	.write = device_write,		// method to call when writing to the device
	.read = device_read			// method to call when reading from the device
};

static int driver_entry(void)
{
	// (3) Register our device with the system

	// Step 1 - Use dynamic allocation to assign our device
	//		a major number -- alloc_chrdev_region(dev_t*, uint fminor, uint count, char* name)

	ret = alloc_chrdev_region(&dev_num, 0, 1, DEVICE_NAME);		//dev_num will hold maj number after this call
	if (ret < 0)
	{
		printk(KERN_ALERT "charmod: failed to allocate a major number");
		return ret;		// propage error code
	}
	printk(KERN_INFO "charmod: module loaded");

	major_number = MAJOR(dev_num);		// Macro to extract the major number and store it in our var
	printk(KERN_INFO "charmod: major number is %d", major_number);
	printk(KERN_INFO "charmod: Use mknod /dev/%s c %d 0 for device file", DEVICE_NAME, major_number);

	// Step 2 - Allocate character device structure and initialize
	mcdev = cdev_alloc();			// Create our cdev structure, initialize our cdev structure
	mcdev->ops = &fops;				// Pass pointer of struct file_operations
	mcdev->owner = THIS_MODULE;

	// (4) Now that we create cdev, we have to add it to the kernel
	// int cdev_add(struct cdev* dev, dev_t num, unsigned int count)
	ret = cdev_add(mcdev, dev_num, 1);
	if (ret < 0)
	{
		printk(KERN_ALERT "charmod: unable to add cdev to kernel");
		return ret;
	}

	// Initialize semaphore
	sema_init(&virtual_device.sem, 1);	// initial value of 1

	return 0;
}

// (5) Does same actions as entry but release in reverse order
static void driver_exit(void)
{
	// Remove character device from the system
	cdev_del(mcdev);

	// Unregister character region
	unregister_chrdev_region(dev_num, 1);
	printk(KERN_ALERT "charmod: unloaded module");
}

// Inform the kernel where to start and stop with our module/driver
module_init(driver_entry);
module_exit(driver_exit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Fake character device driver");
MODULE_AUTHOR("Nico De Witte");