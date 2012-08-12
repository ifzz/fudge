#include <memory.h>
#include <string.h>
#include <modules.h>
#include <arch/x86/ps2/ps2.h>
#include <arch/x86/vga/vga.h>
#include <tty/tty.h>

static unsigned int read_stdin(struct modules_filesystem *self, unsigned int id, unsigned int offset, unsigned int count, void *buffer)
{

    char *stream = buffer;
    unsigned int i;

    for (i = 0; i < count; i++)
    {

        if (!ps2_getc(stream + i))
            break;

    }

    return i;

}

static unsigned int write_stdin(struct modules_filesystem *self, unsigned int id, unsigned int offset, unsigned int count, void *buffer)
{

    char *stream = buffer;
    unsigned int i;

    for (i = 0; i < count; i++)
    {

        if (!ps2_putc(stream + i))
            break;

    }

    return i;

}

static unsigned int write_stdout(struct modules_filesystem *self, unsigned int id, unsigned int offset, unsigned int count, void *buffer)
{

    struct tty_driver *driver = (struct tty_driver *)self->driver;
    char *stream = buffer;
    unsigned int i;

    for (i = 0; i < count; i++)
        driver->putc(driver, stream[i]);

    vga_set_cursor_offset(driver->cursorOffset);

    return count;

}

static unsigned int read_cwd(struct modules_filesystem *self, unsigned int id, unsigned int offset, unsigned int count, void *buffer)
{

    struct tty_driver *driver = (struct tty_driver *)self->driver;
    unsigned int length = string_length(driver->cwdname);

    if (offset > length)
        return 0;

    if (count > length - offset)
        count = length - offset;

    memory_copy(buffer, driver->cwdname + offset, count);

    return count;

}

static unsigned int write_cwd(struct modules_filesystem *self, unsigned int id, unsigned int offset, unsigned int count, void *buffer)
{

    struct tty_driver *driver = (struct tty_driver *)self->driver;

    memory_clear(driver->cwdname, 128);
    memory_copy(driver->cwdname, buffer, count);

    return count;

}

static unsigned int read_root(struct modules_filesystem *self, unsigned int id, unsigned int offset, unsigned int count, void *buffer)
{

    if (offset > 17)
        return 0;

    if (count > 17 - offset)
        count = 17 - offset;

    memory_copy(buffer, "stdin\nstdout\ncwd\n" + offset, count);

    return count;

}

static unsigned int read(struct modules_filesystem *self, unsigned int id, unsigned int offset, unsigned int count, void *buffer)
{

    if (id == 1)
        return read_root(self, id, offset, count, buffer);

    if (id == 2)
        return read_stdin(self, id, offset, count, buffer);

    if (id == 4)
        return read_cwd(self, id, offset, count, buffer);

    return 0;

}

static unsigned int write(struct modules_filesystem *self, unsigned int id, unsigned int offset, unsigned int count, void *buffer)
{

    if (id == 2)
        return write_stdin(self, id, offset, count, buffer);

    if (id == 3)
        return write_stdout(self, id, offset, count, buffer);

    if (id == 4)
        return write_cwd(self, id, offset, count, buffer);

    return 0;

}

static unsigned int walk(struct modules_filesystem *self, unsigned int id, unsigned int count, void *buffer)
{

    if (!count)
        return 1;

    if (memory_match(buffer, "stdin", 5))
        return 2;

    if (memory_match(buffer, "stdout", 6))
        return 3;

    if (memory_match(buffer, "cwd", 3))
        return 4;

    return 0;

}

void tty_filesystem_init(struct modules_filesystem *filesystem, struct modules_driver *driver)
{

    memory_clear(filesystem, sizeof (struct modules_filesystem));

    modules_filesystem_init(filesystem, 0x1451, driver, "tty", 0, 0, read, write, 0, walk, 0); 

}
