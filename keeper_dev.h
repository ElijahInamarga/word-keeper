#ifndef KEEPER_DEV_H
#define KEEPER_DEV_H

#include <linux/fs.h>

struct dev_data
{
  char buf[64];
};

ssize_t my_read(struct file *filp, char __user *user_buf, size_t len, loff_t *offset);

#endif // !KEEPER_DEV_H
