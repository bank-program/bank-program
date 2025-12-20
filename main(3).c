#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define DAILY_LIMIT 50000
#define TRANSACTION_LIMIT 10000
#define MAX_ACCOUNTS 1000
#define DEPOSIT_LIMIT 10000

typedef struct {
    int month;
    int year;
} Date;

typedef struct {
    char accountNumber[20];
    char name[100];
    char address[100];
    double balance;
    char mobile[20];
    Date dob;
    char status[20];
    double dailyWithdrawn;
    int lastDay;
    int lastMonth;
    int lastYear;
} Account;

const char* getMonthName(int month) {
    const char* months[] = {
        "January","February","March","April","May","June",
        "July","August","September","October","November","December"
    };
    if (month >= 1 && month <= 12) return months[month-1];
    return "Unknown";
}

void toLower(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}

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
        strcpy(accounts[total].accountNumber, token);

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
    char searchNumber[20];
    printf("Enter Account Number to search: ");
    scanf("%s", searchNumber);

    int found = 0;
    for (int i = 0; i < total; i++) {
        if (strcmp(accounts[i].accountNumber, searchNumber) == 0) {
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

    if (!found) {
        printf("Account not found.\n");
    }
}

void advancedSearch(Account accounts[], int total) {
    char keyword[50];
    printf("Enter keyword: ");
    scanf("%s", keyword);
    toLower(keyword);

    int found = 0;
    for (int i = 0; i < total; i++) {
        char temp[100];
        strcpy(temp, accounts[i].name);
        toLower(temp);

        if (strstr(temp, keyword)) {
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

void addAccount(Account accounts[], int *total) {
    Account newAcc;

    printf("Enter Account Number: ");
    scanf("%s", newAcc.accountNumber);

    for (int i = 0; i < *total; i++) {
        if (strcmp(accounts[i].accountNumber, newAcc.accountNumber) == 0) {
            printf("Error: Account number already exists!\n");
            return;
        }
    }

    printf("Enter Name: ");
    scanf(" %[^\n]", newAcc.name);

    printf("Enter Address (email): ");
    scanf("%s", newAcc.address);

    printf("Enter Balance: ");
    scanf("%lf", &newAcc.balance);

    printf("Enter Mobile: ");
    scanf("%s", newAcc.mobile);

    printf("Enter month opened (1-12): ");
    scanf("%d", &newAcc.dob.month);
    printf("Enter year opened: ");
    scanf("%d", &newAcc.dob.year);

    strcpy(newAcc.status, "active");

    accounts[*total] = newAcc;
    (*total)++;

    FILE *fp = fopen("account.txt", "a");
    if (fp) {
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
    } else {
        printf("Error: Could not update account.txt\n");
    }
        // Create transaction file and log initial balance
    char histFile[30];
    strcpy(histFile, newAcc.accountNumber);
    strcat(histFile, ".txt");
    FILE *hf = fopen(histFile, "a");
    if (hf) {
        fprintf(hf, "Account created | Initial Balance: %.2f\n", newAcc.balance);
        fclose(hf);
    }

}
void deleteAccount(Account accounts[], int *total) {
    char accNum[20];
    printf("Enter account number to delete: ");
    scanf("%s", accNum);

    int index = -1;

    for (int i = 0; i < *total; i++) {
        if (strcmp(accounts[i].accountNumber, accNum) == 0) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf("Error: Account number does not exist.\n");
        return;
    }

    if (accounts[index].balance > 0) {
        printf("Deletion rejected: Account balance is not zero.\n");
        return;
    }

    for (int i = index; i < *total - 1; i++) {
        accounts[i] = accounts[i + 1];
    }
    (*total)--;


    FILE *fp = fopen("account.txt", "w");
    if (!fp) {
        printf("Error updating file.\n");
        return;
    }

    for (int i = 0; i < *total; i++) {
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
}
void modifyAccount(Account accounts[], int total) {
    char accNum[20];
    printf("Enter account number to modify: ");
    scanf("%s", accNum);

    int index = -1;

    for (int i = 0; i < total; i++) {
        if (strcmp(accounts[i].accountNumber, accNum) == 0) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf("Error: Account number does not exist.\n");
        return;
    }

    printf("\nAccount found. Enter new details:\n");

    printf("Enter new name (current: %s): ", accounts[index].name);
    scanf(" %[^\n]", accounts[index].name);


    printf("Enter new email (current: %s): ", accounts[index].address);
    scanf("%s", accounts[index].address);

    printf("Enter new mobile (current: %s): ", accounts[index].mobile);
    scanf("%s", accounts[index].mobile);

    FILE *fp = fopen("account.txt", "w");
    if (!fp) {
        printf("Error updating file.\n");
        return;
    }

    for (int i = 0; i < total; i++) {
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
}
void changeStatus(Account accounts[], int total) {
    char accNum[20];
    printf("Enter account number: ");
    scanf("%s", accNum);

    int index = -1;


    for (int i = 0; i < total; i++) {
        if (strcmp(accounts[i].accountNumber, accNum) == 0) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf("Error: Account does not exist.\n");
        return;
    }

    printf("Current account status: %s\n", accounts[index].status);
    printf("Enter new status (active / inactive): ");

    char newStatus[20];
    scanf("%s", newStatus);
    toLower(newStatus);


    if (strcmp(accounts[index].status, newStatus) == 0) {
        printf("Attention: Account is already %s.!\n", newStatus);
        return;
    }


    if (strcmp(newStatus, "active") != 0 && strcmp(newStatus, "inactive") != 0) {
        printf("Error: Invalid status.\n");
        return;
    }

    strcpy(accounts[index].status, newStatus);


    FILE *fp = fopen("account.txt", "w");
    if (!fp) {
        printf("Error updating file.\n");
        return;
    }

    for (int i = 0; i < total; i++) {
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
}
void withdraw(Account *acc, double amount, int day, int month, int year) {
    if (strcmp(acc->status, "inactive") == 0) {
        printf("Warning: Account is inactive. Transaction cannot be completed.\n");
        return;
    }

    if (amount > TRANSACTION_LIMIT) {
        printf("Error: Maximum withdrawal per transaction is %.2f\n", (double)TRANSACTION_LIMIT);
        return;
    }

    if (acc->lastDay != day || acc->lastMonth != month || acc->lastYear != year) {
        acc->dailyWithdrawn = 0;
        acc->lastDay = day;
        acc->lastMonth = month;
        acc->lastYear = year;
    }

    if (acc->dailyWithdrawn + amount > DAILY_LIMIT) {
        printf("Error: Daily withdrawal limit of %.2f exceeded!\n", (double)DAILY_LIMIT);
        return;
    }

    if (acc->balance < amount) {
        printf("Error: Insufficient balance.\n");
        return;
    }

    acc->balance -= amount;
    acc->dailyWithdrawn += amount;
    printf("Withdrawal successful! New balance for %s account is: %.2f\n", acc->name, acc->balance);
        // Log withdrawal
    char filename[30];
    strcpy(filename, acc->accountNumber);
    strcat(filename, ".txt");
    FILE *log = fopen(filename, "a");
    if (log) {
        fprintf(log, "Withdraw %.2f | New Balance: %.2f\n", amount, acc->balance);
        fclose(log);
    }

}
void deposit(Account *acc, double amount) {
    if (strcmp(acc->status, "inactive") == 0) {
        printf("Warning: Account is inactive. Transaction cannot be completed.\n");
        return;
    }

    if (amount > DEPOSIT_LIMIT) {
        printf("Error: Maximum deposit per transaction is %.2f\n", (double)DEPOSIT_LIMIT);
        return;
    }

    acc->balance += amount;
    printf("Deposit successful! New balance for %s account is: %.2f\n", acc->name, acc->balance);
        // Log deposit
    char filename[30];
    strcpy(filename, acc->accountNumber);
    strcat(filename, ".txt");
    FILE *log = fopen(filename, "a");
    if (log) {
        fprintf(log, "Deposit %.2f | New Balance: %.2f\n", amount, acc->balance);
        fclose(log);
    }

}
void transfer(Account accounts[], int total) {
    char senderAcc[20], receiverAcc[20];
    double amount;

    // Ask for sender and receiver account numbers
    printf("Enter sender account number: ");
    scanf("%s", senderAcc);

    printf("Enter receiver account number: ");
    scanf("%s", receiverAcc);

    int senderIndex = -1, receiverIndex = -1;

    // Find sender account
    for (int i = 0; i < total; i++) {
        if (strcmp(accounts[i].accountNumber, senderAcc) == 0) {
            senderIndex = i;
            break;
        }
    }

    // Find receiver account
    for (int i = 0; i < total; i++) {
        if (strcmp(accounts[i].accountNumber, receiverAcc) == 0) {
            receiverIndex = i;
            break;
        }
    }


    if (senderIndex == -1) {
        printf("Error: Sender account not found.\n");
        return;
    }
    if (receiverIndex == -1) {
        printf("Error: Receiver account not found.\n");
        return;
    }

    if (senderIndex == receiverIndex) {
        printf("Error: Cannot transfer to the same account.\n");
        return;
    }

    if (strcmp(accounts[senderIndex].status, "inactive") == 0 ||
        strcmp(accounts[receiverIndex].status, "inactive") == 0) {
        printf("Warning: One or both accounts are inactive. Transfer cannot be completed.\n");
        return;
    }

    printf("Enter transfer amount: ");
    scanf("%lf", &amount);

    if (amount <= 0) {
        printf("Error: Invalid transfer amount.\n");
        return;
    }
    if (accounts[senderIndex].balance < amount) {
        printf("Error: Insufficient balance in sender account.\n");
        return;
    }

    accounts[senderIndex].balance -= amount;
    accounts[receiverIndex].balance += amount;

    printf("Transfer successful!\n");
    printf("New balance of sender (%s): %.2f\n", accounts[senderIndex].name, accounts[senderIndex].balance);
    printf("New balance of receiver (%s): %.2f\n", accounts[receiverIndex].name, accounts[receiverIndex].balance);

    FILE *fp = fopen("account.txt", "w");
    if (!fp) {
        printf("Error updating file.\n");
        return;
    }

    for (int i = 0; i < total; i++) {
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
        // Log transfer in sender file
    char senderFile[30];
    strcpy(senderFile, accounts[senderIndex].accountNumber);
    strcat(senderFile, ".txt");
    FILE *logSender = fopen(senderFile, "a");
    if (logSender) {
        fprintf(logSender, "Transfer Out %.2f to %s | New Balance: %.2f\n",
                amount, accounts[receiverIndex].accountNumber, accounts[senderIndex].balance);
        fclose(logSender);
    }

    // Log transfer in receiver file
    char receiverFile[30];
    strcpy(receiverFile, accounts[receiverIndex].accountNumber);
    strcat(receiverFile, ".txt");
    FILE *logReceiver = fopen(receiverFile, "a");
    if (logReceiver) {
        fprintf(logReceiver, "Transfer In %.2f from %s | New Balance: %.2f\n",
                amount, accounts[senderIndex].accountNumber, accounts[receiverIndex].balance);
        fclose(logReceiver);
    }

}
void report() {
    char accNum[20];
    printf("Enter account number: ");
    scanf("%s", accNum);

    char filename[30];
    strcpy(filename, accNum);
    strcat(filename, ".txt");

    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("Error: No transaction history found for account %s.\n", accNum);
        return;
    }

    char lines[1000][200];
    int count = 0;

    while (count < 1000 && fgets(lines[count], sizeof(lines[count]), fp) != NULL) {
        int len = strlen(lines[count]);
        if (len > 0 && lines[count][len - 1] == '\n') {
            lines[count][--len] = '\0';
        }
        if (len > 0 && lines[count][len - 1] == '\r') {
            lines[count][--len] = '\0';
        }
        count++;
    }
    fclose(fp);

    if (count == 0) {
        printf("No transactions recorded for this account.\n");
        return;
    }

    int start = (count >= 5 ? count - 5 : 0);
    int toShow = (count < 5 ? count : 5);
    printf("Last %d transaction(s) for account %s:\n", toShow, accNum);
    for (int i = start; i < count; i++) {
        printf("%s\n", lines[i]);
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

    int choice;
    do {
        printf("\nChoose operation:\n");
        printf("1. Add Account\n");
        printf("2. Query Account\n");
        printf("3. Advanced Search\n");
        printf("4. Delete Account\n");
        printf("5. Modify Account\n");
        printf("6. Change Account Status\n");
        printf("7. Withdraw\n");
        printf("8. Deposit\n");
        printf("9. Exit\n");
        printf("10. Transfer\n");
         printf("11. Report (Last 5 Transactions)\n");
        printf("Enter choice: ");

        scanf("%d", &choice);

        if (choice == 1) addAccount(accounts, &totalAccounts);
        else if (choice == 2) queryAccount(accounts, totalAccounts);
        else if (choice == 3) advancedSearch(accounts, totalAccounts);
        else if (choice == 4) deleteAccount(accounts, &totalAccounts);
        else if (choice == 5) modifyAccount(accounts, totalAccounts);
        else if (choice == 6) changeStatus(accounts, totalAccounts);
        else if (choice == 7) {
            char accNum[20];
            double amount;
            int day, month, year;

            printf("Enter account number: ");
            scanf("%s", accNum);

            int index = -1;
            for (int i = 0; i < totalAccounts; i++) {
                if (strcmp(accounts[i].accountNumber, accNum) == 0) {
                    index = i;
                    break;
                }
            }

            if (index == -1) {
                printf("Account not found.\n");
            } else {
                if (strcmp(accounts[index].status, "inactive") == 0) {
                    printf("Warning: Account is inactive. Transaction cannot be completed.\n");
                } else {
                    printf("Enter withdrawal amount: ");
                    scanf("%lf", &amount);
                    printf("Enter today date (day month year): ");
                    scanf("%d %d %d", &day, &month, &year);
                    withdraw(&accounts[index], amount, day, month, year);
                }
            }
        }
        else if (choice == 8) {
            char accNum[20];
            double amount;

            printf("Enter account number: ");
            scanf("%s", accNum);

            int index = -1;
            for (int i = 0; i < totalAccounts; i++) {
                if (strcmp(accounts[i].accountNumber, accNum) == 0) {
                    index = i;
                    break;
                }
            }

            if (index == -1) {
                printf("Account not found.\n");
            } else {
                if (strcmp(accounts[index].status, "inactive") == 0) {
                    printf("Warning: Account is inactive. Transaction cannot be completed.\n");
                } else {
                    printf("Enter deposit amount: ");
                    scanf("%lf", &amount);
                    deposit(&accounts[index], amount);
                }
            }
        }
        else if (choice == 9) break;

        else if (choice == 10) transfer(accounts, totalAccounts);
            else if (choice == 11) report();

        else printf("Invalid choice.\n");

    } while (choice != 11);

    return 0;
}


