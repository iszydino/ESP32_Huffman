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

// Vẽ text giữa màn hình
void oledCenterText(int y, const String &txt, int size) {
    int16_t x1, y1;
    uint16_t w, h;
    display.setTextSize(size);
    display.setTextColor(WHITE);
    display.getTextBounds(txt, 0, 0, &x1, &y1, &w, &h);
    int x = (128 - w) / 2;
    display.setCursor(x, y);
    display.print(txt);
}

// Hiển thị trạng thái dòng dưới
void oledShowStatus(const String &s) {
    display.fillRect(0, 45, 128, 20, BLACK);  // xóa vùng status
    oledCenterText(48, s, 1);
    display.display();
}
void oledShowDecodeStep(const String &line1, const String &line2, const String &line3)
{
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);

    display.setCursor(0, 0);
    display.println(line1);

    display.setCursor(0, 16);
    display.println(line2);

    display.setCursor(0, 32);
    display.println(line3);

    display.display();
}

void oledShow(int temp, int humid) {
    display.clearDisplay();

    // ==== TEMP ====
    display.setTextSize(2);
    display.setCursor(0, 0);
    display.print((char)0xF8);   // icon nhiệt độ

    display.setCursor(20, 0);
    display.print(temp);
    display.print("C");

    // ==== HUMID ====
    display.setCursor(0, 24);
    display.print((char)0x7E);   // icon giọt nước

    display.setCursor(20, 24);
    display.print(humid);
    display.print("%");

    // ==== LINE ====
    display.drawLine(0, 44, 128, 44, WHITE);

    display.display();
}
