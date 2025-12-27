
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "types.h"
#include "config.h"
#include "storage.h"
#include "trans.h"

void withdraw(Account *acc, double amount)
{
    if (strcmp(acc->status, "inactive") == 0)
    {
        printf("Warning: Account is inactive. Transaction cannot be completed.\n");
        return;
    }

    if (amount > TRANSACTION_LIMIT)
    {
        printf("Error: Maximum withdrawal per transaction is %.2f\n", (double)TRANSACTION_LIMIT);
        return;
    }

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    int day   = tm.tm_mday;
    int month = tm.tm_mon + 1;
    int year  = tm.tm_year + 1900;

    if (acc->lastDay != day || acc->lastMonth != month || acc->lastYear != year)
    {
        acc->dailyWithdrawn = 0;
        acc->lastDay   = day;
        acc->lastMonth = month;
        acc->lastYear  = year;
    }

    if (acc->dailyWithdrawn + amount > DAILY_LIMIT)
    {
        printf("Error: Daily withdrawal limit of %.2f exceeded!\n", (double)DAILY_LIMIT);
        return;
    }

    if (acc->balance < amount)
    {
        printf("Error: Insufficient balance.\n");
        return;
    }

    acc->balance -= amount;
    acc->dailyWithdrawn += amount;
    printf("Withdrawal successful! New balance for %s account is: %.2f\n", acc->name, acc->balance);

    char filename[30];
    strcpy(filename, acc->accountNumber);
    strcat(filename, ".txt");
    FILE *fp = fopen(filename, "a");
    if (fp)
    {
        fprintf(fp, "Withdraw %.2f | New Balance: %.2f | Date: %02d-%02d-%d\n",
                amount, acc->balance, day, month, year);
        fclose(fp);
    }
}

void deposit(Account *acc, double amount)
{
    if (strcmp(acc->status, "inactive") == 0)
    {
        printf("Warning: Account is inactive. Transaction cannot be completed.\n");
        return;
    }

    if (amount > DEPOSIT_LIMIT)
    {
        printf("Error: Maximum deposit per transaction is %.2f\n", (double)DEPOSIT_LIMIT);
        return;
    }

    acc->balance += amount;
    printf("Deposit successful! New balance for %s account is: %.2f\n", acc->name, acc->balance);
    char filename[30];
    strcpy(filename, acc->accountNumber);
    strcat(filename, ".txt");
    FILE *log = fopen(filename, "a");
    if (log)
    {
        fprintf(log, "Deposit %.2f | New Balance: %.2f\n", amount, acc->balance);
        fclose(log);
    }
}

void transfer(Account accounts[], int total)
{
    char senderAcc[20], receiverAcc[20];
    double amount;
    printf("Enter sender account number: ");
    scanf("%19s", senderAcc);

    printf("Enter receiver account number: ");
    scanf("%19s", receiverAcc);

    int senderIndex = -1, receiverIndex = -1;
    for (int i = 0; i < total; i++)
    {
        if (strcmp(accounts[i].accountNumber, senderAcc) == 0)
        {
            senderIndex = i;
            break;
        }
    }
    for (int i = 0; i < total; i++)
    {
        if (strcmp(accounts[i].accountNumber, receiverAcc) == 0)
        {
            receiverIndex = i;
            break;
        }
    }

    if (senderIndex == -1)
    {
        printf("Error: Sender account not found.\n");
        return;
    }
    if (receiverIndex == -1)
    {
        printf("Error: Receiver account not found.\n");
        return;
    }

    if (senderIndex == receiverIndex)
    {
        printf("Error: Cannot transfer to the same account.\n");
        return;
    }

    if (strcmp(accounts[senderIndex].status, "inactive") == 0 ||
        strcmp(accounts[receiverIndex].status, "inactive") == 0)
    {
        printf("Warning: One or both accounts are inactive. Transfer cannot be completed.\n");
        return;
    }

    printf("Enter transfer amount: ");
    scanf("%lf", &amount);

    if (amount <= 0)
    {
        printf("Error: Invalid transfer amount.\n");
        return;
    }
    if (accounts[senderIndex].balance < amount)
    {
        printf("Error: Insufficient balance in sender account.\n");
        return;
    }

    accounts[senderIndex].balance -= amount;
    accounts[receiverIndex].balance += amount;

    printf("Transfer successful!\n");
    printf("New balance of sender (%s): %.2f\n", accounts[senderIndex].name, accounts[senderIndex].balance);
    printf("New balance of receiver (%s): %.2f\n", accounts[receiverIndex].name, accounts[receiverIndex].balance);

    FILE *fp = fopen("accounts.txt", "w");
    if (!fp)
    {
        printf("Error updating file.\n");
        return;
    }

    for (int i = 0; i < total; i++)
    {
        fprintf(fp, "%s,%s,%s,%.2lf,%s,%02d-%d,%s\n",
                accounts[i].accountNumber,
                accounts[i].name,
                accounts[i].address,
                accounts[i].balance,
                accounts[i].mobile,
                accounts[i].dob.month,
                accounts[i].dob.year,
                accounts[i].status);
    }

    fclose(fp);

    char senderFile[30];
    strcpy(senderFile, accounts[senderIndex].accountNumber);
    strcat(senderFile, ".txt");
    FILE *logSender = fopen(senderFile, "a");
    if (logSender)
    {
        fprintf(logSender, "Transfer Out %.2f to %s | New Balance: %.2f\n",
                amount, accounts[receiverIndex].accountNumber, accounts[senderIndex].balance);
        fclose(logSender);
    }
    char receiverFile[30];
    strcpy(receiverFile, accounts[receiverIndex].accountNumber);
    strcat(receiverFile, ".txt");
    FILE *logReceiver = fopen(receiverFile, "a");
    if (logReceiver)
    {
        fprintf(logReceiver, "Transfer In %.2f from %s | New Balance: %.2f\n",
                amount, accounts[senderIndex].accountNumber, accounts[receiverIndex].balance);
        fclose(logReceiver);
    }
    int choice;
    printf("Do you want to save changes? (1 = Yes, 2 = No): ");
    scanf("%d", &choice);
    if (choice == 1)
    {
        saveAccounts(accounts, total);
        printf("Changes confirmed and saved.\n");
    }
    else
    {
        printf("Changes discarded (not saved).\n");
    }
}
