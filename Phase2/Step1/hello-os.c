#include <linux/kernel.h> // Needed for Macro KERN_INFO
#include <linux/module.h> // Needed by all modules

//inital module definiton and implementation
//This function will be called when module is added to kernel
int init_module(void){
	printk(KERN_INFO "Hello OS\n"); // printing Hello OS in dmesg using printk
	return 0;
}

//clean up module definiton and implementation
//This function will be called when module is removed from kernel
void cleanup_module(void){
	printk(KERN_INFO "Goodbye OS\n"); // printing Goodbye OS in dmesg using printk
}

