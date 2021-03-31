#include "WiFi.h"
#include "PubSubClient.h"
#include <SPI.h>
#include <NTPClient.h>
#include <Wire.h>
#include <stdlib.h>

WiFiClient espClient;
PubSubClient client(espClient);

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

int second = 0;


void callback(char* topic, byte* payload, unsigned int length){
  String buf = "";
  for(int i=0; i<length; i++){
    buf = buf+(String)payload[i];
  }

  if(payload[0]=='c'){
    Wire.beginTransmission(4);
    Wire.write("2");
    Wire.endTransmission();

    delay(6*1000);
  }else{
    int secondMQTT = buf.toInt();

    timeClient.update();
    second = timeClient.getSeconds();

    delay((secondMQTT-second)*1000);

    Wire.beginTransmission(4);
    Wire.write("1");
    Wire.endTransmission();

    delay(60*1000);
  }  
}

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

    if(client.connect("xensurTime")){
      Serial.println("connected");  
    }else{
      Serial.println("retrying...");
      delay(2000);
    }
  }  
  ////////////////////////////////////////////////////////////////
  timeClient.begin();
  timeClient.setTimeOffset(25200);
  ////////////////////////////////////////////////////////////////
  Wire.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  timeClient.update();
  second = timeClient.getSeconds();
  delay(1000);

}
