#include "keeper_dev.h"
#include "asm-generic/errno-base.h"
#include "linux/container_of.h"
#include "linux/init.h"
#include "linux/uaccess.h"
#include <linux/kernel.h>

int my_open(struct inode *inodep, struct file *filep)
{
  struct dev_data *wk_datap = container_of(inodep->i_cdev, struct dev_data, cdev);

  filep->private_data = wk_datap;

  // Return error if module is unloaded or unloading
  if(!try_module_get(THIS_MODULE)) {
    return -ENODEV; // open unsuccessful
  }

  return 0;
}

int my_release(struct inode *inodep, struct file *filep)
{
  module_put(THIS_MODULE);
  return 0;
}

ssize_t my_read(struct file *filep, char __user *user_bufp, size_t len, loff_t *offsetp)
{
  struct dev_data *wk_datap = (struct dev_data *)filep->private_data;
  if(wk_datap == NULL) {
    return -ENODEV;
  }

  size_t buf_data_len = strnlen(wk_datap->buf, sizeof(wk_datap->buf));

  if(*offsetp >= buf_data_len) // end of file
  {
    return 0;
  }

  /*
   * Only read the amount of bytes stated by user
   * unless there aren't enough bytes to read
   */
  size_t bytes_unread = buf_data_len - *offsetp;
  size_t bytes_to_read = min(bytes_unread, len);

  if(copy_to_user(user_bufp, wk_datap->buf + *offsetp, bytes_to_read) != 0) {
    return -EFAULT;
  }

  *offsetp += bytes_to_read;

  return bytes_to_read;
}
