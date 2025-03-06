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
        cout << "4. Setup PIN Recovery" << endl;
        cout << "5. Back to Main Menu" << endl;
        cout << "Enter your choice: ";
        
        int choice;
        while (!(cin >> choice) || cin.peek() != '\n' || choice < 1 || choice > 5) {
            cout << "Invalid choice, please enter a number between 1 and 5: ";
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
            case 4: {
                // Setup PIN recovery with security questions
                cout << "\n=== Setup PIN Recovery ===" << endl;
                cout << "This will help you recover your PIN if you forget it." << endl;
                
                // Check if security questions are already set
                if (!userData.securityQuestion.empty() && !userData.securityAnswer.empty()) {
                    cout << "You already have security questions set up." << endl;
                    cout << "Would you like to update them? (y/n): ";
                    char choice;
                    cin >> choice;
                    if (tolower(choice) != 'y') {
                        break;
                    }
                }
                
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                
                cout << "Choose a security question:" << endl;
                cout << "1. What is your mother's maiden name?" << endl;
                cout << "2. What was the name of your first pet?" << endl;
                cout << "3. In what city were you born?" << endl;
                cout << "4. What was the make of your first car?" << endl;
                cout << "5. What is your favorite childhood teacher's name?" << endl;
                cout << "Enter your choice (1-5): ";
                
                int questionChoice;
                while (!(cin >> questionChoice) || cin.peek() != '\n' || questionChoice < 1 || questionChoice > 5) {
                    cout << "Invalid choice. Please enter a number between 1 and 5: ";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
                
                string questions[] = {
                    "What is your mother's maiden name?",
                    "What was the name of your first pet?",
                    "In what city were you born?",
                    "What was the make of your first car?",
                    "What is your favorite childhood teacher's name?"
                };
                
                userData.securityQuestion = questions[questionChoice - 1];
                
                cout << "Enter your answer: ";
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                getline(cin, userData.securityAnswer);
                
                cout << "Security question set up successfully!" << endl;
                break;
            }
            case 5:
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
    cout << "7. Bill Payment" << endl;
    cout << "8. Mobile Loans" << endl;
    cout << "9. QR Payment" << endl;
    cout << "10. Mini Statement" << endl;
    cout << "11. Top Up Account" << endl;
    cout << "12. Help" << endl;
    cout << "13. Exit" << endl;
    cout << "Enter your choice: ";

    int choice;
    while (!(cin >> choice) || cin.peek() != '\n' || choice < 1 || choice > 13) {
        cout << "Invalid choice, please enter a number between 1 and 13: ";
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
            billPayment(userData);
            break;
        case 8:
            mobileLoans(userData);
            break;
        case 9:
            qrPayment(userData);
            break;
        case 10:
            generateMiniStatement(userData);
            break;
        case 11:
            addMoney(userData);
            break;
        case 12:
            showHelp();
            break;
        case 13:
            cout << "Thank you for using M-PESA!" << endl;
            break;
    }
    return choice;
}