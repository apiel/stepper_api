#include <Stepper.h>
#include <MyWifiSettings.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

ESP8266WebServer server(80);

const int stepsPerRevolution = 350;  // change this to fit the number of steps per revolution
Stepper myStepper(stepsPerRevolution, 14, 12, 13, 15); // d 5,6,7.8 // in1,3,2,4
// Stepper myStepper(stepsPerRevolution, 5, 4, 14, 12); // d 1,2,5,6 // in1,3,2,4

int loopCounter = 0;

void handleUp() {
  if (server.hasArg("count")) {
    loopCounter = server.arg("count").toInt();
  }
  server.send(200, "text/plain", "up");
}

void handleDown() {
  if (server.hasArg("count")) {
    loopCounter = server.arg("count").toInt();
  }
  server.send(200, "text/plain", "down");
}

void handleStop() {
  loopCounter = 0;
  server.send(200, "text/plain", "stop");
}

void setup() {
  myStepper.setSpeed(50);
  Serial.begin(115200);

  IPAddress ip(192,168,0,69);
  WiFi.mode(WIFI_STA);
  WiFi.begin(MYWIFISSID, MYWIFIPASSWORD, ip);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("My ip: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("espMotorBlind")) {
    Serial.println("MDNS responder started");
  }

  server.on("/up", handleUp);
  server.on("/down", handleDown);
  server.on("/stop", handleStop);
  Serial.println("HTTP server started");
  
  myStepper.step(-stepsPerRevolution);
}

void loop() {
  //myStepper.step(-stepsPerRevolution);
  //delay(500);
  server.handleClient();
}
