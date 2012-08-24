#include <memory.h>
#include <string.h>
#include <tar.h>
#include <vfs.h>
#include <ramdisk.h>

static unsigned int validate(struct tar_header *header)
{

    unsigned int i;
    unsigned char *address = (unsigned char *)header;
    unsigned int checksum = string_read_num(header->checksum, 8);

    for (i = 0; i < 148; i++)
        checksum -= address[i];

    for (i = 148; i < 156; i++)
        checksum -= 32;

    for (i = 156; i < TAR_BLOCK_SIZE; i++)
        checksum -= address[i];

    return !checksum;

}

static unsigned int parse(struct ramdisk_image *self, void *address)
{

    char *current;

    for (current = address; *current; current += TAR_BLOCK_SIZE)
    {

        struct tar_header *header = (struct tar_header *)current;
        unsigned int size = string_read_num(header->size, 8);

        if (!validate(header))
            return 0;

        self->headers[self->count] = header;
        self->count++;

        current += ((size / TAR_BLOCK_SIZE) + ((size % TAR_BLOCK_SIZE) ? 1 : 0)) * TAR_BLOCK_SIZE;

    }

    return self->count;

}

static unsigned int read_file(struct vfs_filesystem *self, unsigned int id, unsigned int offset, unsigned int count, void *buffer)
{

    struct vfs_ramdisk_filesystem *filesystem = (struct vfs_ramdisk_filesystem *)self;
    struct tar_header *header = filesystem->image->headers[id - 1];
    unsigned int size = string_read_num(header->size, 8);
    unsigned int c = (size > offset) ? size - offset : 0;
    unsigned int data = (unsigned int)header + TAR_BLOCK_SIZE;

    if (c > count)
        c = count;

    memory_copy(buffer, (void *)(data + offset), c);

    return c;

}

static unsigned int read(struct vfs_filesystem *self, unsigned int id, unsigned int offset, unsigned int count, void *buffer)
{

    struct vfs_ramdisk_filesystem *filesystem = (struct vfs_ramdisk_filesystem *)self;
    struct tar_header *header = filesystem->image->headers[id - 1];
    unsigned int length = string_length(header->name);

    if (header->name[length - 1] == '/')
    {

        unsigned int i;
        unsigned int c = 0;
        char *out = buffer;

        if (offset > 0)
            return 0;

        for (i = 0; i < filesystem->image->count; i++)
        {

            struct tar_header *current = filesystem->image->headers[i];
            char *start;
            char *slash;
            unsigned int size;

            if (current == header)
                continue;

            if (!memory_find(current->name, header->name, string_length(current->name), string_length(header->name)))
                continue;

            start = current->name + string_length(header->name);
            size = string_length(start);

            if (!size)
                continue;

            slash = memory_find(start, "/", size, 1);

            if (slash && slash < start + size - 1)
                continue;

            memory_copy(out + c, start, size);
            memory_copy(out + c + size, "\n", 1);
            c += size + 1;

        }

        return c;

    }

    return read_file(self, id, offset, count, buffer);

}

static unsigned int write_file(struct vfs_filesystem *self, unsigned int id, unsigned int offset, unsigned int count, void *buffer)
{

    struct vfs_ramdisk_filesystem *filesystem = (struct vfs_ramdisk_filesystem *)self;
    struct tar_header *header = filesystem->image->headers[id - 1];
    unsigned int size = string_read_num(header->size, 8);
    unsigned int c = (size > offset) ? size - offset : 0;
    unsigned int data = (unsigned int)header + TAR_BLOCK_SIZE;

    if (c > count)
        c = count;

    memory_copy((void *)(data + offset), buffer, c);

    return c;

}

static unsigned int write(struct vfs_filesystem *self, unsigned int id, unsigned int offset, unsigned int count, void *buffer)
{

    struct vfs_ramdisk_filesystem *filesystem = (struct vfs_ramdisk_filesystem *)self;
    struct tar_header *header = filesystem->image->headers[id - 1];
    unsigned int length = string_length(header->name);

    if (header->name[length - 1] == '/')
        return 0;

    return write_file(self, id, offset, count, buffer);

}

static unsigned int walk(struct vfs_filesystem *self, unsigned int id, unsigned int count, char *path)
{

    struct vfs_ramdisk_filesystem *filesystem = (struct vfs_ramdisk_filesystem *)self;

    unsigned int i;

    if (!count)
        return id;

    for (i = 0; i < filesystem->image->count; i++)
    {

        if (memory_match(filesystem->image->headers[i]->name + 6, path, count))
            return i + 1;

    }

    return 0;

}

static unsigned int get_physical(struct vfs_filesystem *self, unsigned int id)
{

    struct vfs_ramdisk_filesystem *filesystem = (struct vfs_ramdisk_filesystem *)self;
    struct tar_header *header = filesystem->image->headers[id - 1];
    unsigned int data = (unsigned int)header + TAR_BLOCK_SIZE;

    return data;

}

void ramdisk_image_init(struct ramdisk_image *image)
{

    memory_clear(image, sizeof (struct ramdisk_image));

    image->parse = parse;

}

void vfs_ramdisk_filesystem_init(struct vfs_ramdisk_filesystem *filesystem, struct ramdisk_image *image)
{

    memory_clear(filesystem, sizeof (struct vfs_ramdisk_filesystem));

    vfs_filesystem_init(&filesystem->base, 1, "ramdisk", 0, 0, read, write, walk, get_physical);

    filesystem->image = image;

}

