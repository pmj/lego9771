#include <linux/module.h>      // for all modules 
#include <linux/init.h>        // for entry/exit macros 
#include <linux/kernel.h>      // for printk priority macros 
#include <asm/current.h>       // process information, just for fun 
#include <linux/sched.h>       // for "struct task_struct" 
#include <linux/ioport.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <asm/io.h>
#include <asm/uaccess.h>

#define LEGO_9771_IO_PORT 925

static struct resource* lego9771_port_res = NULL;
static dev_t lego9771_dev;
static struct cdev lego9771_cdev;

static ssize_t lego9771_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos);
static ssize_t lego9771_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos);

static struct file_operations lego9771_fops = {
	.owner	 = THIS_MODULE,
	.read	 = lego9771_read,
	.write	 = lego9771_write
};


static int lego9771_init(void)
{
	int cur, result;
	lego9771_port_res = request_region(LEGO_9771_IO_PORT, 1, "Lego9771");
	if (!lego9771_port_res)
	{
		printk(KERN_INFO "Failed to acquire Lego 9771 port region.\n");
		return -ENODEV;
	}
	cur = inb(LEGO_9771_IO_PORT);
	printk(KERN_INFO "lego9771 module loaded, port acquired. Current state: %i\n", cur);
	
	result = alloc_chrdev_region(&lego9771_dev, 0, 1, "lego9771");
	printk(KERN_INFO "lego9771 dev major/minor: %i/%i, result:%i\n", MAJOR(lego9771_dev), MINOR(lego9771_dev), result);
	
	cdev_init(&lego9771_cdev, &lego9771_fops);
	lego9771_cdev.owner = THIS_MODULE;
	result = cdev_add(&lego9771_cdev, lego9771_dev, 1);
	printk(KERN_INFO "lego9771 cdev_add result:%i\n", result);
	return 0;
}  

static void lego9771_exit(void) 
{
	cdev_del(&lego9771_cdev);
	release_region(LEGO_9771_IO_PORT, 1);
	printk(KERN_INFO "lego9771 module being unloaded.\n"); 
}  

static ssize_t lego9771_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
	unsigned char *kbuf = kmalloc(count, GFP_KERNEL), *ptr;
	ssize_t retval = count;
	if (!kbuf)
		return -ENOMEM;
	ptr = kbuf;

	while (count--) {
		*(ptr++) = inb(LEGO_9771_IO_PORT);
		rmb();
	}
	if (copy_to_user(buf, kbuf, retval))
		retval = -EFAULT;
	kfree(kbuf);
	return retval;
}

static ssize_t lego9771_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos)
{
	unsigned char *kbuf = kmalloc(count, GFP_KERNEL), *ptr;
	ssize_t retval = count;
	if (!kbuf)
		return -ENOMEM;
	if (copy_from_user(kbuf, buf, count))
	{
		retval = -EFAULT;
	}
	else
	{
		ptr = kbuf;
		while (count--) {
			outb(*(ptr++), LEGO_9771_IO_PORT);
			wmb();
		}
	}
	kfree(kbuf);
	return retval;
}


module_init(lego9771_init);
module_exit(lego9771_exit);

MODULE_AUTHOR("Phil Jordan"); 
MODULE_LICENSE("Dual BSD/GPL"); 
MODULE_DESCRIPTION("Lego DACTA 9771 ISA Interface Card.");
