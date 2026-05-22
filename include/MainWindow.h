#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include "Bank.h"

#include <QMainWindow>
#include <QString>

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
    void handleCreateAccount();
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
