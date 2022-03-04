// StringCompression.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
std::string compress(std::string that)
{
    std::string compressed{};
    auto count = 0;
    char prev = *that.begin();
    for (auto& c : that) {
        if (prev != c) {
            compressed += std::string{ prev } + std::to_string(count);
            count = 0;
        }
        count++;
        prev = c;
    }
    compressed += std::string{ prev } + std::to_string(count);
    return compressed;
}

std::string decompress(std::string that)
{
    std::string decompressed{};

    char prev = *that.begin();
    auto is_number = [](char c) {
        return c >= '0' && c <= '9';
    };

    for (int i = 0; i < that.size(); i++) {
        auto& current = that[i];

        // Build number
        std::string number{};
        int j = i + 1;
        for (; j < that.size(); j++) {
            auto next = that[j];
            if (is_number(next)) {
                number += next;
            }
            else {
                i = j - 1;
                break;
            }
        }

        // Attach amount
        if (!number.empty()) {
            auto count = std::stoi(number);
            for (int i = 0; i < count; i++) {
                decompressed += current;
            }
        }
    }
    return decompressed;
}

int main()
{
    std::string a{ "XAAAAAAAAAAAAAAAAAAAABBBCCCDDDEEEEEX" };
    std::cout << a << "\n";
    auto compressed = compress(a);
    std::cout << compressed << "\n";
    std::cout << decompress(compressed);
}
