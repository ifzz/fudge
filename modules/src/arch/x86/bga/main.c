#include <memory.h>
#include <vfs.h>
#include <modules/modules.h>
#include <nodefs/nodefs.h>
#include <video/video.h>
#include <arch/x86/bga/bga.h>

static struct bga_driver driver;
static struct nodefs_node xres;
static struct nodefs_node yres;
static struct nodefs_node enable;

static unsigned int xres_read(struct nodefs_node *self, unsigned int offset, unsigned int count, void *buffer)
{

    memory_copy(buffer, &driver.xres, 4);

    return 4;

}

static unsigned int xres_write(struct nodefs_node *self, unsigned int offset, unsigned int count, void *buffer)
{

    memory_copy(&driver.xres, buffer, 4);

    return 4;

}

static unsigned int yres_read(struct nodefs_node *self, unsigned int offset,  unsigned int count, void *buffer)
{

    memory_copy(buffer, &driver.yres, 4);

    return 4;

}

static unsigned int yres_write(struct nodefs_node *self, unsigned int offset, unsigned int count, void *buffer)
{

    memory_copy(&driver.yres, buffer, 4);

    return 4;

}

static unsigned int enable_write(struct nodefs_node *self, unsigned int offset, unsigned int count, void *buffer)
{

    driver.set_mode(&driver);

    return 0;

}

static unsigned int read_data(struct video_interface *self, unsigned int offset, unsigned int count, void *buffer)
{

    struct bga_driver *driver = (struct bga_driver *)self->driver;

    memory_copy(buffer, (char *)driver->lfb + offset, count);

    return count;

}

static unsigned int write_data(struct video_interface *self, unsigned int offset, unsigned int count, void *buffer)
{

    struct bga_driver *driver = (struct bga_driver *)self->driver;

    memory_copy((char *)driver->lfb + offset, buffer, count);

    return count;

}

static unsigned int read_bpp(struct video_interface *self)
{

    struct bga_driver *driver = (struct bga_driver *)self->driver;
    
    return driver->bpp;

}

static void write_bpp(struct video_interface *self, unsigned int bpp)
{

    struct bga_driver *driver = (struct bga_driver *)self->driver;

    driver->bpp = bpp;

}

void init()
{

    bga_driver_init(&driver);
    modules_register_driver(&driver.base);
    video_register_interface(&driver.interface, &driver.base, read_data, write_data, read_bpp, write_bpp);

    nodefs_register_node(&xres, "bga_xres", &driver.base.base, xres_read, xres_write);
    nodefs_register_node(&yres, "bga_yres", &driver.base.base, yres_read, yres_write);
    nodefs_register_node(&enable, "bga_enable", &driver.base.base, 0, enable_write);

}

void destroy()
{

    video_unregister_interface(&driver.interface);
    modules_unregister_driver(&driver.base);

}

