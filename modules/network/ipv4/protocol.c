#include <fudge/module.h>
#include <base/base.h>
#include <base/network.h>
#include "ipv4.h"

static unsigned int read(struct base_network_interface *interface, unsigned int offset, unsigned int count, void *buffer)
{

    return 0;

}

static unsigned int write(struct base_network_interface *interface, unsigned int offset, unsigned int count, void *buffer)
{

    return 0;

}

void ipv4_init_protocol(struct base_network_protocol *protocol)
{

    base_network_init_protocol(protocol, "ipv4", read, write);

}
