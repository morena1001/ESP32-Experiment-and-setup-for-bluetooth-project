#include <iostream>
#include <string>
#include <sstream>
#include <iomanip> // For std::setw and std::setfill

// Function to convert a UTF-8 string to its hexadecimal representation
std::string utf8ToHex(const std::string& utf8_string) {
    std::ostringstream oss;
    oss << std::hex << std::uppercase << std::setfill('0'); // Set hex format, uppercase, and fill with '0'

    for (unsigned char c : utf8_string) {
        oss << std::setw(2) << static_cast<int>(c); // Output each byte as a two-digit hex number
    }
    return oss.str();
}

int main() {
    // Example with an ASCII character (single byte in UTF-8)
    std::string char_a = "a";
    std::cout << "UTF-8 for 'a': " << utf8ToHex(char_a) << std::endl; // Expected: 61

    // Example with a multi-byte UTF-8 character (e.g., 'é')
    std::string char_e_acute = "é"; // This character is typically represented by two bytes in UTF-8
    std::cout << "UTF-8 for 'é': " << utf8ToHex(char_e_acute) << std::endl; // Expected: C3A9

    // Example with a Chinese character (e.g., '学')
    std::string char_chinese = "学"; // This character is typically represented by three bytes in UTF-8
    std::cout << "UTF-8 for '学': " << utf8ToHex(char_chinese) << std::endl; // Expected: E5AD A6

    return 0;
}