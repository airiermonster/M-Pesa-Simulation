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
        userFile << data.securityQuestion << endl;
        userFile << data.securityAnswer << endl;
        
        // Ensure credit score is within valid range
        int creditScore = data.creditScore;
        if (creditScore < 0) creditScore = 0;
        if (creditScore > 100) creditScore = 100;
        userFile << creditScore << endl;
        
        // Validate loan data before saving
        int loanAmount = data.currentLoan.amount;
        int remainingAmount = data.currentLoan.remainingAmount;
        bool isActive = data.currentLoan.isActive;
        
        // Ensure consistency in loan data
        if (loanAmount <= 0 || remainingAmount <= 0) {
            isActive = false;
            if (loanAmount < 0) loanAmount = 0;
            if (remainingAmount < 0) remainingAmount = 0;
        }
        
        userFile << loanAmount << endl;
        userFile << remainingAmount << endl;
        userFile << data.currentLoan.dateTaken << endl;
        userFile << data.currentLoan.dueDate << endl;
        userFile << (isActive ? "1" : "0") << endl;
        
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
    
    // Initialize with default values
    data.balance = 0;
    data.creditScore = 70;
    
    // Initialize loan data with safe defaults
    data.currentLoan.amount = 0;
    data.currentLoan.remainingAmount = 0;
    data.currentLoan.dateTaken = "";
    data.currentLoan.dueDate = "";
    data.currentLoan.isActive = false;
    
    ifstream userFile("mpesa_data.txt");
    if (userFile.is_open()) {
        userFile >> data.pin;
        userFile >> data.balance;
        userFile.ignore(numeric_limits<streamsize>::max(), '\n');
        
        getline(userFile, data.name);
        getline(userFile, data.phoneNumber);
        getline(userFile, data.gender);
        getline(userFile, data.region);
        getline(userFile, data.securityQuestion);
        getline(userFile, data.securityAnswer);
        
        // Read credit score with error checking
        if (!(userFile >> data.creditScore)) {
            data.creditScore = 70; // Default if reading fails
            userFile.clear(); // Clear error state
        }
        
        // Read loan data with error checking
        if (!(userFile >> data.currentLoan.amount)) {
            data.currentLoan.amount = 0;
            userFile.clear();
        }
        
        if (!(userFile >> data.currentLoan.remainingAmount)) {
            data.currentLoan.remainingAmount = 0;
            userFile.clear();
        }
        
        userFile.ignore(numeric_limits<streamsize>::max(), '\n');
        
        getline(userFile, data.currentLoan.dateTaken);
        getline(userFile, data.currentLoan.dueDate);
        
        // Read isActive with error checking
        string isActiveStr;
        getline(userFile, isActiveStr);
        if (isActiveStr == "1") {
            data.currentLoan.isActive = true;
        } else {
            data.currentLoan.isActive = false;
        }
        
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
    
    // Final validation to ensure loan data is consistent
    if (data.currentLoan.amount <= 0 || data.currentLoan.remainingAmount <= 0) {
        data.currentLoan.isActive = false;
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
        cout << "+-------------------------+--------------------+---------------+--------------------------------+" << endl;
        cout << "| " << setw(22) << left << "Date/Time" << "| " << setw(18) << left << "Type" 
             << "| " << setw(13) << right << "Amount" << "| " << setw(28) << left << "Details" << "|" << endl;
        cout << "+-------------------------+--------------------+---------------+--------------------------------+" << endl;
        
        // Display most recent transactions first (reverse order)
        int count = 0;
        int maxTransactions = 10; // Show only last 10 transactions
        
        for (auto it = userData.transactionHistory.rbegin(); 
             it != userData.transactionHistory.rend() && count < maxTransactions; 
             ++it, ++count) {
            
            string transaction = *it;
            size_t dashPos = transaction.find(" - ");
            
            if (dashPos != string::npos) {
                string dateTime = transaction.substr(0, dashPos);
                string details = transaction.substr(dashPos + 3);
                
                // Try to extract transaction type and amount
                string type = "Unknown";
                string amount = "N/A";
                string extraDetails = "";
                
                if (details.find("Sent") != string::npos) {
                    type = "Send Money";
                    size_t amountPos = details.find("TZS");
                    if (amountPos != string::npos) {
                        size_t startPos = details.rfind(" ", amountPos - 2);
                        if (startPos != string::npos) {
                            amount = details.substr(startPos + 1, amountPos - startPos - 1);
                            extraDetails = details.substr(details.find("to") + 3);
                        }
                    }
                } else if (details.find("Bought") != string::npos) {
                    type = "Buy Airtime";
                    size_t amountPos = details.find("TZS");
                    if (amountPos != string::npos) {
                        size_t startPos = details.rfind(" ", amountPos - 2);
                        if (startPos != string::npos) {
                            amount = details.substr(startPos + 1, amountPos - startPos - 1);
                            extraDetails = details.substr(details.find("for") + 4);
                        }
                    }
                } else if (details.find("Withdrew") != string::npos) {
                    type = "Withdraw Cash";
                    size_t amountPos = details.find("TZS");
                    if (amountPos != string::npos) {
                        size_t startPos = details.rfind(" ", amountPos - 2);
                        if (startPos != string::npos) {
                            amount = details.substr(startPos + 1, amountPos - startPos - 1);
                            extraDetails = details.substr(details.find("from") + 5);
                        }
                    }
                } else if (details.find("Paid") != string::npos) {
                    type = "Lipa na M-PESA";
                    size_t amountPos = details.find("TZS");
                    if (amountPos != string::npos) {
                        size_t startPos = details.rfind(" ", amountPos - 2);
                        if (startPos != string::npos) {
                            amount = details.substr(startPos + 1, amountPos - startPos - 1);
                            extraDetails = details.substr(details.find("to") + 3);
                        }
                    }
                } else if (details.find("Added") != string::npos) {
                    type = "Top Up";
                    size_t amountPos = details.find("TZS");
                    if (amountPos != string::npos) {
                        size_t startPos = details.rfind(" ", amountPos - 2);
                        if (startPos != string::npos) {
                            amount = details.substr(startPos + 1, amountPos - startPos - 1);
                        }
                    }
                }
                
                cout << "| " << setw(22) << left << dateTime.substr(0, 22) 
                     << "| " << setw(18) << left << type 
                     << "| " << setw(13) << right << amount + " TZS" 
                     << "| " << setw(28) << left << extraDetails.substr(0, 28) << "|" << endl;
            } else {
                cout << "| " << setw(22) << left << "Unknown" 
                     << "| " << setw(18) << left << "Unknown" 
                     << "| " << setw(13) << right << "N/A" 
                     << "| " << setw(28) << left << transaction.substr(0, 28) << "|" << endl;
            }
        }
        
        cout << "+-------------------------+--------------------+---------------+--------------------------------+" << endl;
    }
    
    cout << "\nPress Enter to return to main menu...";
    // Wait for user to press Enter
    if (cin.peek() == '\n') {
        cin.ignore();
    } else {
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    // Don't clear the screen here - let the main function handle it
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
    // Clear any previous input in the buffer
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, userData.name);
    while (userData.name.empty() || userData.name.find_first_not_of(" ") == string::npos) {
        cout << "Name cannot be empty. Please enter your full name: ";
        getline(cin, userData.name);
    }
    
    cout << "Enter your phone number (format: 255xxxxxxxxx): ";
    while (true) {
        cin >> userData.phoneNumber;
        if (userData.phoneNumber.length() == 12 && userData.phoneNumber.substr(0, 3) == "255") {
            break;
        }
        cout << "Invalid phone number. Please enter in format 255xxxxxxxxx: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    
    cout << "Enter your gender (M/F/O): ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
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
        while (!(cin >> pin2) || cin.peek() != '\n') {
            cout << "Invalid input. Please enter a numeric PIN: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        
        if (pin1 == pin2) {
            userData.pin = pin1;
            break;
        } else {
            cout << "PINs do not match. Please try again." << endl;
            cout << "Create your M-PESA PIN (4 digits): ";
        }
    }
    
    userData.balance = 0;
    userData.securityQuestion = "";
    userData.securityAnswer = "";
    
    // Initialize loan data
    userData.currentLoan.amount = 0;
    userData.currentLoan.remainingAmount = 0;
    userData.currentLoan.dateTaken = "";
    userData.currentLoan.dueDate = "";
    userData.currentLoan.isActive = false;
    
    // Initialize credit score (default 70)
    userData.creditScore = 70;
    
    cout << "\nYour account has been created successfully!" << endl;
    cout << "Would you like to add money to your account? (1-Yes, 2-No): ";
    int choice;
    while (!(cin >> choice) || cin.peek() != '\n' || choice < 1 || choice > 2) {
        cout << "Invalid choice. Please enter 1 or 2: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    
    if (choice == 1) {
        addMoney(userData);
    }
}

void generateMiniStatement(const UserData &userData) {
    system("cls");  // Clear screen
    cout << "\n=== M-PESA Mini Statement ===" << endl;
    cout << "Phone Number: " << userData.phoneNumber << endl;
    cout << "Current Balance: " << userData.balance << " TZS" << endl;
    
    if (userData.transactionHistory.empty()) {
        cout << "\nNo transactions found" << endl;
    } else {
        cout << "\nLast 5 Transactions:" << endl;
        cout << "+---------------+--------------------+---------------+" << endl;
        cout << "| " << setw(13) << left << "Date" << "| " << setw(18) << left << "Type" 
             << "| " << setw(13) << right << "Amount" << "|" << endl;
        cout << "+---------------+--------------------+---------------+" << endl;
        
        // Display most recent transactions first (reverse order)
        int count = 0;
        int maxTransactions = 5; // Show only last 5 transactions
        
        for (auto it = userData.transactionHistory.rbegin(); 
             it != userData.transactionHistory.rend() && count < maxTransactions; 
             ++it, ++count) {
            
            string transaction = *it;
            size_t dashPos = transaction.find(" - ");
            
            if (dashPos != string::npos) {
                // Extract date (first 10 characters of the timestamp)
                string dateTime = transaction.substr(0, dashPos);
                string date = dateTime.substr(0, 10);
                string details = transaction.substr(dashPos + 3);
                
                // Try to extract transaction type and amount
                string type = "Unknown";
                string amount = "N/A";
                
                if (details.find("Sent") != string::npos) {
                    type = "Send Money";
                    size_t amountPos = details.find("TZS");
                    if (amountPos != string::npos) {
                        size_t startPos = details.rfind(" ", amountPos - 2);
                        if (startPos != string::npos) {
                            amount = details.substr(startPos + 1, amountPos - startPos - 1);
                        }
                    }
                } else if (details.find("Bought") != string::npos) {
                    type = "Buy Airtime";
                    size_t amountPos = details.find("TZS");
                    if (amountPos != string::npos) {
                        size_t startPos = details.rfind(" ", amountPos - 2);
                        if (startPos != string::npos) {
                            amount = details.substr(startPos + 1, amountPos - startPos - 1);
                        }
                    }
                } else if (details.find("Withdrew") != string::npos) {
                    type = "Withdraw Cash";
                    size_t amountPos = details.find("TZS");
                    if (amountPos != string::npos) {
                        size_t startPos = details.rfind(" ", amountPos - 2);
                        if (startPos != string::npos) {
                            amount = details.substr(startPos + 1, amountPos - startPos - 1);
                        }
                    }
                } else if (details.find("Paid") != string::npos) {
                    type = "Lipa na M-PESA";
                    size_t amountPos = details.find("TZS");
                    if (amountPos != string::npos) {
                        size_t startPos = details.rfind(" ", amountPos - 2);
                        if (startPos != string::npos) {
                            amount = details.substr(startPos + 1, amountPos - startPos - 1);
                        }
                    }
                } else if (details.find("Added") != string::npos) {
                    type = "Top Up";
                    size_t amountPos = details.find("TZS");
                    if (amountPos != string::npos) {
                        size_t startPos = details.rfind(" ", amountPos - 2);
                        if (startPos != string::npos) {
                            amount = details.substr(startPos + 1, amountPos - startPos - 1);
                        }
                    }
                }
                
                cout << "| " << setw(13) << left << date
                     << "| " << setw(18) << left << type 
                     << "| " << setw(13) << right << amount + " TZS" << "|" << endl;
            }
        }
        
        cout << "+---------------+--------------------+---------------+" << endl;
    }
    
    cout << "\nPress Enter to return to main menu...";
    // Wait for user to press Enter
    if (cin.peek() == '\n') {
        cin.ignore();
    } else {
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}