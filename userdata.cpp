#include "mpesa.h"
#include <fstream>
#include <iostream>
#include <chrono>
#include <iomanip>
#include <random>
#include <ctime>

using namespace std;

bool isFirstRun() {
    ifstream userFile("mpesa_data.txt");
    return !userFile.good();
}

void addTransaction(UserData &userData, const string &transaction) {
    auto now = chrono::system_clock::now();
    time_t currentTime = chrono::system_clock::to_time_t(now);
    string timestamp = ctime(&currentTime);
    timestamp.pop_back(); // Remove newline
    userData.transactionHistory.push_back(timestamp + " - " + transaction);
}

void saveUserData(const UserData &data) {
    ofstream userFile("mpesa_data.txt");
    if (userFile.is_open()) {
        userFile << data.pin << endl;
        userFile << data.balance << endl;
        userFile << data.name << endl;
        userFile << data.phoneNumber << endl;
        userFile << data.gender << endl;
        userFile << data.region << endl;
        for (const auto &transaction : data.transactionHistory) {
            userFile << "TRANSACTION:" << transaction << endl;
        }
        userFile.close();
    } else {
        cout << "Error: Unable to save user data" << endl;
    }
}

UserData loadUserData() {
    UserData data;
    ifstream userFile("mpesa_data.txt");
    if (userFile.is_open()) {
        userFile >> data.pin;
        userFile >> data.balance;
        userFile.ignore(numeric_limits<streamsize>::max(), '\n');
        
        getline(userFile, data.name);
        getline(userFile, data.phoneNumber);
        getline(userFile, data.gender);
        getline(userFile, data.region);
        
        string line;
        while (getline(userFile, line)) {
            if (line.substr(0, 12) == "TRANSACTION:") {
                data.transactionHistory.push_back(line.substr(12));
            }
        }
        userFile.close();
    } else {
        cout << "Error: Unable to load user data" << endl;
    }
    return data;
}

void viewAccount(const UserData &userData) {
    system("cls");  // Clear screen
    cout << "\n=== Account Information ===" << endl;
    cout << "Name: " << (userData.name.empty() ? "Not set" : userData.name) << endl;
    cout << "Phone Number: " << (userData.phoneNumber.empty() ? "Not set" : userData.phoneNumber) << endl;
    cout << "Current Balance: " << userData.balance << " TZS" << endl;
    cout << "Gender: " << (userData.gender.empty() ? "Not set" : userData.gender) << endl;
    cout << "Region: " << (userData.region.empty() ? "Not set" : userData.region) << endl;
    
    cout << "\n=== Recent Transactions ===" << endl;
    if (userData.transactionHistory.empty()) {
        cout << "No transactions found" << endl;
    } else {
        cout << setw(25) << left << "Date/Time" << setw(15) << "Type" << setw(15) << "Amount" << "Details" << endl;
        cout << string(70, '-') << endl;
        
        int count = 0;
        for (const auto &transaction : userData.transactionHistory) {
            cout << transaction << endl;
            if (++count >= 10) break; // Show only last 10 transactions
        }
    }
    
    cout << "\nPress Enter to return to main menu...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
    system("cls");
}

string generateTransactionID() {
    const string chars = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    string id = "CC";
    
    // Initialize random seed
    static random_device rd;
    static mt19937 gen(rd());
    static uniform_int_distribution<> dis(0, chars.length() - 1);
    
    // Generate 8 random characters
    for (int i = 0; i < 8; i++) {
        id += chars[dis(gen)];
    }
    
    return id;
}

void initializeNewUser(UserData &userData) {
    cout << "\nLet's set up your M-PESA account!" << endl;
    
    cout << "\nPlease enter your full name: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, userData.name);
    while (userData.name.empty() || userData.name.find_first_not_of(" ") == string::npos) {
        cout << "Name cannot be empty. Please enter your full name: ";
        getline(cin, userData.name);
    }
    
    cout << "Enter your phone number (format: 255xxxxxxxxx): ";
    while (true) {
        getline(cin, userData.phoneNumber);
        if (userData.phoneNumber.length() == 12 && userData.phoneNumber.substr(0, 3) == "255") {
            break;
        }
        cout << "Invalid phone number. Please enter in format 255xxxxxxxxx: ";
    }
    
    cout << "Enter your gender (M/F/O): ";
    getline(cin, userData.gender);
    
    cout << "Enter your region: ";
    getline(cin, userData.region);
    
    cout << "\nCreate your M-PESA PIN (4 digits): ";
    int pin1, pin2;
    while (true) {
        while (!(cin >> pin1) || cin.peek() != '\n' || pin1 < 1000 || pin1 > 9999) {
            cout << "Invalid PIN. Please enter a 4-digit number: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        
        cout << "Confirm your PIN: ";
        while (!(cin >> pin2) || cin.peek() != '\n' || pin2 < 1000 || pin2 > 9999) {
            cout << "Invalid PIN. Please enter a 4-digit number: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        
        if (pin1 == pin2) {
            userData.pin = pin1;
            break;
        } else {
            cout << "PINs do not match. Please try again.\n";
            cout << "Create your M-PESA PIN (4 digits): ";
        }
    }
    
    userData.balance = 0;
    saveUserData(userData);
    
    cout << "\nYour account has been created successfully!" << endl;
    cout << "Would you like to add money to your account? (1-Yes, 2-No): ";
    int choice;
    while (!(cin >> choice) || cin.peek() != '\n' || choice < 1 || choice > 2) {
        cout << "Invalid choice. Please enter 1 for Yes or 2 for No: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    
    if (choice == 1) {
        addMoney(userData);
    }
}