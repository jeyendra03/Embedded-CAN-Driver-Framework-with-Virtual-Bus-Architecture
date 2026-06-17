#include <stdio.h>
#include "Can_bus.h"
#include "CAN_Driver.h"


int CAN_Init(void)
{
    printf("CAN Driver Initialized\n");
    return 0;
}

int CAN_Read(void *buffer)
{
    CAN_Frame *frame = (CAN_Frame *)buffer;

    return CANBus_Pop(frame);
}

int CAN_Write(void *buffer)
{
    CAN_Frame *frame = (CAN_Frame *)buffer;

    return CANBus_Push(frame);
}

int CAN_Close(void)
{
    printf("CAN Driver Closed\n");
    return 0;
}

DriverOps canDriver =
{
    .init  = CAN_Init,
    .read  = CAN_Read,
    .write = CAN_Write,
    .close = CAN_Close
};