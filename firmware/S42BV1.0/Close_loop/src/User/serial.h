/*
 * Jan Swanepoel, 2020
*/

#include "stdio.h"

#define PACKET_LENGTH_MAX   21          // Preamble + Length + Command + Payload + Checksum

enum PARSE_STATUS
{
    PARSE_STATUS_IDLE,
    PARSE_STATUS_PARSING,
    PARSE_STATUS_NEW_PACKET,
    PARSE_STATUS_BAD_PACKET
};

// Message verification error bitfield
enum MSG_ERROR
{
    MSG_ERROR_NONE = 0,
    MSG_ERROR_SEQUENCE = 1,
    MSG_ERROR_CHECKSUM = 2,
} error;

void Init();
uint8_t Parse(uint8_t data);
//void GeneratePacket();