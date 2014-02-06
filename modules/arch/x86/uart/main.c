#include <fudge/module.h>
#include <kernel/vfs.h>
#include <kernel/task.h>
#include <kernel/container.h>
#include <kernel/kernel.h>
#include <base/base.h>
#include <base/terminal.h>
#include "driver.h"

static struct uart_driver driver;

void init()
{

    uart_init_driver(&driver);
    base_register_driver(&driver.base);

}

void destroy()
{

    base_unregister_driver(&driver.base);

}

