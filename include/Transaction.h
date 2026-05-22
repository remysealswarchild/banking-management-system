#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>

// Supported transaction categories in the banking system.
enum class TransactionType {
    CreateAccount,
    Deposit,
    Withdraw,
    TransferOut,
    TransferIn,
    CloseAccount,
    UpdateAccount
};

// Convert transaction enum values to readable text for files, reports, and the GUI.
std::string transactionTypeToString(TransactionType type);

// Convert text from files or GUI controls back into a transaction enum value.
TransactionType stringToTransactionType(const std::string& typeText);

// Return the current local date and time using the format YYYY-MM-DD HH:MM:SS.
std::string getCurrentDateTime();

// Represents one banking transaction record.
class Transaction {
private:
    long transactionId;
    long accountNumber;
    TransactionType type;
    double amount;
    std::string note;
    std::string dateTime;

public:
    Transaction();
    Transaction(long transactionId,
                long accountNumber,
                TransactionType type,
                double amount,
                const std::string& note,
                const std::string& dateTime = getCurrentDateTime());

    long getTransactionId() const;
    long getAccountNumber() const;
    TransactionType getType() const;
    double getAmount() const;
    std::string getNote() const;
    std::string getDateTime() const;
};

#endif // TRANSACTION_H
