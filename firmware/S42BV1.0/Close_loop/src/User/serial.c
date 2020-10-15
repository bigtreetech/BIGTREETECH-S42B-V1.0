/*
 * Jan Swanepoel, 2020
*/

#include "serial.h"

// Parse states
enum PARSE_STATE
{
    PARSE_STATE_IDLE,
    PARSE_STATE_PREAMBLE,
    PARSE_STATE_LENGTH,
    PARSE_STATE_MESSAGEID,
    PARSE_STATE_PAYLOAD,
    PARSE_STATE_CHECKSUM
}parseState;


uint8_t pbIndex;
uint8_t pBytes;
uint8_t seqRead;
uint8_t seqWrite;


void VerifySequence()
{
    // Extract sequence
    uint8_t msgSeq = ((parseBuffer[0] & 0xF0) >> 4);

    if (seqRead != msgSeq)
    {
        error += MSG_ERROR_SEQUENCE;
        seqRead = msgSeq;
        return;
    }

    seqRead++;
}


// Perform CRC8 checksum calculation
uint8_t CalculateCRC8(uint8_t *data, size_t len)
{
    uint8_t crc = 0xff;
    size_t i, j;
    for (i = 0; i < len; i++) {
        crc ^= data[i];
        for (j = 0; j < 8; j++) {
            if ((crc & 0x80) != 0)
                crc = (uint8_t)((crc << 1) ^ 0x31);
            else
                crc <<= 1;
        }
    }
    return crc;
}


void VerifyChecksum()
{
    uint8_t len = (parseBuffer[0] & 0x0F);
    uint8_t crc = CalculateCRC8(parseBuffer, len + 2);

    if (crc != parseBuffer[len + 2])  
        error += MSG_ERROR_CHECKSUM;    
}


void ResetParser()
{
    pbIndex = 0;
    error = MSG_ERROR_NONE;
}


void Serial_Init()
{
    ResetParser();
}


uint8_t Serial_Parse(uint8_t data)
{
    switch(parseState)
    {
        case PARSE_STATE_IDLE:
            if (data == 0xFE)
                parseState = PARSE_STATE_PREAMBLE;
        break;

        case PARSE_STATE_PREAMBLE:  // Read sequence and length
            parseBuffer[pbIndex] = data;
            pBytes = data & 0x0F;
            msgLength = pBytes + 3;
            parseState = PARSE_STATE_LENGTH;
        break;

        case PARSE_STATE_LENGTH:    // Read message ID
            parseBuffer[pbIndex] = data;
            if (pBytes > 0)
                parseState = PARSE_STATE_MESSAGEID;
            else
                parseState = PARSE_STATE_PAYLOAD;   // Skip payload for 0 length payload
        break;

        case PARSE_STATE_MESSAGEID:   // Read payload
            parseBuffer[pbIndex] = data;
            pBytes--;

            if (pBytes < 1) 
                parseState = PARSE_STATE_PAYLOAD;
        break;

        case PARSE_STATE_PAYLOAD:    // Read checksum
            parseBuffer[pbIndex] = data;
             
            parseState = PARSE_STATE_IDLE;
            ResetParser();

            VerifyChecksum();
            if (error == MSG_ERROR_NONE)
            {
                VerifySequence();
                return PARSE_STATUS_NEW_PACKET;
            }
            else
            {
                // Failed CRC check
                return PARSE_STATUS_BAD_PACKET;
            }                    
        break;

        default:
        break;

    }

    // Parse buffer only start storing after the preamble
    if (parseState > PARSE_STATE_PREAMBLE)
    {
        if (pbIndex < PACKET_LENGTH_MAX)
        {
            pbIndex++;
            return PARSE_STATUS_PARSING;
        }
        else
        {
            // Packet parse buffer full!
            ResetParser();
        }
        
    }
    
    return PARSE_STATUS_IDLE;
}


uint8_t Serial_GeneratePacket(uint8_t msgId, uint8_t *payload, uint8_t payloadSize)
{
    payloadSize = payloadSize & 0x0F;

    packetBuffer[0] = 0xFE;
    packetBuffer[1] = ((seqWrite << 4) & 0xF0) | payloadSize;
    packetBuffer[2] = msgId;
    
    for (int i = 0; i < payloadSize; i++)
        packetBuffer[3 + i] = payload[i];
    
    packetBuffer[3 + payloadSize] = CalculateCRC8(&packetBuffer[1], payloadSize + 2);

    seqWrite++;
    return payloadSize + 4;
}

