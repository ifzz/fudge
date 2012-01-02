#include <kernel/error.h>
#include <kernel/log.h>
#include <kernel/mmu.h>

static struct mmu_unit *primary;
static struct mmu_memory memories[8];

void mmu_pagefault(unsigned int address, unsigned int flags)
{

    log_write("ERROR!\n");

    if (!(flags & MMU_ERROR_PRESENT))
        log_write("Page: present\n");

    if (flags & MMU_ERROR_RW)
        log_write("Page: read-only\n");

    if (flags & MMU_ERROR_USER)
        log_write("Page: user-mode\n");

    if (flags & MMU_ERROR_RESERVED)
        log_write("Page: reserved\n");

    if (flags & MMU_ERROR_FETCH)
        log_write("Page: fetch\n");

    log_write("Address: 0x%x\n", address);

    error_panic("PAGE FAULT", __FILE__, __LINE__);

}

static unsigned int mmu_get_unused_slot()
{

    unsigned int i;

    for (i = 0; i < 8; i++)
    {

        struct mmu_memory *memory = &memories[i];

        if (!memory->used)
            return i;

    }

    return 0;

}

struct mmu_memory *mmu_get_task_memory()
{

    unsigned int index = mmu_get_unused_slot();

    if (!index)
        return 0;

    struct mmu_memory *memory = &memories[index];

    mmu_memory_init(memory, 1, (void *)(0x00300000 + index * 0x10000), (void *)0x00000000, 0x10000);

    return memory;

}

void mmu_load_memory(struct mmu_memory *memory)
{

    primary->load_memory(memory);

}

void mmu_map_kernel_memory(struct mmu_memory *memory)
{

    primary->map_kernel_memory(memory);

}

void mmu_map_user_memory(struct mmu_memory *memory)
{

    primary->map_user_memory(memory);

}

void mmu_unmap_memory(struct mmu_memory *memory)
{

    primary->unmap_memory(memory);

}

void mmu_register_unit(struct mmu_unit *unit)
{

    primary = unit;

    struct mmu_memory *memory = &memories[0];

    mmu_memory_init(memory, 1, (void *)0x00000000, (void *)0x00000000, 0x00400000);
    mmu_map_kernel_memory(memory);
    mmu_load_memory(memory);

}

void mmu_memory_init(struct mmu_memory *memory, unsigned int used, void *paddress, void *vaddress, unsigned int size)
{

    memory->used = used;
    memory->paddress = paddress;
    memory->vaddress = vaddress;
    memory->size = size;

}

