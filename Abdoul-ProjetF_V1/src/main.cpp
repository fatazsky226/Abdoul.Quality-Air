

/* #include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include "DHTesp.h"
#include <SPIFFS.h>

/// @brief 
const int analogPin = 35;
const int dhtPin = 17;

DHTesp dht; 

/// @brief 
const char *ssid = "iPhone";
const char *password = "1223334444";

AsyncWebServer server(80);

// Pour le collecte de la CO2
int getCo2Measurement() {
 
  int adcVal = analogRead(analogPin);
 
  float voltage = adcVal * (3.3 / 4095.0);
 
  if (voltage == 0)
  {
    return -1;
  }
  else if (voltage < 0.4)   {
    return -2;
  }
 
  else   {
    float voltageDiference = voltage - 0.4;
    return (int) ((voltageDiference * 5000.0) / 1.6);
  }
 
}
// Partie CO2 terminé

// Variables globales pour stocker les valeurs de capteur
float temperature = 0.0;
float humidity = 0.0;
int co2 = 0;

String processor(const String& var) {
  if(var == "TEMPERATURE") {
    temperature = dht.getTemperature();
    return String(temperature);
  }
  else if(var == "HUMIDITY") {
    humidity = dht.getHumidity();
    return String(humidity);
  }
  else if(var == "CO2") {
    co2 = getCo2Measurement();
    return String(co2);
  }
  return String();
}


void setup()
{
  //----------------------------------------------------Serial
  Serial.begin(9600);
  Serial.println("\n");

  //----------------------------------------------------GPIO
  dht.setup(dhtPin, DHTesp::DHT22);
  pinMode(analogPin, INPUT);

  //----------------------------------------------------SPIFFS
  if(!SPIFFS.begin())
  {
    Serial.println("Erreur SPIFFS...");
    return;
  }

  File root = SPIFFS.open("/");
  File file = root.openNextFile();

  while(file)
  {
    Serial.print("File: ");
    Serial.println(file.name());
    file.close();
    file = root.openNextFile();
  }

  //----------------------------------------------------WIFI
  WiFi.begin(ssid, password);
	Serial.print("Tentative de connexion...");
	
	while(WiFi.status() != WL_CONNECTED)
	{
		Serial.print(".");
		delay(100);
	}
	
	Serial.println("\n");
	Serial.println("Connexion etablie!");
	Serial.print("Adresse IP: ");
	Serial.println(WiFi.localIP());

  //----------------------------------------------------SERVER
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send(SPIFFS, "/index.html", "text/html");
  });

  server.on("/w3.css", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send(SPIFFS, "/w3.css", "text/css");
  });

  server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send(SPIFFS, "/script.js", "text/javascript");
  });

  server.on("/get-sensor-data", HTTP_GET, [](AsyncWebServerRequest *request) {
    float temperature = dht.getTemperature();
    float humidity = dht.getHumidity();
    int co2 = getCo2Measurement();
    String sensorData = String(temperature) + "," + String(humidity) + "," + String(co2);
    request->send(200, "text/plain", sensorData);
  });

  server.begin();
  Serial.println("Serveur actif!");
}

void loop()
{
    delay(5000);
}

*/


#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include "Air.hpp"

const char *ssid = "iPhone";
const char *password = "1223334444";

AsyncWebServer server(80);

Air air(17, 35); // Configuration des broches pour DHT et le capteur analogique

String processor(const String& var) {
  if (var == "TEMPERATURE") {
    return String(air.getTemperature());
  } else if (var == "HUMIDITY") {
    return String(air.getHumidity());
  } else if (var == "CO2") {
    return String(air.getCo2Measurement());
  }
  return String();
}

void setup() {
  Serial.begin(9600);
  Serial.println("\n");

  if (!SPIFFS.begin()) {
    Serial.println("Erreur SPIFFS...");
    return;
  }

  air.begin();

  WiFi.begin(ssid, password);
  Serial.print("Tentative de connexion...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
  }
  Serial.println("\n");
  Serial.println("Connexion établie!");
  Serial.print("Adresse IP: ");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/index.html", "text/html", false, processor);
  });

  server.on("/w3.css", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/w3.css", "text/css");
  });

  server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/script.js", "text/javascript");
  });

  server.on("/get-sensor-data", HTTP_GET, [](AsyncWebServerRequest *request) {
    String sensorData = String(air.getTemperature()) + "," + String(air.getHumidity()) + "," + String(air.getCo2Measurement());
    request->send(200, "text/plain", sensorData);
  });

  server.begin();
  Serial.println("Serveur actif!");
}

void loop() {
  delay(5000);
}
