#ifndef _MY_USART_H_
#define _MY_USART_H_

#include <Arduino.h>
#include <vector>    

void initUART(int baud, int rxPin, int txPin);

void uartSendString(const String &data);
String uartReceiveString();

void uartSendBytes(const std::vector<uint8_t> &packet);
std::vector<uint8_t> uartReceiveBytes();

#endif
