#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kprobes.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/kallsyms.h>
#include <linux/version.h>

#if LINUX_VERSION_CODE > KERNEL_VERSION(3, 14, 0)
  #define  KP_OFFSET    0x204;
#elif LINUX_VERSION_CODE > KERNEL_VERSION(2, 6, 39)
  #define  KP_OFFSET    0xcf;
#else
  #define  KP_OFFSET    0x1ca;
#endif

static unsigned long load5s;
 
/***************************************
 *             kprobe part             * 
 * *************************************/

static struct kprobe kp = {};
 
static int handler_pre(struct kprobe *p, struct pt_regs *regs)
{
    atomic_long_t *a = (atomic_long_t *)(long)kallsyms_lookup_name("calc_load_tasks");                  // 0xffffffff81dc4fa8;
    load5s = atomic_long_read(a);

    return 0;
}
 
static void handler_post(struct kprobe *p, struct pt_regs *regs, unsigned long flags)
{
    //printk(KERN_INFO "post_handler: p->addr = 0x%p, flags = 0x%lx\n", p->addr, regs->flags);
}
 
static int handler_fault(struct kprobe *p, struct pt_regs *regs, int trapnr)
{
    printk(KERN_INFO "fault_handler: p->addr = 0x%p, trap #%dn", p->addr, trapnr);
    return 0;
}


/***************************************
 *              proc part              * 
 * *************************************/

static int hello_proc_show(struct seq_file *m, void *v) {
  seq_printf(m, "%ld\n",load5s);
  return 0;
}

static int load5s_proc_open(struct inode *inode, struct  file *file) {
  return single_open(file, hello_proc_show, NULL);
}

static const struct file_operations load5s_proc_fops = {
  .owner = THIS_MODULE,
  .open = load5s_proc_open,
  .read = seq_read,
  .llseek = seq_lseek,
  .release = single_release,
};

/***************************************
 *  module init && module exit         * 
 * *************************************/

static int __init load5s_init(void)
{
    int ret;
    proc_create("load5s", 0, NULL, &load5s_proc_fops);
    
    kp.pre_handler   = handler_pre;
    kp.post_handler  = handler_post;
    kp.fault_handler = handler_fault;
    kp.symbol_name   = "calc_global_load";
    kp.offset        = KP_OFFSET;

    ret = register_kprobe(&kp);
    if (ret < 0) {
        printk(KERN_INFO "register_kprobe failed, returned %d\n", ret);
        return ret;
    }
    printk(KERN_INFO "Planted kprobe at %p\n", kp.addr);
    

    return 0;
}
 
static void __exit load5s_exit(void)
{
    remove_proc_entry("load5s", NULL);
    unregister_kprobe(&kp);
    printk(KERN_INFO "kprobe at %p unregistered\n", kp.addr);
}
 
module_init(load5s_init)
module_exit(load5s_exit)
MODULE_LICENSE("GPL");
