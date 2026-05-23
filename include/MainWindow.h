#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include "Bank.h"

#include <QMainWindow>
#include <QString>

#include <vector>

class QLabel;
class QComboBox;
class QLineEdit;
class QTableWidget;
class QTabWidget;
class QWidget;

// Main Qt window for the Banking Management System.
class MainWindow : public QMainWindow {
public:
    explicit MainWindow(QWidget* parent = nullptr);

private:
    Bank bank;

    QTabWidget* tabWidget;
    QLabel* totalAccountsLabel;
    QLabel* activeAccountsLabel;
    QLabel* closedAccountsLabel;
    QLabel* totalBalanceLabel;
    QLabel* totalTransactionsLabel;
    QLineEdit* firstNameEdit;
    QLineEdit* lastNameEdit;
    QComboBox* accountTypeCombo;
    QLineEdit* initialBalanceEdit;
    QLineEdit* searchNameEdit;
    QComboBox* sortAccountsCombo;
    QComboBox* filterAccountTypeCombo;
    QLineEdit* minBalanceEdit;
    QLineEdit* maxBalanceEdit;
    QLineEdit* transactionAccountFilterEdit;
    QComboBox* transactionTypeFilterCombo;
    QLineEdit* miniStatementAccountEdit;
    QLineEdit* miniStatementCountEdit;
    QLineEdit* depositAccountNumberEdit;
    QLineEdit* depositAmountEdit;
    QLineEdit* depositNoteEdit;
    QLineEdit* withdrawAccountNumberEdit;
    QLineEdit* withdrawAmountEdit;
    QLineEdit* withdrawNoteEdit;
    QLineEdit* transferFromAccountEdit;
    QLineEdit* transferToAccountEdit;
    QLineEdit* transferAmountEdit;
    QLineEdit* transferNoteEdit;
    QLineEdit* closeAccountNumberEdit;
    QTableWidget* accountsTable;
    QTableWidget* transactionsTable;

    QWidget* createDashboardTab();
    QWidget* createAccountsTab();
    QWidget* createTransactionsTab();
    QWidget* createOperationsTab();
    QWidget* createReportsTab();

    void refreshDashboard();
    void refreshAccountsTable();
    void refreshTransactionsTable();
    void displayAccounts(const std::vector<Account>& accounts);
    void displayTransactions(const std::vector<Transaction>& transactions);
    void handleCreateAccount();
    void handleSearchAccounts();
    void handleSortAccounts();
    void handleFilterAccountsByType();
    void handleFilterAccountsByBalanceRange();
    void handleResetAccountsTable();
    void handleShowAllTransactions();
    void handleFilterTransactionsByAccount();
    void handleFilterTransactionsByType();
    void handleMiniStatement();
    void handleDeposit();
    void handleWithdraw();
    void handleTransfer();
    void handleCloseAccount();
    void setupAccountTable();
    void setupTransactionTable();
    void refreshAfterBankOperation();
    bool readLongInput(QLineEdit* input, const QString& fieldName, long& value);
    bool readDoubleInput(QLineEdit* input, const QString& fieldName, double& value);
    void showPlaceholderMessage(const QString& featureName);
};

#endif // MAIN_WINDOW_H
