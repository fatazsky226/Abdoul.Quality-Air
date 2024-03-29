#ifndef AIR_H
#define AIR_H

#include <Arduino.h>
#include "DHTesp.h"

class Air {
  public:
    Air(int dhtPin, int analogPin);
    void begin();
    float getTemperature();
    float getHumidity();
    int getCo2Measurement();

  private:
    int dhtPin;
    int analogPin;
    DHTesp dht;
};

#endif
