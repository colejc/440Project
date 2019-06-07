//Final Project

//Part 1: Hub Device

//Includes: API call, MQTT updates on a timer, climate info confirm (math)

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <Adafruit_MQTT.h>
#include <Adafruit_MPL115A2.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
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

WiFiClient espClient;             //blah blah blah, espClient
PubSubClient mqtt(espClient);     //blah blah blah, tie PubSub (mqtt) client to WiFi client

char mac[6]; //A MAC address is a 'truly' unique ID for each device, lets use that as our 'truly' unique user ID!!!
char message[201]; //201, as last character in the array is the NULL character, denoting the end of the array

int timerOne = millis();
String trees = "2200"; //Millis between fields being felled

const char* host = "api.github.com";
const int httpsPort = 443;
// Use web browser to view and copy
// SHA1 fingerprint of the certificate
const char* fingerprint = "5F F1 60 31 09 04 3E F2 90 D2 B0 8A 50 38 04 E8 37 9F BC 76";

//Setup https and other things
void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.print("connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Use WiFiClientSecure class to create TLS connection
  WiFiClientSecure client;
  Serial.print("connecting to ");
  Serial.println(host);
  if (!client.connect(host, httpsPort)) {
    Serial.println("connection failed");
    return;
  }

  if (client.verify(fingerprint, host)) {
    Serial.println("certificate matches");
  } else {
    Serial.println("certificate doesn't match");
  }

  String url = "/repos/esp8266/Arduino/commits/master/status";
  Serial.print("requesting URL: ");
  Serial.println(url);

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "User-Agent: BuildFailureDetectorESP8266\r\n" +
               "Connection: close\r\n\r\n");

  Serial.println("request sent");
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      Serial.println("headers received");
      break;
    }
  }

  mqtt.setServer(mqtt_server, 1883);
  timerOne = millis(); //Use treeTimer
}

/////CONNECT/RECONNECT/////Monitor the connection to MQTT server, if down, reconnect
void reconnect() {
  // Loop until we're reconnected
  while (!mqtt.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (mqtt.connect(mac, mqtt_user, mqtt_password)) { //<<---using MAC as client ID, always unique!!!
      Serial.println("connected");
      mqtt.subscribe("fromJon/+"); //we are subscribing to 'theTopic' and all subtopics below that topic
    } else {                        //please change 'theTopic' to reflect your topic you are subscribing to
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
  mqtt.loop(); //this keeps the mqtt connection 'active'

  //Here we just send a regular c-string which is not formatted JSON, or json-ified.
  if (millis() - timerOne > 20000) {
    
    //API call - get air quality reading, include in MQTT message--------------------------
    //key : 6312534f619f47b786f0452a5aedc70e
    //https://api.breezometer.com/air-quality/v2/current-conditions?lat=47.6062&lon=22.3321&key=6312534f619f47b786f0452a5aedc70e
    //lat 47.6062, long 122.3321
    //CALL API AND GET A MESSAGE
    HTTPClient theClient;
    //Serial.println("Making HTTP request");
    String apiCall = "https://api.breezometer.com/air-quality/v2/current-conditions?lat=47.6062&lon=22.3321&key=6312534f619f47b786f0452a5aedc70e";


    //Collect the information from the URL, and check to make sure we haven't encountered any errors.
    theClient.begin(apiCall);
    int httpCode = theClient.GET();
    Serial.println(httpCode);
    if (httpCode > 0) {
      if (httpCode == 200) {
        
        //Print a line that lets us know the correct payload has been received, and organize the information
        //we've grabbed from online in JSON format in the same way we did for getIP().
        //Serial.println("Received HTTP payload.");
        DynamicJsonBuffer jsonBuffer;
        String payload = theClient.getString();
        JsonObject& root = jsonBuffer.parse(payload);
        // Test if parsing succeeds, and if it didn't, output a notice.
        if (!root.success()) {
          Serial.println("parseObject() failed");
          Serial.println(payload);
          return;
        }

      String airQual = root["data"]["indexes"]["baqi"]["category"].as<String>();
      Serial.println(airQual);
      Serial.println(trees);

      //SEND MESSAGE
      //String tres = String(trees);
      sprintf(message, "{\"air\":\"%s\", \"trees\":\"%s\"}", airQual, trees);
      mqtt.publish("fromJon/words", message);

      //Output a line in case httpCode is the incorrect value.
      } else {
        Serial.println("Something went wrong with connecting to the endpoint.");
      } 
    }
    
    timerOne = millis();
  }
}
