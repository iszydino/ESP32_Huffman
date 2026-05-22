#include "MyHuffman.h"

// -------------------------
// BẢNG MÃ HUFFMAN TĨNH 4 BIT
// -------------------------

static int encodeSymbol(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c == '|') return 10;
    if (c == '-') return 11;
    return -1;
}

static char decodeSymbol(uint8_t nibble) {
    if (nibble <= 9) return char('0' + nibble);
    if (nibble == 10) return '|';
    if (nibble == 11) return '-';
    return '?';
}

// =====================================================
//         GIẢI THÍCH QUÁ TRÌNH MÃ HÓA (BÊN GỬI)
// =====================================================

String huffmanExplainEncode(int temp, int humidity)
{
    String raw = String(temp) + "|" + String(humidity);
    String out = "=== ENCODE PROCESS ===\n";

    out += "Original: " + raw + "\n";
    out += "Chars -> nibbles:\n";

    for (int i = 0; i < raw.length(); i++) {
        char c = raw[i];
        int nib = encodeSymbol(c);

        out += " '";
        out += c;
        out += "' -> ";
        out += String(nib, BIN) + "\n";
    }

    out += "Bitstream (groups 4bit): ";
    for (int i = 0; i < raw.length(); i++) {
        int nib = encodeSymbol(raw[i]);
        out += String(nib, BIN) + " ";
    }
    out += "\n";

    return out;
}

// =====================================================
//                    HÀM MÃ HÓA
// =====================================================

std::vector<uint8_t> huffmanEncode(int temp, int humidity)
{
    String raw = String(temp) + "|" + String(humidity);
    std::string text = raw.c_str();

    uint16_t totalBits = text.length() * 4;
    if (totalBits > 255) totalBits = 255;

    std::vector<uint8_t> payload;
    payload.reserve((totalBits + 7) / 8);

    uint8_t cur = 0;
    int bitCount = 0;

    for (char c : text) {
        int nibble = encodeSymbol(c);
        if (nibble < 0) continue;

        for (int bitPos = 3; bitPos >= 0; bitPos--) {
            uint8_t bit = (nibble >> bitPos) & 1;
            cur = (cur << 1) | bit;
            bitCount++;

            if (bitCount == 8) {
                payload.push_back(cur);
                cur = 0;
                bitCount = 0;
            }
        }
    }

    if (bitCount > 0) {
        cur <<= (8 - bitCount);
        payload.push_back(cur);
    }

    std::vector<uint8_t> packet;
    packet.push_back((uint8_t)totalBits);
    packet.insert(packet.end(), payload.begin(), payload.end());

    return packet;
}

// =====================================================
//         HÀM GIẢI MÃ RA CHUỖI "26|60"
// =====================================================

String huffmanDecodeToString(const std::vector<uint8_t> &packet)
{
    if (packet.size() < 2) return "";

    uint16_t totalBits = packet[0];
    size_t needed = 1 + ((totalBits + 7) / 8);
    if (packet.size() < needed) return "";

    int numSymbols = totalBits / 4;
    String res = "";

    for (int sym = 0; sym < numSymbols; sym++) {
        uint8_t nib = 0;

        for (int b = 0; b < 4; b++) {
            int globalBit = sym * 4 + b;
            int byteIndex = 1 + (globalBit / 8);
            int bitPos = 7 - (globalBit % 8);

            uint8_t bit = (packet[byteIndex] >> bitPos) & 1;
            nib = (nib << 1) | bit;
        }

        char c = decodeSymbol(nib);
        res += c;
    }

    return res;
}

// =====================================================
//         GIẢI THÍCH QUÁ TRÌNH GIẢI MÃ (BÊN NHẬN)
// =====================================================

String huffmanExplainDecode(const std::vector<uint8_t> &packet)
{
    String out = "=== DECODE PROCESS ===\n";

    if (packet.size() < 2) return "ERR PACK\n";

    uint16_t totalBits = packet[0];
    out += "Total bits = " + String(totalBits) + "\n";

    // In bitstream
    out += "Bitstream:\n";
    for (int i = 0; i < totalBits; i++) {
        int byteIndex = 1 + (i / 8);
        int bitPos = 7 - (i % 8);
        uint8_t bit = (packet[byteIndex] >> bitPos) & 1;
        out += bit ? '1' : '0';
    }
    out += "\n\n";

    // In từng nibble
    int numSymbols = totalBits / 4;
    out += "Nibbles:\n";

    for (int sym = 0; sym < numSymbols; sym++) {
        uint8_t nib = 0;

        for (int b = 0; b < 4; b++) {
            int globalBit = sym * 4 + b;
            int byteIndex = 1 + (globalBit / 8);
            int bitPos = 7 - (globalBit % 8);
            uint8_t bit = (packet[byteIndex] >> bitPos) & 1;
            nib = (nib << 1) | bit;
        }

        out += String(nib, BIN) + " -> '";
        out += decodeSymbol(nib);
        out += "'\n";
    }

    return out;
}
std::vector<String> huffmanDecodeSteps(const std::vector<uint8_t> &packet)
{
    std::vector<String> steps;

    if (packet.size() < 2) {
        steps.push_back("ERR PACK");
        return steps;
    }

    uint16_t totalBits = packet[0];
    int numSymbols = totalBits / 4;

    // từng ký tự một
    for (int sym = 0; sym < numSymbols; sym++) {
        uint8_t nibble = 0;

        for (int b = 0; b < 4; b++) {
            int globalBit = sym * 4 + b;
            int byteIndex = 1 + (globalBit / 8);
            int bitPos    = 7 - (globalBit % 8);
            uint8_t bit   = (packet[byteIndex] >> bitPos) & 1;
            nibble = (nibble << 1) | bit;
        }

        char c = decodeSymbol(nibble);
        String step = "DEC: " + String(nibble, BIN) + " -> '" + c + "'";
        steps.push_back(step);
    }

    return steps;
}


// =====================================================
//         GIẢI MÃ TRỰC TIẾP RA SỐ
// =====================================================

bool huffmanDecodeToValues(const std::vector<uint8_t> &packet, int &temp, int &humidity)
{
    String decoded = huffmanDecodeToString(packet);
    if (decoded.length() == 0) return false;

    int idx = decoded.indexOf('|');
    if (idx < 0) return false;

    temp = decoded.substring(0, idx).toInt();
    humidity = decoded.substring(idx + 1).toInt();
    return true;
}
