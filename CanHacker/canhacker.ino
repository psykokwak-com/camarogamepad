// Dependencies
// https://github.com/autowp/arduino-canhacker
// https://github.com/autowp/arduino-mcp2515


#include <can.h>
#include <mcp2515.h>

#include <CanHacker.h>
#include <CanHackerLineReader.h>
#include <lib.h>

#include <SPI.h>
//----------------------------------------------------------------------
//------ESP32 CAN BUS setting ---------------------------------------
const int SPI_CS_PIN = 5;
const int INT_PIN = 21;
//---------------------------------------------------------------------
#ifndef LED_BUILTIN
#define LED_BUILTIN 2
#endif

const int SS_RX_PIN = 3;
const int SS_TX_PIN = 4;
//----------------------------------------------------------------
TaskHandle_t hled;
TaskHandle_t huart;
TaskHandle_t hcan;

CanHackerLineReader *lineReader = NULL;
CanHacker *canHacker = NULL;

void setup() {
  Serial.begin(2000000);
  SPI.begin();

  Stream *interfaceStream = &Serial;
  Stream *debugStream = NULL; // Maybe need to patch CanHacker class


  canHacker = new CanHacker(interfaceStream, debugStream, SPI_CS_PIN);

  canHacker->setClock(MCP_16MHZ);
  //canHacker->enableLoopback(); // uncomment this for loopback
  lineReader = new CanHackerLineReader(canHacker);

  pinMode(INT_PIN, INPUT);
}
void loop() {
  vCANTask(NULL);
}

static void vCANTask(void *pvParameters)
{
  (void)pvParameters;

  Serial.println(F("CANTask at core:"));
  Serial.println(xPortGetCoreID());
  for (;;)
  {
    CanHacker::ERROR error;

    if (digitalRead(INT_PIN) == LOW) {
      error = canHacker->processInterrupt();
      handleError(error);
    }

    error = lineReader->process();
    handleError(error);
  }
  vTaskDelay(1);
}

void handleError(const CanHacker::ERROR error) {

  switch (error) {
  case CanHacker::ERROR_OK:
  case CanHacker::ERROR_UNKNOWN_COMMAND:
  case CanHacker::ERROR_NOT_CONNECTED:
  case CanHacker::ERROR_MCP2515_ERRIF:
  case CanHacker::ERROR_INVALID_COMMAND:
    return;

  default:
    break;
  }

  digitalWrite(SPI_CS_PIN, HIGH);
  pinMode(LED_BUILTIN, OUTPUT);

  while (1) {
    int c = (int)error;
    for (int i = 0; i < c; i++) {
      digitalWrite(LED_BUILTIN, HIGH);
      delay(500);
      digitalWrite(LED_BUILTIN, LOW);
      delay(500);
    }

    delay(2000);
  };
}