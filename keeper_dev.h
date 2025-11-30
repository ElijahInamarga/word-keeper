#ifndef KEEPER_DEV_H
#define KEEPER_DEV_H

#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>

#define BUFF_SIZE 64

struct dev_data
{
  struct cdev cdev;
  char buf[BUFF_SIZE];
};

int my_open(struct inode *inodep, struct file *filep);
ssize_t my_read(struct file *filep, char __user *user_bufp, size_t len,
                loff_t *offsetp);

#endif // KEEPER_DEV_H
