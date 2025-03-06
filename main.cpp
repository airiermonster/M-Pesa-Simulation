#include "mpesa.h"
#include <iostream>
#include <limits>
#include <string>
#include <chrono>
#include <thread>
#include <iomanip>

using namespace std;

void displayWelcome() {
    cout << "=====================================================" << endl;
    cout << "Welcome to M-PESA Simulation Program" << endl;
    cout << "=====================================================" << endl;
    cout << "Developed by: Maximillian Urio" << endl;
    cout << "Contact: 0762814092 - Vodacom /0683346262 - Airtel" << endl;
    cout << "GitHub: github.com/airiermonster" << endl;
    cout << "=====================================================" << endl;
}

bool verifyPIN(const UserData &userData) {
    int attempts = 3;
    while (attempts > 0) {
        cout << "Enter your PIN (" << attempts << " attempts remaining): ";
        int enteredPin;
        while (!(cin >> enteredPin) || cin.peek() != '\n') {
            cout << "Invalid input. Please enter a numeric PIN: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        if (enteredPin == userData.pin) {
            return true;
        }

        attempts--;
        cout << "Incorrect PIN!" << endl;
    }
    cout << "Too many incorrect attempts." << endl;
    return false;
}

bool recoverPIN(UserData &userData) {
    if (userData.securityQuestion.empty() || userData.securityAnswer.empty()) {
        cout << "PIN recovery has not been set up for this account." << endl;
        return false;
    }
    
    cout << "\n=== PIN Recovery ===" << endl;
    cout << "Security Question: " << userData.securityQuestion << endl;
    cout << "Enter your answer: ";
    
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    string answer;
    getline(cin, answer);
    
    if (answer == userData.securityAnswer) {
        cout << "Answer correct! Your PIN is: " << userData.pin << endl;
        
        // Ask if they want to change their PIN
        cout << "Would you like to change your PIN? (y/n): ";
        char choice;
        cin >> choice;
        
        if (tolower(choice) == 'y') {
            cout << "Enter new PIN: ";
            int newPin;
            while (!(cin >> newPin) || cin.peek() != '\n' || newPin < 1000 || newPin > 9999) {
                cout << "Invalid PIN. Please enter a 4-digit number: ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            userData.pin = newPin;
            cout << "PIN changed successfully!" << endl;
        }
        
        return true;
    } else {
        cout << "Incorrect answer. PIN recovery failed." << endl;
        return false;
    }
}

int main() {
    displayWelcome();
    
    UserData userData;
    if (isFirstRun()) {
        cout << "\nHello Welcome, Lets begin by setting up your M-Pesa Account." << endl;
        initializeNewUser(userData);
    } else {
        userData = loadUserData();
        if (!verifyPIN(userData)) {
            cout << "Would you like to recover your PIN? (y/n): ";
            char choice;
            cin >> choice;
            
            if (tolower(choice) == 'y') {
                if (!recoverPIN(userData)) {
                    cout << "PIN recovery failed. Please try again later." << endl;
                    return 1;
                }
                saveUserData(userData); // Save if PIN was changed
            } else {
                cout << "Please try again later." << endl;
                return 1;
            }
        }
    }

    int choice;
    do {
        choice = displayMenu(userData);
        if (choice != 13) {
            saveUserData(userData);
            cout << "\nPress Enter to continue...";
            if (cin.peek() == '\n') {
                cin.ignore();
            } else {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            system("cls");
        }
    } while (choice != 13);

    saveUserData(userData);
    cout << "\nThank you for using M-PESA!" << endl;
    cout << "Developed by Maximillian Urio" << endl;
    cout << "For support: 0762814092 - Vodacom /0683346262 - Airtel" << endl;
    cout << "===============================================================" << endl;
    
    return 0;
}