#include<linux/module.h>
#include<linux/version.h>
#include<linux/kmod.h>
/* hello_init ---- 初始化函数，当模块装载时被调用，如果成功装载返回0 否则返回非0值 */
static int __init example_init(void)
{
    printk("I bear a charmed life.\n");
    return 0;
}
/* hello_exit ---- 退出函数，当模块卸载时被调用 */
static void __exit example_exit(void)
{
    printk("Out, out, brief candle\n");
}
module_init(example_init);
module_exit(example_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("hello world");

