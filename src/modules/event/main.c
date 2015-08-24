#include <fudge.h>
#include <kernel.h>
#include <modules/system/system.h>
#include "event.h"

static struct system_node root;
static struct system_node poll;
static struct system_node send;

void event_notify(unsigned int count, void *buffer)
{

    struct event_header *header = buffer;

    header->destination = 0xFFFFFFFF;
    header->source = 0;

    system_write(&poll, 0, count, buffer);

}

static unsigned int send_write(struct system_node *self, unsigned int offset, unsigned int count, void *buffer)
{

    struct event_header *header = buffer;
    struct task *destination;

    header->source = (unsigned int)task_findactive();

    if (header->destination == 0xFFFFFFFF)
    {

        system_write(&poll, 0, count, buffer);

        return count;

    }

    if (header->destination == 0)
        header->destination = header->source;

    destination = (struct task *)header->destination;

    return task_wmessage(destination, count, buffer);

}

void module_init(void)
{

    system_initnode(&poll, SYSTEM_NODETYPE_MAILBOX, "poll");
    system_initnode(&send, SYSTEM_NODETYPE_NORMAL, "send");

    send.write = send_write;

    system_initnode(&root, SYSTEM_NODETYPE_GROUP, "event");
    system_addchild(&root, &poll);
    system_addchild(&root, &send);

}

void module_register(void)
{

    system_registernode(&root);

}

void module_unregister(void)
{

    system_unregisternode(&root);

}

