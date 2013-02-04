#define IDE_BUS1_DATA1                  0x0168
#define IDE_BUS0_DATA1                  0x0170
#define IDE_BUS1_DATA0                  0x01E8
#define IDE_BUS0_DATA0                  0x01F0
#define IDE_BUS1_COMMAND1               0x0366
#define IDE_BUS0_COMMAND1               0x0376
#define IDE_BUS1_COMMAND0               0x03E6
#define IDE_BUS0_COMMAND0               0x03F6

#define IDE_COMMAND_ATAPI_EJECT         0x1B
#define IDE_COMMAND_PIO28_READ          0x20
#define IDE_COMMAND_PIO48_READ          0x24
#define IDE_COMMAND_DMA48_READ          0x25
#define IDE_COMMAND_PIO28_WRITE         0x30
#define IDE_COMMAND_PIO48_WRITE         0x34
#define IDE_COMMAND_DMA48_WRITE         0x35
#define IDE_COMMAND_ATAPI               0xA0
#define IDE_COMMAND_ID_ATAPI            0xA1
#define IDE_COMMAND_ATAPI_READ          0xA8
#define IDE_COMMAND_DMA28_READ          0xC8
#define IDE_COMMAND_DMA28_WRITE         0xCA
#define IDE_COMMAND_ID_ATA              0xEC

#define IDE_DATA_FEATURE                0x01
#define IDE_DATA_COUNT0                 0x02
#define IDE_DATA_LBA0                   0x03
#define IDE_DATA_LBA1                   0x04
#define IDE_DATA_LBA2                   0x05
#define IDE_DATA_SELECT                 0x06
#define IDE_DATA_COMMAND                0x07
#define IDE_DATA_COUNT1                 0x08
#define IDE_DATA_LBA3                   0x09
#define IDE_DATA_LBA4                   0x0A
#define IDE_DATA_LBA5                   0x0B

#define IDE_ID_TYPE                     0x00
#define IDE_ID_SERIAL                   0x0A
#define IDE_ID_MODEL                    0x1B
#define IDE_ID_CAP                      0x31
#define IDE_ID_VALID                    0x35
#define IDE_ID_LBA28MAX                 0x3C
#define IDE_ID_SUPPORT                  0x53
#define IDE_ID_LBA48MAX                 0x64

#define IDE_STATUS_ERROR                (1 << 0)
#define IDE_STATUS_DRQ                  (1 << 3)
#define IDE_STATUS_SRV                  (1 << 4)
#define IDE_STATUS_DF                   (1 << 5)
#define IDE_STATUS_RDY                  (1 << 6)
#define IDE_STATUS_BUSY                 (1 << 7)

#define IDE_DEVICE_TYPE_UNKNOWN         0x00
#define IDE_DEVICE_TYPE_ATA             0x01
#define IDE_DEVICE_TYPE_ATAPI           0x02
#define IDE_DEVICE_TYPE_SATA            0x03
#define IDE_DEVICE_TYPE_SATAPI          0x04

#define IDE_IRQ_PRIMARY                 0x0E
#define IDE_IRQ_SECONDARY               0x0F

#define IDE_BUS_TYPE                    0x0002
#define IDE_DEVICE_TYPE                 0x0002

struct ide_bus;

struct ide_partition
{

    unsigned char boot;
    unsigned char headStart;
    unsigned char sectorStart;
    unsigned char cylinderStart;
    unsigned char systemId;
    unsigned char headEnd;
    unsigned char sectorEnd;
    unsigned char cylinderEnd;
    unsigned int sectorLba;
    unsigned int sectorTotal;

} __attribute__((packed));

struct ide_device
{

    struct base_device base;
    struct ide_bus *bus;
    struct pci_device *pciDevice;
    unsigned int irq;
    unsigned int type;
    unsigned int slave;
    unsigned char model[41];
    void (*configure_ata)(struct ide_device *self);
    void (*configure_atapi)(struct ide_device *self);
    unsigned int lba28Max;
    unsigned int (*read_lba28)(struct ide_device *self, unsigned int sector, unsigned int count, void *buffer);
    void (*read_lba28_async)(struct ide_device *self, unsigned int sector, unsigned int count, void *buffer);
    unsigned int (*write_lba28)(struct ide_device *self, unsigned int sector, unsigned int count, void *buffer);
    void (*write_lba28_async)(struct ide_device *self, unsigned int sector, unsigned int count, void *buffer);
    unsigned int lba48MaxLow;
    unsigned int lba48MaxHigh;
    unsigned int (*read_lba48)(struct ide_device *self, unsigned int sectorlow, unsigned int sectorhigh, unsigned int count, void *buffer);
    void (*read_lba48_async)(struct ide_device *self, unsigned int sectorlow, unsigned int sectorhigh, unsigned int count, void *buffer);
    unsigned int (*write_lba48)(struct ide_device *self, unsigned int sectorlow, unsigned int sectorhigh, unsigned int count, void *buffer);
    void (*write_lba48_async)(struct ide_device *self, unsigned int sectorlow, unsigned int sectorhigh, unsigned int count, void *buffer);

};

struct ide_bus
{

    struct base_bus base;
    unsigned short control;
    unsigned short data;
    struct ide_device devices[8];
    unsigned int devicesCount;
    void (*sleep)(struct ide_bus *self);
    void (*wait)(struct ide_bus *self);
    void (*select)(struct ide_bus *self, unsigned char operation, unsigned int slave);
    void (*set_lba)(struct ide_bus *self, unsigned char count, unsigned char lba0, unsigned char lba1, unsigned char lba2);
    void (*set_lba2)(struct ide_bus *self, unsigned char count, unsigned char lba3, unsigned char lba4, unsigned char lba5);
    void (*set_command)(struct ide_bus *self, unsigned char command);
    unsigned int (*read_block)(struct ide_bus *self, unsigned int count, void *buffer);
    unsigned int (*read_blocks)(struct ide_bus *self, unsigned int count, void *buffer);
    unsigned int (*write_block)(struct ide_bus *self, unsigned int count, void *buffer);
    unsigned int (*write_blocks)(struct ide_bus *self, unsigned int count, void *buffer);

};

void ide_init_device(struct ide_device *device, struct ide_bus *bus, unsigned int irq, unsigned int slave, unsigned int type);
void ide_init_bus(struct ide_bus *bus, unsigned int control, unsigned int data);
