#include <Dhcp.h>
#include <Dns.h>
#include <Ethernet.h>
#include <SPI.h>
#include <MQTT.h>

byte mac[] = { 0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02 };

void callback(char* topic, byte* payload, unsigned int length);

/**
 * if want to use IP address,
 * byte server[] = { XXX,XXX,XXX,XXX };
 * MQTT client(server, 1883, callback);
 * want to use domain name,
 * MQTT client("www.sample.com", 1883, callback);
 **/
EthernetClient ethclient;
MQTT client("www.example.com", 1883, callback, ethclient);

// recieve message
void callback(char* topic, byte* payload, unsigned int length) {
  char p[length + 1];
  memcpy(p, payload, length);
  p[length] = NULL;
  String message(p);

  Serial.println(message);
  delay(1000);
}


void setup() {
  Serial.begin(9600);

  // Get IP address from DHCP Server
  Ethernet.begin(mac);

  Serial.print("My IP address: ");
  for (byte thisByte = 0; thisByte < 4; thisByte++) {
    Serial.print(Ethernet.localIP()[thisByte], DEC);
    Serial.print(".");
  }
  Serial.println();
    
  // connect to the server
  client.connect("sparkclient");

  // publish/subscribe
  if (client.isConnected()) {
    client.publish("/outTopic","hello world");
    client.subscribe("/inTopic");
  }
}

void loop() {
  if (client.isConnected())
    client.loop();
}
