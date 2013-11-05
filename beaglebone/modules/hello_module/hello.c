/*
 hello.c
 Basic test of a module for the beaglebone
*/

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

static int __init hello_init(void)
{
	pr_alert("Good morning mister kernel man.\n");
	return 0;
}

static void __exit hello_exit(void)
{
	pr_alert("Outta here\n");
}

module_init(hello_init);
module_exit(hello_exit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Greets the kernel hello");
MODULE_AUTHOR("We all are.");


