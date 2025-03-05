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
    cout << "Too many incorrect attempts. Please try again later." << endl;
    return false;
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
            return 1;
        }
    }

    int choice;
    do {
        choice = displayMenu(userData);
        if (choice != 8) {
            saveUserData(userData);
            cout << "\nPress Enter to continue...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.get();
            system("cls");
        }
    } while (choice != 8);

    saveUserData(userData);
    cout << "\nThank you for using M-PESA!" << endl;
    cout << "Developed by Maximillian Urio" << endl;
    cout << "For support: 0762814092 - Vodacom /0683346262 - Airtel" << endl;
    cout << "===============================================================" << endl;
    
    return 0;
}