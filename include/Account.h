#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <string>

// Supported account categories in the banking system.
enum class AccountType {
    Savings,
    Current,
    Student
};

// Current lifecycle state of an account.
enum class AccountStatus {
    Active,
    Closed
};

// Convert account enum values to readable text for files, reports, and the GUI.
std::string accountTypeToString(AccountType type);
std::string accountStatusToString(AccountStatus status);

// Convert text from files or GUI controls back into account enum values.
AccountType stringToAccountType(const std::string& typeText);
AccountStatus stringToAccountStatus(const std::string& statusText);

// Represents one customer bank account.
class Account {
private:
    long accountNumber;
    std::string firstName;
    std::string lastName;
    AccountType accountType;
    AccountStatus status;
    double balance;

public:
    Account();
    Account(long accountNumber,
            const std::string& firstName,
            const std::string& lastName,
            AccountType accountType,
            double balance,
            AccountStatus status = AccountStatus::Active);

    long getAccountNumber() const;
    std::string getFirstName() const;
    std::string getLastName() const;
    std::string getFullName() const;
    AccountType getAccountType() const;
    AccountStatus getStatus() const;
    double getBalance() const;

    void setFirstName(const std::string& firstName);
    void setLastName(const std::string& lastName);
    void setAccountType(AccountType accountType);

    void deposit(double amount);
    void withdraw(double amount);
    void closeAccount();
    bool isActive() const;
};

#endif // ACCOUNT_H
