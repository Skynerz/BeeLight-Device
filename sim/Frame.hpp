#ifndef FRAME_HPP
#define FRAME_HPP

#include <cstdint>

#define PORT 7979

enum Command : uint8_t
{
    CMD_HELLO = 0x1,
    CMD_WRITE = 0x2,
    CMD_READ = 0x3,
};

typedef struct
{
    uint8_t tx : 1;  // 1=request, 0=response
    uint8_t cmd : 7; // Command
    uint16_t len;    // sizeof buffer
    uint8_t status;
    union
    {
        uint8_t data[250];
        struct
        {
            char varName[16];
        } read;
        struct
        {
            char varName[16];
            uint8_t data[250 - 16];
        } write;
    } data;
} __attribute__((packed, aligned(4))) CmdFrame; // PDU 258

//-> 1|cmd | len | buffer[] | checksum
//<- 0|cmd | anslen | buffer[] | checksum

#endif