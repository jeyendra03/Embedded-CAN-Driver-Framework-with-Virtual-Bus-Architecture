#ifndef ECU_H
#define ECU_H

#include <stdint.h>
#include "CAN_Driver.h"

typedef struct
{
    const char *name;
} ECU_Node;

int ECU_Send(const ECU_Node *node, uint32_t id, uint8_t dlc, const uint8_t *data);
int ECU_Receive(const ECU_Node *node, CAN_Frame *frame);

#endif
