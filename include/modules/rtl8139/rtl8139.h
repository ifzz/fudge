#ifndef MODULES_RTL8139_H
#define MODULES_RTL8139_H

#define RTL8139_REGISTER_IDR0 0x00
#define RTL8139_REGISTER_IDR1 0x01
#define RTL8139_REGISTER_IDR2 0x02
#define RTL8139_REGISTER_IDR3 0x03
#define RTL8139_REGISTER_IDR4 0x04
#define RTL8139_REGISTER_IDR5 0x05
#define RTL8139_REGISTER_MAR0 0x08
#define RTL8139_REGISTER_MAR1 0x09
#define RTL8139_REGISTER_MAR2 0x0A
#define RTL8139_REGISTER_MAR3 0x0B
#define RTL8139_REGISTER_MAR4 0x0C
#define RTL8139_REGISTER_MAR5 0x0D
#define RTL8139_REGISTER_MAR6 0x0E
#define RTL8139_REGISTER_MAR7 0x0F
#define RTL8139_REGISTER_TSD0 0x10
#define RTL8139_REGISTER_TSD1 0x14
#define RTL8139_REGISTER_TSD2 0x18
#define RTL8139_REGISTER_TSD3 0x1C
#define RTL8139_REGISTER_TSAD0 0x20
#define RTL8139_REGISTER_TSAD1 0x24
#define RTL8139_REGISTER_TSAD2 0x28
#define RTL8139_REGISTER_TSAD3 0x2C
#define RTL8139_REGISTER_RBSTART 0x30
#define RTL8139_REGISTER_ERBCR 0x34
#define RTL8139_REGISTER_ERSR 0x36
#define RTL8139_REGISTER_CR 0x37
#define RTL8139_REGISTER_CAPR 0x38
#define RTL8139_REGISTER_CBR 0x3A
#define RTL8139_REGISTER_IMR 0x3C
#define RTL8139_REGISTER_ISR 0x3E
#define RTL8139_REGISTER_TCR 0x40
#define RTL8139_REGISTER_RCR 0x44
#define RTL8139_REGISTER_TCTR 0x48
#define RTL8139_REGISTER_MPC 0x4C
#define RTL8139_REGISTER_9346CR 0x50
#define RTL8139_REGISTER_CONFIG0 0x51
#define RTL8139_REGISTER_CONFIG1 0x52
#define RTL8139_REGISTER_TIMERINT 0x54
#define RTL8139_REGISTER_MSR 0x58
#define RTL8139_REGISTER_CONFIG3 0x59
#define RTL8139_REGISTER_CONFIG4 0x5A
#define RTL8139_REGISTER_MULINT 0x5C
#define RTL8139_REGISTER_RERID 0x5E
#define RTL8139_REGISTER_TSAD 0x60
#define RTL8139_REGISTER_BMCR 0x62
#define RTL8139_REGISTER_BMSR 0x64
#define RTL8139_REGISTER_ANAR 0x66
#define RTL8139_REGISTER_ANLPAR 0x68
#define RTL8139_REGISTER_ANER 0x6A
#define RTL8139_REGISTER_DIS 0x6C
#define RTL8139_REGISTER_FCSC 0x6E
#define RTL8139_REGISTER_NWAYTR 0x70
#define RTL8139_REGISTER_REC 0x72
#define RTL8139_REGISTER_CSCR 0x74
#define RTL8139_REGISTER_PHY1PARM 0x78
#define RTL8139_REGISTER_TWPARM 0x7C
#define RTL8139_REGISTER_PHY2PARM 0x80
#define RTL8139_REGISTER_CRC0 0x84
#define RTL8139_REGISTER_CRC1 0x85
#define RTL8139_REGISTER_CRC2 0x86
#define RTL8139_REGISTER_CRC3 0x87
#define RTL8139_REGISTER_CRC4 0x88
#define RTL8139_REGISTER_CRC5 0x89
#define RTL8139_REGISTER_CRC6 0x8A
#define RTL8139_REGISTER_CRC7 0x8B
#define RTL8139_REGISTER_WAKEUP0 0x8C
#define RTL8139_REGISTER_WAKEUP1 0x94
#define RTL8139_REGISTER_WAKEUP2 0x9C
#define RTL8139_REGISTER_WAKEUP3 0xA4
#define RTL8139_REGISTER_WAKEUP4 0xAC
#define RTL8139_REGISTER_WAKEUP5 0xB4
#define RTL8139_REGISTER_WAKEUP6 0xBC
#define RTL8139_REGISTER_WAKEUP7 0xC4
#define RTL8139_REGISTER_LSBCRC0 0xCC
#define RTL8139_REGISTER_LSBCRC1 0xCD
#define RTL8139_REGISTER_LSBCRC2 0xCE
#define RTL8139_REGISTER_LSBCRC3 0xCF
#define RTL8139_REGISTER_LSBCRC4 0xD0
#define RTL8139_REGISTER_LSBCRC5 0xD1
#define RTL8139_REGISTER_LSBCRC6 0xD2
#define RTL8139_REGISTER_LSBCRC7 0xD3
#define RTL8139_REGISTER_CONFIG5 0xD8

#define RTL8139_ISR_FLAG_ROK (1 << 0)
#define RTL8139_ISR_FLAG_TOK (1 << 2)

#define RTL8139_HEADER_FLAG_ROK  (1 << 0)
#define RTL8139_HEADER_FLAG_FAE  (1 << 1)
#define RTL8139_HEADER_FLAG_CRC  (1 << 2)
#define RTL8139_HEADER_FLAG_LONG (1 << 3)
#define RTL8139_HEADER_FLAG_RUNT (1 << 4)
#define RTL8139_HEADER_FLAG_ISE  (1 << 5)
#define RTL8139_HEADER_FLAG_BAR  (1 << 13)
#define RTL8139_HEADER_FLAG_PAM  (1 << 14)
#define RTL8139_HEADER_FLAG_MAR  (1 << 15)

#define RTL8139_DRIVER_TYPE 0x0423

struct rtl8139_driver
{

    struct modules_driver base;
    unsigned short io; 
    char mac[6];
    char rx[0x2200];
    char tx0[0x800];
    char tx1[0x800];
    char tx2[0x800];
    char tx3[0x800];
    unsigned int (*read)(struct rtl8139_driver *self, void *buffer);

};

struct rtl8139_header
{

    unsigned short flags;
    unsigned short length;

};

extern void rtl8139_driver_init(struct rtl8139_driver *driver);

#endif

