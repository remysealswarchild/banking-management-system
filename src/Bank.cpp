#include "Bank.h"
#include "Exceptions.h"

#include <algorithm>
#include <cctype>
#include <iterator>
#include <string>

namespace {

std::string normalizeSearchText(std::string text)
{
    // Remove spaces so "John Smith" can match searches like "johnsmith".
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

Bank::Bank()
    : nextAccountNumber(1001),
      nextTransactionId(1)
{
}

long Bank::generateAccountNumber()
{
    return nextAccountNumber++;
}

long Bank::generateTransactionId()
{
    return nextTransactionId++;
}

long Bank::createAccount(const std::string& firstName,
                         const std::string& lastName,
                         AccountType accountType,
                         double openingBalance)
{
    if (openingBalance < 0.0) {
        throw InvalidAmountException("Opening balance cannot be negative.");
    }

    const long accountNumber = generateAccountNumber();
    Account account(accountNumber, firstName, lastName, accountType, openingBalance);

    const auto result = accounts.emplace(accountNumber, account);
    if (!result.second) {
        throw DuplicateAccountException("Account number already exists: " + std::to_string(accountNumber));
    }

    transactions.emplace_back(generateTransactionId(),
                              accountNumber,
                              TransactionType::CreateAccount,
                              openingBalance,
                              "Account created");

    return accountNumber;
}

void Bank::updateCustomerDetails(long accountNumber,
                                 const std::string& firstName,
                                 const std::string& lastName)
{
    Account& account = getAccount(accountNumber);

    if (!account.isActive()) {
        throw AccountClosedException("Cannot update a closed account.");
    }

    account.setFirstName(firstName);
    account.setLastName(lastName);

    transactions.emplace_back(generateTransactionId(),
                              accountNumber,
                              TransactionType::UpdateAccount,
                              0.0,
                              "Customer details updated");
}

Account& Bank::getAccount(long accountNumber)
{
    auto accountIterator = accounts.find(accountNumber);
    if (accountIterator == accounts.end()) {
        throw AccountNotFoundException("Account not found: " + std::to_string(accountNumber));
    }

    return accountIterator->second;
}

const Account& Bank::getAccount(long accountNumber) const
{
    auto accountIterator = accounts.find(accountNumber);
    if (accountIterator == accounts.end()) {
        throw AccountNotFoundException("Account not found: " + std::to_string(accountNumber));
    }

    return accountIterator->second;
}

std::vector<Account> Bank::getAllAccounts() const
{
    std::vector<Account> accountList;
    accountList.reserve(accounts.size());

    for (const auto& accountPair : accounts) {
        accountList.push_back(accountPair.second);
    }

    return accountList;
}

std::vector<Account> Bank::searchAccountsByName(const std::string& name) const
{
    std::vector<Account> matches;
    const std::string searchText = normalizeSearchText(name);

    if (searchText.empty()) {
        return matches;
    }

    auto current = accounts.cbegin();
    while (current != accounts.cend()) {
        // std::find_if is useful here because we are looking for matching names.
        current = std::find_if(current, accounts.cend(), [&searchText](const auto& accountPair) {
            const std::string fullName = normalizeSearchText(accountPair.second.getFullName());
            return fullName.find(searchText) != std::string::npos;
        });

        if (current != accounts.cend()) {
            matches.push_back(current->second);
            ++current;
        }
    }

    return matches;
}

std::vector<Account> Bank::getAccountsSortedByName() const
{
    std::vector<Account> sortedAccounts = getAllAccounts();

    std::sort(sortedAccounts.begin(), sortedAccounts.end(), [](const Account& left, const Account& right) {
        return normalizeSearchText(left.getFullName()) < normalizeSearchText(right.getFullName());
    });

    return sortedAccounts;
}

std::vector<Account> Bank::getAccountsSortedByBalance() const
{
    std::vector<Account> sortedAccounts = getAllAccounts();

    std::sort(sortedAccounts.begin(), sortedAccounts.end(), [](const Account& left, const Account& right) {
        return left.getBalance() < right.getBalance();
    });

    return sortedAccounts;
}

std::vector<Account> Bank::filterAccountsByType(AccountType type) const
{
    const std::vector<Account> allAccounts = getAllAccounts();
    std::vector<Account> filteredAccounts;

    std::copy_if(allAccounts.begin(),
                 allAccounts.end(),
                 std::back_inserter(filteredAccounts),
                 [type](const Account& account) {
                     return account.getAccountType() == type;
                 });

    return filteredAccounts;
}

std::vector<Account> Bank::filterAccountsByBalanceRange(double minBalance, double maxBalance) const
{
    if (minBalance > maxBalance) {
        throw InvalidAmountException("Minimum balance cannot be greater than maximum balance.");
    }

    const std::vector<Account> allAccounts = getAllAccounts();
    std::vector<Account> filteredAccounts;

    std::copy_if(allAccounts.begin(),
                 allAccounts.end(),
                 std::back_inserter(filteredAccounts),
                 [minBalance, maxBalance](const Account& account) {
                     return account.getBalance() >= minBalance && account.getBalance() <= maxBalance;
                 });

    return filteredAccounts;
}

void Bank::deposit(long accountNumber, double amount, const std::string& note)
{
    Account& account = getAccount(accountNumber);
    account.deposit(amount);

    transactions.emplace_back(generateTransactionId(),
                              accountNumber,
                              TransactionType::Deposit,
                              amount,
                              note.empty() ? "Deposit" : note);
}

void Bank::withdraw(long accountNumber, double amount, const std::string& note)
{
    Account& account = getAccount(accountNumber);
    account.withdraw(amount);

    transactions.emplace_back(generateTransactionId(),
                              accountNumber,
                              TransactionType::Withdraw,
                              amount,
                              note.empty() ? "Withdrawal" : note);
}

void Bank::transfer(long fromAccountNumber,
                    long toAccountNumber,
                    double amount,
                    const std::string& note)
{
    if (fromAccountNumber == toAccountNumber) {
        throw InvalidTransferException("Cannot transfer money to the same account.");
    }

    if (amount <= 0.0) {
        throw InvalidAmountException("Transfer amount must be greater than 0.");
    }

    Account& fromAccount = getAccount(fromAccountNumber);
    Account& toAccount = getAccount(toAccountNumber);

    if (!fromAccount.isActive()) {
        throw AccountClosedException("Cannot transfer from a closed account.");
    }

    if (!toAccount.isActive()) {
        throw AccountClosedException("Cannot transfer to a closed account.");
    }

    if (amount > fromAccount.getBalance()) {
        throw InsufficientFundsException("Transfer amount exceeds source account balance.");
    }

    fromAccount.withdraw(amount);
    toAccount.deposit(amount);

    const std::string transferNote = note.empty() ? "Transfer" : note;

    transactions.emplace_back(generateTransactionId(),
                              fromAccountNumber,
                              TransactionType::TransferOut,
                              amount,
                              transferNote + " to account " + std::to_string(toAccountNumber));

    transactions.emplace_back(generateTransactionId(),
                              toAccountNumber,
                              TransactionType::TransferIn,
                              amount,
                              transferNote + " from account " + std::to_string(fromAccountNumber));
}

void Bank::closeAccount(long accountNumber, const std::string& note)
{
    Account& account = getAccount(accountNumber);

    if (!account.isActive()) {
        throw AccountClosedException("Account is already closed.");
    }

    account.closeAccount();

    transactions.emplace_back(generateTransactionId(),
                              accountNumber,
                              TransactionType::CloseAccount,
                              0.0,
                              note.empty() ? "Account closed" : note);
}

std::vector<Transaction> Bank::getAllTransactions() const
{
    return transactions;
}

std::vector<Transaction> Bank::getTransactionsForAccount(long accountNumber) const
{
    // Verify the account exists before filtering its transaction history.
    getAccount(accountNumber);

    std::vector<Transaction> filteredTransactions;

    std::copy_if(transactions.begin(),
                 transactions.end(),
                 std::back_inserter(filteredTransactions),
                 [accountNumber](const Transaction& transaction) {
                     return transaction.getAccountNumber() == accountNumber;
                 });

    return filteredTransactions;
}

std::vector<Transaction> Bank::getTransactionsByType(TransactionType type) const
{
    std::vector<Transaction> filteredTransactions;

    std::copy_if(transactions.begin(),
                 transactions.end(),
                 std::back_inserter(filteredTransactions),
                 [type](const Transaction& transaction) {
                     return transaction.getType() == type;
                 });

    return filteredTransactions;
}

std::vector<Transaction> Bank::getMiniStatement(long accountNumber, int numberOfTransactions) const
{
    if (numberOfTransactions <= 0) {
        throw InvalidAmountException("Mini statement count must be greater than 0.");
    }

    std::vector<Transaction> accountTransactions = getTransactionsForAccount(accountNumber);

    // Clean up any default placeholder records before taking the latest transactions.
    accountTransactions.erase(std::remove_if(accountTransactions.begin(),
                                             accountTransactions.end(),
                                             [](const Transaction& transaction) {
                                                 return transaction.getTransactionId() <= 0;
                                             }),
                              accountTransactions.end());

    if (static_cast<int>(accountTransactions.size()) <= numberOfTransactions) {
        return accountTransactions;
    }

    return std::vector<Transaction>(accountTransactions.end() - numberOfTransactions,
                                    accountTransactions.end());
}

DashboardSummary Bank::getDashboardSummary() const
{
    DashboardSummary summary;
    summary.totalAccounts = static_cast<int>(accounts.size());
    summary.totalTransactions = static_cast<int>(transactions.size());

    std::for_each(accounts.begin(), accounts.end(), [&summary](const auto& accountPair) {
        const Account& account = accountPair.second;

        if (account.isActive()) {
            ++summary.activeAccounts;
        } else {
            ++summary.closedAccounts;
        }

        summary.totalBankBalance += account.getBalance();
    });

    if (!accounts.empty()) {
        const auto highestBalanceAccount = std::max_element(accounts.begin(),
                                                            accounts.end(),
                                                            [](const auto& left, const auto& right) {
                                                                return left.second.getBalance()
                                                                       < right.second.getBalance();
                                                            });

        summary.highestBalanceAccountNumber = highestBalanceAccount->second.getAccountNumber();
        summary.highestBalanceAccountName = highestBalanceAccount->second.getFullName();
        summary.highestBalance = highestBalanceAccount->second.getBalance();
    }

    return summary;
}
