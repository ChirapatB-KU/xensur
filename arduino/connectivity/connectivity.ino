#include "WiFi.h"
#include "PubSubClient.h"

WiFiClient espClient;
PubSubClient client(espClient);


void setup() {
  // put your setup code here, to run once:

  // connect to wifi
  const char* ssid = "xensurnetwork";
  const char* pass = "helloworld";

  Serial.begin(115200);
  WiFi.begin(ssid, pass);

  Serial.print("Connecting to WiFi");

  while(WiFi.status()!=WL_CONNECTED){
    delay(500);
    Serial.print("...");
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

    if(client.connect("xensurInput")){
      Serial.println("connected");  
    }else{
      Serial.println("retrying...");
      delay(2000);
    }
  }
  client.publish("xensurStatus", "device ONLINE.");
  client.subscribe("xensurInput");
}

void callback(char* topic, byte* payload, unsigned int length){
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);

  for(int i=0; i<length; i++){
    Serial.print((char)payload[i]);
  }  
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("xensurInput")) {
      Serial.println("connected");
      // Subscribe
      client.subscribe("xensurInput");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  if(!client.connected()){
    reconnect();  
  }
  client.loop();

}
