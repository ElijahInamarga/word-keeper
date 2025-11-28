#include "linux/err.h"
#include <linux/device.h>
#include <linux/kdev_t.h>
#include <linux/init.h>
#include <linux/module.h>

static struct class *my_classp;
static struct device *my_devicep;
static dev_t dev_num; // unsigned int
static int dev_minor_count = 1; // number of devices under class

static __init int my_init(void)
{
  // Allocate major and minor number for device found in /proc/devices
  int alloc_result = alloc_chrdev_region(&dev_num, 0, dev_minor_count, "word_keeper"); 
  if (alloc_result < 0)
  {
    pr_warn("word_keeper - fail to allocate major/minor number with error %d\n", alloc_result);
    return alloc_result;
  }

  // Create class found in /sys/class
  my_classp = class_create("word_keeper_class"); 
  if (IS_ERR(my_classp))
  {
    int result = PTR_ERR(my_classp);
    pr_warn("word_keeper - failed to create class with error %d\n", result);
    return result;
  }

  // Create device struct and device file found in /dev
  my_devicep = device_create(my_classp, NULL, dev_num, NULL, "word_keeper"); 
  if (IS_ERR(my_devicep))
  {
    int result = PTR_ERR(my_devicep);
    pr_warn("word_keeper - failed to create device with error %d\n", result);
    return result;
  }

  pr_info("word_keeper - Module successfully loaded\n");
  return 0;
}

static void __exit my_exit(void)
{

  pr_info("word_keeper - Module unloaded\n");
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Elijah Inamarga");
MODULE_DESCRIPTION("A driver that reads and writes to a device");
