#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/err.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kdev_t.h>
#include <linux/module.h>

#include "keeper_dev.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Elijah Inamarga");
MODULE_DESCRIPTION("A driver that reads and writes to a device");

static const int                    dev_minor_count = 1; // number of devices under class
static const struct file_operations fops = {.owner = THIS_MODULE,
                                            .open = my_open,
                                            .release = my_release,
                                            .read = my_read};

static dev_t          dev_num; // major and minor numbers
static struct class  *my_classp;
static struct device *my_devicep;
struct dev_data      *wk_datap;

static __init int my_init(void)
{
  int result;

  // Allocate major and minor number for device in /proc/devices
  result = alloc_chrdev_region(&dev_num, 0, dev_minor_count, "word_keeper");
  if(result < 0) {
    pr_warn("word_keeper - fail to allocate major/minor number with error %d\n", result);
    goto fail_alloc;
  }

  // Create class in /sys/class
  my_classp = class_create("word_keeper_class");
  if(IS_ERR(my_classp)) {
    result = PTR_ERR(my_classp);
    pr_warn("word_keeper - failed to create class with error %d\n", result);
    goto fail_class_create;
  }

  /*
   * Create device struct -- representation of actual device
   * Add device struct to a kernel internal registry -- binded to major index
   * Create and bind inert device file in /dev to major/minor
   */
  my_devicep = device_create(my_classp, NULL, dev_num, NULL, "word_keeper");
  if(IS_ERR(my_devicep)) {
    result = PTR_ERR(my_devicep);
    pr_warn("word_keeper - failed to create device with error %d\n", result);
    goto fail_dev_create;
  }

  // Allocate space to dev data
  wk_datap = kzalloc(sizeof(struct dev_data), GFP_KERNEL);
  if(wk_datap == NULL) {
    result = -ENOMEM;
    goto fail_kzalloc;
  }

  const char initial_str[] = "Hello, world!\n";
  strncpy(wk_datap->buf, initial_str, min(sizeof(initial_str), sizeof(wk_datap->buf) - 1));

  wk_datap->buf[sizeof(wk_datap->buf) - 1] = '\0';

  // Attach dev data to device
  dev_set_drvdata(my_devicep, wk_datap);

  /*
   * Initialize cdev struct -- used to connect fops to device file using majors
   * Add cdev struct to a kernel internal registry -- binded to major index
   */
  cdev_init(&wk_datap->cdev, &fops);
  result = cdev_add(&wk_datap->cdev, dev_num, dev_minor_count);
  if(result < 0) {
    pr_warn("word_keeper - failed to add device with error %d\n", result);
    goto fail_cdev_add;
  }

  pr_info("word_keeper - Module successfully loaded\n");
  return 0;

fail_cdev_add:
  kfree(wk_datap);

fail_kzalloc:
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
  cdev_del(&wk_datap->cdev);
  device_destroy(my_classp, dev_num);
  class_destroy(my_classp);
  unregister_chrdev_region(dev_num, 1);

  pr_info("word_keeper - Module unloaded\n");
}

module_init(my_init);
module_exit(my_exit);
