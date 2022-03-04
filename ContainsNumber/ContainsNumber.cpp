// ContainsNumber.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>

int main()
{

    std::string input;
input:
    std::cout << "Please enter a valid number: ";
    std::cin >> input;
    std::cout << "\n";
    try {
        int number = std::stoi(input);
    }
    catch (...) {
        std::cout << "Number is invalid!\n";
        goto input;
    }

    int digit;
    std::cout << "Search for digit: ";
    std::cin >> digit;
    std::cout << "\n";
    while (digit < 0 || digit > 9) {
        std::cout << "Please only type in a single digit!\n";
        std::cout << "Search for digit: ";
        std::cin >> digit;
    }

    char digit_as_char = digit + 48;
    bool is_valid = false;
    for (int i = 0; i < input.size(); i++) {
        if (input[i] == digit_as_char) {
            is_valid = true;
            break;
        }
    }
    if (is_valid) {
        std::cout << input << " contains the number " << digit_as_char << "\n";
    }
    else {
        std::cout << input << " does not contain the number " << digit_as_char << "\n";
    }
    return 0;
}
