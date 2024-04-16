// Psykokwak 2024
// https://github.com/psykokwak-com/camarogamepad

#include <mcp2515.h>
#include <XInput.h>

MCP2515 mcp2515(10);

#define CAN_ID_BRAKE    0x0BE
#define CAN_ID_TROTTLE  0x1A1
#define CAN_ID_STEERING 0x1E5

void printCanFrame(struct can_frame &frame) {
  Serial.print(frame.can_id, HEX);
  Serial.print(" ");
  Serial.print(frame.can_dlc, HEX);
  Serial.print(" ");
  for (int i = 0; i < frame.can_dlc; i++) {
    Serial.print(frame.data[i], HEX);
    Serial.print(" ");
  }
  Serial.println();
}

void setup() {
  Serial.begin(115200);

  mcp2515.reset();
  mcp2515.setBitrate(CAN_500KBPS, MCP_16MHZ);

  mcp2515.setFilterMask(MCP2515::MASK0, false, 0x7FF);
  mcp2515.setFilterMask(MCP2515::MASK1, false, 0x7FF);
  mcp2515.setFilter(MCP2515::RXF0, false, CAN_ID_BRAKE);
  mcp2515.setFilter(MCP2515::RXF1, false, CAN_ID_TROTTLE);
  mcp2515.setFilter(MCP2515::RXF2, false, CAN_ID_STEERING);

  //mcp2515.setLoopbackMode();
  mcp2515.setNormalMode();

  XInput.setTriggerRange(0, 255);
  XInput.setJoystickRange(-9000, 9000);
  XInput.begin();
}

unsigned long t = 0;
void loop() {
  struct can_frame canMsg;
  if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK) {
    //printCanFrame(canMsg);
    switch (canMsg.can_id)
    {
    case CAN_ID_BRAKE:
    {
      digitalWrite(LED_BUILTIN_TX, !digitalRead(LED_BUILTIN_TX));

      int v = canMsg.data[1];

      //Serial.print("Got CAN_ID_BRAKE : ");
      //Serial.println(v);

      XInput.setTrigger(TRIGGER_LEFT, v);
      //Joystick.setBrake(canMsg.data[1]);
      break;
    }
    case CAN_ID_TROTTLE:
    {
      digitalWrite(LED_BUILTIN_RX, !digitalRead(LED_BUILTIN_RX));

      int v = canMsg.data[6];

      //Serial.print("Got CAN_ID_TROTTLE : ");
      //Serial.println(v);

      XInput.setTrigger(TRIGGER_RIGHT, v);
      //Joystick.setAccelerator(canMsg.data[6]);
      break;
    }
    case CAN_ID_STEERING:
    {
      int v = ((canMsg.data[5] << 8) | (canMsg.data[6] << 0)) * 1.0;

      //Serial.print("Got CAN_ID_STEERING : ");
      //Serial.println(v);

      XInput.setJoystickX(JOY_LEFT, v);
      //Joystick.setSteering(((canMsg.data[5] << 8) | (canMsg.data[6] << 0)) * 1.0);
      break;
    }
    }
  }
}