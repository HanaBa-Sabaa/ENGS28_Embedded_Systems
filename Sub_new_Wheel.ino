 /* Name: Grace Qu and Hana Ba-Sabaa
 * Program name: Lab9 IoT
 * Date created: 3/15/22
 * Description:  Subscriber Arduino code that includes temperature controlled motor
 *
 * Dependencies: (what other source files are required) WiFiNINA.h, ArduinoMqttClient.h

 * I/O pins:  (what ports are used, and what are they wired to?) PB1, PB2, PB3, connected to PWM, AIN1, AIN2
 * Revisions: (use this to track the evolution of your program) Based on Mqtt revceiver code (referred in the documantation) https://test.mosquitto.org/
 * 
 */

#include <ArduinoMqttClient.h>
#include <WiFiNINA.h>


// Motor Driver pins
#define PWM1 9
#define AIN1 10
#define AIN2 11
//Wifi information
char ssid[] = "Dartmouth Public";        // network SSID
char pass[] = "";    // network password

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

const char broker[] = "test.mosquitto.org";
int        port     = 1883;
int out; // motor output
const char topic[]  = "sustainable_house"; // topic name
char charac; // message character varaible
String string1; //message character string


void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
 
  while (!Serial) {
   ; // wait for serial port to connect. Needed for native USB port only
  }
  // set up the pins
  pinMode(PWM1,OUTPUT); 
  pinMode(AIN1,OUTPUT);
  pinMode(AIN2,OUTPUT);

  // set the motor to rotate clockwise 
  digitalWrite(AIN1,HIGH); 
  digitalWrite(AIN2,LOW);

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

  // sustainable house topic
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
  while (mqttClient.available()) {
    charac = (char)mqttClient.read();
    //turn message into a string
    string1.concat(charac);

  }
  //turn string into a number (integer)
  int num = string1.toInt();
  //map the number into an input pwm can take in
  out = map(num, 135, 255, 0, 255);
  // set the motor speed
  analogWrite(PWM1,out); 
  //clear the string
  string1 = "";
  
}
