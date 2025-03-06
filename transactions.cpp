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

    // Calculate fees based on amount
    double mpesaFee = 0.0;
    if (amount <= 1000) {
        mpesaFee = 30.0;
    } else if (amount <= 5000) {
        mpesaFee = 50.0;
    } else if (amount <= 10000) {
        mpesaFee = 100.0;
    } else if (amount <= 20000) {
        mpesaFee = 200.0;
    } else if (amount <= 50000) {
        mpesaFee = 400.0;
    } else if (amount <= 100000) {
        mpesaFee = 600.0;
    } else if (amount <= 300000) {
        mpesaFee = 1000.0;
    } else if (amount <= 500000) {
        mpesaFee = 1500.0;
    } else if (amount <= 1000000) {
        mpesaFee = 2000.0;
    } else {
        mpesaFee = 2500.0;
    }
    
    // Calculate government levy (10% of M-Pesa fee)
    double govLevy = mpesaFee * 0.1;
    double totalFee = mpesaFee + govLevy;

    // Display fee breakdown
    cout << "\nFee Breakdown:" << endl;
    cout << "M-Pesa Fee: " << fixed << setprecision(2) << mpesaFee << " TZS" << endl;
    cout << "Government Levy: " << fixed << setprecision(2) << govLevy << " TZS" << endl;
    cout << "Total Fee: " << fixed << setprecision(2) << totalFee << " TZS" << endl;
    cout << "Total Amount to be deducted: " << fixed << setprecision(2) << (amount + totalFee) << " TZS" << endl;
    
    // Confirm transaction
    cout << "\nDo you want to proceed with this transaction? (y/n): ";
    char confirm;
    cin >> confirm;
    if (tolower(confirm) != 'y') {
        cout << "Transaction cancelled." << endl;
        return;
    }

    if (amount + totalFee > userData.balance) {
        cout << "\nTransaction Failed!\n";
        cout << "Insufficient funds to cover amount plus transaction fee of " << fixed << setprecision(2) << totalFee << " TZS\n";
        return;
    }

    showProcessing("Sending money");
    
    string transactionID = generateTransactionID();
    
    // Deduct amount and fees from balance
    userData.balance -= (amount + totalFee);
    
    // Add transaction to history
    stringstream transaction;
    transaction << "Sent " << amount << " TZS to " << recipientName << " (" << phoneNumber << "), Fee: " << totalFee << " TZS, ID: " << transactionID;
    addTransaction(userData, transaction.str());
    
    showSuccess("Money sent successfully!");
    cout << "Transaction ID: " << transactionID << endl;
    cout << "Amount: " << amount << " TZS" << endl;
    cout << "Recipient: " << recipientName << " (" << phoneNumber << ")" << endl;
    cout << "Fee: " << fixed << setprecision(2) << totalFee << " TZS" << endl;
    cout << "New Balance: " << userData.balance << " TZS" << endl;
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
    cout << "Your current balance is: " << userData.balance << " TZS" << endl;
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

    // Calculate fees based on amount
    double mpesaFee = 0.0;
    if (amount <= 1000) {
        mpesaFee = 30.0;
    } else if (amount <= 5000) {
        mpesaFee = 50.0;
    } else if (amount <= 10000) {
        mpesaFee = 100.0;
    } else if (amount <= 20000) {
        mpesaFee = 200.0;
    } else if (amount <= 50000) {
        mpesaFee = 400.0;
    } else if (amount <= 100000) {
        mpesaFee = 600.0;
    } else if (amount <= 300000) {
        mpesaFee = 1000.0;
    } else if (amount <= 500000) {
        mpesaFee = 1500.0;
    } else if (amount <= 1000000) {
        mpesaFee = 2000.0;
    } else {
        mpesaFee = 2500.0;
    }
    
    // ATM withdrawals have an additional fee
    if (choice == 2) {
        mpesaFee += 200.0; // Additional ATM fee
    }
    
    // Calculate government levy (10% of M-Pesa fee)
    double govLevy = mpesaFee * 0.1;
    double totalFee = mpesaFee + govLevy;

    // Display fee breakdown
    cout << "\nFee Breakdown:" << endl;
    cout << "M-Pesa Fee: " << fixed << setprecision(2) << mpesaFee << " TZS" << endl;
    cout << "Government Levy: " << fixed << setprecision(2) << govLevy << " TZS" << endl;
    cout << "Total Fee: " << fixed << setprecision(2) << totalFee << " TZS" << endl;
    cout << "Total Amount to be deducted: " << fixed << setprecision(2) << (amount + totalFee) << " TZS" << endl;
    
    // Confirm transaction
    cout << "\nDo you want to proceed with this transaction? (y/n): ";
    char confirm;
    cin >> confirm;
    if (tolower(confirm) != 'y') {
        cout << "Transaction cancelled." << endl;
        return;
    }

    if (amount + totalFee > userData.balance) {
        showProcessing("Verifying transaction");
        showError("Transaction Failed! Insufficient funds to cover amount plus transaction fee.");
        cout << "Your balance is " << userData.balance << " TZS" << endl;
        return;
    }

    string withdrawType = (choice == 1) ? "Agent" : "ATM";
    showProcessing("Processing withdrawal");
    
    // Deduct amount and fees from balance
    userData.balance -= (amount + totalFee);
    
    string transactionID = generateTransactionID();
    
    // Add transaction to history
    stringstream transaction;
    transaction << "Withdrew " << amount << " TZS from " << withdrawType << ", Fee: " << totalFee << " TZS, ID: " << transactionID;
    addTransaction(userData, transaction.str());
    
    showSuccess("Cash withdrawn successfully!");
    cout << "Transaction ID: " << transactionID << endl;
    cout << "Amount: " << amount << " TZS" << endl;
    cout << "Withdrawal Type: " << withdrawType << endl;
    cout << "Fee: " << fixed << setprecision(2) << totalFee << " TZS" << endl;
    cout << "New Balance: " << userData.balance << " TZS" << endl;
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

void billPayment(UserData &userData) {
    system("cls");
    cout << "\n=== Bill Payment ===" << endl;
    cout << "Your current balance is: " << userData.balance << " TZS" << endl;
    
    cout << "\nSelect bill type:" << endl;
    cout << "1. Electricity (TANESCO)" << endl;
    cout << "2. Water (DAWASCO)" << endl;
    cout << "3. TV Subscription (DSTV/GOTV)" << endl;
    cout << "4. Internet (TTCL/Vodacom/Airtel)" << endl;
    cout << "5. Government Payments (GePG)" << endl;
    cout << "6. Back to Main Menu" << endl;
    cout << "Enter your choice: ";
    
    int choice;
    while (!(cin >> choice) || cin.peek() != '\n' || choice < 1 || choice > 6) {
        cout << "Invalid choice, please enter a number between 1 and 6: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    
    if (choice == 6) {
        return;
    }
    
    string billType;
    switch (choice) {
        case 1: billType = "Electricity (TANESCO)"; break;
        case 2: billType = "Water (DAWASCO)"; break;
        case 3: billType = "TV Subscription"; break;
        case 4: billType = "Internet"; break;
        case 5: billType = "Government Payment"; break;
        default: billType = "Unknown";
    }
    
    string accountNumber;
    cout << "\nEnter " << billType << " account/reference number: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, accountNumber);
    
    // Validate account number based on bill type
    if (choice == 1 && accountNumber.length() != 11) {
        cout << "Invalid TANESCO account number. It should be 11 digits." << endl;
        return;
    } else if (choice == 2 && accountNumber.length() != 8) {
        cout << "Invalid DAWASCO account number. It should be 8 digits." << endl;
        return;
    }
    
    // Simulate bill verification
    showProcessing("Verifying account");
    
    // Generate random bill amount between 10,000 and 200,000 TZS
    srand(time(nullptr) + accountNumber.length());
    int billAmount = 10000 + (rand() % 190000);
    
    cout << "\nBill Details:" << endl;
    cout << "Bill Type: " << billType << endl;
    cout << "Account Number: " << accountNumber << endl;
    cout << "Amount Due: " << billAmount << " TZS" << endl;
    
    cout << "\nDo you want to pay this bill? (y/n): ";
    char confirm;
    cin >> confirm;
    if (tolower(confirm) != 'y') {
        cout << "Bill payment cancelled." << endl;
        return;
    }
    
    // Check if user has enough balance
    if (billAmount > userData.balance) {
        showError("Insufficient funds to pay this bill.");
        cout << "Your balance is " << userData.balance << " TZS" << endl;
        return;
    }
    
    // Process payment
    showProcessing("Processing payment");
    
    // Deduct amount from balance
    userData.balance -= billAmount;
    
    // Generate transaction ID
    string transactionID = generateTransactionID();
    
    // Add transaction to history
    stringstream transaction;
    transaction << "Paid " << billAmount << " TZS for " << billType << " (Acc: " << accountNumber << "), ID: " << transactionID;
    addTransaction(userData, transaction.str());
    
    // Show success message
    showSuccess("Bill payment successful!");
    cout << "\n+------------------------------------------------------------+" << endl;
    cout << "| " << setw(58) << left << "Payment Details" << "|" << endl;
    cout << "+------------------------------------------------------------+" << endl;
    cout << "| " << setw(20) << left << "Transaction ID:" << setw(38) << left << transactionID << "|" << endl;
    cout << "| " << setw(20) << left << "Bill Type:" << setw(38) << left << billType << "|" << endl;
    cout << "| " << setw(20) << left << "Account Number:" << setw(38) << left << accountNumber << "|" << endl;
    cout << "| " << setw(20) << left << "Amount:" << setw(38) << left << fixed << setprecision(2) << billAmount << " TZS" << "|" << endl;
    cout << "| " << setw(20) << left << "Date/Time:" << setw(38) << left << getCurrentDateTime() << "|" << endl;
    cout << "| " << setw(20) << left << "New Balance:" << setw(38) << left << fixed << setprecision(2) << userData.balance << " TZS" << "|" << endl;
    cout << "+------------------------------------------------------------+" << endl;
}

void qrPayment(UserData &userData) {
    system("cls");
    cout << "\n=== QR Code Payment ===" << endl;
    cout << "Your current balance is: " << userData.balance << " TZS" << endl;
    
    cout << "\nSelect QR code source:" << endl;
    cout << "1. Scan QR code (Simulation)" << endl;
    cout << "2. Enter merchant code manually" << endl;
    cout << "3. Back to Main Menu" << endl;
    cout << "Enter your choice: ";
    
    int choice;
    while (!(cin >> choice) || cin.peek() != '\n' || choice < 1 || choice > 3) {
        cout << "Invalid choice, please enter a number between 1 and 3: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    
    if (choice == 3) {
        return;
    }
    
    string merchantCode;
    string merchantName;
    
    if (choice == 1) {
        // Simulate scanning QR code
        showProcessing("Scanning QR code");
        
        // Generate random merchant code
        srand(time(nullptr));
        merchantCode = "M" + to_string(10000 + (rand() % 90000));
        
        // Predefined merchant names
        vector<string> merchants = {
            "Supermarket Plus", "Fashion World", "Tech Galaxy", 
            "Food Haven Restaurant", "Pharmacy Plus", "Bookstore Central"
        };
        
        merchantName = merchants[rand() % merchants.size()];
        
        cout << "\nQR Code scanned successfully!" << endl;
    } else {
        cout << "\nEnter merchant code: ";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        getline(cin, merchantCode);
        
        cout << "Enter merchant name: ";
        getline(cin, merchantName);
    }
    
    cout << "\nMerchant Details:" << endl;
    cout << "Merchant Code: " << merchantCode << endl;
    cout << "Merchant Name: " << merchantName << endl;
    
    cout << "\nEnter amount to pay: ";
    int amount;
    while (!(cin >> amount) || cin.peek() != '\n' || amount <= 0) {
        cout << "Invalid amount, enter a positive number: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    
    // Check if user has enough balance
    if (amount > userData.balance) {
        showError("Insufficient funds for this payment.");
        cout << "Your balance is " << userData.balance << " TZS" << endl;
        return;
    }
    
    // Confirm payment
    cout << "\nYou are about to pay " << amount << " TZS to " << merchantName << "." << endl;
    cout << "Do you want to proceed? (y/n): ";
    char confirm;
    cin >> confirm;
    if (tolower(confirm) != 'y') {
        cout << "Payment cancelled." << endl;
        return;
    }
    
    // Process payment
    showProcessing("Processing payment");
    
    // Deduct amount from balance
    userData.balance -= amount;
    
    // Generate transaction ID
    string transactionID = generateTransactionID();
    
    // Add transaction to history
    stringstream transaction;
    transaction << "Paid " << amount << " TZS to " << merchantName << " via QR code, ID: " << transactionID;
    addTransaction(userData, transaction.str());
    
    // Show success message with ASCII QR code
    showSuccess("QR Payment successful!");
    
    cout << "\n+------------------------------------------------------------+" << endl;
    cout << "| " << setw(58) << left << "Payment Details" << "|" << endl;
    cout << "+------------------------------------------------------------+" << endl;
    cout << "| " << setw(20) << left << "Transaction ID:" << setw(38) << left << transactionID << "|" << endl;
    cout << "| " << setw(20) << left << "Merchant:" << setw(38) << left << merchantName << "|" << endl;
    cout << "| " << setw(20) << left << "Merchant Code:" << setw(38) << left << merchantCode << "|" << endl;
    cout << "| " << setw(20) << left << "Amount:" << setw(38) << left << fixed << setprecision(2) << amount << " TZS" << "|" << endl;
    cout << "| " << setw(20) << left << "Date/Time:" << setw(38) << left << getCurrentDateTime() << "|" << endl;
    cout << "| " << setw(20) << left << "New Balance:" << setw(38) << left << fixed << setprecision(2) << userData.balance << " TZS" << "|" << endl;
    cout << "+------------------------------------------------------------+" << endl;
    
    // Display ASCII QR code (just for fun)
    cout << "\nPayment Receipt QR Code:" << endl;
    cout << "┌───────────────────┐" << endl;
    cout << "│ █▀▀█ █▀▀█ █▀▀█ █ │" << endl;
    cout << "│ █  █ █  █ █  █ █ │" << endl;
    cout << "│ █▀▀█ █▀▀█ █▀▀█ █ │" << endl;
    cout << "│ █  █ █  █ █  █   │" << endl;
    cout << "│ █▀▀█ █▀▀█ █▀▀█ █ │" << endl;
    cout << "│                   │" << endl;
    cout << "│ █ █ █ █▀▀█ █▀▀█  │" << endl;
    cout << "│ █ █ █ █  █ █  █  │" << endl;
    cout << "│ █▀▀▀█ █▀▀█ █▀▀█  │" << endl;
    cout << "│ █   █ █  █ █  █  │" << endl;
    cout << "│ █   █ █▀▀█ █▀▀█  │" << endl;
    cout << "└───────────────────┘" << endl;
}

void mobileLoans(UserData &userData) {
    system("cls");
    cout << "\n=== M-PESA Mobile Loans ===" << endl;
    cout << "Your current balance: " << userData.balance << " TZS" << endl;
    cout << "Your credit score: " << userData.creditScore << "/100" << endl;
    
    // Check if user already has an active loan with additional validation
    if (userData.currentLoan.isActive && 
        userData.currentLoan.amount > 0 && 
        userData.currentLoan.remainingAmount > 0 &&
        !userData.currentLoan.dateTaken.empty() &&
        !userData.currentLoan.dueDate.empty()) {
        
        cout << "\nYou have an active loan:" << endl;
        cout << "Original amount: " << userData.currentLoan.amount << " TZS" << endl;
        cout << "Remaining amount: " << userData.currentLoan.remainingAmount << " TZS" << endl;
        cout << "Date taken: " << userData.currentLoan.dateTaken << endl;
        cout << "Due date: " << userData.currentLoan.dueDate << endl;
        
        cout << "\nOptions:" << endl;
        cout << "1. Make a payment" << endl;
        cout << "2. Back to Main Menu" << endl;
        cout << "Enter your choice: ";
        
        int choice;
        while (!(cin >> choice) || cin.peek() != '\n' || choice < 1 || choice > 2) {
            cout << "Invalid choice, please enter 1 or 2: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        
        if (choice == 2) {
            return;
        }
        
        // Make a payment
        cout << "\nEnter amount to pay (max " << userData.currentLoan.remainingAmount << " TZS): ";
        int paymentAmount;
        while (!(cin >> paymentAmount) || cin.peek() != '\n' || paymentAmount <= 0 || paymentAmount > userData.balance || paymentAmount > userData.currentLoan.remainingAmount) {
            if (paymentAmount > userData.balance) {
                cout << "Insufficient balance. Your balance is " << userData.balance << " TZS. Enter a smaller amount: ";
            } else if (paymentAmount > userData.currentLoan.remainingAmount) {
                cout << "Amount exceeds remaining loan. Enter a smaller amount (max " << userData.currentLoan.remainingAmount << " TZS): ";
            } else {
                cout << "Invalid amount. Please enter a positive number not exceeding your balance: ";
            }
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        
        // Process payment
        showProcessing("Processing loan payment");
        
        // Deduct payment from balance
        userData.balance -= paymentAmount;
        
        // Update loan data
        userData.currentLoan.remainingAmount -= paymentAmount;
        
        // Generate transaction ID
        string transactionID = generateTransactionID();
        
        // Add transaction to history
        stringstream transaction;
        transaction << "Paid " << paymentAmount << " TZS towards loan, Remaining: " << userData.currentLoan.remainingAmount << " TZS, ID: " << transactionID;
        addTransaction(userData, transaction.str());
        
        // Check if loan is fully paid
        if (userData.currentLoan.remainingAmount <= 0) {
            userData.currentLoan.isActive = false;
            userData.creditScore += 5; // Improve credit score for full repayment
            if (userData.creditScore > 100) userData.creditScore = 100;
            
            showSuccess("Loan fully repaid!");
            cout << "Your credit score has improved to " << userData.creditScore << "/100" << endl;
        } else {
            showSuccess("Loan payment successful!");
        }
        
        cout << "\n+------------------------------------------------------------+" << endl;
        cout << "| " << setw(58) << left << "Payment Details" << "|" << endl;
        cout << "+------------------------------------------------------------+" << endl;
        cout << "| " << setw(20) << left << "Transaction ID:" << setw(38) << left << transactionID << "|" << endl;
        cout << "| " << setw(20) << left << "Amount Paid:" << setw(38) << left << fixed << setprecision(2) << paymentAmount << " TZS" << "|" << endl;
        cout << "| " << setw(20) << left << "Remaining Loan:" << setw(38) << left << fixed << setprecision(2) << userData.currentLoan.remainingAmount << " TZS" << "|" << endl;
        cout << "| " << setw(20) << left << "Date/Time:" << setw(38) << left << getCurrentDateTime() << "|" << endl;
        cout << "| " << setw(20) << left << "New Balance:" << setw(38) << left << fixed << setprecision(2) << userData.balance << " TZS" << "|" << endl;
        cout << "+------------------------------------------------------------+" << endl;
    } else {
        // No active loan, offer new loan
        cout << "\nYou are eligible for a loan based on your credit score." << endl;
        
        // Calculate maximum loan amount based on credit score (500 TZS per credit point)
        int maxLoanAmount = userData.creditScore * 500;
        cout << "Maximum loan amount: " << maxLoanAmount << " TZS" << endl;
        
        cout << "\nOptions:" << endl;
        cout << "1. Apply for a loan" << endl;
        cout << "2. Back to Main Menu" << endl;
        cout << "Enter your choice: ";
        
        int choice;
        while (!(cin >> choice) || cin.peek() != '\n' || choice < 1 || choice > 2) {
            cout << "Invalid choice, please enter 1 or 2: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        
        if (choice == 2) {
            return;
        }
        
        // Apply for a loan
        cout << "\nEnter loan amount (max " << maxLoanAmount << " TZS): ";
        int loanAmount;
        while (!(cin >> loanAmount) || cin.peek() != '\n' || loanAmount <= 0 || loanAmount > maxLoanAmount) {
            cout << "Invalid amount. Please enter a positive number not exceeding " << maxLoanAmount << " TZS: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        
        // Calculate interest (10% of loan amount)
        int interest = loanAmount * 0.1;
        int totalRepayment = loanAmount + interest;
        
        cout << "\nLoan Details:" << endl;
        cout << "Loan Amount: " << loanAmount << " TZS" << endl;
        cout << "Interest (10%): " << interest << " TZS" << endl;
        cout << "Total Repayment: " << totalRepayment << " TZS" << endl;
        cout << "Repayment Period: 30 days" << endl;
        
        cout << "\nDo you accept these terms? (y/n): ";
        char confirm;
        cin >> confirm;
        if (tolower(confirm) != 'y') {
            cout << "Loan application cancelled." << endl;
            return;
        }
        
        // Process loan
        showProcessing("Processing loan application");
        
        // Generate current date and due date (30 days from now)
        string currentDate = getCurrentDateTime().substr(0, 10);
        
        // Simple due date calculation (just add 30 to the day)
        string dueDate = currentDate;
        int day = stoi(currentDate.substr(8, 2));
        int month = stoi(currentDate.substr(5, 2));
        int year = stoi(currentDate.substr(0, 4));
        
        day += 30;
        if (day > 30) { // Simplified, assuming all months have 30 days
            day -= 30;
            month++;
            if (month > 12) {
                month = 1;
                year++;
            }
        }
        
        // Format the due date
        stringstream dueDateStream;
        dueDateStream << year << "-" 
                     << (month < 10 ? "0" : "") << month << "-"
                     << (day < 10 ? "0" : "") << day;
        dueDate = dueDateStream.str();
        
        // Update loan data
        userData.currentLoan.amount = totalRepayment;
        userData.currentLoan.remainingAmount = totalRepayment;
        userData.currentLoan.dateTaken = currentDate;
        userData.currentLoan.dueDate = dueDate;
        userData.currentLoan.isActive = true;
        
        // Add loan amount to balance
        userData.balance += loanAmount;
        
        // Generate transaction ID
        string transactionID = generateTransactionID();
        
        // Add transaction to history
        stringstream transaction;
        transaction << "Received loan of " << loanAmount << " TZS, Total repayment: " << totalRepayment << " TZS, ID: " << transactionID;
        addTransaction(userData, transaction.str());
        
        showSuccess("Loan approved and disbursed!");
        cout << "\n+------------------------------------------------------------+" << endl;
        cout << "| " << setw(58) << left << "Loan Details" << "|" << endl;
        cout << "+------------------------------------------------------------+" << endl;
        cout << "| " << setw(20) << left << "Transaction ID:" << setw(38) << left << transactionID << "|" << endl;
        cout << "| " << setw(20) << left << "Loan Amount:" << setw(38) << left << fixed << setprecision(2) << loanAmount << " TZS" << "|" << endl;
        cout << "| " << setw(20) << left << "Interest:" << setw(38) << left << fixed << setprecision(2) << interest << " TZS" << "|" << endl;
        cout << "| " << setw(20) << left << "Total Repayment:" << setw(38) << left << fixed << setprecision(2) << totalRepayment << " TZS" << "|" << endl;
        cout << "| " << setw(20) << left << "Date Taken:" << setw(38) << left << currentDate << "|" << endl;
        cout << "| " << setw(20) << left << "Due Date:" << setw(38) << left << dueDate << "|" << endl;
        cout << "| " << setw(20) << left << "New Balance:" << setw(38) << left << fixed << setprecision(2) << userData.balance << " TZS" << "|" << endl;
        cout << "+------------------------------------------------------------+" << endl;
    }
}