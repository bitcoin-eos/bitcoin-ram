#pragma once

#include <cstdio>
#include <string>
#include <vector>

namespace util {
    static vector<unsigned char> hexStringToBytes(const string& hex) {
        vector<unsigned char> result;
        unsigned char byte;
        for (size_t i = 0; i < hex.length(); i += 2) {
            sscanf(hex.c_str() + i, "%2hhx", &byte);
            result.push_back(byte);
        }
        return result;
    }

    static string hexCharToString(unsigned char hex_value) {
        static const char* hex_chars = "0123456789ABCDEF";

        string hex_str(2, '\0');
        hex_str[0] = hex_chars[(hex_value >> 4) & 0x0F];
        hex_str[1] = hex_chars[hex_value & 0x0F];

        return hex_str;
    }

    static char hexCharToByte(char hexChar) {
        if (hexChar >= '0' && hexChar <= '9') {
            return hexChar - '0';
        } else if (hexChar >= 'A' && hexChar <= 'F') {
            return hexChar - 'A' + 10;
        } else if (hexChar >= 'a' && hexChar <= 'f') {
            return hexChar - 'a' + 10;
        } else {
            eosio::check(false, "Invalid hex character");
        }
    }

    static string hexStringToAscii(const string& hex) {
        assert(hex.size() % 2 == 0);

        string asciiStr;
        asciiStr.reserve(hex.size() / 2);

        for (size_t i = 0; i < hex.size(); i += 2) {
            char highNibble = hex[i];
            char lowNibble = hex[i + 1];

            int byteValue = hexCharToByte(highNibble);
            byteValue <<= 4;
            byteValue += hexCharToByte(lowNibble);

            asciiStr.push_back(static_cast<char>(byteValue));
        }

        return asciiStr;
    }

    static string p2trToString(const vector<tuple<string, string>>& m) {
        string str;
        for (auto it = m.begin(); it != m.end(); ++it) str += std::get<0>(*it) + " " + std::get<1>(*it) + " ";
        return str;
    }
};  // namespace util