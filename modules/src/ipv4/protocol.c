#include <memory.h>
#include <vfs.h>
#include <net/net.h>
#include <ipv4/ipv4.h>

static void handle_read(struct net_protocol *self, unsigned int count, void *buffer)
{

    struct ipv4_protocol *protocol = (struct ipv4_protocol *)self;

    protocol->count = vfs_write(protocol->buffer, 8192, buffer, count, 0);

}

unsigned int read(struct net_protocol *self, struct net_interface *interface, unsigned int offset, unsigned int count, void *buffer)
{

    struct ipv4_protocol *protocol = (struct ipv4_protocol *)self;

    return vfs_read(buffer, count, protocol->buffer, protocol->count, offset);

}

unsigned int write(struct net_protocol *self, struct net_interface *interface, unsigned int offset, unsigned int count, void *buffer)
{

    return 0;

}

void ipv4_init_protocol(struct ipv4_protocol *protocol)
{

    memory_clear(protocol, sizeof (struct ipv4_protocol));

    protocol->base.name = "ipv4";
    protocol->base.handle_read = handle_read;
    protocol->base.read = read;
    protocol->base.write = write;

}
