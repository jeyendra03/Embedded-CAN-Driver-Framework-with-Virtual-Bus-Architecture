#ifndef CAN_BUS_H
#define CAN_BUS_H

#include "CAN_Driver.h"

#define BUS_SIZE 16

int CANBus_Init(void);
int CANBus_Push(const CAN_Frame *frame);
int CANBus_Pop(CAN_Frame *frame);

#endif