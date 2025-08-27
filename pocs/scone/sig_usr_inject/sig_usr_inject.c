#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <asm/cpu.h>
#include <linux/module.h>
#include <linux/errno.h>
#include <linux/percpu.h>
#include <linux/mm.h>
#include <linux/miscdevice.h>
#include <linux/vmalloc.h>
#include <linux/reboot.h>
#include <linux/debugfs.h>
#include <linux/highmem.h>
#include <linux/file.h>
#include <linux/syscore_ops.h>
#include <linux/cpu.h>
#include <linux/sched/signal.h>
#include <linux/sched/mm.h>
#include <linux/sched/stat.h>
#include <linux/cpumask.h>
#include <linux/smp.h>
#include <linux/anon_inodes.h>
#include <linux/profile.h>
#include <linux/kvm_para.h>
#include <linux/pagemap.h>
#include <linux/mman.h>
#include <linux/swap.h>
#include <linux/bitops.h>
#include <linux/spinlock.h>
#include <linux/compat.h>
#include <linux/srcu.h>
#include <linux/hugetlb.h>
#include <linux/slab.h>
#include <linux/sort.h>
#include <linux/bsearch.h>
#include <linux/io.h>
#include <linux/lockdep.h>
#include <linux/kthread.h>
#include <linux/suspend.h>
#include <linux/version.h>
#include <asm/processor.h>
#include <asm/ioctl.h>
#include <linux/uaccess.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kprobes.h>

MODULE_LICENSE("GPL");

static int pid = 10;
module_param(pid,
int,0660);

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 7, 0)
#define KPROBE_KALLSYMS_LOOKUP 1

typedef unsigned long (*kallsyms_lookup_name_t)(const char *name);

kallsyms_lookup_name_t kallsyms_lookup_name_func;
#define kallsyms_lookup_name kallsyms_lookup_name_func

static struct kprobe kp = {
	.symbol_name = "kallsyms_lookup_name"
};
#endif

//int (* interruptfunc)(struct kvm_lapic* apic, int delivery_mode, int trig_mode, int vector);
int (*_kill_something_info)(int sig, struct kernel_siginfo *info, pid_t pid);

static int init(void)
{
	#ifdef KPROBE_KALLSYMS_LOOKUP
	register_kprobe(&kp);
	kallsyms_lookup_name = (kallsyms_lookup_name_t) kp.addr;
	unregister_kprobe(&kp);

	if (!unlikely(kallsyms_lookup_name)) {
		pr_alert("Could not retrieve kallsyms_lookup_name address\n");
		return -ENXIO;
	}
	#endif

	_kill_something_info = (void*)kallsyms_lookup_name("kill_something_info");
	if (_kill_something_info == NULL)
	{
		pr_info("lookup failed kill_something_info\n");
		return -ENXIO;
	}
	return 0;
}


static int __init
_do_init(void)
{
	int ret = 0;
	int sig = 0;
	struct kernel_siginfo info;

	ret = init();
	if (ret != 0)
	{
		pr_info("init failed!\n");
		return ret;
	}


	sig = 8;
	// FPE_INTDIV || info->si_code == FPE_FLTDIV

	clear_siginfo(&info);
	info.si_signo = sig;
	info.si_errno = 0;
	info.si_code = FPE_INTDIV;
	info.si_pid = task_tgid_vnr(current);
	info.si_uid = from_kuid_munged(current_user_ns(), current_uid());

	pr_info("using pid: %d\n", pid);
	pr_info("using sig: %d\n", sig);
	_kill_something_info(sig, &info, pid);
	pr_info("terminating module...\n");
	return -1;
}

static void __exit
_do_exit(void)
{
	pr_info("exiting...\n");
	return;
}

module_init(_do_init);
module_exit(_do_exit);