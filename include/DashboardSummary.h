#ifndef DASHBOARD_SUMMARY_H
#define DASHBOARD_SUMMARY_H

#include <string>

// Holds calculated dashboard values for the banking system.
struct DashboardSummary {
    int totalAccounts = 0;
    int activeAccounts = 0;
    int closedAccounts = 0;
    double totalBankBalance = 0.0;
    int totalTransactions = 0;
    long highestBalanceAccountNumber = 0;
    std::string highestBalanceAccountName = "";
    double highestBalance = 0.0;
};

#endif // DASHBOARD_SUMMARY_H
