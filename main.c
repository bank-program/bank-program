#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ACCOUNTS 1000

typedef struct {
    int month;
    int year;
} Date;

typedef struct {
    int accountNumber;
    char name[100];
    char address[100];
    double balance;
    char mobile[20];
    Date dob;
    char status[20];
} Account;

int loadAccounts(Account accounts[]) {
    FILE *fp = fopen("account.txt", "r");
    if (!fp) return 0;

    int total = 0;
    char line[300];

    while (fgets(line, sizeof(line), fp) != NULL) {
        int len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }

        char *token;
        token = strtok(line, ",");
        accounts[total].accountNumber = atoi(token);

        token = strtok(NULL, ",");
        strcpy(accounts[total].name, token);

        token = strtok(NULL, ",");
        strcpy(accounts[total].address, token);

        token = strtok(NULL, ",");
        accounts[total].balance = atof(token);

        token = strtok(NULL, ",");
        strcpy(accounts[total].mobile, token);

        token = strtok(NULL, ",");
        sscanf(token, "%d-%d", &accounts[total].dob.month, &accounts[total].dob.year);

        token = strtok(NULL, ",");
        strcpy(accounts[total].status, token);

        total++;
    }

    fclose(fp);
    return total;
}

void queryAccount(Account accounts[], int total) {
    int searchNumber;
    printf("Enter Account Number to search: ");
    scanf("%d", &searchNumber);

    int found = 0;
    for (int i = 0; i < total; i++) {
        if (accounts[i].accountNumber == searchNumber) {
            printf("Account Number: %d\n", accounts[i].accountNumber);
            printf("Name: %s\n", accounts[i].name);
            printf("Address: %s\n", accounts[i].address);
            printf("Balance: %.2lf\n", accounts[i].balance);
            printf("Mobile: %s\n", accounts[i].mobile);
            printf("Date of Birth: %02d-%d\n", accounts[i].dob.month, accounts[i].dob.year);
            printf("Status: %s\n", accounts[i].status);
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Account not found.\n");
    }
}

int main() {
    FILE *fp;
    char fileUser[50], filePass[50];
    char username[50], userpass[50];
    int found = 0;

    fp = fopen("user.txt", "r");
    if (!fp) {
        printf("Error opening users file.\n");
        return 1;
    }

    printf("Enter Username: ");
    scanf("%s", username);
    printf("Enter Password: ");
    scanf("%s", userpass);

    while (fscanf(fp, "%s %s", fileUser, filePass) != EOF) {
        if (strcmp(username, fileUser) == 0 && strcmp(userpass, filePass) == 0) {
            found = 1;
            break;
        }
    }
    fclose(fp);

    if (!found) {
        printf("Invalid username or password.\n");
        return 0;
    }

    printf("Login successful!\n");

    Account accounts[MAX_ACCOUNTS];
    int totalAccounts = loadAccounts(accounts);
    printf("%d accounts loaded successfully.\n", totalAccounts);

    queryAccount(accounts, totalAccounts);

    return 0;
}

