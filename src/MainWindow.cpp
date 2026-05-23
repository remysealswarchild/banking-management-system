#include "MainWindow.h"

#include <QAbstractItemView>
#include <QComboBox>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QStringList>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QVBoxLayout>

#include <exception>
#include <vector>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      tabWidget(new QTabWidget(this)),
      totalAccountsLabel(nullptr),
      activeAccountsLabel(nullptr),
      closedAccountsLabel(nullptr),
      totalBalanceLabel(nullptr),
      totalTransactionsLabel(nullptr),
      firstNameEdit(nullptr),
      lastNameEdit(nullptr),
      accountTypeCombo(nullptr),
      initialBalanceEdit(nullptr),
      searchNameEdit(nullptr),
      sortAccountsCombo(nullptr),
      filterAccountTypeCombo(nullptr),
      minBalanceEdit(nullptr),
      maxBalanceEdit(nullptr),
      transactionAccountFilterEdit(nullptr),
      transactionTypeFilterCombo(nullptr),
      miniStatementAccountEdit(nullptr),
      miniStatementCountEdit(nullptr),
      depositAccountNumberEdit(nullptr),
      depositAmountEdit(nullptr),
      depositNoteEdit(nullptr),
      withdrawAccountNumberEdit(nullptr),
      withdrawAmountEdit(nullptr),
      withdrawNoteEdit(nullptr),
      transferFromAccountEdit(nullptr),
      transferToAccountEdit(nullptr),
      transferAmountEdit(nullptr),
      transferNoteEdit(nullptr),
      closeAccountNumberEdit(nullptr),
      accountsTable(nullptr),
      transactionsTable(nullptr)
{
    setWindowTitle("Banking Management System");
    resize(1000, 650);

    tabWidget->addTab(createDashboardTab(), "Dashboard");
    tabWidget->addTab(createAccountsTab(), "Accounts");
    tabWidget->addTab(createTransactionsTab(), "Transactions");
    tabWidget->addTab(createOperationsTab(), "Operations");
    tabWidget->addTab(createReportsTab(), "Reports");

    setCentralWidget(tabWidget);
    refreshDashboard();
}

QWidget* MainWindow::createDashboardTab()
{
    QWidget* tab = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(tab);

    QLabel* titleLabel = new QLabel("Dashboard Summary", tab);
    titleLabel->setStyleSheet("font-size: 20px; font-weight: bold;");

    totalAccountsLabel = new QLabel(tab);
    activeAccountsLabel = new QLabel(tab);
    closedAccountsLabel = new QLabel(tab);
    totalBalanceLabel = new QLabel(tab);
    totalTransactionsLabel = new QLabel(tab);

    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(totalAccountsLabel);
    mainLayout->addWidget(activeAccountsLabel);
    mainLayout->addWidget(closedAccountsLabel);
    mainLayout->addWidget(totalBalanceLabel);
    mainLayout->addWidget(totalTransactionsLabel);
    mainLayout->addStretch();

    return tab;
}

QWidget* MainWindow::createAccountsTab()
{
    QWidget* tab = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(tab);

    QGroupBox* createAccountGroup = new QGroupBox("Create Account", tab);
    QGridLayout* createLayout = new QGridLayout(createAccountGroup);

    firstNameEdit = new QLineEdit(tab);
    lastNameEdit = new QLineEdit(tab);
    accountTypeCombo = new QComboBox(tab);
    initialBalanceEdit = new QLineEdit(tab);
    QPushButton* createButton = new QPushButton("Create Account", tab);

    accountTypeCombo->addItems(QStringList {"Savings", "Current", "Student"});
    initialBalanceEdit->setPlaceholderText("0.00");

    createLayout->addWidget(new QLabel("First Name:", createAccountGroup), 0, 0);
    createLayout->addWidget(firstNameEdit, 0, 1);
    createLayout->addWidget(new QLabel("Last Name:", createAccountGroup), 0, 2);
    createLayout->addWidget(lastNameEdit, 0, 3);
    createLayout->addWidget(new QLabel("Account Type:", createAccountGroup), 1, 0);
    createLayout->addWidget(accountTypeCombo, 1, 1);
    createLayout->addWidget(new QLabel("Initial Balance:", createAccountGroup), 1, 2);
    createLayout->addWidget(initialBalanceEdit, 1, 3);
    createLayout->addWidget(createButton, 2, 0, 1, 4);

    QGroupBox* accountToolsGroup = new QGroupBox("Search, Sort, and Filter", tab);
    QGridLayout* toolsLayout = new QGridLayout(accountToolsGroup);

    searchNameEdit = new QLineEdit(accountToolsGroup);
    sortAccountsCombo = new QComboBox(accountToolsGroup);
    filterAccountTypeCombo = new QComboBox(accountToolsGroup);
    minBalanceEdit = new QLineEdit(accountToolsGroup);
    maxBalanceEdit = new QLineEdit(accountToolsGroup);
    QPushButton* searchButton = new QPushButton("Search", accountToolsGroup);
    QPushButton* sortButton = new QPushButton("Sort", accountToolsGroup);
    QPushButton* filterTypeButton = new QPushButton("Filter Type", accountToolsGroup);
    QPushButton* filterBalanceButton = new QPushButton("Filter Balance", accountToolsGroup);
    QPushButton* resetButton = new QPushButton("Show All Accounts", accountToolsGroup);

    searchNameEdit->setPlaceholderText("Customer name");
    sortAccountsCombo->addItems(QStringList {"Sort by name", "Sort by balance"});
    filterAccountTypeCombo->addItems(QStringList {"Savings", "Current", "Student"});
    minBalanceEdit->setPlaceholderText("Minimum");
    maxBalanceEdit->setPlaceholderText("Maximum");

    toolsLayout->addWidget(new QLabel("Search Name:", accountToolsGroup), 0, 0);
    toolsLayout->addWidget(searchNameEdit, 0, 1);
    toolsLayout->addWidget(searchButton, 0, 2);
    toolsLayout->addWidget(new QLabel("Sort Accounts:", accountToolsGroup), 1, 0);
    toolsLayout->addWidget(sortAccountsCombo, 1, 1);
    toolsLayout->addWidget(sortButton, 1, 2);
    toolsLayout->addWidget(new QLabel("Account Type:", accountToolsGroup), 2, 0);
    toolsLayout->addWidget(filterAccountTypeCombo, 2, 1);
    toolsLayout->addWidget(filterTypeButton, 2, 2);
    toolsLayout->addWidget(new QLabel("Balance Range:", accountToolsGroup), 3, 0);
    toolsLayout->addWidget(minBalanceEdit, 3, 1);
    toolsLayout->addWidget(maxBalanceEdit, 3, 2);
    toolsLayout->addWidget(filterBalanceButton, 3, 3);
    toolsLayout->addWidget(resetButton, 4, 0, 1, 4);

    accountsTable = new QTableWidget(tab);
    setupAccountTable();

    mainLayout->addWidget(createAccountGroup);
    mainLayout->addWidget(accountToolsGroup);
    mainLayout->addWidget(accountsTable);

    connect(createButton, &QPushButton::clicked, this, [this]() {
        handleCreateAccount();
    });

    connect(searchButton, &QPushButton::clicked, this, [this]() {
        handleSearchAccounts();
    });

    connect(sortButton, &QPushButton::clicked, this, [this]() {
        handleSortAccounts();
    });

    connect(filterTypeButton, &QPushButton::clicked, this, [this]() {
        handleFilterAccountsByType();
    });

    connect(filterBalanceButton, &QPushButton::clicked, this, [this]() {
        handleFilterAccountsByBalanceRange();
    });

    connect(resetButton, &QPushButton::clicked, this, [this]() {
        handleResetAccountsTable();
    });

    return tab;
}

QWidget* MainWindow::createTransactionsTab()
{
    QWidget* tab = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(tab);

    QGroupBox* transactionToolsGroup = new QGroupBox("Transaction History", tab);
    QGridLayout* toolsLayout = new QGridLayout(transactionToolsGroup);

    transactionAccountFilterEdit = new QLineEdit(transactionToolsGroup);
    transactionTypeFilterCombo = new QComboBox(transactionToolsGroup);
    miniStatementAccountEdit = new QLineEdit(transactionToolsGroup);
    miniStatementCountEdit = new QLineEdit(transactionToolsGroup);
    QPushButton* showAllButton = new QPushButton("Show All Transactions", transactionToolsGroup);
    QPushButton* filterAccountButton = new QPushButton("Show Account Transactions", transactionToolsGroup);
    QPushButton* filterTypeButton = new QPushButton("Filter Type", transactionToolsGroup);
    QPushButton* miniStatementButton = new QPushButton("Show Mini Statement", transactionToolsGroup);

    transactionAccountFilterEdit->setPlaceholderText("Account number");
    transactionTypeFilterCombo->addItems(QStringList {"CreateAccount", "Deposit", "Withdraw", "TransferOut", "TransferIn", "CloseAccount", "UpdateAccount"});
    miniStatementAccountEdit->setPlaceholderText("Account number");
    miniStatementCountEdit->setPlaceholderText("Number of latest transactions");

    toolsLayout->addWidget(new QLabel("Account:", transactionToolsGroup), 0, 0);
    toolsLayout->addWidget(transactionAccountFilterEdit, 0, 1);
    toolsLayout->addWidget(filterAccountButton, 0, 2);
    toolsLayout->addWidget(new QLabel("Transaction Type:", transactionToolsGroup), 1, 0);
    toolsLayout->addWidget(transactionTypeFilterCombo, 1, 1);
    toolsLayout->addWidget(filterTypeButton, 1, 2);
    toolsLayout->addWidget(new QLabel("Mini Statement:", transactionToolsGroup), 2, 0);
    toolsLayout->addWidget(miniStatementAccountEdit, 2, 1);
    toolsLayout->addWidget(miniStatementCountEdit, 2, 2);
    toolsLayout->addWidget(miniStatementButton, 2, 3);
    toolsLayout->addWidget(showAllButton, 3, 0, 1, 4);

    transactionsTable = new QTableWidget(tab);
    setupTransactionTable();

    mainLayout->addWidget(transactionToolsGroup);
    mainLayout->addWidget(transactionsTable);

    connect(showAllButton, &QPushButton::clicked, this, [this]() {
        handleShowAllTransactions();
    });

    connect(filterAccountButton, &QPushButton::clicked, this, [this]() {
        handleFilterTransactionsByAccount();
    });

    connect(filterTypeButton, &QPushButton::clicked, this, [this]() {
        handleFilterTransactionsByType();
    });

    connect(miniStatementButton, &QPushButton::clicked, this, [this]() {
        handleMiniStatement();
    });

    return tab;
}

QWidget* MainWindow::createOperationsTab()
{
    QWidget* tab = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(tab);

    QGroupBox* depositGroup = new QGroupBox("Deposit", tab);
    QGridLayout* depositLayout = new QGridLayout(depositGroup);
    depositAccountNumberEdit = new QLineEdit(depositGroup);
    depositAmountEdit = new QLineEdit(depositGroup);
    depositNoteEdit = new QLineEdit(depositGroup);
    QPushButton* depositButton = new QPushButton("Deposit", tab);

    depositAccountNumberEdit->setPlaceholderText("Account number");
    depositAmountEdit->setPlaceholderText("Amount");
    depositNoteEdit->setPlaceholderText("Optional note");

    depositLayout->addWidget(new QLabel("Account Number:", depositGroup), 0, 0);
    depositLayout->addWidget(depositAccountNumberEdit, 0, 1);
    depositLayout->addWidget(new QLabel("Amount:", depositGroup), 1, 0);
    depositLayout->addWidget(depositAmountEdit, 1, 1);
    depositLayout->addWidget(new QLabel("Note:", depositGroup), 2, 0);
    depositLayout->addWidget(depositNoteEdit, 2, 1);
    depositLayout->addWidget(depositButton, 3, 0, 1, 2);

    QGroupBox* withdrawGroup = new QGroupBox("Withdraw", tab);
    QGridLayout* withdrawLayout = new QGridLayout(withdrawGroup);
    withdrawAccountNumberEdit = new QLineEdit(withdrawGroup);
    withdrawAmountEdit = new QLineEdit(withdrawGroup);
    withdrawNoteEdit = new QLineEdit(withdrawGroup);
    QPushButton* withdrawButton = new QPushButton("Withdraw", tab);

    withdrawAccountNumberEdit->setPlaceholderText("Account number");
    withdrawAmountEdit->setPlaceholderText("Amount");
    withdrawNoteEdit->setPlaceholderText("Optional note");

    withdrawLayout->addWidget(new QLabel("Account Number:", withdrawGroup), 0, 0);
    withdrawLayout->addWidget(withdrawAccountNumberEdit, 0, 1);
    withdrawLayout->addWidget(new QLabel("Amount:", withdrawGroup), 1, 0);
    withdrawLayout->addWidget(withdrawAmountEdit, 1, 1);
    withdrawLayout->addWidget(new QLabel("Note:", withdrawGroup), 2, 0);
    withdrawLayout->addWidget(withdrawNoteEdit, 2, 1);
    withdrawLayout->addWidget(withdrawButton, 3, 0, 1, 2);

    QGroupBox* transferGroup = new QGroupBox("Transfer", tab);
    QGridLayout* transferLayout = new QGridLayout(transferGroup);
    transferFromAccountEdit = new QLineEdit(transferGroup);
    transferToAccountEdit = new QLineEdit(transferGroup);
    transferAmountEdit = new QLineEdit(transferGroup);
    transferNoteEdit = new QLineEdit(transferGroup);
    QPushButton* transferButton = new QPushButton("Transfer", tab);

    transferFromAccountEdit->setPlaceholderText("From account");
    transferToAccountEdit->setPlaceholderText("To account");
    transferAmountEdit->setPlaceholderText("Amount");
    transferNoteEdit->setPlaceholderText("Optional note");

    transferLayout->addWidget(new QLabel("From Account:", transferGroup), 0, 0);
    transferLayout->addWidget(transferFromAccountEdit, 0, 1);
    transferLayout->addWidget(new QLabel("To Account:", transferGroup), 1, 0);
    transferLayout->addWidget(transferToAccountEdit, 1, 1);
    transferLayout->addWidget(new QLabel("Amount:", transferGroup), 2, 0);
    transferLayout->addWidget(transferAmountEdit, 2, 1);
    transferLayout->addWidget(new QLabel("Note:", transferGroup), 3, 0);
    transferLayout->addWidget(transferNoteEdit, 3, 1);
    transferLayout->addWidget(transferButton, 4, 0, 1, 2);

    QGroupBox* closeGroup = new QGroupBox("Close Account", tab);
    QGridLayout* closeLayout = new QGridLayout(closeGroup);
    closeAccountNumberEdit = new QLineEdit(closeGroup);
    QPushButton* closeButton = new QPushButton("Close Account", tab);

    closeAccountNumberEdit->setPlaceholderText("Account number");

    closeLayout->addWidget(new QLabel("Account Number:", closeGroup), 0, 0);
    closeLayout->addWidget(closeAccountNumberEdit, 0, 1);
    closeLayout->addWidget(closeButton, 1, 0, 1, 2);

    QHBoxLayout* topRowLayout = new QHBoxLayout();
    topRowLayout->addWidget(depositGroup);
    topRowLayout->addWidget(withdrawGroup);

    QHBoxLayout* bottomRowLayout = new QHBoxLayout();
    bottomRowLayout->addWidget(transferGroup);
    bottomRowLayout->addWidget(closeGroup);

    mainLayout->addLayout(topRowLayout);
    mainLayout->addLayout(bottomRowLayout);
    mainLayout->addStretch();

    connect(depositButton, &QPushButton::clicked, this, [this]() {
        handleDeposit();
    });

    connect(withdrawButton, &QPushButton::clicked, this, [this]() {
        handleWithdraw();
    });

    connect(transferButton, &QPushButton::clicked, this, [this]() {
        handleTransfer();
    });

    connect(closeButton, &QPushButton::clicked, this, [this]() {
        handleCloseAccount();
    });

    return tab;
}

QWidget* MainWindow::createReportsTab()
{
    QWidget* tab = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(tab);
    QGridLayout* buttonLayout = new QGridLayout();

    QPushButton* accountsReportButton = new QPushButton("Export Accounts Report", tab);
    QPushButton* transactionsReportButton = new QPushButton("Export Transactions Report", tab);
    QPushButton* accountsCsvButton = new QPushButton("Export Accounts CSV", tab);
    QPushButton* transactionsCsvButton = new QPushButton("Export Transactions CSV", tab);
    QPushButton* backupButton = new QPushButton("Backup Data Files", tab);

    buttonLayout->addWidget(accountsReportButton, 0, 0);
    buttonLayout->addWidget(transactionsReportButton, 0, 1);
    buttonLayout->addWidget(accountsCsvButton, 1, 0);
    buttonLayout->addWidget(transactionsCsvButton, 1, 1);
    buttonLayout->addWidget(backupButton, 2, 0, 1, 2);

    mainLayout->addWidget(new QLabel("Report and backup actions will be connected in a later step.", tab));
    mainLayout->addLayout(buttonLayout);
    mainLayout->addStretch();

    connect(accountsReportButton, &QPushButton::clicked, this, [this]() {
        showPlaceholderMessage("Export Accounts Report");
    });

    connect(transactionsReportButton, &QPushButton::clicked, this, [this]() {
        showPlaceholderMessage("Export Transactions Report");
    });

    connect(accountsCsvButton, &QPushButton::clicked, this, [this]() {
        showPlaceholderMessage("Export Accounts CSV");
    });

    connect(transactionsCsvButton, &QPushButton::clicked, this, [this]() {
        showPlaceholderMessage("Export Transactions CSV");
    });

    connect(backupButton, &QPushButton::clicked, this, [this]() {
        showPlaceholderMessage("Backup Data Files");
    });

    return tab;
}

void MainWindow::refreshDashboard()
{
    const DashboardSummary summary = bank.getDashboardSummary();

    totalAccountsLabel->setText("Total accounts: " + QString::number(summary.totalAccounts));
    activeAccountsLabel->setText("Active accounts: " + QString::number(summary.activeAccounts));
    closedAccountsLabel->setText("Closed accounts: " + QString::number(summary.closedAccounts));
    totalBalanceLabel->setText("Total bank balance: " + QString::number(summary.totalBankBalance, 'f', 2));
    totalTransactionsLabel->setText("Total transactions: " + QString::number(summary.totalTransactions));
}

void MainWindow::refreshAccountsTable()
{
    if (accountsTable == nullptr) {
        return;
    }

    displayAccounts(bank.getAllAccounts());
}

void MainWindow::displayAccounts(const std::vector<Account>& accounts)
{
    if (accountsTable == nullptr) {
        return;
    }

    accountsTable->setRowCount(static_cast<int>(accounts.size()));

    for (int row = 0; row < static_cast<int>(accounts.size()); ++row) {
        const Account& account = accounts[row];

        accountsTable->setItem(row, 0, new QTableWidgetItem(QString::number(account.getAccountNumber())));
        accountsTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(account.getFullName())));
        accountsTable->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(accountTypeToString(account.getAccountType()))));
        accountsTable->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(accountStatusToString(account.getStatus()))));
        accountsTable->setItem(row, 4, new QTableWidgetItem(QString::number(account.getBalance(), 'f', 2)));
    }
}

void MainWindow::refreshTransactionsTable()
{
    if (transactionsTable == nullptr) {
        return;
    }

    displayTransactions(bank.getAllTransactions());
}

void MainWindow::displayTransactions(const std::vector<Transaction>& transactions)
{
    if (transactionsTable == nullptr) {
        return;
    }

    transactionsTable->setRowCount(static_cast<int>(transactions.size()));

    for (int row = 0; row < static_cast<int>(transactions.size()); ++row) {
        const Transaction& transaction = transactions[row];

        transactionsTable->setItem(row, 0, new QTableWidgetItem(QString::number(transaction.getTransactionId())));
        transactionsTable->setItem(row, 1, new QTableWidgetItem(QString::number(transaction.getAccountNumber())));
        transactionsTable->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(transactionTypeToString(transaction.getType()))));
        transactionsTable->setItem(row, 3, new QTableWidgetItem(QString::number(transaction.getAmount(), 'f', 2)));
        transactionsTable->setItem(row, 4, new QTableWidgetItem(QString::fromStdString(transaction.getNote())));
        transactionsTable->setItem(row, 5, new QTableWidgetItem(QString::fromStdString(transaction.getDateTime())));
    }
}

void MainWindow::handleCreateAccount()
{
    bool balanceIsValid = false;
    const QString balanceText = initialBalanceEdit->text().trimmed();
    const double initialBalance = balanceText.isEmpty() ? 0.0 : balanceText.toDouble(&balanceIsValid);

    if (!balanceText.isEmpty() && !balanceIsValid) {
        QMessageBox::warning(this, "Invalid Input", "Please enter a valid initial balance.");
        return;
    }

    try {
        const AccountType accountType = stringToAccountType(accountTypeCombo->currentText().toStdString());
        const long accountNumber = bank.createAccount(firstNameEdit->text().toStdString(),
                                                      lastNameEdit->text().toStdString(),
                                                      accountType,
                                                      initialBalance);

        firstNameEdit->clear();
        lastNameEdit->clear();
        initialBalanceEdit->clear();
        accountTypeCombo->setCurrentIndex(0);

        refreshAccountsTable();
        refreshTransactionsTable();
        refreshDashboard();

        QMessageBox::information(this,
                                 "Account Created",
                                 "Account created successfully.\nAccount number: "
                                     + QString::number(accountNumber));
    } catch (const std::exception& exception) {
        QMessageBox::critical(this, "Create Account Failed", exception.what());
    }
}

void MainWindow::handleSearchAccounts()
{
    const QString searchText = searchNameEdit->text().trimmed();

    if (searchText.isEmpty()) {
        QMessageBox::warning(this, "Invalid Input", "Please enter a name to search.");
        return;
    }

    try {
        displayAccounts(bank.searchAccountsByName(searchText.toStdString()));
    } catch (const std::exception& exception) {
        QMessageBox::critical(this, "Search Failed", exception.what());
    }
}

void MainWindow::handleSortAccounts()
{
    try {
        if (sortAccountsCombo->currentText() == "Sort by name") {
            displayAccounts(bank.getAccountsSortedByName());
        } else {
            displayAccounts(bank.getAccountsSortedByBalance());
        }
    } catch (const std::exception& exception) {
        QMessageBox::critical(this, "Sort Failed", exception.what());
    }
}

void MainWindow::handleFilterAccountsByType()
{
    try {
        const AccountType accountType = stringToAccountType(filterAccountTypeCombo->currentText().toStdString());
        displayAccounts(bank.filterAccountsByType(accountType));
    } catch (const std::exception& exception) {
        QMessageBox::critical(this, "Filter Failed", exception.what());
    }
}

void MainWindow::handleFilterAccountsByBalanceRange()
{
    double minimumBalance = 0.0;
    double maximumBalance = 0.0;

    if (!readDoubleInput(minBalanceEdit, "minimum balance", minimumBalance)
        || !readDoubleInput(maxBalanceEdit, "maximum balance", maximumBalance)) {
        return;
    }

    try {
        displayAccounts(bank.filterAccountsByBalanceRange(minimumBalance, maximumBalance));
    } catch (const std::exception& exception) {
        QMessageBox::critical(this, "Filter Failed", exception.what());
    }
}

void MainWindow::handleResetAccountsTable()
{
    searchNameEdit->clear();
    minBalanceEdit->clear();
    maxBalanceEdit->clear();
    sortAccountsCombo->setCurrentIndex(0);
    filterAccountTypeCombo->setCurrentIndex(0);
    refreshAccountsTable();
}

void MainWindow::handleShowAllTransactions()
{
    transactionAccountFilterEdit->clear();
    miniStatementAccountEdit->clear();
    miniStatementCountEdit->clear();
    transactionTypeFilterCombo->setCurrentIndex(0);
    refreshTransactionsTable();
}

void MainWindow::handleFilterTransactionsByAccount()
{
    long accountNumber = 0;

    if (!readLongInput(transactionAccountFilterEdit, "account number", accountNumber)) {
        return;
    }

    try {
        displayTransactions(bank.getTransactionsForAccount(accountNumber));
    } catch (const std::exception& exception) {
        QMessageBox::critical(this, "Transaction Filter Failed", exception.what());
    }
}

void MainWindow::handleFilterTransactionsByType()
{
    try {
        const TransactionType transactionType = stringToTransactionType(transactionTypeFilterCombo->currentText().toStdString());
        displayTransactions(bank.getTransactionsByType(transactionType));
    } catch (const std::exception& exception) {
        QMessageBox::critical(this, "Transaction Filter Failed", exception.what());
    }
}

void MainWindow::handleMiniStatement()
{
    long accountNumber = 0;
    long transactionCount = 0;

    if (!readLongInput(miniStatementAccountEdit, "account number", accountNumber)
        || !readLongInput(miniStatementCountEdit, "number of transactions", transactionCount)) {
        return;
    }

    try {
        displayTransactions(bank.getMiniStatement(accountNumber, static_cast<int>(transactionCount)));
    } catch (const std::exception& exception) {
        QMessageBox::critical(this, "Mini Statement Failed", exception.what());
    }
}

void MainWindow::handleDeposit()
{
    long accountNumber = 0;
    double amount = 0.0;

    if (!readLongInput(depositAccountNumberEdit, "account number", accountNumber)
        || !readDoubleInput(depositAmountEdit, "amount", amount)) {
        return;
    }

    try {
        bank.deposit(accountNumber, amount, depositNoteEdit->text().toStdString());
        depositAccountNumberEdit->clear();
        depositAmountEdit->clear();
        depositNoteEdit->clear();
        refreshAfterBankOperation();
        QMessageBox::information(this, "Deposit Successful", "Deposit completed successfully.");
    } catch (const std::exception& exception) {
        QMessageBox::critical(this, "Deposit Failed", exception.what());
    }
}

void MainWindow::handleWithdraw()
{
    long accountNumber = 0;
    double amount = 0.0;

    if (!readLongInput(withdrawAccountNumberEdit, "account number", accountNumber)
        || !readDoubleInput(withdrawAmountEdit, "amount", amount)) {
        return;
    }

    try {
        bank.withdraw(accountNumber, amount, withdrawNoteEdit->text().toStdString());
        withdrawAccountNumberEdit->clear();
        withdrawAmountEdit->clear();
        withdrawNoteEdit->clear();
        refreshAfterBankOperation();
        QMessageBox::information(this, "Withdrawal Successful", "Withdrawal completed successfully.");
    } catch (const std::exception& exception) {
        QMessageBox::critical(this, "Withdrawal Failed", exception.what());
    }
}

void MainWindow::handleTransfer()
{
    long fromAccountNumber = 0;
    long toAccountNumber = 0;
    double amount = 0.0;

    if (!readLongInput(transferFromAccountEdit, "from account number", fromAccountNumber)
        || !readLongInput(transferToAccountEdit, "to account number", toAccountNumber)
        || !readDoubleInput(transferAmountEdit, "amount", amount)) {
        return;
    }

    try {
        bank.transfer(fromAccountNumber, toAccountNumber, amount, transferNoteEdit->text().toStdString());
        transferFromAccountEdit->clear();
        transferToAccountEdit->clear();
        transferAmountEdit->clear();
        transferNoteEdit->clear();
        refreshAfterBankOperation();
        QMessageBox::information(this, "Transfer Successful", "Transfer completed successfully.");
    } catch (const std::exception& exception) {
        QMessageBox::critical(this, "Transfer Failed", exception.what());
    }
}

void MainWindow::handleCloseAccount()
{
    long accountNumber = 0;

    if (!readLongInput(closeAccountNumberEdit, "account number", accountNumber)) {
        return;
    }

    try {
        bank.closeAccount(accountNumber, "Account closed from GUI");
        closeAccountNumberEdit->clear();
        refreshAfterBankOperation();
        QMessageBox::information(this, "Account Closed", "Account closed successfully.");
    } catch (const std::exception& exception) {
        QMessageBox::critical(this, "Close Account Failed", exception.what());
    }
}

void MainWindow::setupAccountTable()
{
    accountsTable->setColumnCount(5);
    accountsTable->setHorizontalHeaderLabels(QStringList {"Account No.", "Full Name", "Type", "Status", "Balance"});
    accountsTable->horizontalHeader()->setStretchLastSection(true);
    accountsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    accountsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    refreshAccountsTable();
}

void MainWindow::setupTransactionTable()
{
    transactionsTable->setColumnCount(6);
    transactionsTable->setHorizontalHeaderLabels(QStringList {"ID", "Account No.", "Type", "Amount", "Note", "Date/Time"});
    transactionsTable->horizontalHeader()->setStretchLastSection(true);
    transactionsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    transactionsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    refreshTransactionsTable();
}

void MainWindow::refreshAfterBankOperation()
{
    refreshAccountsTable();
    refreshTransactionsTable();
    refreshDashboard();
}

bool MainWindow::readLongInput(QLineEdit* input, const QString& fieldName, long& value)
{
    bool isValid = false;
    value = input->text().trimmed().toLong(&isValid);

    if (!isValid) {
        QMessageBox::warning(this, "Invalid Input", QString("Please enter a valid ") + fieldName + ".");
        return false;
    }

    return true;
}

bool MainWindow::readDoubleInput(QLineEdit* input, const QString& fieldName, double& value)
{
    bool isValid = false;
    value = input->text().trimmed().toDouble(&isValid);

    if (!isValid) {
        QMessageBox::warning(this, "Invalid Input", QString("Please enter a valid ") + fieldName + ".");
        return false;
    }

    return true;
}

void MainWindow::showPlaceholderMessage(const QString& featureName)
{
    QMessageBox::information(this,
                             "Feature Not Connected Yet",
                             featureName + " UI is ready. Logic will be connected in a later step.");
}
