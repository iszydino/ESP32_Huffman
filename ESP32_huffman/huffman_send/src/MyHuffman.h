#ifndef _MY_HUFFMAN_H_
#define _MY_HUFFMAN_H_

#include <Arduino.h>
#include <vector>

std::vector<uint8_t> huffmanEncode(int temp, int humidity);

String huffmanDecodeToString(const std::vector<uint8_t> &packet);

bool huffmanDecodeToValues(const std::vector<uint8_t> &packet, int &temp, int &humidity);

// Giải thích quá trình mã hóa -> BÊN GỬI
String huffmanExplainEncode(int temp, int humidity);

// Giải thích quá trình giải mã -> BÊN NHẬN
String huffmanExplainDecode(const std::vector<uint8_t> &packet);
std::vector<String> huffmanDecodeSteps(const std::vector<uint8_t> &packet);


#endif
