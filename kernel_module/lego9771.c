#include <linux/module.h>      // for all modules 
#include <linux/init.h>        // for entry/exit macros 
#include <linux/kernel.h>      // for printk priority macros 
#include <asm/current.h>       // process information, just for fun 
#include <linux/sched.h>       // for "struct task_struct" 

static int lego9771_init(void)
{
	printk(KERN_INFO "lego9771 module being loaded.\n");
	printk(KERN_INFO "User space process is '%s'\n", current->comm);
	printk(KERN_INFO "User space PID is  %i\n", current->pid);
	return 0;
}  

static void lego9771_exit(void) 
{
	printk(KERN_INFO "lego9771 module being unloaded.\n"); 
}  

module_init(lego9771_init);
module_exit(lego9771_exit);

MODULE_AUTHOR("Phil Jordan"); 
MODULE_LICENSE("Dual BSD/GPL"); 
MODULE_DESCRIPTION("Lego DACTA 9771 ISA.");
