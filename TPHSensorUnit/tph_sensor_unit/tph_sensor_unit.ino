#define TINY_BME280_I2C

#include <RCSwitch.h>
#include <TinyBME280.h>
#include <ArduinoUniqueID.h>

#define DELAY_MS 10000      //10 secs

#ifndef DELAY_MS
  #define DELAY_MS 300000   //5 mins
#endif

#define RF_PIN 3
#define LED_PIN 1

RCSwitch rfSwitch = RCSwitch();
tiny::BME280 bmp280;
char idBuf[UniqueIDsize+1];
uint8_t hash = 0;
uint8_t messageNo = 0;

void setup() {                                                                      
  rfSwitch.enableTransmit(RF_PIN);
  pinMode(LED_PIN, OUTPUT);
  bmp280.beginI2C(0x76);
  for (size_t i = 0; i < UniqueIDsize; i++)
  {
    digitalWrite(LED_PIN, HIGH);
    hash = (hash*31) ^ UniqueID[i];
    idBuf[i] = (UniqueID[i] <= 9 ? '0' + UniqueID[i] : 'A' - 10 + UniqueID[i]);
    //rfSwitch.send(UniqueID[i], 4);
    digitalWrite(LED_PIN, LOW);
    delay(10);
  }
  idBuf[UniqueIDsize] = '\0';
}

void loop() {
  digitalWrite(LED_PIN, HIGH);
  /*
  char data[134];
  sprintf(
    data,
    "{\"temp\":%d,\"pres\":%u,\"hum\":%u,\"id\":\"%s\",\"batt\":%d,\"ids\":%d}",
    BME280temperature(), 
    BME280pressure(),
    BME280humidity(),
    idBuf,
    min(22528/analogRead(12),100),
    UniqueIDsize);
  sendString(data);
  */
  sendInt32(bmp280.readFixedTempC(), 1);
  sendInt32(bmp280.readFixedPressure(), 2);
  sendInt32(bmp280.readFixedHumidity(), 3);
  sendInt32(min(22528/analogRead(12),100),4);
  sendInt32(UniqueIDsize,5);
  digitalWrite(LED_PIN, LOW);
  delay(DELAY_MS);//Wait for the defined delay
}

void sendInt32(int32_t val, int8_t no) {
  val = val | (((int32_t)no) << 24);
  if (val == 0)  {
    val = 0xFFFFFFFF;
  }
  rfSwitch.send(val, 32);
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
