#ifndef MYOLED_H
#define MYOLED_H

#include <Arduino.h>
#include <Adafruit_SSD1306.h>

bool initOLED(int w, int h, int sda, int scl);
void oledShow(int temp, int humid);
void oledShowStatus(const String &s);

#endif
