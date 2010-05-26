#include <linux/module.h>      // for all modules 
#include <linux/init.h>        // for entry/exit macros 
#include <linux/kernel.h>      // for printk priority macros 
#include <asm/current.h>       // process information, just for fun 
#include <linux/sched.h>       // for "struct task_struct" 
#include <linux/ioport.h>       // for "struct task_struct" 
#include <asm/io.h>       // for "struct task_struct" 

#define LEGO_9771_IO_PORT 925

static struct resource* lego_port = NULL;

static int lego9771_init(void)
{
	int cur;
	printk(KERN_INFO "lego9771 module being loaded.\n");
	printk(KERN_INFO "User space process is '%s'\n", current->comm);
	lego_port = request_region(LEGO_9771_IO_PORT, 1, "Lego9771");
	if (!lego_port)
	{
		printk(KERN_WARNING "Failed to acquire Lego 9771 port region.\n");
		return 1;
	}
	cur = inb(LEGO_9771_IO_PORT);
	printk(KERN_INFO "User space PID is  %i\n", current->pid);
	printk(KERN_INFO "Lego state is  %i\n", cur);
	outb_p(0xCC, LEGO_9771_IO_PORT);
	return 0;
}  

static void lego9771_exit(void) 
{
	release_region(LEGO_9771_IO_PORT, 1);
	printk(KERN_INFO "lego9771 module being unloaded.\n"); 
}  

module_init(lego9771_init);
module_exit(lego9771_exit);

MODULE_AUTHOR("Phil Jordan"); 
MODULE_LICENSE("Dual BSD/GPL"); 
MODULE_DESCRIPTION("Lego DACTA 9771 ISA Interface Card.");
