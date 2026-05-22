#include "Transaction.h"
#include "Exceptions.h"

#include <algorithm>
#include <cctype>
#include <ctime>
#include <iomanip>
#include <sstream>

namespace {

std::string normalizeText(std::string text)
{
    text.erase(std::remove_if(text.begin(), text.end(), [](unsigned char ch) {
                   return std::isspace(ch) || ch == '_' || ch == '-';
               }),
               text.end());

    std::transform(text.begin(), text.end(), text.begin(), [](unsigned char ch) {
        return static_cast<char>(std::tolower(ch));
    });

    return text;
}

std::tm getLocalTime(std::time_t currentTime)
{
    std::tm localTime {};

#if defined(_WIN32)
    localtime_s(&localTime, &currentTime);
#else
    localtime_r(&currentTime, &localTime);
#endif

    return localTime;
}

} // namespace

std::string transactionTypeToString(TransactionType type)
{
    switch (type) {
    case TransactionType::CreateAccount:
        return "CreateAccount";
    case TransactionType::Deposit:
        return "Deposit";
    case TransactionType::Withdraw:
        return "Withdraw";
    case TransactionType::TransferOut:
        return "TransferOut";
    case TransactionType::TransferIn:
        return "TransferIn";
    case TransactionType::CloseAccount:
        return "CloseAccount";
    case TransactionType::UpdateAccount:
        return "UpdateAccount";
    }

    throw BankException("Unknown transaction type.");
}

TransactionType stringToTransactionType(const std::string& typeText)
{
    const std::string value = normalizeText(typeText);

    if (value == "createaccount") {
        return TransactionType::CreateAccount;
    }

    if (value == "deposit") {
        return TransactionType::Deposit;
    }

    if (value == "withdraw" || value == "withdrawal") {
        return TransactionType::Withdraw;
    }

    if (value == "transferout") {
        return TransactionType::TransferOut;
    }

    if (value == "transferin") {
        return TransactionType::TransferIn;
    }

    if (value == "closeaccount") {
        return TransactionType::CloseAccount;
    }

    if (value == "updateaccount") {
        return TransactionType::UpdateAccount;
    }

    throw BankException("Invalid transaction type: " + typeText);
}

std::string getCurrentDateTime()
{
    const std::time_t currentTime = std::time(nullptr);
    const std::tm localTime = getLocalTime(currentTime);

    std::ostringstream output;
    output << std::put_time(&localTime, "%Y-%m-%d %H:%M:%S");

    return output.str();
}

Transaction::Transaction()
    : transactionId(0),
      accountNumber(0),
      type(TransactionType::Deposit),
      amount(0.0),
      note(""),
      dateTime("")
{
}

Transaction::Transaction(long transactionId,
                         long accountNumber,
                         TransactionType type,
                         double amount,
                         const std::string& note,
                         const std::string& dateTime)
    : transactionId(transactionId),
      accountNumber(accountNumber),
      type(type),
      amount(amount),
      note(note),
      dateTime(dateTime)
{
}

long Transaction::getTransactionId() const
{
    return transactionId;
}

long Transaction::getAccountNumber() const
{
    return accountNumber;
}

TransactionType Transaction::getType() const
{
    return type;
}

double Transaction::getAmount() const
{
    return amount;
}

std::string Transaction::getNote() const
{
    return note;
}

std::string Transaction::getDateTime() const
{
    return dateTime;
}
