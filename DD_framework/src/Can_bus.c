#include <string.h>
#include "Can_bus.h"
#include "config.h"

static ECU *ecuList[MAX_ECUS];

static int ecuCount = 0;

static int head = 0;
static int tail = 0;
static int count = 0;

int CANBus_Init(void)
{
    head = 0;
    tail = 0;
    count = 0;

    return 0;
}

int CANBus_Push(const CAN_Frame *frame)
{
    if(count >= BUS_SIZE)
    {
        return -1;
    }

    bus[tail] = *frame;

    tail = (tail + 1) % BUS_SIZE;
    count++;

    return 0;
}

int CANBus_Pop(CAN_Frame *frame)
{
    if(count == 0)
    {
        return -1;
    }

    *frame = bus[head];

    head = (head + 1) % BUS_SIZE;
    count--;

    return 0;
}