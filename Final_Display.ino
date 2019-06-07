//Final Project

//Part 2: Display

//Includes: Button/Motion sensor, LCD/LEDs

//motion updates air quality? button makes light show?

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <Adafruit_MQTT.h>
#include <Adafruit_MPL115A2.h>
#include <ESP8266WiFi.h>    //Requisite Libraries . . .
#include <ESP8266HTTPClient.h>
#include "Wire.h"           //
#include <PubSubClient.h>   //
#include <ArduinoJson.h>    //

#define ssid "University of Washington"   //You have seen this before
#define password "" //
#define mqtt_server "mediatedspaces.net"  //this is its address, unique to the server
#define mqtt_user "hcdeiot"               //this is its server login, unique to the server
#define mqtt_password "esp8266"           //this is it server password, unique to the server

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

WiFiClient espClient;             //blah blah blah, espClient
PubSubClient mqtt(espClient);     //blah blah blah, tie PubSub (mqtt) client to WiFi client

char mac[6];
char message[201];

int count = 10;

int treeTimer = 2200;
int buttonState = 0;
int lightType = 1;

/////SETUP_WIFI/////
void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println(WiFi.macAddress());
}

/////SETUP/////
void setup() {
  Serial.begin(115200);
  setup_wifi();
  mqtt.setServer(mqtt_server, 1883);
  mqtt.setCallback(callback); //register the callback function
  
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(2000); // Pause for 2 seconds

  Serial.println("should clear display now");
  display.clearDisplay();

  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(14, OUTPUT);
  pinMode(15, OUTPUT);
  pinMode(16, OUTPUT);
  pinMode(0, OUTPUT);
  pinMode(2, INPUT);


  //factory setting
  digitalWrite(13, HIGH);
  digitalWrite(14, HIGH);
  digitalWrite(16, HIGH);
  digitalWrite(12, LOW);
  digitalWrite(15, LOW);
  digitalWrite(0, LOW);
}

//CONNECT/RECONNECT//
void reconnect() {
  // Loop until we're reconnected
  while (!mqtt.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (mqtt.connect(mac, mqtt_user, mqtt_password)) {
      Serial.println("connected");
      mqtt.subscribe("fromJon/+");
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqtt.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

/////LOOP/////
void loop() {
  if (!mqtt.connected()) {
    reconnect();
  }
  buttonState = digitalRead(2);
  if (buttonState == HIGH) {
    lightType = -(lightType);
  } else {
    //do nothing
  }

  display.clearDisplay();
  display.setTextSize(2);      // Normal 1:1 pixel scale
  display.setTextColor(WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner
  display.cp437(true);         // Use full 256 char 'Code Page 437' font
  display.display();

  if (lightType == -1) {          //THE FORMAT WITH LIGHTS
    display.clearDisplay();
    display.display();
    display.setCursor(0, 0);
    display.println("[][][][][]");
    display.print("[][][][][]");
    display.display();
    digitalWrite(13, HIGH);
    digitalWrite(14, HIGH);
    digitalWrite(16, LOW);
    digitalWrite(12, LOW);
    digitalWrite(15, LOW);
    digitalWrite(0, HIGH);
    delay(treeTimer);
    
    display.clearDisplay();
    display.display();
    display.setCursor(0, 0);
    display.println("[][][][][]");
    display.print("[][][][]");
    display.display();
    digitalWrite(13, HIGH);
    digitalWrite(14, LOW);
    digitalWrite(16, HIGH);
    digitalWrite(12, LOW);
    digitalWrite(15, HIGH);
    digitalWrite(0, LOW);
    delay(treeTimer);
    
    display.clearDisplay();
    display.display();
    display.setCursor(0, 0);
    display.println("[][][][][]");
    display.print("[][][]");
    display.display();
    digitalWrite(13, LOW);
    digitalWrite(14, HIGH);
    digitalWrite(16, HIGH);
    digitalWrite(12, HIGH);
    digitalWrite(15, LOW);
    digitalWrite(0, LOW);
    delay(treeTimer);
    
    display.clearDisplay();
    display.display();
    display.setCursor(0, 0);
    display.println("[][][][][]");
    display.print("[][]");
    display.display();
    digitalWrite(13, HIGH);
    digitalWrite(14, HIGH);
    digitalWrite(16, LOW);
    digitalWrite(12, LOW);
    digitalWrite(15, LOW);
    digitalWrite(0, HIGH);
    delay(treeTimer);
    
    display.clearDisplay();
    display.display();
    display.setCursor(0, 0);
    display.println("[][][][][]");
    display.print("[]");
    display.display();
    digitalWrite(13, HIGH);
    digitalWrite(14, LOW);
    digitalWrite(16, HIGH);
    digitalWrite(12, LOW);
    digitalWrite(15, HIGH);
    digitalWrite(0, LOW);
    delay(treeTimer);
    
    display.clearDisplay();
    display.display();
    display.setCursor(0, 0);
    display.println("[][][][][]");
    display.display();
    digitalWrite(13, LOW);
    digitalWrite(14, HIGH);
    digitalWrite(16, HIGH);
    digitalWrite(12, HIGH);
    digitalWrite(15, LOW);
    digitalWrite(0, LOW);
    delay(treeTimer);
    
    display.clearDisplay();
    display.display();
    display.setCursor(0, 0);
    display.println("[][][][]");
    display.display();
    digitalWrite(13, HIGH);
    digitalWrite(14, HIGH);
    digitalWrite(16, LOW);
    digitalWrite(12, LOW);
    digitalWrite(15, LOW);
    digitalWrite(0, HIGH);
    delay(treeTimer);
    
    display.clearDisplay();
    display.display();
    display.setCursor(0, 0);
    display.println("[][][]");
    display.display();
    digitalWrite(13, HIGH);
    digitalWrite(14, LOW);
    digitalWrite(16, HIGH);
    digitalWrite(12, LOW);
    digitalWrite(15, HIGH);
    digitalWrite(0, LOW);
    delay(treeTimer);
    
    display.clearDisplay();
    display.display();
    display.setCursor(0, 0);
    display.println("[][]");
    display.display();
    digitalWrite(13, LOW);
    digitalWrite(14, HIGH);
    digitalWrite(16, HIGH);
    digitalWrite(12, HIGH);
    digitalWrite(15, LOW);
    digitalWrite(0, LOW);
    delay(treeTimer);
    
    display.clearDisplay();
    display.display();
    display.setCursor(0, 0);
    display.println("[]");
    display.display();
    digitalWrite(13, HIGH);
    digitalWrite(14, HIGH);
    digitalWrite(16, LOW);
    digitalWrite(12, LOW);
    digitalWrite(15, LOW);
    digitalWrite(0, HIGH);
    delay(treeTimer);
    
    display.clearDisplay();
    display.display();
    display.setCursor(0, 0);
    display.println("");
    display.display();
    digitalWrite(13, LOW);
    digitalWrite(14, LOW);
    digitalWrite(16, LOW);
    digitalWrite(12, HIGH);
    digitalWrite(15, HIGH);
    digitalWrite(0, HIGH);
    delay(treeTimer);
  } else {                  // THE NORMAL FORMAT
    digitalWrite(13, HIGH);
    digitalWrite(14, HIGH);
    digitalWrite(16, HIGH);
    digitalWrite(12, LOW);
    digitalWrite(15, LOW);
    digitalWrite(0, LOW);
    display.clearDisplay();
    display.display();
    display.setCursor(0, 0);
    display.println("[][][][][]");
    display.print("[][][][][]");
    display.display();
    delay(treeTimer);
    display.clearDisplay();
    display.display();
    display.setCursor(0, 0);
    display.println("[][][][][]");
    display.print("[][][][]");
    display.display();
    delay(treeTimer);
    display.clearDisplay();
    display.display();
    display.setCursor(0, 0);
    display.println("[][][][][]");
    display.print("[][][]");
    display.display();
    delay(treeTimer);
    display.clearDisplay();
    display.display();
    display.setCursor(0, 0);
    display.println("[][][][][]");
    display.print("[][]");
    display.display();
    delay(treeTimer);
    display.clearDisplay();
    display.display();
    display.setCursor(0, 0);
    display.println("[][][][][]");
    display.print("[]");
    display.display();
    delay(treeTimer);
    display.clearDisplay();
    display.display();
    display.setCursor(0, 0);
    display.println("[][][][][]");
    display.display();
    delay(treeTimer);
    display.clearDisplay();
    display.display();
    display.setCursor(0, 0);
    display.println("[][][][]");
    display.display();
    delay(treeTimer);
    display.clearDisplay();
    display.display();
    display.setCursor(0, 0);
    display.println("[][][]");
    display.display();
    delay(treeTimer);
    display.clearDisplay();
    display.display();
    display.setCursor(0, 0);
    display.println("[][]");
    display.display();
    delay(treeTimer);
    display.clearDisplay();
    display.display();
    display.setCursor(0, 0);
    display.println("[]");
    display.display();
    delay(treeTimer);
    display.clearDisplay();
    display.display();
    display.setCursor(0, 0);
    display.println("");
    display.display();
    delay(treeTimer);
  }
  
  mqtt.loop();
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.println();
  Serial.println("Message arrived.");


  DynamicJsonBuffer  jsonBuffer; //blah blah blah a DJB
  JsonObject& root = jsonBuffer.parseObject(payload); //parse it!
  if (!root.success()) { //well?
    Serial.println("parseObject() failed, are you sure this message is JSON formatted.");
    return;
  }

  //Grab information from JSON
  String airQual = root["air"].as<String>();
  String trees = root["trees"].as<String>();

  treeTimer = trees.toInt();
  if (lightType == 1) {
    if ((airQual == "Excellent air quality") || (airQual == "Good air quality")) {
      digitalWrite(13, HIGH);
      digitalWrite(14, HIGH);
      digitalWrite(16, HIGH);
      digitalWrite(12, LOW);
      digitalWrite(15, LOW);
      digitalWrite(0, LOW);
    } else {
      digitalWrite(13, LOW);
      digitalWrite(14, LOW);
      digitalWrite(16, LOW);
      digitalWrite(12, HIGH);
      digitalWrite(15, HIGH);
      digitalWrite(0, HIGH);
    }
  }
}

void draw(void) {
  display.clearDisplay();

  display.setTextSize(2);      // Normal 1:1 pixel scale
  display.setTextColor(WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner
  display.cp437(true);         // Use full 256 char 'Code Page 437' font

  display.display();
  display.println("[][][][][]");
  display.print("[][][][][]");
  display.display();

  delay(2000);
}
