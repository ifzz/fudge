#ifndef KBD_H
#define KBD_H

#define KBD_BUFFER_SIZE 256

typedef struct kbd_device
{

    char buffer[KBD_BUFFER_SIZE];
    cbuffer_t cbuffer;
    unsigned char toggleAlt;
    unsigned char toggleCtrl;
    unsigned char toggleShift;

} kbd_device_t;

extern kbd_device_t keyboard;

extern void kbd_handler(registers_t *r);
extern void kbd_init();

#endif

