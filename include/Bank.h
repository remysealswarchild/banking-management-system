#ifndef BANK_H
#define BANK_H

#include "Account.h"
#include "DashboardSummary.h"
#include "Transaction.h"

#include <map>
#include <string>
#include <vector>

// Backend service layer for account and transaction operations.
class Bank {
private:
    std::map<long, Account> accounts;
    std::vector<Transaction> transactions;
    long nextAccountNumber;
    long nextTransactionId;

public:
    Bank();

    long generateAccountNumber();
    long generateTransactionId();

    long createAccount(const std::string& firstName,
                       const std::string& lastName,
                       AccountType accountType,
                       double openingBalance);

    void updateCustomerDetails(long accountNumber,
                               const std::string& firstName,
                               const std::string& lastName);

    Account& getAccount(long accountNumber);
    const Account& getAccount(long accountNumber) const;
    std::vector<Account> getAllAccounts() const;
    std::vector<Account> searchAccountsByName(const std::string& name) const;
    std::vector<Account> getAccountsSortedByName() const;
    std::vector<Account> getAccountsSortedByBalance() const;
    std::vector<Account> filterAccountsByType(AccountType type) const;
    std::vector<Account> filterAccountsByBalanceRange(double minBalance, double maxBalance) const;

    void deposit(long accountNumber, double amount, const std::string& note = "");
    void withdraw(long accountNumber, double amount, const std::string& note = "");
    void transfer(long fromAccountNumber,
                  long toAccountNumber,
                  double amount,
                  const std::string& note = "");
    void closeAccount(long accountNumber, const std::string& note = "");

    std::vector<Transaction> getAllTransactions() const;
    std::vector<Transaction> getTransactionsForAccount(long accountNumber) const;
    std::vector<Transaction> getTransactionsByType(TransactionType type) const;
    std::vector<Transaction> getMiniStatement(long accountNumber, int numberOfTransactions) const;

    DashboardSummary getDashboardSummary() const;

    void saveAccounts() const;
    void loadAccounts();
    void saveTransactions() const;
    void loadTransactions();
    void saveAll() const;
    void loadAll();

    void backupDataFiles() const;
    void exportAccountsReport(const std::string& filePath) const;
    void exportTransactionsReport(const std::string& filePath) const;
    void exportAccountsCsv(const std::string& filePath) const;
    void exportTransactionsCsv(const std::string& filePath) const;
};

#endif // BANK_H
