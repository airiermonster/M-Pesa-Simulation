#ifndef MPESA_H
#define MPESA_H

#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

struct UserData {
    int pin;
    int balance;
    string name;
    string phoneNumber;
    string gender;
    string region;
    vector<string> transactionHistory;
};
// Function declarations
string generateTransactionID();
string getNetwork(const string &phoneNumber);
void withdrawCash(UserData &userData);
void buyAirtime(UserData &userData);
void sendMoney(UserData &userData);
void lipaNaMpesa(UserData &userData);
void viewAccount(const UserData &userData);
void viewSettings(UserData &userData);
bool isFirstRun();
void saveUserData(const UserData &data);
UserData loadUserData();
void addTransaction(UserData &userData, const string &transaction);
void initializeNewUser(UserData &userData);
int displayMenu(UserData &userData);
void showProcessing(const string &message);
void showSuccess(const string &message);
void showError(const string &message);
void showHelp();
void addMoney(UserData &userData);
string getCurrentDateTime();

#endif