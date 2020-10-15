/*
 * Serial messages 
 * 
 * Contributors:
 * - Jan Swanepoel, 2020
 * 
 * Packet Format
 * -------------
 * [PREAMBLE][LENGTH][MESSAGE_ID][PAYLOAD][CHECKSUM]
 * 
 * [PREAMBLE]
 * Start byte which is 0xFE
 * 
 * [LENGTH]
 * Actually holds length and a sequence number. 
 * The lower 4 bits are the length of the payload. So maximum payload size is 15 bytes.
 * The upper 4 bits is a sequence number which is increased with each packet.
 * 
 * [MESSAGE_ID]
 * Number which identify the message type.
 * 
 * [PAYLOAD]
 * The message data
 * 
 * [CHECKSUM]
 * Single CRC8 checksum byte calculated over the LENGTH, MESSAGE_ID and PAYLOAD bytes. 
 * The checksum seed is 0xFF.
*/

#include "stdio.h"
#include "messages.h"

#define PACKET_LENGTH_MAX       20              // Preamble + Length + Command + Payload + Checksum

            
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

uint8_t parseBuffer[PACKET_LENGTH_MAX];     // Buffer used for parsing the received message
uint8_t msgLength;                          // Length of the received message without preamble
uint8_t packetBuffer[PACKET_LENGTH_MAX];    // Hold the last generated packet

void Serial_Init();
uint8_t Serial_Parse(uint8_t data);
uint8_t Serial_GeneratePacket(uint8_t msgId, uint8_t *payload, uint8_t payloadSize);
