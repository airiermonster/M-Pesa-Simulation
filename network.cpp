#include "mpesa.h"
#include <unordered_map>
#include <string>

using namespace std;

const unordered_map<string, string> networkMap = {
    {"071", "MIC Tanzania PLC"},
    {"065", "MIC Tanzania PLC"},
    {"067", "MIC Tanzania PLC"},
    {"073", "Tanzania Telecommunications Corporation"},
    {"074", "Vodacom Tanzania PLC"},
    {"075", "Vodacom Tanzania PLC"},
    {"076", "Vodacom Tanzania PLC"},
    {"077", "Zanzibar Telecom PLC"},
    {"078", "Airtel Tanzania PLC"},
    {"068", "Airtel Tanzania PLC"},
    {"069", "Airtel Tanzania PLC"},
    {"066", "Smile Communications Tanzania Limited"},
    {"061", "Viettel Tanzania PLC"},
    {"062", "Viettel Tanzania PLC"},
    {"063", "Mkulima African Telecommunication Company Limited"},
    {"064", "Wiafrica Tanzania Limited"},
    {"072", "MO Mobile Holding Limited"}
};

string formatPhoneNumber(const string &input) {
    string formatted;
    
    if (input.substr(0, 3) == "255") {
        // For numbers starting with 255, take next 2 digits and add 0
        formatted = "0" + input.substr(3, 2);
    } else if (input[0] == '+' && input.substr(1, 3) == "255") {
        // For numbers starting with +255, take next 2 digits and add 0
        formatted = "0" + input.substr(4, 2);
    } else if (input[0] == '0') {
        // For numbers starting with 0, take first 3 digits
        formatted = input.substr(0, 3);
    } else {
        // For any other format, assume it's the network code directly
        formatted = "0" + input.substr(0, 2);
    }
    
    return formatted;
}

string getNetwork(const string &phoneNumber) {
    if (phoneNumber.empty() || phoneNumber.length() < 3) {
        return "Invalid Number";
    }

    string networkCode = formatPhoneNumber(phoneNumber);
    auto it = networkMap.find(networkCode);
    return (it != networkMap.end()) ? it->second : "Unknown Network";
}