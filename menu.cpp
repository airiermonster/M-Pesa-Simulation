#include "mpesa.h"
#include <iostream>
#include <iomanip>
#include <chrono>
#include <thread>

using namespace std;

// Remove showHelp function from here as it's already in utils.cpp

void viewSettings(UserData &userData) {
    while (true) {
        cout << "\n=== Settings ===" << endl;
        cout << "1. Change PIN" << endl;
        cout << "2. Update Personal Information" << endl;
        cout << "3. Language Settings" << endl;
        cout << "4. Back to Main Menu" << endl;
        cout << "Enter your choice: ";
        
        int choice;
        while (!(cin >> choice) || cin.peek() != '\n' || choice < 1 || choice > 4) {
            cout << "Invalid choice, please enter a number between 1 and 4: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        switch (choice) {
            case 1: {
                cout << "Enter current PIN: ";
                int currentPin;
                cin >> currentPin;
                if (currentPin == userData.pin) {
                    cout << "Enter new PIN: ";
                    int newPin;
                    while (!(cin >> newPin) || cin.peek() != '\n' || newPin < 1000 || newPin > 9999) {
                        cout << "Invalid PIN. Please enter a 4-digit number: ";
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    }
                    userData.pin = newPin;
                    cout << "PIN changed successfully!" << endl;
                } else {
                    cout << "Incorrect PIN!" << endl;
                }
                break;
            }
            case 2: {
                cout << "Enter your full name: ";
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                getline(cin, userData.name);
                cout << "Enter your region: ";
                getline(cin, userData.region);
                cout << "Information updated successfully!" << endl;
                break;
            }
            case 3:
                cout << "Language settings are currently not available." << endl;
                break;
            case 4:
                return;
        }
        
        cout << "\nPress Enter to continue...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
    }
}

// Remove duplicate viewAccount function here

int displayMenu(UserData &userData) {
    cout << "\n=== M-PESA Menu ===" << endl;
    cout << "1. Send Money" << endl;
    cout << "2. Buy Airtime" << endl;
    cout << "3. Withdraw Cash" << endl;
    cout << "4. Lipa na M-PESA" << endl;
    cout << "5. My Account" << endl;
    cout << "6. Settings" << endl;
    cout << "7. Help" << endl;
    cout << "8. Top Up Account" << endl;  // New option
    cout << "9. Exit" << endl;           // Changed from 8 to 9
    cout << "Enter your choice: ";

    int choice;
    while (!(cin >> choice) || cin.peek() != '\n' || choice < 1 || choice > 8) {
        cout << "Invalid choice, please enter a number between 1 and 8: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    switch (choice) {
        case 1:
            sendMoney(userData);
            break;
        case 2:
            buyAirtime(userData);
            break;
        case 3:
            withdrawCash(userData);
            break;
        case 4:
            lipaNaMpesa(userData);
            break;
        case 5:
            viewAccount(userData);
            break;
        case 6:
            viewSettings(userData);
            break;
        case 7:
            showHelp();
            break;
        case 8:
            addMoney(userData);
            break;
        case 9:
            cout << "Thank you for using M-PESA!" << endl;
            break;
    }
    return choice;
}