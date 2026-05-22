#include "MyHuffman.h"

// -------------------------
// BẢNG MÃ HUFFMAN TĨNH 4 BIT
// -------------------------
//
// 0  -> 0000 (0)
// 1  -> 0001 (1)
// 2  -> 0010 (2)
// 3  -> 0011 (3)
// 4  -> 0100 (4)
// 5  -> 0101 (5)
// 6  -> 0110 (6)
// 7  -> 0111 (7)
// 8  -> 1000 (8)
// 9  -> 1001 (9)
// '|'-> 1010 (10)
// '-'-> 1011 (11)

static int encodeSymbol(char c) {
    if (c >= '0' && c <= '9') {
        return c - '0';  // 0..9
    }
    if (c == '|') {
        return 10;
    }
    if (c == '-') {
        return 11;
    }
    return -1;  // ký tự không hỗ trợ
}

static char decodeSymbol(uint8_t nibble) {
    if (nibble <= 9) {
        return char('0' + nibble);
    }
    if (nibble == 10) {
        return '|';
    }
    if (nibble == 11) {
        return '-';
    }
    return '?';  // lỗi / không hợp lệ
}

// ========================
//       HÀM MÃ HÓA
// ========================

String huffmanExplain(const std::vector<uint8_t> &packet)
{
    String out = "";

    if (packet.size() < 2) return "ERR PACK";

    uint16_t totalBits = packet[0];
    int numSymbols = totalBits / 4;

    out += "bits=" + String(totalBits) + "\n";

    // In toàn bộ chuỗi bit nhận được
    out += "bitstream:\n";

    for (int i = 0; i < totalBits; i++) {
        int byteIndex = 1 + (i / 8);
        int bitPos = 7 - (i % 8);
        uint8_t bit = (packet[byteIndex] >> bitPos) & 0x01;
        out += (bit ? "1" : "0");
    }
    out += "\n";

    // In từng nibble
    out += "decode:\n";

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

        out += String(nib, BIN) + " -> ";
        out += "'";
        out += c;
        out += "'\n";
    }

    return out;
}

std::vector<uint8_t> huffmanEncode(int temp, int humidity)
{
    // Chuỗi gốc dạng: "26|60"
    String raw = String(temp) + "|" + String(humidity);
    std::string text = raw.c_str();

    uint16_t totalBits = text.length() * 4;  // mỗi ký tự 4 bit

    // Với dữ liệu DHT11 (0..99) thì totalBits chắc chắn < 255
    if (totalBits > 255) {
        totalBits = 255; // tránh tràn, nhưng thực tế không bao giờ tới
    }

    std::vector<uint8_t> payload;
    payload.reserve((totalBits + 7) / 8);

    uint8_t curByte = 0;
    int bitCount = 0;

    // Pack từng nibble (4 bit) vào byte, MSB trước
    for (char c : text) {
        int nibble = encodeSymbol(c);
        if (nibble < 0) {
            // Ký tự không hợp lệ, bỏ qua hoặc bạn có thể xử lý lỗi riêng
            continue;
        }

        for (int bitPos = 3; bitPos >= 0; bitPos--) {
            uint8_t bit = (nibble >> bitPos) & 0x01;
            curByte = (curByte << 1) | bit;
            bitCount++;

            if (bitCount == 8) {
                payload.push_back(curByte);
                curByte = 0;
                bitCount = 0;
            }
        }
    }

    // Nếu còn dư bit, pad thêm 0 ở LSB
    if (bitCount > 0) {
        curByte <<= (8 - bitCount);
        payload.push_back(curByte);
    }

    // Đóng gói final packet: [0] = totalBits, [1..] = payload
    std::vector<uint8_t> packet;
    packet.reserve(payload.size() + 1);
    packet.push_back((uint8_t)totalBits);
    packet.insert(packet.end(), payload.begin(), payload.end());

    return packet;
}

// ========================
//       HÀM GIẢI MÃ
// ========================

String huffmanDecodeToString(const std::vector<uint8_t> &packet)
{
    if (packet.size() < 2) {
        return "";
    }

    uint16_t totalBits = packet[0];
    size_t neededBytes = 1 + ((totalBits + 7) / 8);

    if (packet.size() < neededBytes) {
        // Dữ liệu không đủ
        return "";
    }

    int numSymbols = totalBits / 4;  // mỗi ký tự 4 bit
    String result = "";

    // Đọc từng 4 bit thành 1 ký tự
    for (int sym = 0; sym < numSymbols; sym++) {
        uint8_t nibble = 0;

        for (int b = 0; b < 4; b++) {
            int globalBitIndex = sym * 4 + b;      // bit thứ mấy trong toàn bộ payload
            int byteIndex      = 1 + (globalBitIndex / 8); // byte nào trong packet
            int bitInByte      = 7 - (globalBitIndex % 8); // bit nào trong byte (MSB trước)

            uint8_t bit = (packet[byteIndex] >> bitInByte) & 0x01;
            nibble = (nibble << 1) | bit;
        }

        char ch = decodeSymbol(nibble);
        result += ch;
    }

    return result;
}

bool huffmanDecodeToValues(const std::vector<uint8_t> &packet, int &temp, int &humidity)
{
    String decoded = huffmanDecodeToString(packet);
    if (decoded.length() == 0) {
        return false;
    }

    int sepIndex = decoded.indexOf('|');
    if (sepIndex < 0) {
        return false;  // không có ký tự '|'
    }

    String tempStr  = decoded.substring(0, sepIndex);
    String humidStr = decoded.substring(sepIndex + 1);

    temp    = tempStr.toInt();
    humidity = humidStr.toInt();

    return true;
}
