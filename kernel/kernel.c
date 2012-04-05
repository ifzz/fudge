#include <kernel/error.h>
#include <kernel/vfs.h>
#include <kernel/ramdisk.h>
#include <kernel/kernel.h>
#include <kernel/log.h>
#include <kernel/mmu.h>
#include <kernel/modules.h>
#include <kernel/runtime.h>
#include <kernel/symbol.h>
#include <kernel/syscall.h>

static struct kernel_arch *kernelArch;

void kernel_disable_interrupts()
{

    kernelArch->disable_interrupts();

}

void kernel_enable_interrupts()
{

    kernelArch->enable_interrupts();

}

void kernel_reboot()
{

    kernelArch->reboot();

}

void kernel_halt()
{

    kernelArch->halt();

}

void kernel_arch_init(struct kernel_arch *arch, void (*setup)(struct kernel_arch *arch), void (*reboot)(), void (*halt)(), void (*enable_interrupts)(), void (*disable_interrupts)(), void (*enter_usermode)(unsigned int ip, unsigned int sp), void *stack, void (*set_stack)(void *address), unsigned int ramdiskc, void **ramdiskv)
{

    arch->setup = setup;
    arch->reboot = reboot;
    arch->halt = halt;
    arch->enable_interrupts = enable_interrupts;
    arch->disable_interrupts = disable_interrupts;
    arch->enter_usermode = enter_usermode;
    arch->stack = stack;
    arch->set_stack = set_stack;
    arch->ramdiskc = ramdiskc;
    arch->ramdiskv = ramdiskv;

}

void kernel_init(struct kernel_arch *arch)
{

    kernelArch = arch;

    log_write("[kernel] Initializing\n");

    if (!kernelArch)
        error_panic("No architecture registered", __FILE__, __LINE__);

    kernelArch->setup(kernelArch);

    vfs_init();
    modules_init();
    syscall_init();
    runtime_init();
    ramdisk_init(kernelArch->ramdiskc, kernelArch->ramdiskv);
    symbol_init();

    unsigned int index = syscall_execute("/ramdisk/bin/init");

    if (!index)
        error_panic("Could not start init", __FILE__, __LINE__);

    struct runtime_task *task = runtime_get_task(index);

    kernelArch->set_stack(kernelArch->stack);
    kernelArch->enter_usermode(task->registers.ip, task->registers.sp);

}

