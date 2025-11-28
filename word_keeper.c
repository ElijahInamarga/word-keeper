#include <linux/init.h>
#include <linux/module.h>

static __init int init(void)
{
  return 0;
}

static void __exit exit(void)
{

}

module_init(init);
module_exit(exit)

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Elijah Inamarga");
MODULE_DESCRIPTION("A driver that reads and writes to a device");
