#include "mpesa.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <iomanip>
#include <sstream>
#include <algorithm>

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
    cout << "7. Bill Payment - Pay utility bills and other services" << endl;
    cout << "8. Mobile Loans - Apply for quick loans or repay existing loans" << endl;
    cout << "9. QR Payment - Make payments using QR codes" << endl;
    cout << "10. Mini Statement - View a quick summary of recent transactions" << endl;
    cout << "11. Top Up Account - Add money to your M-PESA account" << endl;
    cout << "12. Help - Get assistance with M-PESA services" << endl;
    cout << "13. Exit - Close the M-PESA application" << endl;
    
    cout << "\nFor assistance, please call customer care: 100" << endl;
    
    cout << "\nPress Enter to continue...";
    if (cin.peek() == '\n') {
        cin.ignore();
    } else {
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

void addMoney(UserData &userData) {
    cout << "\n=== Add Money to Account ===" << endl;
    cout << "1. From Bank" << endl;
    cout << "2. From M-PESA Agent" << endl;
    cout << "3. From Credit/Debit Card" << endl;
    cout << "4. From Mobile Banking" << endl;
    cout << "Choose option (1-4): ";
    
    int choice;
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    while (!(cin >> choice) || cin.peek() != '\n' || choice < 1 || choice > 4) {
        cout << "Invalid choice. Please enter a number between 1 and 4: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    string source;
    switch (choice) {
        case 1: source = "Bank"; break;
        case 2: source = "M-PESA Agent"; break;
        case 3: source = "Credit/Debit Card"; break;
        case 4: source = "Mobile Banking"; break;
        default: source = "Unknown";
    }
    
    // For card payments, ask for card details
    if (choice == 3) {
        cout << "\n=== Card Details ===" << endl;
        string cardNumber, expiryDate, cvv;
        
        cout << "Enter Card Number (16 digits): ";
        while (true) {
            cin >> cardNumber;
            if (cardNumber.length() == 16 && all_of(cardNumber.begin(), cardNumber.end(), ::isdigit)) {
                break;
            }
            cout << "Invalid card number. Please enter a 16-digit number: ";
        }
        
        cout << "Enter Expiry Date (MM/YY): ";
        while (true) {
            cin >> expiryDate;
            if (expiryDate.length() == 5 && expiryDate[2] == '/' && 
                isdigit(expiryDate[0]) && isdigit(expiryDate[1]) && 
                isdigit(expiryDate[3]) && isdigit(expiryDate[4])) {
                break;
            }
            cout << "Invalid expiry date. Please use format MM/YY: ";
        }
        
        cout << "Enter CVV (3 digits): ";
        while (true) {
            cin >> cvv;
            if (cvv.length() == 3 && all_of(cvv.begin(), cvv.end(), ::isdigit)) {
                break;
            }
            cout << "Invalid CVV. Please enter a 3-digit number: ";
        }
        
        // Mask card number for display
        string maskedCard = "XXXX-XXXX-XXXX-" + cardNumber.substr(12, 4);
        source += " (" + maskedCard + ")";
    }
    
    // For mobile banking, ask for bank selection
    if (choice == 4) {
        cout << "\n=== Select Bank ===" << endl;
        cout << "1. CRDB Bank" << endl;
        cout << "2. NMB Bank" << endl;
        cout << "3. NBC Bank" << endl;
        cout << "4. Equity Bank" << endl;
        cout << "5. Other" << endl;
        cout << "Choose bank (1-5): ";
        
        int bankChoice;
        while (!(cin >> bankChoice) || cin.peek() != '\n' || bankChoice < 1 || bankChoice > 5) {
            cout << "Invalid choice. Please enter a number between 1 and 5: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        
        string banks[] = {"CRDB Bank", "NMB Bank", "NBC Bank", "Equity Bank", "Other Bank"};
        source += " (" + banks[bankChoice - 1] + ")";
    }

    cout << "\nEnter amount to deposit: ";
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
        showError("Amount exceeds maximum deposit limit of " + to_string(static_cast<int>(MAX_DEPOSIT)) + " TZS");
        return;
    }

    // Add validation for minimum deposit amount
    const double MIN_DEPOSIT = 1000.0; // 1000 TZS
    if (amount < MIN_DEPOSIT) {
        showError("Amount is below minimum deposit limit of " + to_string(static_cast<int>(MIN_DEPOSIT)) + " TZS");
        return;
    }

    showProcessing("Processing deposit");
    userData.balance += amount;
    
    string transactionID = generateTransactionID();
    
    showSuccess("Deposit successful!");
    cout << "\n+------------------------------------------------------------+" << endl;
    cout << "| " << setw(58) << left << "Transaction Details" << "|" << endl;
    cout << "+------------------------------------------------------------+" << endl;
    cout << "| " << setw(20) << left << "Transaction ID:" << setw(38) << left << transactionID << "|" << endl;
    cout << "| " << setw(20) << left << "Amount:" << setw(38) << left << fixed << setprecision(2) << amount << " TZS" << "|" << endl;
    cout << "| " << setw(20) << left << "Source:" << setw(38) << left << source << "|" << endl;
    cout << "| " << setw(20) << left << "Date/Time:" << setw(38) << left << getCurrentDateTime() << "|" << endl;
    cout << "| " << setw(20) << left << "New Balance:" << setw(38) << left << fixed << setprecision(2) << userData.balance << " TZS" << "|" << endl;
    cout << "+------------------------------------------------------------+" << endl;

    // Add transaction to history
    stringstream transaction;
    transaction << "Added " << fixed << setprecision(2) << amount << " TZS from " << source << ", ID: " << transactionID;
    addTransaction(userData, transaction.str());
    
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