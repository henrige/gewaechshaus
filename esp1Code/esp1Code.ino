// Libs und creds
#include "creds.h"
#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#include <DHT.h>
#include <Adafruit_Sensor.h>

// Instanzen
WiFiClient espClient;
PubSubClient client(espClient);

unsigned long lastMsg;
unsigned int payload;
const int ldr = A0;

void setup() {
  // Serielle Kommunikation initialisieren, extra bisschen delay damit erster Print auch sichtbar ist nachdem der ESP erst Müll ausspuckt
  Serial.begin(115200);
  delay(2000);
  Serial.println("");
  Serial.println("Verbinde mit Wlan...");
  //Onboardled deklarieren 
  int onboardLed = 2;
  bool onboardLedStatus = false;
  pinMode(onboardLed, OUTPUT);
  // Mit Wlan verbinden, siehe creds.h (auf github versteckt natürlich hihi)
  WiFi.begin(ssid, password);
  // Wartet bis Wlan verbunden ist und blinkt onboardled
  while(WiFi.status() != WL_CONNECTED) {
    digitalWrite(onboardLed, onboardLedStatus);
    onboardLedStatus = !onboardLedStatus;
    delay(500);
    Serial.print(".");
  }
  Serial.println("Mit Wlan verbunden, IP: ");
  Serial.print(WiFi.localIP());
  onboardLedStatus = true;          // Led iwie negiert
  digitalWrite(onboardLed, onboardLedStatus);
  // MQTT Server Details aufsetzen
  client.setServer(mqtt_server, 1883);
}

void loop() {
  // Checkt ob MQTT Broker verbunden ist
  if(!client.connected()) {
    reconnect();
  }
  // Zeitabstand zwischen gesendeten Nachrichten
  long now = millis();
  if (now - lastMsg > 5000) {
    lastMsg = now;
    // Hier Payload eingeben
    int ldrHelligkeit;
    ldrHelligkeit = analogRead(A0);
    Serial.print("Helligkeit: ");
    Serial.println(ldrHelligkeit);
    // Konvertieren vom Integer vom Payload in ein Character Array, kann auch länger sein
    char msg_out[8];
    sprintf(msg_out, "%d", ldrHelligkeit);
    // Und sendet hier Nachricht an Broker
    client.publish("/gewaechshaus/esp8266data", msg_out);
  }

}

void reconnect() {
  while(!client.connected()) {
    Serial.println("");
    Serial.print("MQTT nicht verbunden, versuche zu verbinden... ");
    // Macht irgendeine Client-ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Versuche zu verbinden
    if(client.connect(clientId.c_str(), mqtt_user, mqtt_pwd)) {
      Serial.println("Verbunden.");
    } else {
      Serial.print("Fehler, code=");
      Serial.print(client.state());
      delay(5000);
    }
  }
}


















