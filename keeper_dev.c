#include "keeper_dev.h"
#include <linux/kernel.h>

ssize_t my_read(struct file *filp, char __user *user_buf, size_t len,
                loff_t *offset)
{
  printk("Reading works!\n");
  return 0;
}
