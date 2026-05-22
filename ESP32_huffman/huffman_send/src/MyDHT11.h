#ifndef MYDHT_H
#define MYDHT_H

#include <DHT.h>  
#include <Arduino.h>

void initDHT(uint8_t pin, uint8_t type); 
float readTemperature();  
float readHumidity(); 

#endif