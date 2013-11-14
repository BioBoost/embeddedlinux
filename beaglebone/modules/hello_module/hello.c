/*
 hello.c
 Basic test of a module for the beaglebone
*/

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/time.h>

/* Used for tracking time the module ran */
struct timeval start_tv;
struct timeval end_tv;

static int __init hello_init(void)
{
	pr_info("hello: Good morning mister kernel man.");
	do_gettimeofday(&start_tv);
	return 0;
}

static void __exit hello_exit(void)
{
	do_gettimeofday(&end_tv);
	pr_info("hello: Outta here. Module ran for %lld seconds",
		((long long)end_tv.tv_sec - (long long)start_tv.tv_sec));
}

module_init(hello_init);
module_exit(hello_exit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Greets the kernel hello");
MODULE_AUTHOR("We all are.");
