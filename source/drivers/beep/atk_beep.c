/*
 * support code for ATK Beep
 * Copyright 2024 wuyue. Subject to GPLv2.
 * 
 * 3Q to 
 * source/linux/linux-imx-rel_imx_4.1.15_2.1.0_ga/drivers/watchdog/ath79_wdt.c
 * source/linux/linux-imx-rel_imx_4.1.15_2.1.0_ga/drivers/mxc/sim/imx_sim.c
 * source/linux/linux-imx-rel_imx_4.1.15_2.1.0_ga/drivers/input/touchscreen/w90p910_ts.c
 * 
 * include:
 * misc
 * timer
 * beep(gpio)
 * lock
 * 
 */
#include <linux/of_gpio.h>
#include <linux/poll.h>
#include <linux/miscdevice.h>
#include <linux/platform_device.h>
#include <linux/spinlock.h>


#define DRIVER_NAME	"atkeep"

struct misc_beep
{
	int beep_gpio;
	spinlock_t lock;
	struct timer_list timer;
};
static struct miscdevice atk_beep_miscdev;

static int miscbeep_open(struct inode *inode, struct file *filp)
{
	struct misc_beep *atk_beep = dev_get_drvdata(atk_beep_miscdev.parent);
	if (atk_beep == NULL) {
		pr_err("Failed to get atk_beep pointer\n");
		return -EINVAL;
	}

	pr_info("%s : get memy %p\n", __func__, atk_beep);
	filp->private_data = atk_beep;

	spin_lock_init(&atk_beep->lock);

	return 0;
}

static void atk_beep_start(unsigned long data)
{
	struct misc_beep *misc_beep = (struct misc_beep *) data;

	pr_info("%s : beep start!\n", __func__);
	gpio_set_value(misc_beep->beep_gpio, 0);
}

static ssize_t miscbeep_write(struct file *filp, const char __user *buf, size_t cnt, loff_t *offt)
{
	unsigned long flags;
	unsigned char databuf;
	int rNum;

	struct misc_beep *atk_beep  = (struct misc_beep *)filp->private_data;
	pr_info("%s : get memy %p\n", __func__, atk_beep);

	rNum = copy_from_user(&databuf, buf, 1);
	spin_lock_irqsave(&atk_beep->lock, flags);
	if (rNum >= 0 && databuf != 0)
	{
		mod_timer(&atk_beep->timer, jiffies + msecs_to_jiffies(10));
	}else
	{
		gpio_set_value(atk_beep->beep_gpio, 1);
	}
	spin_unlock_irqrestore(&atk_beep->lock, flags);
	return 0;
}

static const struct file_operations atk_beep_fops = {
	.owner		= THIS_MODULE,
	.open = miscbeep_open,
	.write = miscbeep_write,
};

static struct miscdevice atk_beep_miscdev = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = DRIVER_NAME,
	.fops = &atk_beep_fops,
};

static int atk_beep_probe(struct platform_device *pdev)
{
	int err = 0;
	struct misc_beep *atk_beep = NULL;
	struct device_node *of_node = pdev->dev.of_node;

	pr_info("%s : enter probe\n", __func__);

	atk_beep = devm_kzalloc(&pdev->dev, sizeof(struct misc_beep),
				GFP_KERNEL);
	if (!atk_beep) {
		dev_err(&pdev->dev, "can't allocate enough memory\n");
		return -ENOMEM;
	}

	atk_beep->beep_gpio = of_get_named_gpio(of_node, "beep-gpio", 0);
	if(!gpio_is_valid(atk_beep->beep_gpio)) {
		printk("miscbeep：Failed to get beep-gpio\n");
		return -EINVAL;
	}
	
	err = gpio_request(atk_beep->beep_gpio, "beep");
	if(err) {
		printk("beep: Failed to request beep-gpio\n");
		return err;
	}else{
		pr_info("%s : GPIO %d get!\n", __func__, atk_beep->beep_gpio);
	}
	gpio_direction_output(atk_beep->beep_gpio, 1);
	// gpio_set_value(atk_beep->beep_gpio, 0);

	err = misc_register(&atk_beep_miscdev);
	if (err) {
		dev_err(&pdev->dev,
			"unable to register misc device, err=%d\n", err);
		goto err_beep_disable;
	}

	// init_timer(&atk_beep->timer);
	// atk_beep->timer.function = mg_times_out;
	// atk_beep->timer.data = (unsigned long)atk_beep;
	setup_timer(&atk_beep->timer, atk_beep_start,
		    (unsigned long)atk_beep);


	platform_set_drvdata(pdev, atk_beep);
	atk_beep_miscdev.parent = &(pdev->dev);

	return 0;

err_beep_disable:
	gpio_free(atk_beep->beep_gpio);
	return err;
}

static int atk_beep_remove(struct platform_device *pdev)
{
	struct misc_beep *atk_beep = platform_get_drvdata(pdev);

	gpio_set_value(atk_beep->beep_gpio, 1);
	gpio_free(atk_beep->beep_gpio);
	del_timer_sync(&atk_beep->timer);

	misc_deregister(&atk_beep_miscdev);

	return 0;
}


static void atk_beep_shutdown(struct platform_device *pdev)
{
	pr_info("%s : See You Again\n", __func__);
}

static const struct of_device_id atk_beep_match[] = {
	{ .compatible = "imx6ul-beep" },
	{},
};

static struct platform_driver atk_beep_driver = {
	.probe		= atk_beep_probe,
	.remove		= atk_beep_remove,
	.shutdown	= atk_beep_shutdown,
	.driver		= {
		.name	= DRIVER_NAME,
		.of_match_table = of_match_ptr(atk_beep_match),
	},
};

module_platform_driver(atk_beep_driver);

MODULE_DESCRIPTION("ATK Beep driver");
MODULE_AUTHOR("wuyue");
MODULE_LICENSE("GPL v2");
MODULE_ALIAS("platform:" DRIVER_NAME);
