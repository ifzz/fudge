struct service_backend
{

    struct resource resource;
    unsigned int id;
    unsigned int (*read)(void *buffer, unsigned int count, unsigned int offset);
    unsigned int (*write)(void *buffer, unsigned int count, unsigned int offset);
    unsigned int (*map)(unsigned int offset, unsigned int count);

};

struct service_state
{

    struct list_item link;
    struct task *task;
    unsigned int id;
    unsigned int offset;
    unsigned int current;

};

struct service_protocol
{

    struct resource resource;
    unsigned int (*match)(struct service_backend *backend);
    unsigned int (*root)(struct service_backend *backend);
    unsigned int (*parent)(struct service_backend *backend, struct service_state *state);
    unsigned int (*child)(struct service_backend *backend, struct service_state *state, char *path, unsigned int length);
    unsigned int (*create)(struct service_backend *backend, struct service_state *state, char *name, unsigned int length);
    unsigned int (*destroy)(struct service_backend *backend, struct service_state *state, char *name, unsigned int length);
    unsigned int (*step)(struct service_backend *backend, struct service_state *state);
    unsigned int (*open)(struct service_backend *backend, struct service_state *state);
    unsigned int (*close)(struct service_backend *backend, struct service_state *state);
    unsigned int (*read)(struct service_backend *backend, struct service_state *state, void *buffer, unsigned int count);
    unsigned int (*write)(struct service_backend *backend, struct service_state *state, void *buffer, unsigned int count);
    unsigned int (*seek)(struct service_backend *backend, struct service_state *state, unsigned int offset);
    unsigned int (*map)(struct service_backend *backend, struct service_state *state);

};

struct service
{

    struct container_server *server;
    struct service_state state;

};

struct service_backend *service_findbackend(unsigned int id);
struct service_protocol *service_findprotocol(struct service_backend *backend);
void service_initbackend(struct service_backend *backend, unsigned int id, unsigned int (*read)(void *buffer, unsigned int count, unsigned int offset), unsigned int (*write)(void *buffer, unsigned int count, unsigned int offset), unsigned int (*map)(unsigned int offset, unsigned int count));
void service_initstate(struct service_state *state, struct task *task);
void service_initprotocol(struct service_protocol *protocol, unsigned int (*match)(struct service_backend *backend), unsigned int (*root)(struct service_backend *backend), unsigned int (*parent)(struct service_backend *backend, struct service_state *state), unsigned int (*child)(struct service_backend *backend, struct service_state *state, char *path, unsigned int length), unsigned int (*create)(struct service_backend *backend, struct service_state *state, char *name, unsigned int length), unsigned int (*destroy)(struct service_backend *backend, struct service_state *state, char *name, unsigned int length), unsigned int (*step)(struct service_backend *backend, struct service_state *state), unsigned int (*open)(struct service_backend *backend, struct service_state *state), unsigned int (*close)(struct service_backend *backend, struct service_state *state), unsigned int (*read)(struct service_backend *backend, struct service_state *state, void *buffer, unsigned int count), unsigned int (*write)(struct service_backend *backend, struct service_state *state, void *buffer, unsigned int count), unsigned int (*seek)(struct service_backend *backend, struct service_state *state, unsigned int offset), unsigned int (*map)(struct service_backend *backend, struct service_state *state));
void service_init(struct service *service, struct task *task);
void service_setupcpio(void);
