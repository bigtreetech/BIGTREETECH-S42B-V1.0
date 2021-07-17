#include "telemetry.h"
#include "serial.h"
#include "usart.h"
#include "main.h"

// TODO:
// - Make it more generic for future use.

uint32_t prevStreamTickCount;

void StreamAngle()
{
  if (tickCount < (prevStreamTickCount + 5))
    return;

  struct Serial_Msg_Angle streamedAng;
  streamedAng.angle = (float)y;

  // Use 'y' which is read in the controller loop
  uint8_t len = Serial_GeneratePacket(SERIAL_MSG_ANGLE, &streamedAng, sizeof(streamedAng));
  UART1_Write(packetBuffer, len);

  prevStreamTickCount = tickCount;
}