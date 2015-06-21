#define TASK_DESCRIPTORS                32
#define TASK_MAILBOXSIZE                4096

struct task_state
{

    struct list_item item;
    struct {unsigned long ip; unsigned long sp;} registers;
    unsigned int blocked;

};

struct task_mailbox
{

    struct list_item item;
    struct buffer buffer;
    unsigned char data[TASK_MAILBOXSIZE];

};

struct task
{

    struct resource resource;
    struct task_state state;
    struct task_mailbox mailbox;
    struct vfs_descriptor descriptors[TASK_DESCRIPTORS];

};

unsigned int task_rmessage(struct task *task, unsigned int size, unsigned int count, void *buffer);
unsigned int task_wmessage(struct task *task, unsigned int size, unsigned int count, void *buffer);
void task_init(struct task *task);