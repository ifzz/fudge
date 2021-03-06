#include <fudge.h>
#include <kernel.h>
#include <modules/system/system.h>
#include <modules/event/event.h>
#include "timer.h"

static struct system_node root;

void timer_notify(struct timer_interface *interface, void *buffer, unsigned int count)
{

    kernel_multicast(&interface->sleeplinks, buffer, count);

}

void timer_notifytick(struct timer_interface *interface, unsigned int counter)
{

    struct {struct event_header header; struct event_timertick timertick;} message;

    message.header.type = EVENT_TIMERTICK;
    message.header.source = 0;
    message.header.destination = 0;
    message.timertick.counter = counter;

    event_multicast(&interface->eventlinks, &message.header, sizeof (struct event_header) + sizeof (struct event_timertick));

}

static unsigned int interfacesleep_open(struct system_node *self, struct service_state *state)
{

    struct timer_interface *interface = self->resource->data;

    list_add(&interface->sleeplinks, &state->link);

    return state->id;

}

static unsigned int interfacesleep_close(struct system_node *self, struct service_state *state)
{

    struct timer_interface *interface = self->resource->data;

    list_remove(&interface->sleeplinks, &state->link);

    return state->id;

}

static unsigned int interfaceevent_open(struct system_node *self, struct service_state *state)
{

    struct timer_interface *interface = self->resource->data;

    list_add(&interface->eventlinks, &state->link);

    return state->id;

}

static unsigned int interfaceevent_close(struct system_node *self, struct service_state *state)
{

    struct timer_interface *interface = self->resource->data;

    list_remove(&interface->eventlinks, &state->link);

    return state->id;

}

void timer_registerinterface(struct timer_interface *interface, unsigned int id)
{

    resource_register(&interface->resource);
    system_addchild(&interface->root, &interface->sleep);
    system_addchild(&interface->root, &interface->event);
    system_addchild(&root, &interface->root);

    interface->id = id;

}

void timer_unregisterinterface(struct timer_interface *interface)
{

    resource_unregister(&interface->resource);
    system_removechild(&interface->root, &interface->sleep);
    system_removechild(&interface->root, &interface->event);
    system_removechild(&root, &interface->root);

}

void timer_initinterface(struct timer_interface *interface)
{

    resource_init(&interface->resource, RESOURCE_TIMERINTERFACE, interface);
    system_initresourcenode(&interface->root, SYSTEM_NODETYPE_GROUP | SYSTEM_NODETYPE_MULTI, "if", &interface->resource);
    system_initresourcenode(&interface->sleep, SYSTEM_NODETYPE_NORMAL, "sleep", &interface->resource);
    system_initresourcenode(&interface->event, SYSTEM_NODETYPE_NORMAL, "event", &interface->resource);

    interface->sleep.open = interfacesleep_open;
    interface->sleep.close = interfacesleep_close;
    interface->sleep.read = system_readtask;
    interface->event.open = interfaceevent_open;
    interface->event.close = interfaceevent_close;
    interface->event.read = system_readtask;

}

void module_init(void)
{

    system_initnode(&root, SYSTEM_NODETYPE_GROUP, "timer");

}

void module_register(void)
{

    system_registernode(&root);

}

void module_unregister(void)
{

    system_unregisternode(&root);

}

