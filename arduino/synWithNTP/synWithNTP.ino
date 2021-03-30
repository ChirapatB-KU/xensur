#include "WiFi.h"
#include "PubSubClient.h"
#include <SPI.h>

WiFiClient espClient;
PubSubClient client(espClient);

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
}

void callback(char* topic, byte* payload, unsigned int length){
  
}

void loop() {
  // put your main code here, to run repeatedly:

}
