#include <kernel/modules.h>
#include <modules/bga/bga.h>

static struct bga_driver driver;

void init()
{

    bga_driver_init(&driver);
    modules_register_driver(&driver.base);

}

void destroy()
{

    modules_unregister_driver(&driver.base);

}

