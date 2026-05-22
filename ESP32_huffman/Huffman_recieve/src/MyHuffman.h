#ifndef _MY_HUFFMAN_H_
#define _MY_HUFFMAN_H_

#include <Arduino.h>
#include <vector>

std::vector<uint8_t> huffmanEncode(int temp, int humidity);

String huffmanDecodeToString(const std::vector<uint8_t> &packet);

bool huffmanDecodeToValues(const std::vector<uint8_t> &packet, int &temp, int &humidity);
String huffmanExplain(const std::vector<uint8_t> &packet);

#endif
