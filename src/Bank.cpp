#include "Bank.h"
#include "Exceptions.h"

#include <algorithm>
#include <cctype>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <sstream>
#include <string>
#include <system_error>

namespace {

const std::filesystem::path DATA_FOLDER = "data";
const std::filesystem::path BACKUP_FOLDER = "backup";
const std::filesystem::path REPORTS_FOLDER = "reports";
const std::filesystem::path ACCOUNTS_FILE = DATA_FOLDER / "accounts.txt";
const std::filesystem::path TRANSACTIONS_FILE = DATA_FOLDER / "transactions.txt";
const std::filesystem::path ACCOUNTS_BACKUP_FILE = BACKUP_FOLDER / "accounts_backup.txt";
const std::filesystem::path TRANSACTIONS_BACKUP_FILE = BACKUP_FOLDER / "transactions_backup.txt";

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

void ensureDataFolderExists()
{
    std::error_code error;
    std::filesystem::create_directories(DATA_FOLDER, error);

    if (error) {
        throw FileOperationException("Could not create data folder: " + error.message());
    }
}

void ensureFolderExists(const std::filesystem::path& folderPath, const std::string& folderName)
{
    std::error_code error;
    std::filesystem::create_directories(folderPath, error);

    if (error) {
        throw FileOperationException("Could not create " + folderName + " folder: " + error.message());
    }
}

std::filesystem::path prepareOutputPath(const std::string& filePath)
{
    std::filesystem::path outputPath(filePath);
    if (outputPath.has_parent_path()) {
        ensureFolderExists(outputPath.parent_path(), "output");
    }

    return outputPath;
}

std::vector<std::string> splitLine(const std::string& line, char delimiter)
{
    std::vector<std::string> parts;
    std::string part;
    std::stringstream stream(line);

    while (std::getline(stream, part, delimiter)) {
        parts.push_back(part);
    }

    return parts;
}

bool shouldSkipLine(const std::string& line)
{
    return line.empty() || line.front() == '#';
}

std::string csvEscape(const std::string& value)
{
    const bool needsQuotes = value.find_first_of(",\"\n") != std::string::npos;
    if (!needsQuotes) {
        return value;
    }

    std::string escapedValue = "\"";
    for (char ch : value) {
        if (ch == '"') {
            escapedValue += "\"\"";
        } else {
            escapedValue += ch;
        }
    }

    escapedValue += '"';
    return escapedValue;
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

void Bank::saveAccounts() const
{
    ensureDataFolderExists();

    std::ofstream outputFile(ACCOUNTS_FILE.string());
    if (!outputFile) {
        throw FileOperationException("Could not open data/accounts.txt for writing.");
    }

    for (const auto& accountPair : accounts) {
        const Account& account = accountPair.second;

        outputFile << account.getAccountNumber() << '|'
                   << account.getFirstName() << '|'
                   << account.getLastName() << '|'
                   << accountTypeToString(account.getAccountType()) << '|'
                   << accountStatusToString(account.getStatus()) << '|'
                   << account.getBalance() << '\n';
    }

    if (!outputFile) {
        throw FileOperationException("Could not write account data to data/accounts.txt.");
    }
}

void Bank::loadAccounts()
{
    ensureDataFolderExists();

    accounts.clear();
    nextAccountNumber = 1001;

    if (!std::filesystem::exists(ACCOUNTS_FILE)) {
        return;
    }

    std::ifstream inputFile(ACCOUNTS_FILE.string());
    if (!inputFile) {
        return;
    }

    std::string line;
    long highestAccountNumber = 1000;

    while (std::getline(inputFile, line)) {
        if (shouldSkipLine(line)) {
            continue;
        }

        const std::vector<std::string> parts = splitLine(line, '|');
        if (parts.size() != 6) {
            continue;
        }

        try {
            const long accountNumber = std::stol(parts[0]);
            const std::string& firstName = parts[1];
            const std::string& lastName = parts[2];
            const AccountType accountType = stringToAccountType(parts[3]);
            const AccountStatus status = stringToAccountStatus(parts[4]);
            const double balance = std::stod(parts[5]);

            accounts.emplace(accountNumber,
                             Account(accountNumber,
                                     firstName,
                                     lastName,
                                     accountType,
                                     balance,
                                     status));

            highestAccountNumber = std::max(highestAccountNumber, accountNumber);
        } catch (const std::exception&) {
            // Keep loading simple: skip invalid records and continue with the rest.
            continue;
        }
    }

    nextAccountNumber = highestAccountNumber + 1;
}

void Bank::saveTransactions() const
{
    ensureDataFolderExists();

    std::ofstream outputFile(TRANSACTIONS_FILE.string());
    if (!outputFile) {
        throw FileOperationException("Could not open data/transactions.txt for writing.");
    }

    for (const Transaction& transaction : transactions) {
        outputFile << transaction.getTransactionId() << '|'
                   << transaction.getAccountNumber() << '|'
                   << transactionTypeToString(transaction.getType()) << '|'
                   << transaction.getAmount() << '|'
                   << transaction.getNote() << '|'
                   << transaction.getDateTime() << '\n';
    }

    if (!outputFile) {
        throw FileOperationException("Could not write transaction data to data/transactions.txt.");
    }
}

void Bank::loadTransactions()
{
    ensureDataFolderExists();

    transactions.clear();
    nextTransactionId = 1;

    if (!std::filesystem::exists(TRANSACTIONS_FILE)) {
        return;
    }

    std::ifstream inputFile(TRANSACTIONS_FILE.string());
    if (!inputFile) {
        return;
    }

    std::string line;
    long highestTransactionId = 0;

    while (std::getline(inputFile, line)) {
        if (shouldSkipLine(line)) {
            continue;
        }

        const std::vector<std::string> parts = splitLine(line, '|');
        if (parts.size() != 6) {
            continue;
        }

        try {
            const long transactionId = std::stol(parts[0]);
            const long accountNumber = std::stol(parts[1]);
            const TransactionType type = stringToTransactionType(parts[2]);
            const double amount = std::stod(parts[3]);
            const std::string& note = parts[4];
            const std::string& dateTime = parts[5];

            transactions.emplace_back(transactionId, accountNumber, type, amount, note, dateTime);
            highestTransactionId = std::max(highestTransactionId, transactionId);
        } catch (const std::exception&) {
            // Keep loading simple: skip invalid records and continue with the rest.
            continue;
        }
    }

    nextTransactionId = highestTransactionId + 1;
}

void Bank::saveAll() const
{
    saveAccounts();
    saveTransactions();
}

void Bank::loadAll()
{
    loadAccounts();
    loadTransactions();
}

void Bank::backupDataFiles() const
{
    ensureFolderExists(BACKUP_FOLDER, "backup");

    std::error_code error;

    if (std::filesystem::exists(ACCOUNTS_FILE)) {
        std::filesystem::copy_file(ACCOUNTS_FILE,
                                   ACCOUNTS_BACKUP_FILE,
                                   std::filesystem::copy_options::overwrite_existing,
                                   error);

        if (error) {
            throw FileOperationException("Could not back up data/accounts.txt: " + error.message());
        }
    }

    error.clear();

    if (std::filesystem::exists(TRANSACTIONS_FILE)) {
        std::filesystem::copy_file(TRANSACTIONS_FILE,
                                   TRANSACTIONS_BACKUP_FILE,
                                   std::filesystem::copy_options::overwrite_existing,
                                   error);

        if (error) {
            throw FileOperationException("Could not back up data/transactions.txt: " + error.message());
        }
    }
}

void Bank::exportAccountsReport(const std::string& filePath) const
{
    ensureFolderExists(REPORTS_FOLDER, "reports");
    const std::filesystem::path outputPath = prepareOutputPath(filePath);

    std::ofstream outputFile(outputPath.string());
    if (!outputFile) {
        throw FileOperationException("Could not open account report file for writing: " + filePath);
    }

    outputFile << "Banking Management System - Accounts Report\n";
    outputFile << "==========================================\n\n";

    for (const auto& accountPair : accounts) {
        const Account& account = accountPair.second;

        outputFile << "Account Number: " << account.getAccountNumber() << '\n'
                   << "Name: " << account.getFullName() << '\n'
                   << "First Name: " << account.getFirstName() << '\n'
                   << "Last Name: " << account.getLastName() << '\n'
                   << "Account Type: " << accountTypeToString(account.getAccountType()) << '\n'
                   << "Status: " << accountStatusToString(account.getStatus()) << '\n'
                   << "Balance: " << account.getBalance() << "\n\n";
    }

    if (!outputFile) {
        throw FileOperationException("Could not write account report: " + filePath);
    }
}

void Bank::exportTransactionsReport(const std::string& filePath) const
{
    ensureFolderExists(REPORTS_FOLDER, "reports");
    const std::filesystem::path outputPath = prepareOutputPath(filePath);

    std::ofstream outputFile(outputPath.string());
    if (!outputFile) {
        throw FileOperationException("Could not open transaction report file for writing: " + filePath);
    }

    outputFile << "Banking Management System - Transactions Report\n";
    outputFile << "===============================================\n\n";

    for (const Transaction& transaction : transactions) {
        outputFile << "Transaction ID: " << transaction.getTransactionId() << '\n'
                   << "Account Number: " << transaction.getAccountNumber() << '\n'
                   << "Type: " << transactionTypeToString(transaction.getType()) << '\n'
                   << "Amount: " << transaction.getAmount() << '\n'
                   << "Note: " << transaction.getNote() << '\n'
                   << "Date/Time: " << transaction.getDateTime() << "\n\n";
    }

    if (!outputFile) {
        throw FileOperationException("Could not write transaction report: " + filePath);
    }
}

void Bank::exportAccountsCsv(const std::string& filePath) const
{
    ensureFolderExists(REPORTS_FOLDER, "reports");
    const std::filesystem::path outputPath = prepareOutputPath(filePath);

    std::ofstream outputFile(outputPath.string());
    if (!outputFile) {
        throw FileOperationException("Could not open accounts CSV file for writing: " + filePath);
    }

    outputFile << "account_number,first_name,last_name,full_name,account_type,status,balance\n";

    for (const auto& accountPair : accounts) {
        const Account& account = accountPair.second;

        outputFile << account.getAccountNumber() << ','
                   << csvEscape(account.getFirstName()) << ','
                   << csvEscape(account.getLastName()) << ','
                   << csvEscape(account.getFullName()) << ','
                   << accountTypeToString(account.getAccountType()) << ','
                   << accountStatusToString(account.getStatus()) << ','
                   << account.getBalance() << '\n';
    }

    if (!outputFile) {
        throw FileOperationException("Could not write accounts CSV file: " + filePath);
    }
}

void Bank::exportTransactionsCsv(const std::string& filePath) const
{
    ensureFolderExists(REPORTS_FOLDER, "reports");
    const std::filesystem::path outputPath = prepareOutputPath(filePath);

    std::ofstream outputFile(outputPath.string());
    if (!outputFile) {
        throw FileOperationException("Could not open transactions CSV file for writing: " + filePath);
    }

    outputFile << "transaction_id,account_number,type,amount,note,date_time\n";

    for (const Transaction& transaction : transactions) {
        outputFile << transaction.getTransactionId() << ','
                   << transaction.getAccountNumber() << ','
                   << transactionTypeToString(transaction.getType()) << ','
                   << transaction.getAmount() << ','
                   << csvEscape(transaction.getNote()) << ','
                   << csvEscape(transaction.getDateTime()) << '\n';
    }

    if (!outputFile) {
        throw FileOperationException("Could not write transactions CSV file: " + filePath);
    }
}
