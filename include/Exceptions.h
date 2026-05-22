#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <stdexcept>
#include <string>

// Base exception for all banking system errors.
class BankException : public std::runtime_error {
public:
    explicit BankException(const std::string& message)
        : std::runtime_error(message)
    {
    }
};

// Thrown when an account number cannot be found.
class AccountNotFoundException : public BankException {
public:
    explicit AccountNotFoundException(const std::string& message)
        : BankException(message)
    {
    }
};

// Thrown when trying to create an account that already exists.
class DuplicateAccountException : public BankException {
public:
    explicit DuplicateAccountException(const std::string& message)
        : BankException(message)
    {
    }
};

// Thrown when a money amount is zero, negative, or otherwise invalid.
class InvalidAmountException : public BankException {
public:
    explicit InvalidAmountException(const std::string& message)
        : BankException(message)
    {
    }
};

// Thrown when an account does not have enough balance for an operation.
class InsufficientFundsException : public BankException {
public:
    explicit InsufficientFundsException(const std::string& message)
        : BankException(message)
    {
    }
};

// Thrown when an operation is attempted on a closed account.
class AccountClosedException : public BankException {
public:
    explicit AccountClosedException(const std::string& message)
        : BankException(message)
    {
    }
};

// Thrown when a transfer request is not valid.
class InvalidTransferException : public BankException {
public:
    explicit InvalidTransferException(const std::string& message)
        : BankException(message)
    {
    }
};

// Thrown when reading from or writing to a file fails.
class FileOperationException : public BankException {
public:
    explicit FileOperationException(const std::string& message)
        : BankException(message)
    {
    }
};

#endif // EXCEPTIONS_H
