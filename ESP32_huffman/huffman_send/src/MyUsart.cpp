#include "MyUsart.h"

HardwareSerial SerialUART(2);

void initUART(int baud, int rxPin, int txPin) {
    SerialUART.begin(baud, SERIAL_8N1, rxPin, txPin);
}

void uartSendString(const String &data) {
    SerialUART.println(data);
}

String uartReceiveString() {
    if (SerialUART.available()) {
        return SerialUART.readString();
    }
    return "";
}



void uartSendBytes(const std::vector<uint8_t> &packet) {
    for (uint8_t b : packet) {
        SerialUART.write(b);
    }
}

std::vector<uint8_t> uartReceiveBytes() {
    std::vector<uint8_t> data;

    while (SerialUART.available()) {
        data.push_back(SerialUART.read());
    }

    return data;
}
