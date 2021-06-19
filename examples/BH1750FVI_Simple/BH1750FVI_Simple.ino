

// Includes: Wifi, MQTT, JSON
#include<ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

// Includes: Sensors
#include <Wire.h>
#include "DHT.h"
#include <BH1750FVI.h>

// Defines: Wifi and MQTT 
#define mqtt_server "192.168.1.8"

// Defines: Sensors
#define DHTPIN 2  //D4
#define DHTTYPE DHT11


// Declarations: Wifi and MQTT
char* ssid     = " xx ";
const char* password = " xx ";
WiFiClient espClient;
PubSubClient client(espClient);

// Declarations: JSON
StaticJsonDocument<1024> jsonDoc;

typedef struct keyAndValue_ {
//   char key[100];
//   char value[100];
   String key;
   String value;
} keyAndValue_t;


// Declarations: Sensors
DHT DHT_dht(DHTPIN, DHTTYPE);
uint8_t ADDRESSPIN = 13;
BH1750FVI::eDeviceAddress_t DEVICEADDRESS = BH1750FVI::k_DevAddress_H;
BH1750FVI::eDeviceMode_t DEVICEMODE = BH1750FVI::k_DevModeContHighRes;
BH1750FVI LightSensor(ADDRESSPIN, DEVICEADDRESS, DEVICEMODE);



void setup() {
  Serial.begin(115200);
  Serial.println("");
  Serial.println("Starting connecting WiFi.");
  delay(1000);
  yield();
  WiFi.begin(ssid, password);
  yield();
  while (WiFi.status() != WL_CONNECTED) {
    yield();
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  // setup sensors
  DHT_dht.begin();
  LightSensor.begin(); 
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("sensorstation")) {
      Serial.println("connected");
      client.subscribe("sensorstation/command");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length)
{
  Serial.println(" callback");
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  delay(5000);  
  //MeasureDHT();
  //MeasureFakeSensor(); //word deels overschreven door de volgende, return value van jsondoc maken en aan publish voeren?
  MeasureBH1750FVI();  //en de dubbele komt niet aan bij nodered...
  PublishValues();
}

void PublishValues() {
  char buffer[1024];
  serializeJson(jsonDoc, buffer);
  Serial.println(buffer);
  client.publish("sensorstation/json", buffer);  
  Serial.println("Published");
}

// DHT
void MeasureDHTorg()
{
  /*
    DHTPIN 2  //D4
  */

  float DHT_temp = 0;
  float DHT_hum = 0;
  float DHT_heatindex = 0;
  char tempString[8];
  
  DHT_temp = DHT_dht.readTemperature();
  DHT_hum = DHT_dht.readHumidity();
  DHT_heatindex = DHT_dht.computeHeatIndex(DHT_temp, DHT_hum, false);
  //if (isnan(DHT_temp) || isnan(DHT_hum) || isnan(DHT_heatindex)) {
  //  Serial.println("Failed to read from DHT sensor!");
  //  return;
  //}

//todo, create root above sensor (buiten deze dht.
  //add values to json
  jsonDoc["sensor"] = "dht11";
  jsonDoc["sensornumber"] = 1;
  jsonDoc["data"]["temperature"] = dtostrf(DHT_temp, 5, 2, tempString);
  jsonDoc["data"]["humidity"] = dtostrf(DHT_hum, 5, 2, tempString);
  //jsonDoc["data"]["heatindex"] = dtostrf(DHT_heatindex, 5, 2, tempString);
  //jsonDoc["data"]["heatindex"] = "1234";
  Serial.print("DHT_temp     : "); Serial.print(DHT_temp); Serial.println(" C");
  Serial.print("DHT_hum      : "); Serial.print(DHT_hum); Serial.println(" %");
  Serial.print("DHT_heatindex: "); Serial.print(DHT_heatindex); Serial.println(" *C");
  Serial.println("");
}

void AddData(String Sensor, uint8_t SensorNumber, keyAndValue_t SensorData[], uint8_t Count)
{  
  /*
  JsonObject measurement = jsonDoc.createNestedObject("measurement");
  measurement["sensor"] = Sensor;
  measurement["sensornumber"] = SensorNumber;

  JsonObject measurement_data = measurement.createNestedObject("data");
  //measurement_data["temperature"] = "24.10";
  //measurement_data["humidity"] = "57.00";  
  for (int x = 0; x <= 1; x++) {
    //for (int x = 0; x <= sizeof(SensorData)/sizeof(SensorData[0]); x++) {
    measurement_data[SensorData[x].key] = SensorData[x].value;    
  }*/

  jsonDoc["measurement"]["sensor"] = Sensor;
  jsonDoc["measurement"]["sensornumber"] = SensorNumber;
  for (int x = 0; x < Count; x++) {
    jsonDoc["measurement"]["data"][String(SensorData[x].key)] = SensorData[x].value;
  }

  Serial.println("AddData:");
  for (int x = 0; x < Count; x++) {
    Serial.print(SensorData[x].key);
    Serial.println(SensorData[x].value);
  }
  Serial.println("-------");
}

void AddDataold(String Sensor, uint8_t SensorNumber, String DataName, String DataValue)
{  
  String Name(DataName);
  jsonDoc["measurement"]["sensor"] = Sensor;
  jsonDoc["measurement"]["sensornumber"] = SensorNumber;
  jsonDoc["measurement"]["data"][Name] = DataValue;
}

// DHT
void MeasureDHT()
{
  /*
    DHTPIN 2  //D4
  */

  float DHT_temp = 0;
  float DHT_hum = 0;
  float DHT_heatindex = 0;
  char tempString[8];
  
  DHT_temp = DHT_dht.readTemperature();
  DHT_hum = DHT_dht.readHumidity();
  DHT_heatindex = DHT_dht.computeHeatIndex(DHT_temp, DHT_hum, false);
  //if (isnan(DHT_temp) || isnan(DHT_hum) || isnan(DHT_heatindex)) {
  //  Serial.println("Failed to read from DHT sensor!");
  //  return;
  //}

//todo, create root above sensor (buiten deze dht.
  //add values to json
  jsonDoc["sensor"] = "dht11";
  jsonDoc["sensornumber"] = 1;
  jsonDoc["data"]["temperature"] = dtostrf(DHT_temp, 5, 2, tempString);
  jsonDoc["data"]["humidity"] = dtostrf(DHT_hum, 5, 2, tempString);
  //jsonDoc["data"]["heatindex"] = dtostrf(DHT_heatindex, 5, 2, tempString);
  //jsonDoc["data"]["heatindex"] = "1234";
  Serial.print("DHT_temp     : "); Serial.print(DHT_temp); Serial.println(" C");
  Serial.print("DHT_hum      : "); Serial.print(DHT_hum); Serial.println(" %");
  Serial.print("DHT_heatindex: "); Serial.print(DHT_heatindex); Serial.println(" *C");
  Serial.println("");
}

void MeasureBH1750FVI()
{
  /*  
    VCC  <-> 3V3 [grey]
    GND  <-> Gnd [purple]
    SDA  <-> D2  [green]
    SCL  <-> D1  [blue]
    ADDR <-> RX  [yellow]
  */
  
  uint16_t lux = LightSensor.GetLightIntensity();
  Serial.print("Light: ");
  Serial.println(lux);

  keyAndValue_t SensorData[] = {
    {"lux", String(lux)}
  };

  // Serial.println("SensorData:");
  // for (int x = 0; x < sizeof(SensorData)/sizeof(SensorData[0]); x++) {
  //   Serial.print(SensorData[x].key);
  //   Serial.println(SensorData[x].value);
  // }
  //uint8_t Count = (sizeof(SensorData)/sizeof(SensorData[0]));    <---- werkt voor geen kloot
  AddData("BH1750VI", 1, SensorData, 1);
}

// FakeSensor
void MeasureFakeSensor()
{
  float FakeValA = 10.123;
  float FakeValB = 20.123;

  keyAndValue_t SensorData[] = {
    {"FakeValA", String(FakeValA, 2)},
    {"FakeValB", String(FakeValB, 3)}
  };

  //uint8_t Count = (sizeof(SensorData)/sizeof(SensorData[0]));
  AddData("FakeSensor", 1, SensorData, 2);
}
