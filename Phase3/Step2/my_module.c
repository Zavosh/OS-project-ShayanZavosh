#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/delay.h>

struct task_struct *thread_st;
int task_page_num[10000];

int thread_func(void* unused){
	struct task_struct * task;//PCB of each process
	for_each_process(task){//loop over all processes
		if (task->mm != NULL)//check for having virtual memory
		{
			task_page_num[(int)task->pid] = (int)task->mm->total_vm;//initialization of page number array
			printk(KERN_INFO "Memory of process %d is %d page(s).\n", task->pid, (int) task->mm->total_vm); //printing page numbers in dmesg
		}
	}
	while (!kthread_should_stop())
	{
		//printk(KERN_INFO "YES");
		for_each_process(task){ //loop over all processes
			if (task->mm != NULL)
			{
				//printk(KERN_INFO "pid %d\n is not NULL!", task->pid);
				if (task->mm->total_vm != task_page_num[(int)task->pid]) {//change in page nums
					printk(KERN_INFO "Memory of process %d is %d page(s) changed.\n", task->pid, (int)task->mm->total_vm - task_page_num[(int)task->pid]);//print the change
					task_page_num[(int)task->pid] = (int)task->mm->total_vm; //update new page num
				}	
			} else if (task_page_num[(int)task->pid] != 0) { //removed task
				printk(KERN_INFO "Memory of process %d is %d page(s) changed!", task->pid, - task_page_num[(int)task->pid]);//printing change in page num
				task_page_num[(int)task->pid] = 0; //update new page num
			}
		}
		ssleep(1);//sleep for a second
	}
	return 0;
}
//init module
int __init init_thread(void){
	printk(KERN_INFO "Creating Thread\n");
	thread_st = kthread_run(thread_func, NULL, "mythread");//running a new kernel thread called mythread!
	if (thread_st)
		printk(KERN_INFO "Thread Created Successfully\n");
	else
		printk(KERN_ERR "Thread Creation failed\n");
	return 0;
}
//exit module
void __exit cleanup_thread(void){
	printk(KERN_INFO "Cleaning up\n");
	if (thread_st){
		kthread_stop(thread_st);//stopping thread by setting a flag
		printk(KERN_INFO "Thread stopped");
	}
}

MODULE_LICENSE("sh");
module_init(init_thread);
module_exit(cleanup_thread);

