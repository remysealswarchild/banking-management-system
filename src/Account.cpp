#include "Account.h"
#include "Exceptions.h"

#include <algorithm>
#include <cctype>

namespace {

std::string normalizeText(std::string text)
{
    text.erase(std::remove_if(text.begin(), text.end(), [](unsigned char ch) {
                   return std::isspace(ch);
               }),
               text.end());

    std::transform(text.begin(), text.end(), text.begin(), [](unsigned char ch) {
        return static_cast<char>(std::tolower(ch));
    });

    return text;
}

} // namespace

std::string accountTypeToString(AccountType type)
{
    switch (type) {
    case AccountType::Savings:
        return "Savings";
    case AccountType::Current:
        return "Current";
    case AccountType::Student:
        return "Student";
    }

    throw BankException("Unknown account type.");
}

AccountType stringToAccountType(const std::string& typeText)
{
    const std::string value = normalizeText(typeText);

    if (value == "savings") {
        return AccountType::Savings;
    }

    if (value == "current") {
        return AccountType::Current;
    }

    if (value == "student") {
        return AccountType::Student;
    }

    throw BankException("Invalid account type: " + typeText);
}

std::string accountStatusToString(AccountStatus status)
{
    switch (status) {
    case AccountStatus::Active:
        return "Active";
    case AccountStatus::Closed:
        return "Closed";
    }

    throw BankException("Unknown account status.");
}

AccountStatus stringToAccountStatus(const std::string& statusText)
{
    const std::string value = normalizeText(statusText);

    if (value == "active") {
        return AccountStatus::Active;
    }

    if (value == "closed") {
        return AccountStatus::Closed;
    }

    throw BankException("Invalid account status: " + statusText);
}

Account::Account()
    : accountNumber(0),
      firstName(""),
      lastName(""),
      accountType(AccountType::Savings),
      status(AccountStatus::Active),
      balance(0.0)
{
}

Account::Account(long accountNumber,
                 const std::string& firstName,
                 const std::string& lastName,
                 AccountType accountType,
                 double balance,
                 AccountStatus status)
    : accountNumber(accountNumber),
      firstName(firstName),
      lastName(lastName),
      accountType(accountType),
      status(status),
      balance(balance)
{
    if (balance < 0.0) {
        throw InvalidAmountException("Opening balance cannot be negative.");
    }
}

long Account::getAccountNumber() const
{
    return accountNumber;
}

std::string Account::getFirstName() const
{
    return firstName;
}

std::string Account::getLastName() const
{
    return lastName;
}

std::string Account::getFullName() const
{
    if (firstName.empty()) {
        return lastName;
    }

    if (lastName.empty()) {
        return firstName;
    }

    return firstName + " " + lastName;
}

AccountType Account::getAccountType() const
{
    return accountType;
}

AccountStatus Account::getStatus() const
{
    return status;
}

double Account::getBalance() const
{
    return balance;
}

void Account::setFirstName(const std::string& firstName)
{
    this->firstName = firstName;
}

void Account::setLastName(const std::string& lastName)
{
    this->lastName = lastName;
}

void Account::setAccountType(AccountType accountType)
{
    this->accountType = accountType;
}

void Account::deposit(double amount)
{
    if (!isActive()) {
        throw AccountClosedException("Cannot deposit into a closed account.");
    }

    if (amount <= 0.0) {
        throw InvalidAmountException("Deposit amount must be greater than 0.");
    }

    balance += amount;
}

void Account::withdraw(double amount)
{
    if (!isActive()) {
        throw AccountClosedException("Cannot withdraw from a closed account.");
    }

    if (amount <= 0.0) {
        throw InvalidAmountException("Withdrawal amount must be greater than 0.");
    }

    if (amount > balance) {
        throw InsufficientFundsException("Withdrawal amount exceeds account balance.");
    }

    balance -= amount;
}

void Account::closeAccount()
{
    status = AccountStatus::Closed;
}

bool Account::isActive() const
{
    return status == AccountStatus::Active;
}
