#ifndef KEEPER_DEV_H
#define KEEPER_DEV_H

#include <linux/types.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/fs.h>

#define BUFF_SIZE 64

struct dev_data {
    struct cdev cdev;
    char        buf[BUFF_SIZE];
};

int     my_open(struct inode *inodep, struct file *filep);
int     my_release(struct inode *inodep, struct file *filep);
ssize_t my_read(struct file *filep, char __user *user_bufp, size_t len,
                loff_t *offsetp);
ssize_t my_write(struct file *filep, const char __user *user_bufp, size_t len,
                 loff_t *offsetp);

#endif // KEEPER_DEV_H
