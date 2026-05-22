#include "MyOled.h"
#include <Wire.h>

Adafruit_SSD1306 display(128, 64, &Wire);

bool initOLED(int w, int h, int sda, int scl) {
    Wire.begin(sda, scl);
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        return false;
    }
    display.clearDisplay();
    display.display();
    return true;
}

void oledCenterText(int y, const String &txt, int size = 1) {
    int16_t x1, y1;
    uint16_t w, h;
    display.setTextSize(size);
    display.setTextColor(WHITE);
    display.getTextBounds(txt, 0, 0, &x1, &y1, &w, &h);
    int x = (128 - w) / 2;
    display.setCursor(x, y);
    display.print(txt);
}

void oledShowStatus(const String &s) {
    oledCenterText(48, s, 1);
}

void oledShow(int temp, int humid) {
    display.clearDisplay();
    // ==== TEMP ====
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.print((char)0xF8);  
    display.setCursor(20, 0);
    display.print(temp);
    display.print("C");

    // ==== HUMID ====
    display.setCursor(0, 24);
    display.print((char)0x7E);   
    display.setCursor(20, 24);
    display.print(humid);
    display.print("%");

    // ==== LINE ====
    display.drawLine(0, 44, 128, 44, WHITE);

    // ==== STATUS ====
    oledCenterText(50, "RX OK", 1);

    display.display();
}
