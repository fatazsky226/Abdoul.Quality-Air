#ifndef CAPTEUR_H
#define CAPTEUR_H

#include <Arduino.h> // Inclusion de la bibliothèque Arduino
#include "DHTesp.h" // Inclusion de la bibliothèque DHTesp pour le capteur de température et d'humidité

// Définition de la classe Capteur
class Capteur {
    public:
        Capteur(int pin, int analogPin);
        void begin();
        float getTemperature();
        float getHumidity();
        int getCO2Level();

    private:
        int _pin; // Broche pour le capteur de température et d'humidité
        int _analogPin; // Broche pour le capteur de CO2
        DHTesp _dht; // Objet pour le capteur de température et d'humidité
    };

#endif
