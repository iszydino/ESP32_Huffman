#include "MyDHT11.h"

static DHT dht(0, 0); 

void initDHT(uint8_t pin, uint8_t type) {
    dht = DHT(pin, type);
    dht.begin();
}

float readTemperature() {
    return dht.readTemperature();
}

float readHumidity() {
    return dht.readHumidity();
}