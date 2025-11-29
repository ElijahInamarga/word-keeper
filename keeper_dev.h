#ifndef KEEPER_DEV_H
#define KEEPER_DEV_H

#include <linux/fs.h>

ssize_t my_read(struct file *filp, char *user_buf, size_t len, loff_t *offset);

#endif // !KEEPER_DEV_H
