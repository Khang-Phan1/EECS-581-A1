#include <iostream>
#include <print>
using namespace std;

bool extractIPv4(const std::string& str, unsigned long& outAddress, int& outPort) {
    return true;
}

int main() {
    string userString;
    string ipv4Address = "192.168.1.1";
    unsigned long outAddress = 3232235777;
    int outPort = 0;
    bool isValidAddress;

    while (1) {
        userString = "";
        cout << "\nEnter a string (or \'END\' to quit): ";
        cin >> userString;
        if (userString == "END") {
            cout << "Program terminated.";
            break;
        } 

        isValidAddress = extractIPv4(userString, outAddress, outPort);
        if (isValidAddress) {
            string address = to_string(outAddress);
            string port;
            if (outPort != 0) {
                port = to_string(outPort);
            }
            else {
                port = "none";
            }
            cout << "Extracted IPv4 Address: " + ipv4Address + " (decimal value: " + address + ", port: " + port + ")\n";
        }
        else {
            cout << "Invalid input: no valid IPv4 address found.\n";
        }
    }

    return 0;
}