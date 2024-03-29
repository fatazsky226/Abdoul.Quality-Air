#include "Air.hpp"

Air::Air(int dhtPin, int analogPin) : dhtPin(dhtPin), analogPin(analogPin) {}

void Air::begin() {
  dht.setup(dhtPin, DHTesp::DHT22);
  pinMode(analogPin, INPUT);
}

float Air::getTemperature() {
  return dht.getTemperature();
}

float Air::getHumidity() {
  return dht.getHumidity();
}

int Air::getCo2Measurement() {
  int adcVal = analogRead(analogPin);
  float voltage = adcVal * (3.3 / 4095.0);
  if (voltage == 0) {
    return -1;
  } else if (voltage < 0.4) {
    return -2;
  } else {
    float voltageDifference = voltage - 0.4;
    return static_cast<int>((voltageDifference * 5000.0) / 1.6);
  }
}
