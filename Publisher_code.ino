/* Name: Grace Qu and Hana Ba-Sabaa
 * Program name: Lab9 IoT
 * Date created: 3/15/22
 * Description:  Publisher Arduino code that includes light sensor controlling an LED light (or temperature sensor controlling the DC motor)
 *
 * Dependencies: (what other source files are required) WiFiNINA.h, ArduinoMqttClient.h arduino_secrets.h

 * I/O pins:  (what ports are used, and what are they wired to?) Analog pin 0 (A0), connected to light (or temperature) senssor
 * Revisions: (use this to track the evolution of your program) Based on MQTT sender code (referred in the documentation)
 * 
 */

#include "config.h"
#include <ArduinoMqttClient.h>
#include <WiFiNINA.h>
#include "arduino_secrets.h"

char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

const char broker[] = "test.mosquitto.org";
int        port     = 1883;
const char topic[]  = "sustainable_house";


//set interval for sending messages (milliseconds)
const long interval = 8000;
unsigned long previousMillis = 0;

int count = 0;

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // attempt to connect to Wifi network:
  Serial.print("Attempting to connect to WPA SSID: ");
  Serial.println(ssid);
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    // failed, retry
    Serial.print(".");
    delay(5000);
  }

  Serial.println("You're connected to the network");
  Serial.println();

  Serial.print("Attempting to connect to the MQTT broker: ");
  Serial.println(broker);

  if (!mqttClient.connect(broker, port)) {
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqttClient.connectError());

    while (1);
  }

  Serial.println("You're connected to the MQTT broker!");
  Serial.println();
}

void loop() {
  // call poll() regularly to allow the library to send MQTT keep alive which
  // avoids being disconnected by the broker
  mqttClient.poll();
  

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    // save the last time a message was sent
    previousMillis = currentMillis;

    //record random value from A0, A1 and A2
    uint32_t sensor_value = analogRead(A0);

    Serial.print("Sending message to topic: ");
    Serial.println(topic);
    Serial.println(sensor_value);

    // send message
    mqttClient.beginMessage(topic);
    mqttClient.print(sensor_value);
    mqttClient.endMessage();
  }   
}
