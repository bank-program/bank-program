#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_ACCOUNTS 1000

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

    // search for account
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
    printf("\nChoose operation:\n1. Add Account\n2. Query Account\n3. Advanced Search\n4. Delete Account\n5. Modify Account\n6. Change Account Status\nEnter choice: ");
    scanf("%d", &choice);

    if (choice == 1) addAccount(accounts, &totalAccounts);
    else if (choice == 2) queryAccount(accounts, totalAccounts);
    else if (choice == 3) advancedSearch(accounts, totalAccounts);
    else if (choice == 4) deleteAccount(accounts, &totalAccounts);
    else if (choice == 5) modifyAccount(accounts, totalAccounts);
    else if (choice == 6) changeStatus(accounts, totalAccounts);
    else printf("Invalid choice.\n");

    return 0;
}

