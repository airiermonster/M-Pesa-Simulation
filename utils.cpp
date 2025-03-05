#include "mpesa.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <iomanip>
#include <sstream>

using namespace std;

void showProcessing(const string &message = "Processing") {
    cout << message;
    for (int i = 0; i < 3; ++i) {
        cout << ".";
        cout.flush();
        this_thread::sleep_for(chrono::milliseconds(500));
        cout << ".";
        cout.flush();
        this_thread::sleep_for(chrono::milliseconds(500));
        cout << "\b\b";  // Backspace to remove dots
    }
    cout << "...Done!" << endl;
    this_thread::sleep_for(chrono::milliseconds(500));
}

void showSuccess(const string &message) {
    cout << "\n✓ " << message << endl;
    this_thread::sleep_for(chrono::milliseconds(1000));
}

void showError(const string &message) {
    cout << "\n✗ " << message << endl;
    this_thread::sleep_for(chrono::milliseconds(1000));
}

void showHelp() {
    cout << "\n=== M-PESA Help ===" << endl;
    cout << "1. Send Money - Transfer money to other M-PESA users" << endl;
    cout << "2. Buy Airtime - Purchase airtime for yourself or others" << endl;
    cout << "3. Withdraw Cash - Get cash from M-PESA agents or ATMs" << endl;
    cout << "4. Lipa na M-PESA - Pay for goods and services" << endl;
    cout << "5. My Account - View your account details and transactions" << endl;
    cout << "6. Settings - Manage your account settings" << endl;
    cout << "\nFor assistance, please call customer care: 100" << endl;
    
    cout << "\nPress Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

void addMoney(UserData &userData) {
    cout << "\n=== Add Money to Account ===" << endl;
    cout << "1. From Bank" << endl;
    cout << "2. From M-PESA Agent" << endl;
    cout << "Choose option (1-2): ";
    
    int choice;
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    while (!(cin >> choice) || cin.peek() != '\n' || choice < 1 || choice > 2) {
        cout << "Invalid choice. Please enter 1 or 2: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    cout << "Enter amount to deposit: ";
    double amount;
    while (true) {
        cin.clear();
        if (!(cin >> amount)) {
            cout << "Invalid amount. Please enter a numeric value: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        
        if (amount <= 0) {
            cout << "Amount must be greater than 0. Please try again: ";
            continue;
        }
        
        if (cin.peek() != '\n') {
            cout << "Invalid input. Please enter a valid number: ";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        
        break;
    }

    // Add validation for maximum deposit amount
    const double MAX_DEPOSIT = 10000000.0; // 10 million TZS
    if (amount > MAX_DEPOSIT) {
        showError("Amount exceeds maximum deposit limit of " + to_string(MAX_DEPOSIT) + " TZS");
        return;
    }

    showProcessing("Processing deposit");
    userData.balance += amount;
    
    string source = (choice == 1) ? "Bank" : "M-PESA Agent";
    string transactionID = generateTransactionID();
    
    showSuccess("Deposit successful!");
    cout << "\n" << transactionID << " Confirmed. Tsh" << fixed << setprecision(2) 
         << amount << " deposited from " << source << ". New balance is Tsh"
         << userData.balance << "." << endl;

    addTransaction(userData, transactionID + " - Deposited " + to_string(amount) + " TZS from " + source);
    saveUserData(userData);
}

string getCurrentDateTime() {
    auto now = chrono::system_clock::now();
    time_t currentTime = chrono::system_clock::to_time_t(now);
    tm* ltm = localtime(&currentTime);
    
    stringstream ss;
    ss << (ltm->tm_mday) << "/" << (ltm->tm_mon + 1) << "/" << (ltm->tm_year - 100)
       << " at " << setfill('0') << setw(2) << ltm->tm_hour << ":" 
       << setfill('0') << setw(2) << ltm->tm_min << " " 
       << (ltm->tm_hour >= 12 ? "PM" : "AM");
    
    return ss.str();
}