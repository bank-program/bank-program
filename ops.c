
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "types.h"
#include "validate.h"
#include "storage.h"
#include "ops.h"

void addAccount(Account accounts[], int *total)
{
    Account newAcc;

    printf("Enter Account Number: ");
    scanf("%19s", newAcc.accountNumber);
    if (!validateAccountNumber(newAcc.accountNumber))
    {
        printf("Error: Invalid account number! Must be exactly 10 digits.\n");
        return;
    }

    for (int i = 0; i < *total; i++)
    {
        if (strcmp(accounts[i].accountNumber, newAcc.accountNumber) == 0)
        {
            printf("Error: Account number already exists!\n");
            return;
        }
    }

    printf("Enter Name: ");
    scanf(" %99[^\n]", newAcc.name);

    printf("Enter Address (email): ");
    scanf("%99s", newAcc.address);
    if (!validateEmail(newAcc.address))
    {
        printf("Error: Invalid email format!\n");
        return;
    }

    printf("Enter Balance: ");
    scanf("%lf", &newAcc.balance);
    if (!validatePositive(newAcc.balance))
    {
        printf("Error: Balance must be non-negative.\n");
        return;
    }

    printf("Enter Mobile: ");
    scanf("%19s", newAcc.mobile);
    if (!validateMobile(newAcc.mobile))
    {
        printf("Error: Invalid mobile number! Must be 11 digits.\n");
        return;
    }

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    newAcc.dob.month = tm.tm_mon + 1;
    newAcc.dob.year  = tm.tm_year + 1900;

    strcpy(newAcc.status, "active");
    newAcc.dailyWithdrawn = 0;
    newAcc.lastDay   = tm.tm_mday;
    newAcc.lastMonth = tm.tm_mon + 1;
    newAcc.lastYear  = tm.tm_year + 1900;

    accounts[*total] = newAcc;
    (*total)++;

    FILE *fp = fopen("accounts.txt", "a");
    if (fp)
    {
        fprintf(fp, "%s,%s,%s,%.2lf,%s,%02d-%d,%s\n",
                newAcc.accountNumber,
                newAcc.name,
                newAcc.address,
                newAcc.balance,
                newAcc.mobile,
                newAcc.dob.month,
                newAcc.dob.year,
                newAcc.status);
        fclose(fp);
        printf("Account added successfully!\n");
    }
    else
    {
        printf("Error: Could not update accounts.txt\n");
    }

    char histFile[30];
    strcpy(histFile, newAcc.accountNumber);
    strcat(histFile, ".txt");
    FILE *hf = fopen(histFile, "a");
    if (hf)
    {
        fprintf(hf, "Account created | Initial Balance: %.2f | Date: %02d-%d\n",
                newAcc.balance,
                newAcc.dob.month,
                newAcc.dob.year);
        fclose(hf);
    }

    int choice;
    printf("Do you want to save changes? (1 = Yes, 2 = No): ");
    scanf("%d", &choice);
    if (choice == 1)
    {
        saveAccounts(accounts, *total);
        printf("Changes confirmed and saved.\n");
    }
    else
    {
        printf("Changes discarded (not saved).\n");
    }
}

void deleteAccount(Account accounts[], int *total)
{
    char accNum[20];
    printf("Enter account number to delete: ");
    scanf("%19s", accNum);

    int index = -1;

    for (int i = 0; i < *total; i++)
    {
        if (strcmp(accounts[i].accountNumber, accNum) == 0)
        {
            index = i;
            break;
        }
    }

    if (index == -1)
    {
        printf("Error: Account number does not exist.\n");
        return;
    }

    if (accounts[index].balance > 0)
    {
        printf("Deletion rejected: Account balance is not zero.\n");
        return;
    }

    for (int i = index; i < *total - 1; i++)
    {
        accounts[i] = accounts[i + 1];
    }
    (*total)--;

    FILE *fp = fopen("accounts.txt", "w");
    if (!fp)
    {
        printf("Error updating file.\n");
        return;
    }

    for (int i = 0; i < *total; i++)
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
    printf("Account deleted successfully.\n");
    int choice;
    printf("Do you want to save changes? (1 = Yes, 2 = No): ");
    scanf("%d", &choice);
    if (choice == 1)
    {
        saveAccounts(accounts, *total);
        printf("Changes confirmed and saved.\n");
    }
    else
    {
        printf("Changes discarded (not saved).\n");
    }

}

void modifyAccount(Account accounts[], int total)
{
    char accNum[20];
    printf("Enter account number to modify: ");
    scanf("%19s", accNum);

    int index = -1;
    for (int i = 0; i < total; i++)
    {
        if (strcmp(accounts[i].accountNumber, accNum) == 0)
        {
            index = i;
            break;
        }
    }

    if (index == -1)
    {
        printf("Error: Account number does not exist.\n");
        return;
    }

    printf("\nAccount found. Enter new details:\n");

    printf("Enter new name (current: %s): ", accounts[index].name);
    scanf(" %99[^\n]", accounts[index].name);

    do
    {
        printf("Enter new email (current: %s): ", accounts[index].address);
        scanf("%99s", accounts[index].address);
        if (!validateEmail(accounts[index].address))
        {
            printf("Error: Invalid email format! Try again.\n");
        }
    }
    while (!validateEmail(accounts[index].address));

    do
    {
        printf("Enter new mobile (current: %s): ", accounts[index].mobile);
        scanf("%19s", accounts[index].mobile);
        if (!validateMobile(accounts[index].mobile))
        {
            printf("Error: Invalid mobile number! Must be 11 digits. Try again.\n");
        }
    }
    while (!validateMobile(accounts[index].mobile));

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
    printf("Account updated successfully.\n");

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

void changeStatus(Account accounts[], int total)
{
    char accNum[20];
    printf("Enter account number: ");
    scanf("%19s", accNum);

    int index = -1;

    for (int i = 0; i < total; i++)
    {
        if (strcmp(accounts[i].accountNumber, accNum) == 0)
        {
            index = i;
            break;
        }
    }

    if (index == -1)
    {
        printf("Error: Account does not exist.\n");
        return;
    }

    printf("Current account status: %s\n", accounts[index].status);
    printf("Enter new status (active / inactive): ");

    char newStatus[20];
    scanf("%19s", newStatus);
    toLower(newStatus);

    if (strcmp(accounts[index].status, newStatus) == 0)
    {
        printf("Attention: Account is already %s.!\n", newStatus);
        return;
    }

    if (strcmp(newStatus, "active") != 0 && strcmp(newStatus, "inactive") != 0)
    {
        printf("Error: Invalid status.\n");
        return;
    }

    strcpy(accounts[index].status, newStatus);

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
    printf("Account status updated successfully.\n");
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

void deleteMultiple(Account accounts[], int *total)
{
    int option;
    printf("\n--- Delete Multiple Accounts ---\n");
    printf("1. By Date (YYYY-MM)\n");
    printf("2. Inactive Accounts (>90 days, balance = 0)\n");
    printf("Enter choice: ");
    scanf("%d", &option);

    int deletedCount = 0;

    if (option == 1) {
        int year, month;
        printf("Enter date (YYYY-MM): ");
        scanf("%d-%d", &year, &month);

        for (int i = 0; i < *total; ) {
            if (accounts[i].dob.year == year &&
                accounts[i].dob.month == month &&
                accounts[i].balance == 0)
            {
                for (int j = i; j < *total - 1; j++)
                    accounts[j] = accounts[j + 1];
                (*total)--;
                deletedCount++;
                continue;
            }
            i++;
        }

        if (deletedCount == 0)
            printf("No accounts found for given month/year with zero balance.\n");
        else
            printf("Deletion complete. %d account(s) deleted.\n", deletedCount);
    }
    else if (option == 2) {
        time_t t = time(NULL);
        struct tm tm = *localtime(&t);

        for (int i = 0; i < *total; ) {
            if (strcmp(accounts[i].status, "inactive") == 0 &&
                accounts[i].balance == 0)
            {
                int diffMonths = (tm.tm_year + 1900 - accounts[i].lastYear) * 12 +
                                 (tm.tm_mon + 1 - accounts[i].lastMonth);

                if (diffMonths > 3) {  // ~90 days
                    for (int j = i; j < *total - 1; j++)
                        accounts[j] = accounts[j + 1];
                    (*total)--;
                    deletedCount++;
                    continue;
                }
            }
            i++;
        }

        if (deletedCount == 0)
            printf("No inactive accounts eligible for deletion.\n");
        else
            printf("Deletion complete. %d account(s) deleted.\n", deletedCount);
    }
    else {
        printf("Invalid choice.\n");
        return;
    }

    if (deletedCount > 0) {
        saveAccounts(accounts, *total);
    }
}
