
#include <stdio.h>
#include <string.h>
#include "types.h"
#include "validate.h"
#include "search.h"

void queryAccount(Account accounts[], int total)
{
    char searchNumber[20];
    printf("Enter Account Number to search: ");
    scanf("%19s", searchNumber);

    int found = 0;
    for (int i = 0; i < total; i++)
    {
        if (strcmp(accounts[i].accountNumber, searchNumber) == 0)
        {
            printf("Account Number: %s\n", accounts[i].accountNumber);
            printf("Name: %s\n", accounts[i].name);
            printf("Address: %s\n", accounts[i].address);
            printf("Balance: %.2lf\n", accounts[i].balance);
            printf("Mobile: %s\n", accounts[i].mobile);
            printf("Date of Birth: %s %d\n", getMonthName(accounts[i].dob.month), accounts[i].dob.year);
            printf("Status: %s\n", accounts[i].status);
            found = 1;
            break;
        }
    }

    if (!found)
    {
        printf("Account not found.\n");
    }
}

void advancedSearch(Account accounts[], int total)
{
    char keyword[50];
    printf("Enter keyword: ");
    scanf("%49s", keyword);
    toLower(keyword);

    int found = 0;
    for (int i = 0; i < total; i++)
    {
        char temp[100];
        strcpy(temp, accounts[i].name);
        toLower(temp);

        if (strstr(temp, keyword))
        {
            printf("Account Number: %s\n", accounts[i].accountNumber);
            printf("Name: %s\n", accounts[i].name);
            printf("Address: %s\n", accounts[i].address);
            printf("Balance: %.2lf\n", accounts[i].balance);
            printf("Mobile: %s\n", accounts[i].mobile);
            printf("Date of Birth: %s %d\n", getMonthName(accounts[i].dob.month), accounts[i].dob.year);
            printf("Status: %s\n\n", accounts[i].status);
            found = 1;
        }
    }
    if (!found) printf("No matches found.\n");
}
