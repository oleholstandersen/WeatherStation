
#include <RCSwitch.h>
#include <TinyBME280.h>
#include <ArduinoUniqueID.h>

#define DELAY_MS 30000      //30 secs

#ifndef DELAY_MS
  #define DELAY_MS 300000   //5 mins
#endif

#define RF_PIN 3
#define LED_PIN 1

RCSwitch rfSwitch = RCSwitch();
char idBuf[UniqueIDsize+1];
uint8_t hash = 0;
uint8_t messageNo = 0;

void setup() {                                                                      
  rfSwitch.enableTransmit(RF_PIN);
  pinMode(LED_PIN, OUTPUT);
  BME280setup();
  for (size_t i = 0; i < UniqueIDsize; i++)
  {
    digitalWrite(LED_PIN, HIGH);
    hash = (hash*31) ^ UniqueID[i];
    idBuf[i] = (UniqueID[i] <= 9 ? '0' + UniqueID[i] : 'A' - 10 + UniqueID[i]);
    rfSwitch.send(UniqueID[i], 4);
    digitalWrite(LED_PIN, LOW);
    delay(10);
  }
  idBuf[UniqueIDsize] = '\0';
}

void loop() {
  digitalWrite(LED_PIN, HIGH);
  char data[134];
  sprintf(
    data,
    "{\"temp\": %d,\"pres\": %u,\"hum\": %u,\"id\": \"%s\",\"batt\": %d}",
    BME280temperature(), 
    BME280pressure(),
    BME280humidity(),
    idBuf,
    min(22528/analogRead(12),100));
  sendString(data);
  digitalWrite(LED_PIN, LOW);
  delay(DELAY_MS);//Wait for the defined delay
}

void sendString(char *str) {
  int i = 0;
  while(str[i] != '\0' && i <= 0xFF)
  {
    rfSwitch.send(
      ((uint32_t)hash << 24) | ((uint32_t)messageNo << 16) | ((uint32_t)i << 8) | ((uint32_t)str[i]),
      32);
     i++;
     delay(10);
  }
  messageNo++;
}
