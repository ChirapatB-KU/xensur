#include "WiFi.h"
#include "PubSubClient.h"
#include <SPI.h>
#include <NTPClient.h>
#include <Wire.h>

WiFiClient espClient;
PubSubClient client(espClient);

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

int second = 0;
char message_buff[100];

void setup() {
  // put your setup code here, to run once:

  // led on board
  int ledPin = 2;
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  // connect to wifi
  const char* ssid = "xensurnetwork";
  const char* pass = "helloworld";

  Serial.begin(115200);
  WiFi.begin(ssid, pass);

  Serial.print("Connecting to WiFi");

  while(WiFi.status()!=WL_CONNECTED){
    digitalWrite(ledPin, HIGH);
    delay(250);
    digitalWrite(ledPin, LOW);
    delay(250);
    Serial.print("...");
  }

  //connected
  for(int i=0; i<3; i++){
    digitalWrite(ledPin, HIGH);
    delay(100);
    digitalWrite(ledPin, LOW);
    delay(100);
  }

  Serial.println("\nConnected to the WiFi network");
  ////////////////////////////////////////////////////////////////

  //connect to MQTT
  
  const char* mqttServer = "test.mosquitto.org";
  const int mqttPort = 1883;

  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);

  while(!client.connected()){
    Serial.println("Connecting to MQTT...");

    if(client.connect("xensurClock")){
      Serial.println("connected");  
    }else{
      Serial.println("retrying...");
      delay(2000);
    }
  }
  client.publish("xensurStatus", "LDR ONLINE.");
  client.subscribe("xensurClock");
  ////////////////////////////////////////////////////////////////
  timeClient.begin();
  timeClient.setTimeOffset(25200);
  ////////////////////////////////////////////////////////////////
  Wire.begin();
}

void callback(char* topic, byte* payload, unsigned int length){
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);

  for(int i=0; i<length; i++){
    Serial.print((char)payload[i]);
  }

  if(payload[0]=='c'){
    Serial.println("start calibrate");
    Wire.beginTransmission(80);
    Wire.write("2");
    Wire.endTransmission();

    delay(6*1000);
  }else{
    Serial.println("start game");

    int secondMQTT = payload[0];

    if(length==2){
        secondMQTT = payload[0]*10+payload[1];
    }

    timeClient.update();
    second = timeClient.getSeconds();

    delay((secondMQTT-second)*1000);

    Wire.beginTransmission(80);
    Wire.write("1");
    Wire.endTransmission();

    delay(60*1000);

    Wire.requestFrom(80, 6);
    while(Wire.available()){
      char c = Wire.read();
      Serial.print(c);
    }
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("xensurClock")) {
      Serial.println("connected");
      // Subscribe
      client.subscribe("xensurClock");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void reconnectWifi(){
  // led on board
  int ledPin = 2;
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  // connect to wifi
  const char* ssid = "xensurnetwork";
  const char* pass = "helloworld";

  Serial.begin(115200);
  WiFi.begin(ssid, pass);

  Serial.print("Connecting to WiFi");

  while(WiFi.status()!=WL_CONNECTED){
    digitalWrite(ledPin, HIGH);
    delay(250);
    digitalWrite(ledPin, LOW);
    delay(250);
    Serial.print("...");
  }

  //connected
  for(int i=0; i<3; i++){
    digitalWrite(ledPin, HIGH);
    delay(100);
    digitalWrite(ledPin, LOW);
    delay(100);
  }
  
  Serial.println("\nConnected to the WiFi network");
}

void loop() {
  // put your main code here, to run repeatedly:
  if(WiFi.status()!=WL_CONNECTED){
    reconnectWifi();  
  }
  if(!client.connected()){
    reconnect();  
  }
  client.loop();

}