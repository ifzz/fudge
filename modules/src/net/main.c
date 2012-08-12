#include <modules.h>
#include <net/net.h>

static struct net_driver driver;
static struct modules_filesystem filesystem;

void net_register_interface(struct net_interface *interface, struct modules_base *module, unsigned int (*read)(struct net_interface *self, unsigned int offset, unsigned int count, void *buffer), unsigned int (*write)(struct net_interface *self, unsigned int offset, unsigned int count, void *buffer))
{

    driver.register_interface(&driver, interface, module, read, write);

}

void net_register_protocol(struct net_protocol *protocol, char *name)
{

    driver.register_protocol(&driver, protocol, name);

}

void net_unregister_interface(struct net_interface *interface)
{

    driver.unregister_interface(&driver, interface);

}

void net_unregister_protocol(struct net_protocol *protocol)
{

    driver.unregister_protocol(&driver, protocol);

}

struct modules_filesystem *get_filesystem()
{

    return &filesystem;

}

void init()
{

    net_driver_init(&driver);
    modules_register_driver(&driver.base);

    net_filesystem_init(&filesystem, &driver.base);
    modules_register_filesystem(&filesystem);

}

void destroy()
{

    modules_unregister_filesystem(&filesystem);
    modules_unregister_driver(&driver.base);

}
