#ifndef DRIVER_H
#define DRIVER_H

typedef struct
{
    int (*init)(void);
    int (*read)(void *buffer);
    int (*write)(void *buffer);
    int (*close)(void);

} DriverOps;

#endif
