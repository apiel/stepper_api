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
int direction = 1;

void handleUp() {
  direction = 1;
  loopCounter = 1;
  if (server.hasArg("count")) {
    loopCounter = server.arg("count").toInt();
  }
  server.send(200, "text/plain", "up");
}

void handleDown() {
  direction = -1;
  loopCounter = 1;
  if (server.hasArg("count")) {
    loopCounter = server.arg("count").toInt();
  }
  server.send(200, "text/plain", "down");
}

void handleStop() {
  loopCounter = 0;
  server.send(200, "text/plain", "stop");
}

void handleNotFound() {
  server.send(404, "text/plain", "No route");
}

void setup() {
  myStepper.setSpeed(50);
  Serial.begin(115200);

  //IPAddress gateway(192,168,0,1); 
  //IPAddress subnet(255,255,255,0);
  //IPAddress ip(192,168,0,69);
  //WiFi.config(local_ip, gateway, subnet);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(MYWIFISSID, MYWIFIPASSWORD);
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
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
  if (loopCounter > 0) {
    myStepper.step(stepsPerRevolution*direction);
    loopCounter--;
  }
  delay(1);
}
