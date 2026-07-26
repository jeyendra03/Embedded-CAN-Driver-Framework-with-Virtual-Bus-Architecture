# Embedded CAN Driver Framework with Virtual Bus Architecture

A modular Embedded C firmware framework that simulates two ECUs (nodes)
communicating over a CAN bus. Since no physical CAN hardware is used, a
**virtual CAN bus** — a ring buffer — stands in for the real bus, so the
full software stack (application → ECU layer → driver → bus) can be
built and tested without a microcontroller.

This is an ongoing project. Current phase: two ECUs exchanging a single
CAN frame over a shared ring buffer, using a function-pointer-based
driver interface (HAL) so the application layer never depends on how
the bus is actually implemented.

## Architecture

```
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
```

**One-line flow:** ECU_A → `ECU_Send` → `canDriver.write` → `CANBus_Push`
copies the frame into `bus[tail]`. ECU_B → `ECU_Receive` → `canDriver.read`
→ `CANBus_Pop` copies it out of `bus[head]`. `bus[]` in `Can_bus.c` is the
only shared state — both ECUs go through the same `canDriver` interface
without knowing about each other.

## File structure

```
DD_framework/
├── Main.c              application entry point
├── inc/
│   ├── config.h         shared constants (BUS_SIZE, MAX_ECUS)
│   ├── driver.h          generic DriverOps interface (function pointers)
│   ├── CAN_Driver.h       CAN_Frame struct + driver prototypes
│   ├── Can_bus.h          virtual bus prototypes
│   └── ecu.h              ECU_Node struct + ECU_Send/ECU_Receive prototypes
└── src/
    ├── CAN_Driver.c       HAL implementation, registers canDriver
    ├── Can_bus.c          ring buffer implementation
    └── ecu.c              ECU-level send/receive logic
```

## Functions

| File | Function | Purpose |
|---|---|---|
| `CAN_Driver.c` | `CAN_Init()` | Prints status, calls `CANBus_Init()` |
| `CAN_Driver.c` | `CAN_Read(void*)` | Casts buffer to `CAN_Frame*`, calls `CANBus_Pop()` |
| `CAN_Driver.c` | `CAN_Write(void*)` | Casts buffer to `CAN_Frame*`, calls `CANBus_Push()` |
| `CAN_Driver.c` | `CAN_Close()` | Prints shutdown status |
| `Can_bus.c` | `CANBus_Init()` | Resets `head`, `tail`, `count` |
| `Can_bus.c` | `CANBus_Push(const CAN_Frame*)` | Enqueues a frame; returns -1 if full |
| `Can_bus.c` | `CANBus_Pop(CAN_Frame*)` | Dequeues a frame; returns -1 if empty |
| `ecu.c` | `ECU_Send(node, id, dlc, data)` | Builds a frame, sends via `canDriver.write` |
| `ecu.c` | `ECU_Receive(node, frame)` | Receives via `canDriver.read` |

