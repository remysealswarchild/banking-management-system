# Requirements

## Project Title

**Banking Management System in C++ with Qt GUI**

## Purpose

The purpose of this project is to build a simple desktop banking management system for a school assignment. The application will demonstrate core object-oriented programming concepts in C++, file handling, exception handling, STL containers and algorithms, and a basic graphical user interface using Qt Widgets.

This project is not intended to be a real banking product. It is an educational system designed to simulate common banking operations in a controlled local environment.

## Goals

- Provide a simple GUI for managing bank accounts and transactions.
- Store account and transaction data in local text files.
- Demonstrate clean C++ class design and encapsulation.
- Use STL containers and algorithms in meaningful places.
- Generate simple text and CSV reports.
- Keep the project manageable for school-level implementation.

## Users

The expected user is a student, teacher, or evaluator running the project locally on a desktop computer.

The user should be able to:

- Add and manage customer bank accounts.
- Perform basic banking transactions.
- View summaries, reports, and transaction history.
- Save and load data from files without needing a database.

## Functional Requirements

### FR-01: Create Account

The system shall allow the user to create a new bank account.

Required account information:

- Account number
- Customer name
- Customer phone number or contact detail
- Account type
- Opening balance
- Account status

Rules:

- The account number shall be generated automatically.
- The account type shall be one of: Savings, Current, or Student.
- The account status shall be Active by default.
- The opening balance shall not be negative.

### FR-02: Automatic Account Number Generation

The system shall automatically generate a unique account number for every new account.

Rules:

- The generated number must not duplicate an existing account number.
- The next account number should be based on the highest existing account number when data is loaded from file.

### FR-03: View Account Details

The system shall allow the user to view details for a selected account.

Displayed details should include:

- Account number
- Customer name
- Contact details
- Account type
- Current balance
- Account status
- Date created, if stored

### FR-04: Update Customer Details

The system shall allow the user to update customer information for an existing active account.

Editable fields may include:

- Customer name
- Contact details
- Account type

Rules:

- The account number should not be changed after creation.
- The account balance should only be changed through transactions.

### FR-05: Deposit Money

The system shall allow the user to deposit money into an active account.

Rules:

- Deposit amount must be greater than zero.
- Deposit must increase the account balance.
- A transaction record must be created.
- Closed accounts cannot receive deposits.

### FR-06: Withdraw Money

The system shall allow the user to withdraw money from an active account.

Rules:

- Withdrawal amount must be greater than zero.
- Withdrawal amount must not exceed the account balance.
- Withdrawal must decrease the account balance.
- A transaction record must be created.
- Closed accounts cannot be used for withdrawals.

### FR-07: Transfer Money

The system shall allow the user to transfer money from one active account to another active account.

Rules:

- Transfer amount must be greater than zero.
- Source and destination accounts must both exist.
- Source and destination accounts must be different.
- Source account must have enough balance.
- Source account balance must decrease.
- Destination account balance must increase.
- A transaction record must be created for the transfer.

### FR-08: Close Account

The system shall allow the user to close an existing account.

Rules:

- The account status shall be changed from Active to Closed.
- Closed accounts shall remain visible in account records.
- Closed accounts shall not allow deposit, withdrawal, or transfer operations.
- Existing transaction history for the account shall be preserved.

### FR-09: Display All Accounts

The system shall display all accounts in a table or list view.

Displayed columns may include:

- Account number
- Customer name
- Account type
- Balance
- Account status

### FR-10: Search Account By Name

The system shall allow the user to search accounts by customer name.

Rules:

- Search may be partial or exact.
- Search should be case-insensitive if feasible.
- Matching results should be displayed clearly in the GUI.

### FR-11: Sort Accounts By Name

The system shall allow accounts to be sorted alphabetically by customer name.

Expected technical use:

- Copy account records from `std::map<long, Account>` into a temporary `std::vector<Account>`.
- Use `std::sort` with a name comparison function.

### FR-12: Sort Accounts By Balance

The system shall allow accounts to be sorted by current balance.

Rules:

- Sorting may be ascending or descending.
- The original account map should not need to be reordered.

### FR-13: Filter Accounts By Account Type

The system shall allow the user to filter displayed accounts by account type.

Supported account types:

- Savings
- Current
- Student

### FR-14: Filter Accounts By Balance Range

The system shall allow the user to filter accounts by minimum and maximum balance.

Rules:

- Minimum balance must not be greater than maximum balance.
- Results should include accounts within the selected range.

### FR-15: Display Transaction History

The system shall display transaction history.

Transaction details should include:

- Transaction ID
- Related account number
- Destination account number, if applicable
- Transaction type
- Amount
- Date and time
- Notes or description

### FR-16: Transaction ID Generation

The system shall automatically generate a unique transaction ID for every transaction.

Rules:

- The generated ID must not duplicate an existing transaction ID.
- The next transaction ID should be based on the highest existing transaction ID when data is loaded from file.

### FR-17: Transaction Date And Time Timestamps

The system shall record the date and time for every transaction.

Rules:

- The timestamp should be generated automatically when the transaction is created.
- A readable date and time format should be used in the GUI and reports.

### FR-18: Transaction Notes Or Descriptions

The system shall allow a transaction to include a short note or description.

Examples:

- Cash deposit
- ATM withdrawal
- Transfer to student account
- Account opening deposit

### FR-19: Filter Transactions By Account

The system shall allow transaction history to be filtered by account number.

Rules:

- Transactions where the account is either the source or destination should be included for transfers.

### FR-20: Filter Transactions By Transaction Type

The system shall allow transaction history to be filtered by transaction type.

Supported transaction types:

- Deposit
- Withdrawal
- Transfer

### FR-21: Mini Statement

The system shall show the last N transactions for a selected account.

Rules:

- N may be selected by the user or configured with a default value.
- The most recent transactions should be shown first.

### FR-22: Dashboard Summary

The system shall provide a dashboard summary.

The summary may include:

- Total number of accounts
- Number of active accounts
- Number of closed accounts
- Total balance across active accounts
- Number of transactions
- Highest account balance
- Most recent transaction

### FR-23: Save Data To Files

The system shall save account and transaction data to local files.

Expected files:

- `data/accounts.txt`
- `data/transactions.txt`

Rules:

- Data should be saved in a consistent structured text format.
- The system should handle file write errors using exception handling.

### FR-24: Load Data From Files

The system shall load account and transaction data from local files when the application starts or when requested.

Rules:

- Missing files should be handled gracefully.
- Invalid or incomplete records should not crash the application.
- The next account number and next transaction ID should be updated after loading existing data.

### FR-25: Export Account Reports

The system shall export account reports.

Expected files:

- `reports/accounts_report.txt`
- `reports/accounts_report.csv`

The reports should include:

- Account number
- Customer name
- Account type
- Balance
- Account status

### FR-26: Export Transaction Reports

The system shall export transaction reports.

Expected files:

- `reports/transactions_report.txt`
- `reports/transactions_report.csv`

The reports should include:

- Transaction ID
- Account number
- Destination account number, if applicable
- Transaction type
- Amount
- Date and time
- Notes

### FR-27: Optional Backup Before Saving Data

The system should optionally create backups before overwriting saved data.

Expected backup files:

- `backup/accounts_backup.txt`
- `backup/transactions_backup.txt`

Rules:

- Backup should copy the most recent saved data before a new save operation.
- Backup failure should be reported to the user but should not corrupt existing data.

## Non-Functional Requirements

### NFR-01: Usability

The GUI should be simple, clear, and suitable for a school project demonstration.

The user should be able to perform the main operations from visible buttons, menus, tables, forms, or tabs.

### NFR-02: Reliability

The system should validate user input before performing operations.

Examples:

- Empty customer name should not be accepted.
- Negative deposit or withdrawal values should not be accepted.
- Transfer to the same account should not be accepted.
- Closed accounts should not be used for new transactions.

### NFR-03: Maintainability

The code should separate responsibilities into classes.

Expected separation:

- `Account` stores account data and account-level behavior.
- `Transaction` stores transaction data.
- `Bank` manages accounts, transactions, searching, sorting, filtering, saving, loading, reports, and backups.
- `MainWindow` manages the Qt GUI.

### NFR-04: Portability

The project should be buildable with CMake and Qt on common desktop platforms.

Target environments:

- Windows
- Linux
- macOS, if Qt and CMake are available

### NFR-05: Educational Value

The implementation should clearly demonstrate:

- Object-oriented programming
- Encapsulation
- Classes and objects
- Public and private access specifiers
- STL containers
- STL algorithms
- File handling
- Exception handling
- Basic Qt Widgets GUI programming

## Technical Requirements

### Programming Language

- C++17 or later

### GUI Framework

- Qt Widgets

### Build System

- CMake

### Main Data Structures

Accounts:

```cpp
std::map<long, Account> accounts;
```

Transactions:

```cpp
std::vector<Transaction> transactions;
```

### Required STL Algorithms

The project should use the following STL algorithms where appropriate:

- `std::find_if`
- `std::sort`
- `std::copy_if`
- `std::for_each`
- `std::remove_if`
- `std::max_element`

Suggested usage:

| Algorithm | Possible Use |
| --- | --- |
| `std::find_if` | Search account by customer name or search transaction by ID |
| `std::sort` | Sort copied account list by name or balance |
| `std::copy_if` | Filter accounts or transactions |
| `std::for_each` | Generate summaries or format report rows |
| `std::remove_if` | Remove temporary filtered values or clean invalid imported records |
| `std::max_element` | Find highest balance or highest existing ID |

## GUI Requirements

The Qt GUI should include simple views for:

- Dashboard
- Account creation
- Account details
- Deposit
- Withdrawal
- Transfer
- Account list
- Transaction history
- Reports

The GUI should use appropriate Qt Widgets such as:

- `QMainWindow`
- `QTableWidget` or `QTableView`
- `QLineEdit`
- `QComboBox`
- `QPushButton`
- `QLabel`
- `QSpinBox` or `QDoubleSpinBox`
- `QMessageBox`

## Data Requirements

### Account Data

Each account record should store:

- Account number
- Customer name
- Contact details
- Account type
- Balance
- Status

### Transaction Data

Each transaction record should store:

- Transaction ID
- Account number
- Destination account number, if applicable
- Transaction type
- Amount
- Date and time
- Notes or description

## Error Handling Requirements

The system should use exception handling for important error cases.

Expected error categories:

- Account not found
- Invalid amount
- Insufficient funds
- Account closed
- File read error
- File write error
- Invalid data format

## Out Of Scope

The following features are intentionally not included:

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

## Acceptance Criteria

The project will be considered complete when:

- The Qt application builds successfully.
- A user can create, update, view, close, search, sort, and filter accounts.
- A user can deposit, withdraw, and transfer money using valid inputs.
- Transaction history is recorded and displayed.
- Mini statements and dashboard summaries are available.
- Account and transaction data can be saved and loaded from files.
- Account and transaction reports can be exported in text and CSV format.
- Invalid inputs are handled without crashing the application.
- The code demonstrates the required C++ and STL concepts.

