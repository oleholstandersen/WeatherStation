#include <RCSwitch.h>
#include <TinyBME280.h>
#include <ArduinoUniqueID.h>


RCSwitch rfSwitch = RCSwitch();
String id;
char data[200];

void setup() {
  Serial.begin(9600);
  rfSwitch.enableTransmit(10);
  BME280setup();
  id = "";
  char hex[4];
  for (size_t i = 0; i < UniqueIDsize; i++)
  {
    sprintf(hex, "%X", UniqueID[i]);
    id.concat(hex);
  }
}

void loop() {
  sprintf(
    data,
    "{\"temp\": %d,\"pres\": %u,\"hum\": %u,\"id\": \"%s\"}",
    BME280temperature(), 
    BME280pressure(),
    BME280humidity(),
    id);
  rfSwitch.send(data);
  delay(300000);//Wait 5 minutes
}
