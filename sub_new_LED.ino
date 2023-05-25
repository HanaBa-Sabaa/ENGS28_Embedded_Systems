 /* Name: Grace Qu and Hana Ba-Sabaa
 * Program name: Lab9 IoT
 * Date created: 3/15/22
 * Description:  Subscriber Arduino code that includes liggt-sensor controlled LED light
 *
 * Dependencies: (what other source files are required) WiFiNINA.h, ArduinoMqttClient.h

 * I/O pins:  (what ports are used, and what are they wired to?) PB0, connected to LED
 * Revisions: (use this to track the evolution of your program) Based on Mqtt revceiver code (referred in the documantation) https://test.mosquitto.org/
 * 
 */
#include <ArduinoMqttClient.h>
#include <WiFiNINA.h>
// set up the LED
#define L1 8

//wifi information
char ssid[] = "Dartmouth Public";        // your network SSID
char pass[] = "";    // your network password

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

const char broker[] = "test.mosquitto.org";
int        port     = 1883;
const char topic[]  = "sustainable_house"; // set up topics for communication
char charac; // message variable
String string1; // string variable 


void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
 
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  //set pin output to LED
  pinMode(L1,OUTPUT);
  

  // attempt to connect to Wifi network:
  Serial.print("Attempting to connect to SSID: ");
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

  // set the message receive callback
  mqttClient.onMessage(onMqttMessage);

  Serial.print("Subscribing to topic: ");
  Serial.println(topic);
  Serial.println();

  // subscribe to a topic
  mqttClient.subscribe(topic);

  Serial.print("Topic: ");
  Serial.println(topic);
  Serial.println();

}

void loop() {
  // call poll() regularly to allow the library to receive MQTT messages and 
  // send MQTT keep alive which avoids being disconnected by the broker
  mqttClient.poll(); 
  
}


void onMqttMessage(int messageSize) {


  // use the Stream interface to print the contents
  int i=0;
  while (mqttClient.available()) {
    charac = (char)mqttClient.read();
    // turn message into string
    string1.concat(charac); 

  }
 
// turn string into integer
int num = string1.toInt();


// set up the LED
// use the message to control the LED
if (num>400){
  digitalWrite(L1,LOW); //LED off if enough light
}
else {
  digitalWrite(L1,HIGH); //LED on if not enough light
}

string1 = "";

}
