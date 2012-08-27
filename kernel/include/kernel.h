#ifndef KERNEL_KERNEL_H
#define KERNEL_KERNEL_H

struct kernel_interface
{

    void (*start)(struct kernel_interface *self);
    void (*setup)(struct kernel_interface *self);
    void (*enter_usermode)(unsigned int ip, unsigned int sp);
    unsigned int ramdiskc;
    void **ramdiskv;

};

void kernel_interface_init(struct kernel_interface *interface, void (*setup)(struct kernel_interface *self), void (*enter_usermode)(unsigned int ip, unsigned int sp), unsigned int ramdiskc, void **ramdiskv);

#endif

