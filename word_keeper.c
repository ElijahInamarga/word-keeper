#include <linux/init.h>
#include <linux/module.h>

static __init int my_init(void)
{
  pr_info("word_keeper - Module loaded\n");
  return 0;
}

static void __exit my_exit(void)
{

  pr_info("word_keeper - Module unloaded\n");
}

module_init(my_init);
module_exit(my_exit)

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Elijah Inamarga");
MODULE_DESCRIPTION("A driver that reads and writes to a device");
