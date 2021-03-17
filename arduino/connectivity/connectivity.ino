#include "WiFi.h"

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
}


void loop() {
  // put your main code here, to run repeatedly:

}
