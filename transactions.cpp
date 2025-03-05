#include "mpesa.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <iomanip>
#include <sstream>

using namespace std;

// Remove showProcessing function from here as it's already in utils.cpp

void sendMoney(UserData &userData) {
    cout << "Your current balance is: " << userData.balance << " TZS" << endl;
    cout << "Enter amount to send: ";
    int amount;
    while (!(cin >> amount) || cin.peek() != '\n' || amount <= 0 || amount > userData.balance) {
        cout << "Invalid amount, enter a positive number not exceeding your balance: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    cout << "Enter recipient's phone number (format 255xxxxxxxxx): ";
    string phoneNumber;
    while (!(cin >> phoneNumber) || cin.peek() != '\n' || phoneNumber.length() != 12 || phoneNumber.substr(0, 3) != "255") {
        cout << "Invalid phone number, enter a twelve-digit number starting with 255: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    string network = getNetwork(phoneNumber);
    cout << "Network Provider: " << network << endl;
    cout << "Enter recipient's name: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    string recipientName;
    getline(cin, recipientName);

    // Calculate fees
    double mpesaFee = 30.00;
    double govLevy = 0.00;
    double totalFee = mpesaFee + govLevy;

    if (amount + totalFee > userData.balance) {
        cout << "\nTransaction Failed!\n";
        cout << "Insufficient funds to cover amount plus transaction fee of " << fixed << setprecision(2) << totalFee << " TZS\n";
        return;
    }

    showProcessing("Sending money");
    
    string transactionID = generateTransactionID();
    auto now = chrono::system_clock::now();
    time_t currentTime = chrono::system_clock::to_time_t(now);
    tm* ltm = localtime(&currentTime);
    
    stringstream dateStr;
    dateStr << (ltm->tm_mon + 1) << "/" << ltm->tm_mday << "/" << (ltm->tm_year - 100)
            << " at " << setfill('0') << setw(2) << ltm->tm_hour << ":" 
            << setfill('0') << setw(2) << ltm->tm_min << " " 
            << (ltm->tm_hour < 12 ? "AM" : "PM");

    userData.balance -= (amount + totalFee);
    
    cout << "\n" << transactionID << " Confirmed. Tsh" << fixed << setprecision(2) 
         << amount << " sent to " << phoneNumber << " - " << recipientName 
         << " on " << dateStr.str() << ". Total fee Tsh" << totalFee 
         << " (M-Pesa fee Tsh" << mpesaFee << " + Government levy Tsh" << govLevy 
         << "). New balance is Tsh" << userData.balance << "." << endl;

    addTransaction(userData, transactionID + " - Sent " + to_string(amount) + " TZS to " + recipientName);
}

void buyAirtime(UserData &userData) {
    cout << "1. Buy for yourself\n2. Buy for another number\nEnter your choice: ";
    int choice;
    while (!(cin >> choice) || cin.peek() != '\n' || choice < 1 || choice > 2) {
        cout << "Invalid choice, please enter 1 or 2: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    cout << "Enter amount to buy: ";
    int amount;
    while (!(cin >> amount) || cin.peek() != '\n' || amount <= 0 || amount > userData.balance) {
        cout << "Invalid amount, enter a positive number not exceeding your balance: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    string phoneNumber = userData.phoneNumber;
    if (choice == 2) {
        cout << "Enter the phone number: ";
        cin >> phoneNumber;
    }

    string network = getNetwork(phoneNumber);
    cout << "Network: " << network << endl;
    
    showProcessing("Buying airtime");
    userData.balance -= amount;
    addTransaction(userData, "Bought " + to_string(amount) + " TZS airtime for " + phoneNumber + " (" + network + ")");
    cout << "Airtime purchased successfully!" << endl;
    cout << "Your new balance is: " << userData.balance << " TZS" << endl;
}

void withdrawCash(UserData &userData) {
    cout << "1. Agent Withdrawal\n2. ATM Withdrawal\nEnter your choice: ";
    int choice;
    while (!(cin >> choice) || cin.peek() != '\n' || choice < 1 || choice > 2) {
        cout << "Invalid choice, please enter 1 or 2: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    cout << "Enter amount to withdraw: ";
    int amount;
    while (!(cin >> amount) || cin.peek() != '\n' || amount <= 0 || amount > userData.balance) {
        cout << "Invalid amount, enter a positive number not exceeding your balance: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    if (amount > userData.balance) {
        showProcessing("Verifying transaction");
        showError("Transaction Failed! Insufficient funds.");
        cout << "Your balance is " << userData.balance << " TZS" << endl;
        return;
    }

    string withdrawType = (choice == 1) ? "Agent" : "ATM";
    showProcessing("Processing withdrawal");
    userData.balance -= amount;
    
    string transactionID = generateTransactionID();
    showSuccess("Cash withdrawn successfully!");
    cout << "Your new balance is: " << userData.balance << " TZS" << endl;
    
    addTransaction(userData, "Withdrew " + to_string(amount) + " TZS from " + withdrawType);
}


void lipaNaMpesa(UserData &userData) {
    cout << "Enter business number: ";
    string businessNumber;
    cin >> businessNumber;
    
    cout << "Enter business name: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    string businessName;
    getline(cin, businessName);

    cout << "Enter amount: ";
    int amount;
    while (!(cin >> amount) || cin.peek() != '\n' || amount <= 0 || amount > userData.balance) {
        cout << "Invalid amount, enter a positive number not exceeding your balance: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    // Calculate fees
    double mpesaFee = 220.00;
    double govLevy = 0.00;
    double totalFee = mpesaFee + govLevy;

    if (amount + totalFee > userData.balance) {
        cout << "\nTransaction Failed!\n";
        cout << "Insufficient funds to cover amount plus transaction fee of " << fixed << setprecision(2) << totalFee << " TZS\n";
        return;
    }

    showProcessing("Processing payment");
    
    string transactionID = generateTransactionID();
    auto now = chrono::system_clock::now();
    time_t currentTime = chrono::system_clock::to_time_t(now);
    tm* ltm = localtime(&currentTime);
    
    stringstream dateStr;
    dateStr << (ltm->tm_mon + 1) << "/" << ltm->tm_mday << "/" << (ltm->tm_year - 100)
            << " at " << setfill('0') << setw(2) << ltm->tm_hour << ":" 
            << setfill('0') << setw(2) << ltm->tm_min << " " 
            << (ltm->tm_hour < 12 ? "AM" : "PM");

    userData.balance -= (amount + totalFee);
    
    cout << "\n" << transactionID << " Confirmed. Tsh" << fixed << setprecision(2) 
         << amount << " sent to " << businessName << " for account " << businessNumber 
         << " on " << dateStr.str() << ". Total fee Tsh" << totalFee 
         << " (M-Pesa fee Tsh" << mpesaFee << " + Government Levy Tsh" << govLevy 
         << "). Balance is Tsh" << userData.balance << "." << endl;

    addTransaction(userData, transactionID + " - Paid " + to_string(amount) + " TZS to " + businessName);
}