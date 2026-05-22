#include "main.h"
#include "MyHuffman.h"
#include "MyUsart.h"
#include "MyOled.h"

void setup() { 
    Serial.begin(115200); 
    Serial.println("ESP32 Sender Start...");

    // DHT11
    initDHT(DHT_PIN, DHT_TYPE);

    // OLED
    if (!initOLED(SCREEN_WIDTH, SCREEN_HEIGHT, I2C_SDA, I2C_SCL)) {   
        Serial.println("OLED ERROR!");
        while (1);
    }
    oledShowStatus("START...");

    // UART
    initUART(9600, UART_RX, UART_TX);
    Serial.println("UART OK!");
} 
 
void loop() { 
    // ======================================================
    //                1. ĐỌC CẢM BIẾN
    // ======================================================
    float t = readTemperature(); 
    float h = readHumidity(); 

    if (isnan(t) || isnan(h)) { 
        Serial.println("❌ Loi doc cam bien!"); 
        oledShowStatus("Sensor ERR");
        delay(500);
        return;
    } 

    int ti = (int)t;
    int hi = (int)h;

    Serial.println("=== SENSOR DATA ===");
    Serial.printf("Nhiet do: %.1f *C\n", t);
    Serial.printf("Do am   : %.1f %%\n", h);

    // Hiển thị lên OLED
    oledShow(ti, hi);
    oledShowStatus("ENCODING...");

    // ======================================================
    //       2. IN QUÁ TRÌNH MÃ HÓA HUFFMAN (BÊN GỬI)
    // ======================================================
    Serial.println("=== HUFFMAN ENCODE PROCESS ===");
    Serial.println(huffmanExplainEncode(ti, hi));

    // ======================================================
    //       3. MÃ HÓA & GỬI QUA UART
    // ======================================================
    std::vector<uint8_t> packet = huffmanEncode(ti, hi);

    uartSendBytes(packet);   // gửi nhị phân

    Serial.print("UART TX Huffman (HEX): ");
    for (uint8_t b : packet) {
        if (b < 0x10) Serial.print('0');
        Serial.print(b, HEX);
        Serial.print(' ');
    }
    Serial.println();
    Serial.println("------------------------------------");

    oledShowStatus("SENT!");

    // ======================================================
    //       4. (TÙY CHỌN) NHẬN PHẢN HỒI
    // ======================================================
    std::vector<uint8_t> rx = uartReceiveBytes();
    if (!rx.empty()) {
        Serial.print("UART RX RAW (from receiver): ");
        for (uint8_t b : rx) {
            if (b < 0x10) Serial.print('0');
            Serial.print(b, HEX);
            Serial.print(' ');
        }
        Serial.println();
    }

    delay(1500);
}
