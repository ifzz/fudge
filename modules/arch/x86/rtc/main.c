#include <lib/string.h>
#include <kernel/modules.h>
#include <kernel/vfs.h>
#include <modules/nodefs/nodefs.h>
#include <modules/rtc/rtc.h>

static struct rtc_device device;
static struct nodefs_node timestamp;

static unsigned int timestamp_read(struct nodefs_node *self, unsigned int offset, unsigned int count, void *buffer)
{

    string_write(buffer, "%d", 0);

    return string_length(buffer);

}

void init()
{

    rtc_device_init(&device);
    modules_register_device(&device.base);

    struct nodefs_driver *nodefs = (struct nodefs_driver *)modules_get_driver(NODEFS_DRIVER_TYPE);

    if (!nodefs)
        return;

    nodefs->register_node(nodefs, &timestamp, "rtc/timestamp", &device.base.module, timestamp_read, 0);

}

void destroy()
{

    modules_unregister_device(&device.base);

}

