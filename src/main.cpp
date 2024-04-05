#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <SPIFFS.h>
#include "ESPAsyncWebServer.h"
#include "Capteur.hpp"


// Déclaration des informations du réseau WiFi
const char* ssid = "iPhone";
const char* password = "1223334444";

// Initialisation du capteur avec les broches correspondantes
Capteur capteur(17, 35); // Broche DHT22 est 17, broche analogique est 35

AsyncWebServer server(80); // Création d'un serveur web asynchrone sur le port 80

// Fonction pour récupérer les valeurs de capteur et les traiter pour l'affichage sur la page web
String recuperation(const String& var) {
    if (var == "TEMP") { // Si la variable demandée est la température
        float temperature = capteur.getTemperature();
        if (isnan(temperature)) { // Si la lecture de la température a échoué
            return "Erreur de lecture de la température";
            } 
        else {
            return String(temperature); // Retourne la température
            }
        }
    else if (var == "HUMIDITY") {
        float humidity = capteur.getHumidity();
        if (isnan(humidity)) {
            return "Erreur de lecture de l'humidite";
        }
        else {
            return String(humidity);
        }
    }
    else if (var == "GAZ") {
        int co2Level = capteur.getCO2Level();
        if (co2Level == -1) {
            return "Erreur de lecture de la tension : Tension nulle";
        }
        else if (co2Level == -2) {
            return "Erreur de lecture de la tension : Tension inférieure à 0.4V";
        }
        else {
            return String(co2Level);
        }
    }
    return String(); // Retourne une chaîne vide par défaut
}

// Fonction pour initialiser la connexion WiFi
void initWiFi() {
    WiFi.mode(WIFI_STA); // Configuration en mode Station (client)
    WiFi.begin(ssid, password); 

    Serial.print("Connexion au WiFi ..");

    while (WiFi.status() != WL_CONNECTED) { // Attendre la connexion
        Serial.print('.'); 
        delay(1000);
    }
    Serial.println(WiFi.localIP()); // Affichage de l'adresse IP attribuée
}

// Fonction pour initialiser le système de fichiers SPIFFS
void initSPIFFS() {
    if (!SPIFFS.begin(true)) { // Montage du système de fichiers SPIFFS en lecture-écriture
        Serial.println("Une erreur s'est produite lors du montage de SPIFFS");
    } else {
        Serial.println("SPIFFS monté avec succès");
    }
}
void setup() {
     Serial.begin(9600); // Initialisation de la communication série

    capteur.begin(); // Initialisation du capteur
    initWiFi(); // Initialisation de la connexion WiFi
    initSPIFFS(); // Initialisation du système de fichiers SPIFFS

    // Définition de la gestion de la requête HTTP pour la racine "/" envoi du contenu du fichier index.html qui se trouve dans le SPIFFS
    // La fonction récupération est utilisée pour traiter les variables dynamiques dans le fichier HTML
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/index.html", "text/html", false, recuperation);
    });

    server.serveStatic("/", SPIFFS, "/"); // Gestion des fichiers statiques

    server.begin(); // Démarrage du serveur web
}

void loop() {
}
