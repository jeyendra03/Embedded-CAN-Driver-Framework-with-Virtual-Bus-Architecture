#include <stdio.h>
#include "ecu.h"

int ECU_Send(const ECU_Node *node, uint32_t id, uint8_t dlc, const uint8_t *data)
{
    CAN_Frame frame;
    uint8_t i;

    frame.id = id;
    frame.dlc = dlc;

    for (i = 0; i < dlc && i < 8; i++)
    {
        frame.data[i] = data[i];
    }

    if (canDriver.write(&frame) != 0)
    {
        printf("[%s] TX FAILED -> ID: 0x%X (bus full)\n", node->name, id);
        return -1;
    }

    printf("[%s] TX -> ID: 0x%X, DLC: %d\n", node->name, frame.id, frame.dlc);
    return 0;
}

int ECU_Receive(const ECU_Node *node, CAN_Frame *frame)
{
    if (canDriver.read(frame) != 0)
    {
        printf("[%s] RX FAILED (bus empty)\n", node->name);
        return -1;
    }

    printf("[%s] RX <- ID: 0x%X, DLC: %d\n", node->name, frame->id, frame->dlc);
    return 0;
}
