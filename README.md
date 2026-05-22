# Banking Management System in C++ with Qt GUI

A school project for building a desktop **Banking Management System** using **C++**, **Qt Widgets**, **CMake**, and local file storage.

This project is designed to demonstrate object-oriented programming, STL containers, STL algorithms, file handling, exception handling, and a simple GUI-based workflow. It is an educational simulation and is not intended for real banking use.

## Project Status

This repository currently contains the project structure, starter files, and documentation. Full application logic will be implemented later.

## Features

### Core Banking Features

- Create account
- View account details
- Deposit money
- Withdraw money
- Transfer money
- Close account
- Display all accounts
- Display transaction history
- Save and load data from files

### Additional Manageable Features

- Automatic account number generation
- Update customer details
- Search account by name
- Sort accounts by name
- Sort accounts by balance
- Filter accounts by account type
- Filter accounts by balance range
- Filter transactions by account
- Filter transactions by transaction type
- Account status: Active or Closed
- Account type: Savings, Current, or Student
- Transaction ID generation
- Transaction date and time timestamps
- Transaction notes or descriptions
- Mini statement showing last N transactions
- Dashboard summary
- Export account reports
- Export transaction reports
- Optional backup before saving data

## Technical Focus

The project is focused on learning and demonstrating the following C++ concepts:

- Object-oriented programming
- Encapsulation
- Classes and objects
- Access specifiers
- STL containers
- `std::map<long, Account>` for account storage
- `std::vector<Transaction>` for transaction storage
- STL algorithms:
  - `std::find_if`
  - `std::sort`
  - `std::copy_if`
  - `std::for_each`
  - `std::remove_if`
  - `std::max_element`
- File handling
- Exception handling
- Simple Qt GUI using Qt Widgets

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

## Project Structure

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

## Main Modules

### Account

Represents a customer bank account.

Planned responsibilities:

- Store account number, customer details, account type, balance, and status.
- Provide controlled access through public methods.
- Support account-level actions such as deposit, withdrawal, and closing.

### Transaction

Represents a single banking transaction.

Planned responsibilities:

- Store transaction ID, account number, destination account number, type, amount, timestamp, and note.
- Support deposit, withdrawal, and transfer transaction records.

### Bank

Acts as the main business logic layer.

Planned responsibilities:

- Manage all accounts using `std::map<long, Account>`.
- Manage all transactions using `std::vector<Transaction>`.
- Generate account numbers and transaction IDs.
- Search, sort, and filter accounts and transactions.
- Save and load data from files.
- Export reports and create optional backups.

### MainWindow

Represents the Qt Widgets GUI.

Planned responsibilities:

- Display dashboard information.
- Display account and transaction tables.
- Collect user input from forms.
- Call `Bank` methods.
- Show success and error messages.

## Documentation

Detailed project documentation is available in:

- [Requirements](docs/requirements.md)
- [Design](docs/design.md)

Screenshots can be added later in:

- `docs/screenshots/`

## Data Files

The application will use local text files for storage:

- `data/accounts.txt`
- `data/transactions.txt`

Expected report output files:

- `reports/accounts_report.txt`
- `reports/accounts_report.csv`
- `reports/transactions_report.txt`
- `reports/transactions_report.csv`

Expected backup files:

- `backup/accounts_backup.txt`
- `backup/transactions_backup.txt`

## Build Requirements

Install the following before building:

- C++17 compatible compiler
- CMake 3.16 or later
- Qt 5 or Qt 6 with Qt Widgets

## Build Instructions

From the project root:

```bash
cmake -S . -B build
cmake --build build
```

On Windows, the executable will usually be generated inside the `build` directory depending on the selected CMake generator.

## Planned Usage

After implementation, the user should be able to:

1. Start the Qt desktop application.
2. Create a new account.
3. Deposit, withdraw, or transfer money.
4. View account details and transaction history.
5. Search, sort, and filter account records.
6. Generate mini statements and dashboard summaries.
7. Save and load account and transaction data.
8. Export reports in text and CSV format.
9. Optionally create backup files before saving.

## Suggested GUI Screens

- Dashboard
- Create account
- Account details
- Deposit
- Withdrawal
- Transfer
- Account list
- Transaction history
- Reports

## Error Handling Plan

The project will use exception handling for cases such as:

- Account not found
- Invalid amount
- Insufficient funds
- Closed account operation
- File read error
- File write error
- Invalid data format

The GUI should catch these errors and display clear messages using Qt message boxes.

## Educational Notes

This project is intentionally file-based instead of database-based. This keeps the scope manageable and makes it easier to focus on C++ fundamentals.

The project should demonstrate a clear separation between:

- Data models
- Business logic
- GUI code
- File/report handling

## License

This project is created for educational use as a school assignment.

