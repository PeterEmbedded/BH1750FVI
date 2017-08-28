/*
  This is a simple example to test the BH1750 Light sensor
  
  Connect the sensor to a NodeMCU ESP8266 as follows:
  VCC  <-> 3V3 grijs
  GND  <-> Gnd paars
  SDA  <-> D2  groen
  SCL  <-> D1  blauw
  ADDR <-> RX  geel
 */

#include <Wire.h>
#include <BH1750FVI.h>

// Settings
uint8_t ADDRESSPIN = 13;
BH1750FVI::eDeviceAddress_t DEVICEADDRESS = BH1750FVI::k_DevAddress_H;
BH1750FVI::eDeviceMode_t DEVICEMODE = BH1750FVI::k_DevModeContHighRes;

// Create the Lightsensor instance
BH1750FVI LightSensor(ADDRESSPIN, DEVICEADDRESS, DEVICEMODE);

void setup() 
{
  Serial.begin(115200);
  LightSensor.begin();  
  Serial.println("Running...");
}

void loop()
{
  uint16_t lux = LightSensor.GetLightIntensity();
  Serial.printf("Light: %d lux", lux);
  delay(250);
}
