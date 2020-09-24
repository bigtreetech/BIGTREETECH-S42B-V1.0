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
    PARSE_STATE_COMMAND,
    PARSE_STATE_PAYLOAD,
    PARSE_STATE_CHECKSUM
}parseState;


uint8_t parseBuffer[20];
uint8_t pbIndex;
uint8_t pBytes;
uint8_t cBytes;
uint8_t seq;


void ResetParser();


void Init()
{
    ResetParser();
}


void VerifySequence()
{
    // Extract sequence
    uint8_t msgSeq = ((parseBuffer[0] & 0xF0) >> 4);

    if (seq != msgSeq)
    {
        error += MSG_ERROR_SEQUENCE;
        seq = msgSeq;
        return;
    }

    seq++;
}


void VerifyChecksum()
{
    u_int16_t crc = 0xffff;
    uint8_t len = (parseBuffer[0] & 0x0F);

    for (uint8_t i = 0; i < len; i++)
        {
            uint8_t ch = (uint8_t)(parseBuffer[i] ^ (uint8_t)(crc & 0x00ff));
            ch = (uint8_t)(ch ^ (ch << 4));
            crc = (uint16_t)((crc >> 8) ^ (ch << 8) ^ (ch << 3) ^ (ch >> 4));
        }

    if (((crc & 0xFF00) != parseBuffer[len - 2]) || ((crc & 0x00FF) != parseBuffer[len - 1]))  
        error += MSG_ERROR_CHECKSUM;
}


void ResetParser()
{
    pbIndex = 0;
    cBytes = 2;
    error = MSG_ERROR_NONE;
}


uint8_t Parse(uint8_t data)
{
    switch(parseState)
    {
        case PARSE_STATE_IDLE:
            if (data == 0xFE)
                parseState = PARSE_STATE_PREAMBLE;
        break;

        case PARSE_STATE_PREAMBLE:  // Read sequence and length
            pBytes = data & 0x0F;
            parseState = PARSE_STATE_LENGTH;
        break;

        case PARSE_STATE_LENGTH:    // Read command
            parseState = PARSE_STATE_COMMAND;
        break;

        case PARSE_STATE_COMMAND:   // Read payload
            pBytes--;

            if (pBytes < 1) 
                parseState = PARSE_STATE_PAYLOAD;
        break;

        case PARSE_STATE_PAYLOAD:    // Read checksum
            cBytes--;

            if (cBytes < 1)
            { 
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
                    return PARSE_STATUS_NEW_PACKET;
                }        
            }
        break;

        default:
        break;

    }

    if (parseState != PARSE_STATE_IDLE)
    {
        parseBuffer[pbIndex] = data;
        pbIndex++;
        return PARSE_STATUS_PARSING;
    }
    
    return PARSE_STATUS_IDLE;
}