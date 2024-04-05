#include "Capteur.hpp"

// Définition du constructeur de la classe Capteur
Capteur::Capteur(int pin, int analogPin) : _pin(pin), _analogPin(analogPin) {
    // Initialisation des variables membres
}

// Fonction pour initialiser le capteur
void Capteur::begin() {
    _dht.setup(_pin, DHTesp::DHT22); // Configuration du capteur de température et d'humidité
}

// Fonction pour obtenir la température
float Capteur::getTemperature() {
    return _dht.getTemperature(); // Récupération de la température depuis le capteur
}

// Fonction pour obtenir l'humidité
float Capteur::getHumidity() {
    return _dht.getHumidity(); 
}

// Fonction pour obtenir le niveau de CO2
int Capteur::getCO2Level() {
    int adcVal = analogRead(_analogPin); 
    float voltage = adcVal * (3.3 / 4095.0); // Conversion de la valeur analogique en tension
  
    // Vérification des différents niveaux de tension et retour de la valeur de CO2 correspondante
    if (voltage == 0) {
        return -1; // Aucune tension détectée
    } 
    else if (voltage < 0.4) {
        return -2; // Tension inférieure à la valeur minimale
    }
    else {
        float voltageDifference = voltage - 0.4; 
        return static_cast<int>((voltageDifference * 5000.0) / 1.6); // Calcul du niveau de CO2 en ppm
    }
}
