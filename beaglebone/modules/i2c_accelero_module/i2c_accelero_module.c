#include <linux/init.h>
#include <linux/module.h>
#include <linux/i2c.h>


static const struct i2c_device_id accelero_id[] = {
	{ "accelero", 0 },
	{ }
};
MODULE_DEVICE_TABLE(i2c, accelero_id);

#ifdef CONFIG_OF
static const struct of_device_id accelero_dt_ids[] = {
	{ .compatible = "bioboost,accelero", },
	{ }
};
MODULE_DEVICE_TABLE(of, accelero_dt_ids);
#endif


/* (1) Initializes the device and registers it with
the appropriate kernel framework
	struct i2c_client *client: represents the i2c device itself
	struct i2c_device_id *id: device ID entry that matched the device probed
*/
static int accelero_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	/* Initialize the device */
	/* Register to a kernel framework */

	pr_alert("accelero: probe function called");

	return 0;
}

/* (2) Responsible for unregistering from the kernel framework
and to shut it down
	struct i2c_client *client: represents the i2c device itself
*/
static int accelero_remove(struct i2c_client *client)
{
	/* Unregister the device from the kernel framework */
	/* Shut down the device */

	pr_alert("accelero: remove function called");

	return 0;
}

/* (3) struct i2c_driver - represent an I2C device driver
Tells the kernel what functions to map on device driver actions */
static struct i2c_driver accelero_driver = {
	.probe = accelero_probe,
	.remove = accelero_remove,
	.id_table = accelero_id,
	.driver = {
		.name = "accelero",
		.owner = THIS_MODULE,
		.of_match_table = of_match_ptr(accelero_dt_ids),
	},
};


module_i2c_driver(accelero_driver);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple i2c driver for accelero meter");
MODULE_AUTHOR("Nico De Witte");
