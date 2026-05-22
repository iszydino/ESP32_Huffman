#include <Arduino.h>
#include "MyUsart.h"
#include "MyHuffman.h"
#include "MyOled.h"
#include "main.h"

void setup() {
    Serial.begin(115200);
    Serial.println("ESP32 Receiver Start...");

    initUART(9600, UART_RX, UART_TX);
    Serial.println("UART Ready!");
    if (!initOLED(SCREEN_WIDTH, SCREEN_HEIGHT, I2C_SDA, I2C_SCL)) {
        Serial.println("OLED ERROR!");
        while(1);
    }
    Serial.println("OLED Ready!");

    oledShowStatus("Waiting...");
}

void loop() {

    // =====================================================
    //                1. NHẬN DỮ LIỆU TỪ UART
    // =====================================================
    std::vector<uint8_t> rx = uartReceiveBytes();

    if (!rx.empty()) {

        Serial.print("UART RX RAW: ");
        for (uint8_t b : rx) {
            if (b < 0x10) Serial.print('0');
            Serial.print(b, HEX);
            Serial.print(' ');
        }
        Serial.println();

        // =====================================================
        //                2. GIẢI MÃ HUFFMAN
        // =====================================================
        int temp, humid;

        if (huffmanDecodeToValues(rx, temp, humid)) {

            Serial.println("=== Huffman Decode OK ===");
            Serial.print("Temp: "); Serial.print(temp); Serial.println(" °C");
            Serial.print("Humid: "); Serial.print(humid); Serial.println(" %");

            // =====================================================
            //                3. HIỂN THỊ LÊN OLED
            // =====================================================
            oledShow(temp, humid);        // giao diện đẹp
            oledShowStatus("RX OK");      // dòng trạng thái

        } else {
            Serial.println("❌ Lỗi giải mã Huffman!");
            oledShowStatus("Decode ERR");
        }
    }

    delay(100);
}
