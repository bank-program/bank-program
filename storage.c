
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "types.h"
#include "storage.h"

int loadAccounts(Account accounts[])
{
    FILE *fp = fopen("accounts.txt", "r");
    if (!fp) return 0;

    int total = 0;
    char line[300];

    while (fgets(line, sizeof(line), fp) != NULL)
    {
        int len = (int)strlen(line);
        if (len > 0 && line[len - 1] == '\n')
        {
            line[len - 1] = '\0';
        }

        char *token;
        token = strtok(line, ",");
        if (!token) continue;
        strcpy(accounts[total].accountNumber, token);

        token = strtok(NULL, ",");
        strcpy(accounts[total].name, token ? token : "");

        token = strtok(NULL, ",");
        strcpy(accounts[total].address, token ? token : "");

        token = strtok(NULL, ",");
        accounts[total].balance = token ? atof(token) : 0.0;

        token = strtok(NULL, ",");
        strcpy(accounts[total].mobile, token ? token : "");

        token = strtok(NULL, ",");
        if (token) sscanf(token, "%d-%d", &accounts[total].dob.month, &accounts[total].dob.year);

        token = strtok(NULL, ",");
        strcpy(accounts[total].status, token ? token : "active");

        // initialize tracking fields to safe values
        accounts[total].dailyWithdrawn = 0;
        accounts[total].lastDay   = 0;
        accounts[total].lastMonth = 0;
        accounts[total].lastYear  = 0;

        total++;
        if (total >= 1000) break; // safety cap
    }

    fclose(fp);
    return total;
}

void saveAccounts(Account accounts[], int total)
{
    FILE *fp = fopen("accounts.txt", "w");
    if (!fp)
    {
        printf("Error: Could not save accounts to file.\n");
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
    printf("Accounts saved successfully!\n");
}

void report()
{
    char accNum[20];
    printf("Enter account number: ");
    scanf("%19s", accNum);

    char filename[30];
    strcpy(filename, accNum);
    strcat(filename, ".txt");

    FILE *fp = fopen(filename, "r");
    if (!fp)
    {
        printf("Error: No transaction history found for account %s.\n", accNum);
        return;
    }

    char lines[1000][200];
    int count = 0;

    while (count < 1000 && fgets(lines[count], sizeof(lines[count]), fp) != NULL)
    {
        int len = (int)strlen(lines[count]);
        if (len > 0 && lines[count][len - 1] == '\n')
        {
            lines[count][--len] = '\0';
        }
        if (len > 0 && lines[count][len - 1] == '\r')
        {
            lines[count][--len] = '\0';
        }
        count++;
    }
    fclose(fp);

    if (count == 0)
    {
        printf("No transactions recorded for this account.\n");
        return;
    }

    int start  = (count >= 5 ? count - 5 : 0);
    int toShow = (count < 5 ? count : 5);
    printf("Last %d transaction(s) for account %s:\n", toShow, accNum);
    for (int i = start; i < count; i++)
    {
        printf("%s\n", lines[i]);
    }
}
