#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/kprobes.h>

static long j_sys_mkdir(const char __user *pathname, umode_t mode)
{
    long ret = 0;
    ret = jprobe_return();
    printk(KERN_INFO "mkdir system call is called for path: %s", pathname);
    return ret;
}

static struct jprobe jp = {
    .entry = j_sys_mkdir,
    .kp.symbol_name = "sys_mkdir"
};

static int __init hook_init(void)
{
    int ret;
    ret = register_jprobe(&jp);
    if (ret < 0) {
        printk(KERN_INFO "register_jprobe failed, returned %d\n", ret);
        return ret;
    }
    printk(KERN_INFO "Planted jprobe at %p, handler addr %p\n", jp.kp.addr, jp.entry);
    return 0;
}

static void __exit hook_exit(void)
{
    unregister_jprobe(&jp);
    printk(KERN_INFO "jprobe unregistered\n");
}

module_init(hook_init)
module_exit(hook_exit)
MODULE_LICENSE("GPL");
