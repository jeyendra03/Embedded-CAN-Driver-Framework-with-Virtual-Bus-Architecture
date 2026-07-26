┌────────────────────────────────────────────────────────────┐
│ Main.c — Application layer                                  │
│   ECU_Node ecuA = {"ECU_A"}, ecuB = {"ECU_B"};               │
│   canDriver.init();                                          │
│   ECU_Send(&ecuA, 0x101, 2, txData);                         │
│   ECU_Receive(&ecuB, &rxFrame);                              │
│   canDriver.close();                                         │
└─────────────────────────┬─────────────────────────────────────┘
                          │ direct function calls
                          ▼
┌────────────────────────────────────────────────────────────┐
│ ecu.c / ecu.h — ECU (node) layer                             │
│   ECU_Send(node, id, dlc, data)                              │
│      → builds a local CAN_Frame                             │
│      → canDriver.write(&frame)                               │
│      → printf "[ECU_A] TX -> ID: 0x101"                      │
│   ECU_Receive(node, frame)                                   │
│      → canDriver.read(frame)                                 │
│      → printf "[ECU_B] RX <- ID: 0x101"                      │
└─────────────────────────┬─────────────────────────────────────┘
                          │ calls through canDriver's function pointers
                          ▼
┌────────────────────────────────────────────────────────────┐
│ driver.h + CAN_Driver.h/.c — HAL (registration point)         │
│   DriverOps canDriver = {                                    │
│       .init = CAN_Init,  .read = CAN_Read,                   │
│       .write = CAN_Write, .close = CAN_Close };               │
│   CAN_Write(buf) → casts to CAN_Frame* → CANBus_Push(frame)  │
│   CAN_Read(buf)  → casts to CAN_Frame* → CANBus_Pop(frame)   │
│   (this is the swappable layer — Main.c/ecu.c never call     │
│    CAN_Write/CAN_Read by name, only canDriver.write/.read)   │
└─────────────────────────┬─────────────────────────────────────┘
                          │ CANBus_Push() / CANBus_Pop()
                          ▼
┌────────────────────────────────────────────────────────────┐
│ Can_bus.c / .h — Virtual CAN bus (shared ring buffer)         │
│   static CAN_Frame bus[BUS_SIZE];                             │
│   static int head, tail, count;                               │
│                                                                │
│   CANBus_Push(frame):                                         │
│       if full → return -1                                     │
│       bus[tail] = *frame; tail = (tail+1)%BUS_SIZE; count++;  │
│   CANBus_Pop(frame):                                           │
│       if empty → return -1                                    │
│       *frame = bus[head]; head = (head+1)%BUS_SIZE; count--;  │
│                                                                │
│   config.h → BUS_SIZE 16  (capacity of bus[])                 │
└────────────────────────────────────────────────────────────┘
