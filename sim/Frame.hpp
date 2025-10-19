#ifndef FRAME_HPP
#define FRAME_HPP

#include <cstdint>

#define PORT 7979
#define INET_ADDR "127.0.0.1"

enum Command : uint8_t
{
    CMD_CLOSE = 0x0,
    CMD_HELLO = 0x1,
    CMD_WRITE = 0x2,
    CMD_READ = 0x3,
};

typedef struct
{
    uint8_t type : 1; // 1=request, 0=response
    uint8_t cmd : 7;  // Command
    uint16_t len;     // sizeof buffer
    uint8_t status;
    union
    {
        uint8_t data[250];
        struct
        {
            char varName[64];
        } read;
        struct
        {
            char varName[64];
            uint8_t data[250 - 64];
        } write;
    } data;
} __attribute__((packed, aligned(4))) CmdFrame; // PDU 258

//-> 1|cmd | len | buffer[] | checksum
//<- 0|cmd | anslen | buffer[] | checksum

#endif
