#include <linux/init.h>
#include <linux/module.h>
#include <linux/i2c.h>
#include <linux/delay.h>

int ret;			// Used to save return value (save kernel stack space)
char buffer[10];	// Simple buffer

/* Prototypes */
static int LM75B_read_temp(struct i2c_client *client);

static const struct i2c_device_id LM75B_id[] = {
	{ "LM75B", 0 },
	{ }
};
MODULE_DEVICE_TABLE(i2c, LM75B_id);

#ifdef CONFIG_OF
static const struct of_device_id LM75B_dt_ids[] = {
	{ .compatible = "bioboost,LM75B", },
	{ }
};
MODULE_DEVICE_TABLE(of, LM75B_dt_ids);
#endif


/* (1) Initializes the device and registers it with the appropriate kernel framework
	struct i2c_client *client: represents the i2c device itself
	struct i2c_device_id *id: device ID entry that matched the device probed
*/
static int LM75B_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	pr_alert("LM75B: probe function called\n");

	/* Initialize the device */

	/* Get the current temperature from the sensor */
	LM75B_read_temp(client);

	/* Register to a kernel framework */

	return 0;
}

/* (2) Responsible for unregistering from the kernel framework and to shut it down
	struct i2c_client *client: represents the i2c device itself
*/
static int LM75B_remove(struct i2c_client *client)
{
	/* Unregister the device from the kernel framework */
	/* Shut down the device */

	pr_alert("LM75B: remove function called\n");

	return 0;
}

/* (3) struct i2c_driver - represent an I2C device driver
Tells the kernel what functions to map on device driver actions */
static struct i2c_driver LM75B_driver = {
	.probe = LM75B_probe,
	.remove = LM75B_remove,
	.id_table = LM75B_id,
	.driver = {
		.name = "LM75B",
		.owner = THIS_MODULE,
		.of_match_table = of_match_ptr(LM75B_dt_ids),
	},
};

static int LM75B_read_temp(struct i2c_client *client)
{
	int degrees;

	/* Set the pointer to read the temp of the device */

	/* Read the two data bytes from the device */
	ret = i2c_master_recv(client, buffer, 2);
	if (ret != 2) {
		pr_alert("LM75B: unable to read temp\n");
		goto out;
	}

	degrees = buffer[0];	/* MSB */
	pr_alert("LM75B: current temperature is %d degrees\n", degrees);

out:
	return 0;
}

module_i2c_driver(LM75B_driver);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple i2c driver for LM75B");
MODULE_AUTHOR("Nico De Witte");
