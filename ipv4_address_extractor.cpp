/*
Prologue Comment
File: ipv4_address_extractor.cpp
Description: Takes a user's string input and extracts an ipv4 address
             from it. Displays the dotted decimal and 32-bit forms
             along with a port if the string contains one.
Inputs:  A user string
Outputs: The dotted decimal and 32-bit ipv4 address and port if
         applicable. Otherwise, lets user know that there is no
         valid address.
External sources: Original user input handler and display, with
                  assistance by ChatGPT for parsing and extraction
                  logic.
Author: Khang Phan
Created: [sept 22 2026]
*/

#include <iostream>
#include <string>
using namespace std;

// Helper function to convert the 32-bit IP address to dotted decimal.
string convertBinaryToDotted(const unsigned long outAddress) {
    string a = to_string((outAddress >> 24) & 0xFF);
    string b = to_string((outAddress >> 16) & 0xFF);
    string c = to_string((outAddress >> 8) & 0xFF);
    string d = to_string(outAddress & 0xFF);
    return a + "." + b + "." + c + "." + d;
}

// Parse a decimal number from str starting at pos.
//
// The number must:
//   - contain 1 to maxDigits digits
//   - have no leading zero unless it is exactly "0"
//   - have a value <= maxValue
//
// On success, pos is advanced past the number.
static bool parseNumber(const std::string& str, size_t& pos, int maxDigits, int maxValue, int& value) {
    size_t start = pos;

    // Checks that the first character is a digit.
    if (pos >= str.length() || str[pos] < '0' || str[pos] > '9') {
        return false;
    }

    // Count # of digits.
    int digitCount = 0;
    while (pos < str.length() && str[pos] >= '0' && str[pos] <= '9') {
        ++digitCount;
        ++pos;

        if (digitCount > maxDigits) {
            return false;
        }
    }

    // No leading zero unless the number is exactly "0".
    if (digitCount > 1 && str[start] == '0') {
        return false;
    }

    // Convert string to integer type.
    value = 0;

    for (size_t i = start; i < pos; ++i) {
        int digit = str[i] - '0';

        // Check for overflow against maxValue before multiplying.
        if (value > (maxValue - digit) / 10) {
            return false;
        }

        value = value * 10 + digit;
    }

    return true;
}

// Returns true if a valid address was found, false otherwise.
// On success:
//   outAddress holds the 32-bit IPv4 value.
//   outPort holds the port number, or -1 if no port was present.
// On failure:
//   outAddress is set to 0.
//   outPort is set to -1.
bool extractIPv4(const std::string& str, unsigned long& outAddress, int& outPort) {
    // Required failure values.
    outAddress = 0;
    outPort = -1;

    size_t i = 0;

    while (i < str.length()) {
        // Skip garbage characters.
        // A candidate token consists only of:
        // digits, '.', ':'
        if (!((str[i] >= '0' && str[i] <= '9') || str[i] == '.' || str[i] == ':')) {
            ++i;
            continue;
        }

        // Current position in string being parsed
        size_t pos = i;

        // End position of the entire candidate token.
        while (i < str.length() && ((str[i] >= '0' && str[i] <= '9') || str[i] == '.' || str[i] == ':')) {
            ++i;
        }
        size_t tokenEnd = i;

        int octets[4];

        bool valid = true;

        // Parse the four IPv4 octets.
        for (int octet = 0; octet < 4; ++octet) {
            if (!parseNumber(str, pos, 3, 255, octets[octet])) {
                valid = false;
                break;
            }

            // First three octets must be followed by '.'.
            if (octet < 3) {
                if (pos >= tokenEnd || str[pos] != '.') {
                    valid = false;
                    break;
                }

                ++pos;
            }
        }

        // After the fourth octet, there may be an optional port.
        int port = -1;

        if (valid && pos < tokenEnd) {
            // The only valid thing after the fourth octet is ":port".
            if (str[pos] != ':') {
                valid = false;
            }
            else {
                ++pos;

                if (!parseNumber(str, pos, 5, 65535, port)) {
                    valid = false;
                }
            }
        }

        // The entire candidate token must have been consumed.
        // This prevents accepting things such as 192.168.1.1:80:90.
        if (pos != tokenEnd) {
            valid = false;
        }

        if (!valid) {
            continue;
        }

        // Construct the 32-bit IPv4 value
        outAddress =
            (static_cast<unsigned long>(octets[0]) << 24) |
            (static_cast<unsigned long>(octets[1]) << 16) |
            (static_cast<unsigned long>(octets[2]) << 8)  |
             static_cast<unsigned long>(octets[3]);

        outPort = port;

        return true;
    }

    return false;
}

int main() {
    string userString;
    unsigned long outAddress;
    int outPort;

    while (true) {
        // Ask for user string, breaks loop
        // and ends program if 'END'
        cout << "\nEnter a string (or \'END\' to quit): ";
        getline(cin, userString);
        if (userString == "END") {
            cout << "Program terminated.";
            break;
        } 

        // Check for valid address in string, extracts it
        if (extractIPv4(userString, outAddress, outPort)) {
            string address = to_string(outAddress);
            string port = (outPort != -1) ? to_string(outPort) : "none";
            string dottedAddr = convertBinaryToDotted(outAddress);

            cout << "Extracted IPv4 address: " + dottedAddr + " (decimal value: " + address + ", port: " + port + ")\n";
        }
        else {
            cout << "Invalid input: no valid IPv4 address found.\n";
        }
    }

    return 0;
}