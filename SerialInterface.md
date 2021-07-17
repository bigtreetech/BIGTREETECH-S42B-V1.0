# Serial Protocol
Below is a brief description of the serial interface protocol used by TrueStep.

## Packet Format
The serial packet consists of the following parts:
[PREAMBLE] [LENGTH] [MESSAGE_ID] [PAYLOAD] [CHECKSUM]

[PREAMBLE]
Start byte which is 0xFE
 
[LENGTH]
Byte which holds the packet length and a packet sequence number. 
The lower 4 bits are the length of the payload. So maximum payload size is 15 bytes.
The upper 4 bits is a sequence number which is increased with each packet and rolls over after 15. The idea is to be able to detect missing packets.


[MESSAGE_ID]
Number which identify the message content. The id's are defined in messages.h

[PAYLOAD]
The message data
 
[CHECKSUM]
Single CRC8 checksum byte calculated over the LENGTH, MESSAGE_ID and PAYLOAD bytes. 
The checksum seed is 0xFF.


## Messages
Below are some of the messages that has already been defined in messages.h

### Parameter Reading
Use message ID value SERIAL_MSG_GETPARAM (Message ID 2).
First byte of the payload contains the parameter ID.
The response message depends on the requested parameter. For example when reading the PID P term gain the response would be of type SERIAL_MSG_PARAM_KP.

### Parameter Writing
Use message ID value SERIAL_MSG_SETPARAM (Message ID 3).
First byte of the payload contains the parameter ID, the next two bytes are the value to set the parameter.

