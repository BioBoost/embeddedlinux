#include <linux/init.h>
#include <linux/module.h>
#include <linux/i2c.h>
#include <linux/delay.h>
#include <linux/kernel.h>


int ret;				// Used to hold return values of functions; Safe on kernel stack space !


// name -> appears in /proc/devices
#define DEVICE_NAME		"nunchuck"

static const struct i2c_device_id nunchuck_id[] = {
	{ "nunchuck", 0 },
	{ }
};
MODULE_DEVICE_TABLE(i2c, nunchuck_id);

#ifdef CONFIG_OF
static const struct of_device_id nunchuck_dt_ids[] = {
	{ .compatible = "nintendo,nunchuck", },
	{ }
};
MODULE_DEVICE_TABLE(of, nunchuck_dt_ids);
#endif

/*
	Initializes the device and registers it with the appropriate kernel framework
	struct i2c_client *client: represents the i2c device itself
	struct i2c_device_id *id: device ID entry that matched the device probed
*/
static int nunchuck_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	pr_alert("nunchuck: probe function called\n");

	return 0;
}

/*
	Responsible for unregistering from the kernel framework and to shut it down
	struct i2c_client *client: represents the i2c device itself
*/
static int nunchuck_remove(struct i2c_client *client)
{
	pr_alert("nunchuck: remove function called\n");

	return 0;
}

/*
	struct i2c_driver - represents an I2C device driver
	Tells the kernel what functions to map on device driver actions
*/
static struct i2c_driver nunchuck_driver = {
	.probe = nunchuck_probe,
	.remove = nunchuck_remove,
	.id_table = nunchuck_id,
	.driver = {
		.name = "nunchuck",
		.owner = THIS_MODULE,
		.of_match_table = of_match_ptr(nunchuck_dt_ids),
	},
};

module_i2c_driver(nunchuck_driver);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple i2c driver for nintendo nunchuck");
MODULE_AUTHOR("Nico De Witte");
