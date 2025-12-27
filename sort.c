
#include <stdio.h>
#include <string.h>
#include "types.h"
#include "sort.h"

void displayAccount(Account acc)
{
    printf("Account Number: %s\n", acc.accountNumber);
    printf("Name: %s\n", acc.name);
    printf("Address: %s\n", acc.address);
    printf("Balance: %.2lf\n", acc.balance);
    printf("Mobile: %s\n", acc.mobile);
    printf("Date of Birth: %s %d\n", getMonthName(acc.dob.month), acc.dob.year);
    printf("Status: %s\n\n", acc.status);
}

void sortByName(Account accounts[], int total)
{
    for (int i = 0; i < total - 1; i++)
    {
        for (int j = 0; j < total - i - 1; j++)
        {
            if (strcmp(accounts[j].name, accounts[j+1].name) > 0)
            {
                Account temp = accounts[j];
                accounts[j] = accounts[j+1];
                accounts[j+1] = temp;
            }
        }
    }
}

void sortByBalance(Account accounts[], int total)
{
    for (int i = 0; i < total - 1; i++)
    {
        for (int j = 0; j < total - i - 1; j++)
        {
            if (accounts[j].balance > accounts[j+1].balance)
            {
                Account temp = accounts[j];
                accounts[j] = accounts[j+1];
                accounts[j+1] = temp;
            }
        }
    }
}

void sortByDate(Account accounts[], int total)
{
    for (int i = 0; i < total - 1; i++)
    {
        for (int j = 0; j < total - i - 1; j++)
        {
            if (accounts[j].dob.year > accounts[j+1].dob.year ||
                    (accounts[j].dob.year == accounts[j+1].dob.year &&
                     accounts[j].dob.month > accounts[j+1].dob.month))
            {
                Account temp = accounts[j];
                accounts[j] = accounts[j+1];
                accounts[j+1] = temp;
            }
        }
    }
}

void sortByStatus(Account accounts[], int total)
{
    for (int i = 0; i < total - 1; i++)
    {
        for (int j = 0; j < total - i - 1; j++)
        {
            if (strcmp(accounts[j].status, accounts[j+1].status) > 0)
            {
                Account temp = accounts[j];
                accounts[j] = accounts[j+1];
                accounts[j+1] = temp;
            }
        }
    }
}

void print(Account accounts[], int total)
{
    int choice;
    printf("Sort by:\n");
    printf("1. Name\n");
    printf("2. Balance\n");
    printf("3. Date Opened\n");
    printf("4. Status\n");
    printf("Enter choice: ");
    scanf("%d", &choice);

    if      (choice == 1) sortByName(accounts, total);
    else if (choice == 2) sortByBalance(accounts, total);
    else if (choice == 3) sortByDate(accounts, total);
    else if (choice == 4) sortByStatus(accounts, total);
    else
    {
        printf("Invalid choice.\n");
        return;
    }

    printf("\n--- Sorted Accounts ---\n");
    for (int i = 0; i < total; i++)
    {
        displayAccount(accounts[i]);
    }
}
