#include <fudge.h>
#include <kernel.h>
#include <modules/system/system.h>
#include "pipe.h"

static struct system_node root;
static struct system_node clone;

static unsigned int read(struct pipe_end *endself, struct pipe_end *endtarget, struct service_state *state, void *buffer, unsigned int count)
{

    count = buffer_rcfifo(&endself->buffer, count, buffer);

    if (!count && endtarget->node.refcount)
    {

        list_add(&endself->readlinks, &state->link);
        task_setstatus(state->link.data, TASK_STATUS_BLOCKED);

    }

    system_wakeup(&endtarget->writelinks);

    return count;

}

static unsigned int write(struct pipe_end *endself, struct pipe_end *endtarget, struct service_state *state, void *buffer, unsigned int count)
{

    count = buffer_wcfifo(&endtarget->buffer, count, buffer);

    if (!count)
    {

        list_add(&endself->writelinks, &state->link);
        task_setstatus(state->link.data, TASK_STATUS_BLOCKED);

    }

    system_wakeup(&endtarget->readlinks);

    return count;

}

static unsigned int end0_read(struct system_node *self, struct service_state *state, void *buffer, unsigned int count)
{

    struct pipe *pipe = (struct pipe *)self->parent;

    return read(&pipe->end0, &pipe->end1, state, buffer, count);

}

static unsigned int end0_write(struct system_node *self, struct service_state *state, void *buffer, unsigned int count)
{

    struct pipe *pipe = (struct pipe *)self->parent;

    return write(&pipe->end0, &pipe->end1, state, buffer, count);

}

static unsigned int end1_read(struct system_node *self, struct service_state *state, void *buffer, unsigned int count)
{

    struct pipe *pipe = (struct pipe *)self->parent;

    return read(&pipe->end1, &pipe->end0, state, buffer, count);

}

static unsigned int end1_write(struct system_node *self, struct service_state *state, void *buffer, unsigned int count)
{

    struct pipe *pipe = (struct pipe *)self->parent;

    return write(&pipe->end1, &pipe->end0, state, buffer, count);

}

static unsigned int clone_child(struct system_node *self, char *path, unsigned int length)
{

    struct list_item *current;

    for (current = root.children.head; current; current = current->next)
    {

        struct system_node *node = current->data;
        struct pipe *pipe = current->data;

        if (node == self)
            continue;

        if (pipe->end0.node.refcount || pipe->end1.node.refcount)
            continue;

        return node->child(node, path, length);

    }

    return 0;

}

void pipe_init(struct pipe *pipe, unsigned int count0, void *data0, unsigned int count1, void *data1)
{

    buffer_init(&pipe->end0.buffer, count0, data0);
    buffer_init(&pipe->end1.buffer, count1, data1);
    system_initnode(&pipe->end0.node, SYSTEM_NODETYPE_NORMAL, "0");
    system_initnode(&pipe->end1.node, SYSTEM_NODETYPE_NORMAL, "1");

    pipe->end0.node.read = end0_read;
    pipe->end0.node.write = end0_write;
    pipe->end1.node.read = end1_read;
    pipe->end1.node.write = end1_write;

    system_initnode(&pipe->root, SYSTEM_NODETYPE_GROUP | SYSTEM_NODETYPE_MULTI, "pipe");
    system_addchild(&pipe->root, &pipe->end0.node);
    system_addchild(&pipe->root, &pipe->end1.node);

}

void pipe_register(struct pipe *pipe)
{

    system_addchild(&root, &pipe->root);

}

void pipe_unregister(struct pipe *pipe)
{

    system_removechild(&root, &pipe->root);

}

void module_init(void)
{

    system_initnode(&root, SYSTEM_NODETYPE_GROUP, "pipe");
    system_initnode(&clone, SYSTEM_NODETYPE_GROUP, "clone");

    clone.child = clone_child;

    system_addchild(&root, &clone);

}

void module_register(void)
{

    system_registernode(&root);

}

void module_unregister(void)
{

    system_unregisternode(&root);

}

