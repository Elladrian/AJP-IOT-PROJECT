
#include <stdio.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "DHTesp.h"

Adafruit_SSD1306 display(128, 64, &Wire, -1);

#define HTTP_REST_PORT 80
#define WIFI_RETRY_DELAY 500
#define MAX_WIFI_INIT_RETRY 50

#define SSID "" 
#define SSID_PASSWORD ""

const char* wifi_ssid = "";
const char* wifi_passwd = "";

int timer = 0;

ESP8266WebServer http_rest_server(HTTP_REST_PORT);

int init_wifi() {
  int retries = 0;
  Serial.println("Connecting to WiFi ...");

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.print("Connecting to WiFi ...");
  display.display();

  WiFi.mode(WIFI_STA);
  WiFi.begin(wifi_ssid, wifi_passwd);
  // check the status of WiFi connection to be WL_CONNECTED
  while ((WiFi.status() != WL_CONNECTED) && (retries < MAX_WIFI_INIT_RETRY)) {
    retries++;
    delay(WIFI_RETRY_DELAY);
    Serial.print(".");
    // Dots will print to show still connecting...
  }
  return WiFi.status(); // return the WiFi connection status
}

void post_time() {
  http_rest_server.sendHeader("Access-Control-Allow-Origin", "*");
  http_rest_server.send(200, "application/json");

  String post_body = http_rest_server.arg("time");
  Serial.println(post_body);

  timer = post_body.toInt();
  Serial.print("timer");
  Serial.print(timer);
}

/**
    config REST API function
*/
void config_rest_server_routing() {
  http_rest_server.enableCORS(true);
  Serial.println("Cors enabled");

  http_rest_server.on("/", HTTP_GET, []() {
    
    http_rest_server.sendHeader("Access-Control-Allow-Origin", "*");
    http_rest_server.send(200, "text/html", "Welcome to the ESP8266 REST Web Server");
  });

  http_rest_server.on("/set-timer", HTTP_POST, [](){
  http_rest_server.sendHeader("Access-Control-Allow-Origin", "*");
  http_rest_server.send(200, "text/html", "Welcome to the ESP8266 REST Web Server");

  String post_body = http_rest_server.arg("time");
  Serial.println(post_body);

  timer = post_body.toInt();
  Serial.print("timer");
  Serial.print(timer);
  });
}

void setup(void) {
  Serial.begin(115200);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  Serial.println("MCU Program Started...");
  if (init_wifi() == WL_CONNECTED) {
    Serial.println("WIFI Connetted");
    Serial.print(wifi_ssid);
    Serial.print("--- IP: ");
    Serial.println(WiFi.localIP());
  }
  else {
    Serial.print("Error connecting to: ");
    Serial.println(wifi_ssid);
  }

  config_rest_server_routing();

  http_rest_server.begin();
  Serial.println("HTTP REST Server Started");
}

void loop(void) {
  http_rest_server.handleClient();

  while(timer > 0){
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0,0);
    display.print("Local IP: ");
    display.println(WiFi.localIP());
    display.print("Timer: ");
    display.println(timer);

    timer = timer - 1;
    Serial.print(timer);
    display.display();
    delay(1000);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.print("Local IP: ");
  display.println(WiFi.localIP());
  display.print("Timer - Ended");
  display.display();
}