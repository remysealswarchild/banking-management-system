#include "Bank.h"
#include "Exceptions.h"

#include <cassert>
#include <cmath>
#include <iostream>
#include <vector>

bool almostEqual(double left, double right)
{
    return std::fabs(left - right) < 0.001;
}

int main()
{
    Bank bank;

    // 1. Create account successfully.
    const long aliceAccount = bank.createAccount("Alice", "Brown", AccountType::Savings, 1000.0);
    const long bobAccount = bank.createAccount("Bob", "Smith", AccountType::Current, 500.0);
    const long charlieAccount = bank.createAccount("Charlie", "Adams", AccountType::Student, 50.0);

    assert(bank.getAllAccounts().size() == 3);
    assert(bank.getAccount(aliceAccount).getFullName() == "Alice Brown");

    // 2. Account number generation works.
    assert(aliceAccount == 1001);
    assert(bobAccount == 1002);
    assert(charlieAccount == 1003);

    // 3. Deposit increases balance.
    bank.deposit(aliceAccount, 200.0, "Test deposit");
    assert(almostEqual(bank.getAccount(aliceAccount).getBalance(), 1200.0));

    // 4. Withdrawal decreases balance.
    bank.withdraw(aliceAccount, 100.0, "Test withdrawal");
    assert(almostEqual(bank.getAccount(aliceAccount).getBalance(), 1100.0));

    // 5. Transfer moves money between accounts.
    bank.transfer(aliceAccount, bobAccount, 300.0, "Test transfer");
    assert(almostEqual(bank.getAccount(aliceAccount).getBalance(), 800.0));
    assert(almostEqual(bank.getAccount(bobAccount).getBalance(), 800.0));

    // 6. Insufficient funds throws exception.
    bool insufficientFundsThrown = false;
    try {
        bank.withdraw(charlieAccount, 1000.0, "Too large");
    } catch (const InsufficientFundsException&) {
        insufficientFundsThrown = true;
    }
    assert(insufficientFundsThrown);

    // 7. Invalid amount throws exception.
    bool invalidAmountThrown = false;
    try {
        bank.deposit(aliceAccount, -10.0, "Invalid deposit");
    } catch (const InvalidAmountException&) {
        invalidAmountThrown = true;
    }
    assert(invalidAmountThrown);

    // 8. Closed account cannot deposit.
    bank.closeAccount(charlieAccount, "Close test account");

    bool closedAccountThrown = false;
    try {
        bank.deposit(charlieAccount, 10.0, "Deposit into closed account");
    } catch (const AccountClosedException&) {
        closedAccountThrown = true;
    }
    assert(closedAccountThrown);

    // 9. Search account by name works.
    const std::vector<Account> searchResults = bank.searchAccountsByName("alice");
    assert(searchResults.size() == 1);
    assert(searchResults[0].getAccountNumber() == aliceAccount);

    // 10. Sort accounts by name works.
    const std::vector<Account> accountsByName = bank.getAccountsSortedByName();
    assert(accountsByName.size() == 3);
    assert(accountsByName[0].getFullName() == "Alice Brown");
    assert(accountsByName[1].getFullName() == "Bob Smith");
    assert(accountsByName[2].getFullName() == "Charlie Adams");

    // 11. Sort accounts by balance works.
    const std::vector<Account> accountsByBalance = bank.getAccountsSortedByBalance();
    assert(accountsByBalance.size() == 3);
    assert(almostEqual(accountsByBalance[0].getBalance(), 50.0));
    assert(accountsByBalance[0].getBalance() <= accountsByBalance[1].getBalance());
    assert(accountsByBalance[1].getBalance() <= accountsByBalance[2].getBalance());

    // 12. Filter accounts by type works.
    const std::vector<Account> savingsAccounts = bank.filterAccountsByType(AccountType::Savings);
    assert(savingsAccounts.size() == 1);
    assert(savingsAccounts[0].getAccountNumber() == aliceAccount);

    // 13. Filter accounts by balance range works.
    const std::vector<Account> smallBalanceAccounts = bank.filterAccountsByBalanceRange(40.0, 60.0);
    assert(smallBalanceAccounts.size() == 1);
    assert(smallBalanceAccounts[0].getAccountNumber() == charlieAccount);

    // 14. Filter transactions by account works.
    const std::vector<Transaction> aliceTransactions = bank.getTransactionsForAccount(aliceAccount);
    assert(aliceTransactions.size() == 4);

    // 15. Mini statement returns the correct number of transactions.
    const std::vector<Transaction> miniStatement = bank.getMiniStatement(aliceAccount, 2);
    assert(miniStatement.size() == 2);
    assert(miniStatement[0].getType() == TransactionType::Withdraw);
    assert(miniStatement[1].getType() == TransactionType::TransferOut);

    // 16. Dashboard summary returns correct totals.
    const DashboardSummary summary = bank.getDashboardSummary();
    assert(summary.totalAccounts == 3);
    assert(summary.activeAccounts == 2);
    assert(summary.closedAccounts == 1);
    assert(summary.totalTransactions == 8);
    assert(almostEqual(summary.totalBankBalance, 1650.0));
    assert(summary.highestBalanceAccountNumber == aliceAccount);
    assert(summary.highestBalanceAccountName == "Alice Brown");
    assert(almostEqual(summary.highestBalance, 800.0));

    std::cout << "All Bank backend tests passed.\n";
    return 0;
}

