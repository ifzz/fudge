#ifndef KERNEL_VFS_H
#define KERNEL_VFS_H

struct vfs_interface
{

    unsigned int rootid;
    void (*open)(struct vfs_interface *self, unsigned int id);
    void (*close)(struct vfs_interface *self, unsigned int id);
    unsigned int (*read)(struct vfs_interface *self, unsigned int id, unsigned int offset, unsigned int count, void *buffer);
    unsigned int (*write)(struct vfs_interface *self, unsigned int id, unsigned int offset, unsigned int count, void *buffer);
    unsigned int (*parent)(struct vfs_interface *self, unsigned int id);
    unsigned int (*walk)(struct vfs_interface *self, unsigned int id, unsigned int count, char *path);
    unsigned int (*get_physical)(struct vfs_interface *self, unsigned int id);

};

unsigned int vfs_read(void *value, unsigned int length, unsigned int offset, unsigned int count, void *buffer);
unsigned int vfs_write(void *value, unsigned int length, unsigned int offset, unsigned int count, void *buffer);
void vfs_init_interface(struct vfs_interface *interface, unsigned int rootid, char *name, void (*open)(struct vfs_interface *self, unsigned int id), void (*close)(struct vfs_interface *self, unsigned int id), unsigned int (*read)(struct vfs_interface *self, unsigned int id, unsigned int offset, unsigned int count, void *buffer), unsigned int (*write)(struct vfs_interface *self, unsigned int id, unsigned int offset, unsigned int count, void *buffer), unsigned int (*parent)(struct vfs_interface *self, unsigned int id), unsigned int (*walk)(struct vfs_interface *self, unsigned int id, unsigned int count, char *path), unsigned int (*get_physical)(struct vfs_interface *self, unsigned int id));

#endif

