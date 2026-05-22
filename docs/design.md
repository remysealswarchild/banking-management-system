# Design

## Project Title

**Banking Management System in C++ with Qt GUI**

## Overview

This document describes the planned design for a school-level Banking Management System written in C++ with a Qt Widgets graphical user interface.

The application will manage bank accounts and transaction records using local files. It will use object-oriented design, STL containers, STL algorithms, exception handling, and a simple GUI. The system will avoid advanced production features such as authentication, networking, real payment processing, and database integration.

## Design Goals

- Keep the design simple enough for a school project.
- Separate data models, banking logic, and GUI code.
- Use C++ classes with clear responsibilities.
- Store accounts in `std::map<long, Account>`.
- Store transactions in `std::vector<Transaction>`.
- Use STL algorithms for searching, sorting, filtering, and summaries.
- Save and load data using local text files.
- Provide a clean Qt Widgets interface for common operations.

## High-Level Architecture

```text
+-------------------+
|    MainWindow     |
|   Qt Widgets GUI  |
+---------+---------+
          |
          v
+---------+---------+
|       Bank        |
| Business logic    |
| Accounts map      |
| Transactions list |
+----+---------+----+
     |         |
     v         v
+----+----+ +--+-------------+
| Account | | Transaction    |
+---------+ +----------------+
     |
     v
+-----------------------------+
| Files, Reports, and Backup  |
+-----------------------------+
```

## Folder Structure

```text
banking-management-system/
|
|-- README.md
|-- .gitignore
|-- CMakeLists.txt
|
|-- docs/
|   |-- requirements.md
|   |-- design.md
|   |-- screenshots/
|
|-- data/
|   |-- accounts.txt
|   |-- transactions.txt
|
|-- reports/
|   |-- accounts_report.txt
|   |-- transactions_report.txt
|   |-- accounts_report.csv
|   |-- transactions_report.csv
|
|-- backup/
|   |-- accounts_backup.txt
|   |-- transactions_backup.txt
|
|-- include/
|   |-- Account.h
|   |-- Transaction.h
|   |-- Bank.h
|   |-- Exceptions.h
|   |-- DashboardSummary.h
|   |-- MainWindow.h
|
|-- src/
|   |-- Account.cpp
|   |-- Transaction.cpp
|   |-- Bank.cpp
|   |-- MainWindow.cpp
|   |-- main.cpp
|
|-- tests/
|   |-- test_bank.cpp
|
|-- scripts/
    |-- create_project_structure.sh
    |-- create_project_structure.bat
```

## Main Components

### Account

Files:

- `include/Account.h`
- `src/Account.cpp`

The `Account` class represents one bank account.

Responsibilities:

- Store account number.
- Store customer name and contact details.
- Store account type.
- Store current balance.
- Store account status.
- Provide methods for reading and updating account data.
- Provide account-level validation where suitable.

Suggested fields:

```cpp
long accountNumber;
std::string customerName;
std::string contactDetails;
AccountType accountType;
double balance;
AccountStatus status;
```

Suggested enums:

```cpp
enum class AccountType {
    Savings,
    Current,
    Student
};

enum class AccountStatus {
    Active,
    Closed
};
```

Suggested methods:

```cpp
long getAccountNumber() const;
std::string getCustomerName() const;
std::string getContactDetails() const;
AccountType getAccountType() const;
double getBalance() const;
AccountStatus getStatus() const;

void updateCustomerDetails(const std::string& name, const std::string& contact);
void updateAccountType(AccountType type);
void deposit(double amount);
void withdraw(double amount);
void close();
bool isActive() const;
```

Encapsulation note:

- Data members should be private.
- Public methods should control valid access and changes.
- Balance should not be modified directly from outside the class.

### Transaction

Files:

- `include/Transaction.h`
- `src/Transaction.cpp`

The `Transaction` class represents one banking operation.

Responsibilities:

- Store transaction ID.
- Store source account number.
- Store destination account number for transfers.
- Store transaction type.
- Store amount.
- Store date and time.
- Store notes or description.

Suggested fields:

```cpp
long transactionId;
long accountNumber;
long destinationAccountNumber;
TransactionType type;
double amount;
std::string timestamp;
std::string note;
```

Suggested enum:

```cpp
enum class TransactionType {
    Deposit,
    Withdrawal,
    Transfer
};
```

Suggested methods:

```cpp
long getTransactionId() const;
long getAccountNumber() const;
long getDestinationAccountNumber() const;
TransactionType getType() const;
double getAmount() const;
std::string getTimestamp() const;
std::string getNote() const;
```

### Bank

Files:

- `include/Bank.h`
- `src/Bank.cpp`

The `Bank` class is the main business logic layer.

Responsibilities:

- Manage all accounts.
- Manage all transactions.
- Generate account numbers.
- Generate transaction IDs.
- Create, update, close, search, sort, and filter accounts.
- Perform deposits, withdrawals, and transfers.
- Save and load data from files.
- Export account and transaction reports.
- Create optional backups before saving.
- Build dashboard summaries.

Main containers:

```cpp
std::map<long, Account> accounts;
std::vector<Transaction> transactions;
```

Suggested fields:

```cpp
std::map<long, Account> accounts;
std::vector<Transaction> transactions;
long nextAccountNumber;
long nextTransactionId;
```

Suggested methods:

```cpp
long createAccount(const std::string& name,
                   const std::string& contact,
                   AccountType type,
                   double openingBalance);

const Account& getAccount(long accountNumber) const;
void updateCustomerDetails(long accountNumber,
                           const std::string& name,
                           const std::string& contact);

void deposit(long accountNumber, double amount, const std::string& note);
void withdraw(long accountNumber, double amount, const std::string& note);
void transfer(long fromAccount, long toAccount, double amount, const std::string& note);
void closeAccount(long accountNumber);

std::vector<Account> getAllAccounts() const;
std::vector<Account> searchAccountsByName(const std::string& name) const;
std::vector<Account> sortAccountsByName() const;
std::vector<Account> sortAccountsByBalance(bool ascending) const;
std::vector<Account> filterAccountsByType(AccountType type) const;
std::vector<Account> filterAccountsByBalanceRange(double minBalance, double maxBalance) const;

std::vector<Transaction> getTransactionHistory() const;
std::vector<Transaction> filterTransactionsByAccount(long accountNumber) const;
std::vector<Transaction> filterTransactionsByType(TransactionType type) const;
std::vector<Transaction> getMiniStatement(long accountNumber, std::size_t count) const;

DashboardSummary getDashboardSummary() const;

void loadData();
void saveData(bool createBackup);
void exportAccountReports() const;
void exportTransactionReports() const;
```

### DashboardSummary

File:

- `include/DashboardSummary.h`

The `DashboardSummary` structure stores calculated dashboard values.

Suggested fields:

```cpp
int totalAccounts;
int activeAccounts;
int closedAccounts;
int totalTransactions;
double totalActiveBalance;
double highestBalance;
long highestBalanceAccountNumber;
std::string latestTransactionSummary;
```

This type keeps the GUI simple because `MainWindow` can request one summary object instead of calculating summary values itself.

### Exceptions

File:

- `include/Exceptions.h`

Custom exceptions make error handling clearer and more testable.

Suggested exception types:

```cpp
class BankException : public std::runtime_error;
class AccountNotFoundException : public BankException;
class InvalidAmountException : public BankException;
class InsufficientFundsException : public BankException;
class AccountClosedException : public BankException;
class FileOperationException : public BankException;
class InvalidDataException : public BankException;
```

The GUI can catch these exceptions and show messages using `QMessageBox`.

### MainWindow

Files:

- `include/MainWindow.h`
- `src/MainWindow.cpp`

The `MainWindow` class manages the Qt Widgets interface.

Responsibilities:

- Display dashboard values.
- Display account and transaction tables.
- Read form input from the user.
- Call methods on the `Bank` object.
- Refresh GUI tables after changes.
- Show success and error messages.

The GUI should not contain business rules. Business rules should stay in the `Bank` and model classes.

## Data Flow

### Startup Flow

```text
Application starts
        |
        v
Create Bank object
        |
        v
Load accounts and transactions from files
        |
        v
Create MainWindow
        |
        v
Display dashboard, accounts, and transaction history
```

### Save Flow

```text
User saves data or exits application
        |
        v
Optional backup is created
        |
        v
Accounts are written to data/accounts.txt
        |
        v
Transactions are written to data/transactions.txt
```

### Transaction Flow

```text
User enters transaction details
        |
        v
MainWindow validates basic form completeness
        |
        v
Bank validates account rules and amount rules
        |
        v
Account balance is updated
        |
        v
Transaction object is created
        |
        v
Transaction is added to transactions vector
        |
        v
GUI tables and dashboard are refreshed
```

## GUI Design

The GUI should be simple and practical. A tab-based layout is recommended.

Suggested main sections:

| Section | Purpose |
| --- | --- |
| Dashboard | Show account and transaction summary |
| Accounts | Create, view, update, close, search, sort, and filter accounts |
| Transactions | Deposit, withdraw, transfer, view history, filter transactions |
| Reports | Export text and CSV reports |
| Settings | Optional backup setting and save/load controls |

Suggested widgets:

| Widget | Usage |
| --- | --- |
| `QMainWindow` | Main application window |
| `QTabWidget` | Separate dashboard, accounts, transactions, and reports |
| `QTableWidget` or `QTableView` | Display accounts and transactions |
| `QLineEdit` | Customer name, contact, search text, notes |
| `QComboBox` | Account type, status, transaction type filters |
| `QDoubleSpinBox` | Money input |
| `QSpinBox` | Mini statement transaction count |
| `QPushButton` | Actions such as create, deposit, withdraw, save, export |
| `QMessageBox` | Error and success messages |

## Account Operations Design

### Create Account

1. User enters customer details, account type, and opening balance.
2. `MainWindow` sends values to `Bank::createAccount`.
3. `Bank` validates the input.
4. `Bank` generates a new account number.
5. A new `Account` object is inserted into `std::map<long, Account>`.
6. If opening balance is greater than zero, an opening deposit transaction may be recorded.
7. GUI refreshes the account table and dashboard.

### Update Customer Details

1. User selects an account.
2. User edits name, contact, or account type.
3. `Bank` checks that the account exists.
4. Account details are updated through public methods.
5. GUI refreshes account details.

### Close Account

1. User selects an account.
2. `Bank` checks that the account exists.
3. Account status is changed to Closed.
4. Account remains stored in the map.
5. Future deposit, withdrawal, and transfer actions are blocked for the closed account.

## Transaction Operations Design

### Deposit

1. User enters account number, amount, and optional note.
2. `Bank` validates account existence, status, and amount.
3. Account balance is increased.
4. A Deposit transaction is added to the transaction vector.

### Withdrawal

1. User enters account number, amount, and optional note.
2. `Bank` validates account existence, status, amount, and available balance.
3. Account balance is decreased.
4. A Withdrawal transaction is added to the transaction vector.

### Transfer

1. User enters source account, destination account, amount, and optional note.
2. `Bank` validates both accounts.
3. `Bank` confirms the accounts are different and active.
4. `Bank` confirms sufficient funds in the source account.
5. Source balance is decreased.
6. Destination balance is increased.
7. A Transfer transaction is added to the transaction vector.

## Searching, Sorting, And Filtering

The account map is optimized for account-number lookup. For display sorting and filtering, the map values can be copied into vectors.

### Search Account By Name

Suggested algorithm:

- Use `std::find_if` for single-match searching.
- Use `std::copy_if` for collecting all matching accounts.

### Sort Accounts By Name

Suggested algorithm:

- Copy accounts into `std::vector<Account>`.
- Use `std::sort` with customer name comparison.

### Sort Accounts By Balance

Suggested algorithm:

- Copy accounts into `std::vector<Account>`.
- Use `std::sort` with balance comparison.

### Filter Accounts By Type

Suggested algorithm:

- Use `std::copy_if` to copy accounts with the selected account type.

### Filter Accounts By Balance Range

Suggested algorithm:

- Use `std::copy_if` to copy accounts where balance is between minimum and maximum values.

### Filter Transactions

Suggested algorithm:

- Use `std::copy_if` to filter by account number.
- Use `std::copy_if` to filter by transaction type.

### Dashboard Summary

Suggested algorithms:

- Use `std::for_each` to count active and closed accounts.
- Use `std::max_element` to find the account with the highest balance.
- Use the last transaction in `std::vector<Transaction>` as the most recent transaction if transactions are stored in chronological order.

### Temporary Removal Or Cleanup

Suggested algorithm:

- Use `std::remove_if` for cleaning invalid temporary records during import or for removing invalid entries from temporary vectors before display.

## File Design

The project uses local text files instead of a database.

### Account File

Path:

```text
data/accounts.txt
```

Suggested delimiter format:

```text
accountNumber|customerName|contactDetails|accountType|balance|status
```

Example:

```text
1001|Alice Johnson|alice@example.com|Savings|5000.00|Active
1002|Mark Smith|mark@example.com|Student|300.00|Closed
```

### Transaction File

Path:

```text
data/transactions.txt
```

Suggested delimiter format:

```text
transactionId|accountNumber|destinationAccountNumber|type|amount|timestamp|note
```

Example:

```text
1|1001|0|Deposit|5000.00|2026-05-22 10:30:00|Opening balance
2|1001|1002|Transfer|200.00|2026-05-22 11:00:00|Student fee transfer
```

For non-transfer transactions, `destinationAccountNumber` can be stored as `0`.

## Report Design

### Account Reports

Paths:

```text
reports/accounts_report.txt
reports/accounts_report.csv
```

Text report should be readable by humans.

CSV report should be suitable for opening in spreadsheet software.

Suggested CSV columns:

```text
account_number,customer_name,contact_details,account_type,balance,status
```

### Transaction Reports

Paths:

```text
reports/transactions_report.txt
reports/transactions_report.csv
```

Suggested CSV columns:

```text
transaction_id,account_number,destination_account_number,type,amount,timestamp,note
```

## Backup Design

Backup files:

```text
backup/accounts_backup.txt
backup/transactions_backup.txt
```

Before saving new data, the system may copy the current data files to the backup folder. This protects the previous saved state if a save operation fails.

Recommended save order:

1. Check whether backup option is enabled.
2. Copy existing data files to backup files.
3. Write accounts to a temporary file.
4. Write transactions to a temporary file.
5. Replace the original data files after successful writes.
6. Report errors using exceptions and GUI messages.

For a school project, direct file writing is acceptable, but temporary files are a cleaner design if time allows.

## Exception Handling Design

The `Bank` class should throw exceptions for invalid operations. The `MainWindow` class should catch exceptions and display friendly messages.

Examples:

| Situation | Suggested Exception |
| --- | --- |
| Account number does not exist | `AccountNotFoundException` |
| Amount is zero or negative | `InvalidAmountException` |
| Withdrawal exceeds balance | `InsufficientFundsException` |
| Account is closed | `AccountClosedException` |
| File cannot be opened | `FileOperationException` |
| File record cannot be parsed | `InvalidDataException` |

## Validation Rules

- Customer name must not be empty.
- Account type must be Savings, Current, or Student.
- Account status must be Active or Closed.
- Deposit amount must be greater than zero.
- Withdrawal amount must be greater than zero.
- Withdrawal amount must not exceed balance.
- Transfer amount must be greater than zero.
- Source and destination accounts must be different.
- Closed accounts cannot be used for new transactions.
- Minimum balance filter must not be greater than maximum balance filter.

## Build Design

The project uses CMake.

Expected build flow:

```bash
cmake -S . -B build
cmake --build build
```

Qt Widgets should be linked through CMake using either Qt 6 or Qt 5, depending on the installed environment.

## Testing Design

The `tests/test_bank.cpp` file can be used for simple tests of the `Bank` class.

Suggested tests:

- Create account with valid data.
- Reject account with negative opening balance.
- Deposit increases balance.
- Withdrawal decreases balance.
- Withdrawal rejects insufficient funds.
- Transfer updates both accounts.
- Closed account rejects transactions.
- Search by name returns matching accounts.
- Sort by balance returns expected order.
- Save and load preserve account and transaction data.

## Out Of Scope

The following items are not part of the design:

- Login system
- Authentication
- Admin roles
- Loans
- Credit cards
- Database integration
- Online banking
- Real payment processing
- Networking
- Multithreading
- Advanced analytics
- Production-level security

