#include "linux/err.h"
#include "linux/fs.h"
#include <linux/device.h>
#include <linux/kdev_t.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/cdev.h>

static struct class *my_classp;
static struct device *my_devicep;
static dev_t dev_num; // unsigned int
static int dev_minor_count = 1; // number of devices under class
static struct cdev my_cdev;
static struct file_operations fops = {};

static __init int my_init(void)
{
  int result;
  // Allocate major and minor number for device found in /proc/devices
  result = alloc_chrdev_region(&dev_num, 0, dev_minor_count, "word_keeper"); 
  if (result < 0)
  {
    pr_warn("word_keeper - fail to allocate major/minor number with error %d\n", result);
    goto fail_alloc;
  }

  // Create class found in /sys/class
  my_classp = class_create("word_keeper_class"); 
  if (IS_ERR(my_classp))
  {
    result = PTR_ERR(my_classp);
    pr_warn("word_keeper - failed to create class with error %d\n", result);
    goto fail_class_create;
  }

  // Bind class and major/minor numbers
  my_devicep = device_create(my_classp, NULL, dev_num, NULL, "word_keeper"); 
  if (IS_ERR(my_devicep))
  {
    result = PTR_ERR(my_devicep);
    pr_warn("word_keeper - failed to create device with error %d\n", result);
    goto fail_dev_create;
  }
  
  // Bind fops and major/minor numbers
  cdev_init(&my_cdev, &fops);
  result = cdev_add(&my_cdev, dev_num, dev_minor_count);
  if (result < 0)
  {
    pr_warn("word_keeper - failed to add device with error %d\n", result);
    goto fail_cdev_add;
  }

  pr_info("word_keeper - Module successfully loaded\n");
  return 0;

fail_cdev_add:
  device_destroy(my_classp, dev_num);

fail_dev_create:
  class_destroy(my_classp);

fail_class_create:
  unregister_chrdev_region(dev_num, 1);

fail_alloc:
  return result;
}

static void __exit my_exit(void)
{
  cdev_del(&my_cdev);
  device_destroy(my_classp, dev_num);
  class_destroy(my_classp);
  unregister_chrdev_region(dev_num, 1);
  pr_info("word_keeper - Module unloaded\n");
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Elijah Inamarga");
MODULE_DESCRIPTION("A driver that reads and writes to a device");
