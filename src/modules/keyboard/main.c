#include <fudge.h>
#include <kernel.h>
#include <modules/system/system.h>
#include <modules/event/event.h>
#include "keyboard.h"

void keyboard_notify(struct keyboard_interface *interface, unsigned int size, unsigned int count, void *buffer)
{

    rendezvous_writelist(&interface->data.mailboxes, size, count, buffer);
    event_notify(EVENT_KEYBOARD, size, count, buffer);

}

void keyboard_registerinterface(struct keyboard_interface *interface, unsigned int id)
{

    system_addchild(&interface->root, &interface->data);
    system_registernode(&interface->root);

    interface->id = id;

}

void keyboard_unregisterinterface(struct keyboard_interface *interface)
{

    system_removechild(&interface->root, &interface->data);
    system_unregisternode(&interface->root);

}

void keyboard_initinterface(struct keyboard_interface *interface)
{

    resource_init(&interface->resource, 0, interface);
    system_initnode(&interface->root, SYSTEM_NODETYPE_GROUP | SYSTEM_NODETYPE_MULTI, "keyboard");
    system_initnode(&interface->data, SYSTEM_NODETYPE_MAILBOX, "data");

    interface->data.resource = &interface->resource;

}

