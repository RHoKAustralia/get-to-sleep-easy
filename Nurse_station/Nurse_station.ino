// Nurse Station
//
// RHoK
//
// last changed 03.06.2018

// for udp:
#include <WiFiUdp.h>

#include <ESP8266WiFi.h>
//#include <WiFiClient.h>
//#include <ESP8266WebServer.h>

const char *ssid = "NurseStation";
const char *password = "NurseStation";

//ESP8266WebServer server(80);
WiFiServer server(80);

// udp:
WiFiUDP Udp;
#define UDP_PORT 11111  // local port to listen on
char incomingPacket[255];  // buffer for incoming packets
char  replyPacekt[] = "master has received";  // a reply string to send back

int buttonSeq = 0; // sequence of the received button/table pressed, 1 is first
int buttonId = 0; // id of pressed button
int ledStatus = 0; // led on button

/* Just a little test message.  Go to http://192.168.4.1 in a web browser
  connected to this access point to see it.
*/
/*
  void handleRoot() {
  server.send(200, "text/html", "<h1>You are connected</h1>");
  Serial.println("You are connected");
  }
*/

IPAddress client; //(192, 168, 4, 2);

// setup code here, to run once:
void setup() {
  pinMode(2, OUTPUT); // this is gpio02, led //RED
  digitalWrite(2, LOW);

  pinMode(4, OUTPUT); //gpio4 led GREEN
  digitalWrite(4, LOW);

  // initialize the pushbutton pin as an input:
  pinMode(0, INPUT); // gpio0

  Serial.begin(115200);
  delay(10); // not sure if necessary
  Serial.print("\n180603\n");


  WiFi.mode(WIFI_AP);
  //  WiFi.softAP(""); //ssid, password);
  WiFi.softAP(ssid, password);
  WiFi.begin();

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("IP:");
  Serial.println(myIP);


  // udp setup:
  uint8_t ret = Udp.begin(UDP_PORT);
  if (ret) Serial.printf("Now listening at IP %s, UDP port %d\n", WiFi.localIP().toString().c_str(), UDP_PORT);
  else Serial.printf("Error opening udp\n");

}

// main code here, to run repeatedly:
void loop() {
  // handle udp:
  int packetSize = Udp.parsePacket();
  if (packetSize)
  {
    // receive incoming UDP packets
    Serial.printf("Getting something");
    Serial.printf("Received %d bytes from %s, port %d\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort());

    client = Udp.remoteIP();

    int len = Udp.read(incomingPacket, 255);
    if (len > 0)
    {
      incomingPacket[len] = 0;
    }
    Serial.printf("UDP packet contents: %s\n", incomingPacket);
    // send back a reply, to the IP address and port we got the packet from
    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    Udp.write(replyPacekt);
    Udp.endPacket();
  }
 // else{
  //  Serial.println("Nothing");
 // }

  //Waiting for incoming UDP
  if (packetSize)
  {
    Serial.printf("Received %d bytes from %s, port %d\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort());
    int len = Udp.read(incomingPacket, 255);
    if (len > 0)
    {
      incomingPacket[len] = 0;
    }
    Serial.printf("UDP packet contents: %s\n", incomingPacket);

    // Fall warning - RED LED on
    if (incomingPacket == "f") {
      digitalWrite(2, HIGH);
    }
    
    // Sitting warning = GREEN LED on
    else if (incomingPacket == "s") {
      digitalWrite(4, HIGH);  
    }
       
    // Moving warning - flashing LED RED
    else if (incomingPacket == "m") {
      digitalWrite(2, HIGH);
      delay(500);
      digitalWrite(2, LOW);
    }
    else{ //no warning messages sent - lights off
      digitalWrite(2, LOW);
      digitalWrite(4, LOW);
      Serial.println("No Warnings"); 
    }
    
  }

  // check if button is pressed:
  if (digitalRead(0) == LOW) {
    Serial.println("+");
    delay(100); // debounce delay
    buttonSeq = 0; // reset sequence for another round of questions
    digitalWrite(2, LOW); // led off

    //    IPAddress gatewayIP(192, 168, 4, 2);
    static char packet[] = "master button pressed";
    // send back a reply, to the IP address and port we got the packet from
    //    Udp.beginPacket(gatewayIP, UDP_PORT);
    Udp.beginPacket(client, UDP_PORT);
    Udp.write(packet);
    Udp.endPacket();
    Serial.printf("packet sent to %s, port %d\n", client.toString().c_str(), UDP_PORT);
  }
}
