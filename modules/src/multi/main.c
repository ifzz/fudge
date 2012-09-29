#include <memory.h>
#include <vfs.h>
#include <binary.h>
#include <runtime.h>
#include <syscall.h>
#include <arch/x86/mmu.h>
#include <multi/multi.h>

static struct runtime_task tasks[MULTI_TASK_SLOTS];

static struct runtime_task *get_task(unsigned int index)
{

    if (!index || index >= MULTI_TASK_SLOTS)
        return 0;

    return &tasks[index];

}

static unsigned int get_task_slot()
{

    unsigned int i;

    for (i = 1; i < MULTI_TASK_SLOTS; i++)
    {

        if (!tasks[i].status.used)
            return i;

    }

    return 0;

}

static void schedule()
{

    unsigned int i;

    for (i = MULTI_TASK_SLOTS - 1; i > 0; i--)
    {

        if (!tasks[i].status.used)
            continue;

        if (tasks[i].status.idle)
            continue;

        runtime_set_task(&tasks[i]);

        mmu_load_memory(i);

        break;

    }

}

static void notify_pre_event(struct runtime_task *task, unsigned int index)
{

    unsigned int i;

    for (i = 1; i < MULTI_TASK_SLOTS - 1; i++)
    {

        if (!tasks[i].status.used)
            continue;

        if (!tasks[i].status.idle)
            continue;

        if (!tasks[i].events[index].callback)
            continue;

        tasks[i].status.idle = 0;
        runtime_init_registers(&tasks[i].registers, tasks[i].events[index].callback, RUNTIME_TASK_VADDRESS_BASE + RUNTIME_TASK_ADDRESS_SIZE, RUNTIME_TASK_VADDRESS_BASE + RUNTIME_TASK_ADDRESS_SIZE, tasks[i].registers.status);

    }

}

static void notify_post_event(struct runtime_task *task, unsigned int index)
{

    schedule();

}

static unsigned int spawn(struct runtime_task *task, void *stack)
{

    struct multi_spawn_args *args = stack;
    struct runtime_descriptor *descriptor = runtime_get_task_descriptor(task, args->index);
    unsigned int index = get_task_slot();
    struct runtime_task *ntask = get_task(index);
    unsigned int entry;

    if (!descriptor || !descriptor->interface->read)
        return 0;

    if (!ntask)
        return 0;

    mmu_map_user_memory(index, RUNTIME_TASK_PADDRESS_BASE + index * RUNTIME_TASK_ADDRESS_SIZE, RUNTIME_TASK_VADDRESS_BASE, RUNTIME_TASK_ADDRESS_SIZE);
    mmu_load_memory(index);

    entry = binary_copy_program(descriptor->interface, descriptor->id);

    if (!entry)
        return 0;

    memory_copy(ntask, task, sizeof (struct runtime_task));

    runtime_init_registers(&ntask->registers, entry, RUNTIME_TASK_VADDRESS_BASE + RUNTIME_TASK_ADDRESS_SIZE, RUNTIME_TASK_VADDRESS_BASE + RUNTIME_TASK_ADDRESS_SIZE, index);

    ntask->notify_pre_event = notify_pre_event;
    ntask->notify_post_event = notify_post_event;

    schedule();

    return index;

}

void init()
{

    memory_clear(tasks, sizeof (struct runtime_task) * MULTI_TASK_SLOTS);

    syscall_set_routine(SYSCALL_INDEX_SPAWN, spawn);

}

void destroy()
{

}

