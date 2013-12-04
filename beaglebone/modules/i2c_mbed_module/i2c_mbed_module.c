#include <linux/init.h>
#include <linux/module.h>
#include <linux/i2c.h>


static const struct i2c_device_id mbed_id[] = {
	{ "mbed", 0 },
	{ }
};
MODULE_DEVICE_TABLE(i2c, mbed_id);

#ifdef CONFIG_OF
static const struct of_device_id mbed_dt_ids[] = {
	{ .compatible = "bioboost,mbed", },
	{ }
};
MODULE_DEVICE_TABLE(of, mbed_dt_ids);
#endif


/* (1) Initializes the device and registers it with the appropriate kernel framework
	struct i2c_client *client: represents the i2c device itself
	struct i2c_device_id *id: device ID entry that matched the device probed
*/
static int mbed_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	/* Initialize the device */
	/* Register to a kernel framework */

	pr_alert("mbed: probe function called");

	return 0;
}

/* (2) Responsible for unregistering from the kernel framework and to shut it down
	struct i2c_client *client: represents the i2c device itself
*/
static int mbed_remove(struct i2c_client *client)
{
	/* Unregister the device from the kernel framework */
	/* Shut down the device */

	pr_alert("mbed: remove function called");

	return 0;
}

/* (3) struct i2c_driver - represent an I2C device driver
Tells the kernel what functions to map on device driver actions */
static struct i2c_driver mbed_driver = {
	.probe = mbed_probe,
	.remove = mbed_remove,
	.id_table = mbed_id,
	.driver = {
		.name = "mbed",
		.owner = THIS_MODULE,
		.of_match_table = of_match_ptr(mbed_dt_ids),
	},
};


module_i2c_driver(mbed_driver);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple i2c driver for mbed");
MODULE_AUTHOR("Nico De Witte");
