#include "Bank.h"
#include "Exceptions.h"

#include <iomanip>
#include <iostream>
#include <vector>

void printAccounts(const std::vector<Account>& accounts)
{
    std::cout << "\nAccounts\n";
    std::cout << "--------\n";

    for (const Account& account : accounts) {
        std::cout << account.getAccountNumber() << " | "
                  << account.getFullName() << " | "
                  << accountTypeToString(account.getAccountType()) << " | "
                  << accountStatusToString(account.getStatus()) << " | "
                  << std::fixed << std::setprecision(2)
                  << account.getBalance() << '\n';
    }
}

void printTransactions(const std::vector<Transaction>& transactions)
{
    std::cout << "\nTransactions\n";
    std::cout << "------------\n";

    for (const Transaction& transaction : transactions) {
        std::cout << transaction.getTransactionId() << " | "
                  << transaction.getAccountNumber() << " | "
                  << transactionTypeToString(transaction.getType()) << " | "
                  << std::fixed << std::setprecision(2)
                  << transaction.getAmount() << " | "
                  << transaction.getNote() << " | "
                  << transaction.getDateTime() << '\n';
    }
}

void printDashboard(const DashboardSummary& summary)
{
    std::cout << "\nDashboard Summary\n";
    std::cout << "-----------------\n";
    std::cout << "Total accounts: " << summary.totalAccounts << '\n';
    std::cout << "Active accounts: " << summary.activeAccounts << '\n';
    std::cout << "Closed accounts: " << summary.closedAccounts << '\n';
    std::cout << "Total transactions: " << summary.totalTransactions << '\n';
    std::cout << "Total bank balance: " << std::fixed << std::setprecision(2)
              << summary.totalBankBalance << '\n';
    std::cout << "Highest balance account: "
              << summary.highestBalanceAccountNumber << " - "
              << summary.highestBalanceAccountName << " ("
              << summary.highestBalance << ")\n";
}

int main()
{
    try {
        Bank bank;

        std::cout << "Banking Management System Console Demo\n";
        std::cout << "======================================\n";

        const long aliceAccount = bank.createAccount("Alice", "Brown", AccountType::Savings, 1000.0);
        const long bobAccount = bank.createAccount("Bob", "Smith", AccountType::Current, 500.0);
        const long charlieAccount = bank.createAccount("Charlie", "Adams", AccountType::Student, 100.0);

        bank.deposit(aliceAccount, 250.0, "Salary deposit");
        bank.withdraw(bobAccount, 100.0, "Cash withdrawal");
        bank.transfer(aliceAccount, charlieAccount, 150.0, "Student allowance");
        bank.closeAccount(bobAccount, "Customer requested closure");

        printAccounts(bank.getAllAccounts());
        printTransactions(bank.getAllTransactions());
        printDashboard(bank.getDashboardSummary());

        std::cout << "\nSearch for 'alice':\n";
        printAccounts(bank.searchAccountsByName("alice"));

        std::cout << "\nMini statement for account " << aliceAccount << ":\n";
        printTransactions(bank.getMiniStatement(aliceAccount, 3));

        std::cout << "\nConsole demo completed successfully.\n";
    } catch (const BankException& exception) {
        std::cerr << "Bank error: " << exception.what() << '\n';
        return 1;
    } catch (const std::exception& exception) {
        std::cerr << "Unexpected error: " << exception.what() << '\n';
        return 1;
    }

    return 0;
}

