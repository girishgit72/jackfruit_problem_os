#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/mm.h>
#include <linux/sched/signal.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/rcupdate.h>

#define DEVICE_NAME "container_monitor"
#define SET_MEM_LIMIT _IOW('a', 'a', int *)

static int major;
static int mem_limit = 100; // MB
static struct task_struct *monitor_thread;

// IOCTL
static long device_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
if (cmd == SET_MEM_LIMIT) {
copy_from_user(&mem_limit, (int *)arg, sizeof(mem_limit));
printk("🔥 Memory limit set to %d MB\n", mem_limit);
}
return 0;
}

static int device_open(struct inode *inode, struct file *file)
{
printk("Monitor: Device opened\n");
return 0;
}

static int device_release(struct inode *inode, struct file *file)
{
printk("Monitor: Device closed\n");
return 0;
}

static struct file_operations fops = {
.unlocked_ioctl = device_ioctl,
.open = device_open,
.release = device_release,
};

// SAFE MONITOR (NO KILL)
static int monitor_fn(void *data)
{
while (!kthread_should_stop()) {

struct task_struct *task;

rcu_read_lock();

for_each_process(task) {

if (!task->mm)
continue;

if (task->pid < 1000)
continue;

unsigned long rss = get_mm_rss(task->mm) << (PAGE_SHIFT - 10); // KB

if (rss > mem_limit * 1024) {
printk("⚠️ Memory exceeded by PID %d (%lu KB)\n", task->pid, rss);
}
}

rcu_read_unlock();

msleep(2000);
}

return 0;
}

// INIT
static int __init monitor_init(void)
{
major = register_chrdev(0, DEVICE_NAME, &fops);
printk("🔥 Memory Monitor Loaded, major = %d\n", major);

monitor_thread = kthread_run(monitor_fn, NULL, "mem_monitor");

return 0;
}

// EXIT
static void __exit monitor_exit(void)
{
if (monitor_thread)
kthread_stop(monitor_thread);

unregister_chrdev(major, DEVICE_NAME);
printk("❌ Memory Monitor Unloaded\n");
}

module_init(monitor_init);
module_exit(monitor_exit);

MODULE_LICENSE("GPL");
