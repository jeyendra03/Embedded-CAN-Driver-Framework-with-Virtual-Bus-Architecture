#include <stdio.h>
#include "CAN_Driver.h"
#include "ecu.h"

int main(void)
{
    uint8_t txData[8] = { 0x55, 0xAA, 0, 0, 0, 0, 0, 0 };
    CAN_Frame rxFrame;

    ECU_Node ecuA = { .name = "ECU_A" };
    ECU_Node ecuB = { .name = "ECU_B" };

    canDriver.init();

    ECU_Send(&ecuA, 0x101, 2, txData);
    ECU_Receive(&ecuB, &rxFrame);

    canDriver.close();

    return 0;
}
