#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define DAILY_LIMIT 50000
#define TRANSACTION_LIMIT 10000
#define MAX_ACCOUNTS 1000
#define DEPOSIT_LIMIT 10000
#include <time.h>

// ===== Colors & styles (ANSI) =====
#define RESET       "\033[0m"
#define BOLD        "\033[1m"
#define UNDERLINE   "\033[4m"

// Foreground colors
#define FG_RED      "\033[38;5;196m"
#define FG_GREEN    "\033[38;5;46m"
#define FG_YELLOW   "\033[38;5;226m"
#define FG_CYAN     "\033[38;5;51m"
#define FG_BLUE     "\033[38;5;27m"
#define FG_MAGENTA  "\033[38;5;201m"
#define FG_WHITE    "\033[38;5;15m"
#define FG_GREY     "\033[38;5;245m"
#define FG_GOLD     "\033[38;5;178m"

// Background blocks for headers
#define BG_BLUE     "\033[48;5;27m"
#define BG_PURPLE   "\033[48;5;91m"
#define BG_GREEN    "\033[48;5;34m"
#define BG_RED      "\033[48;5;196m"
#define BG_GOLD     "\033[48;5;178m"


void msg_info(const char *text)
{
    printf("%s%s%s\n", FG_CYAN, text, RESET);
}
void msg_ok(const char *text)
{
    printf("%s%s%s\n", FG_GREEN, text, RESET);
}
void msg_warn(const char *text)
{
    printf("%s%s%s\n", FG_YELLOW, text, RESET);
}
void msg_err(const char *text)
{
    printf("%s%s%s\n", FG_RED, text, RESET);
}


void header(const char *title)
{
    printf("%s%s %s %s\n", BG_BLUE, BOLD, title, RESET);
}


void subheader(const char *title)
{
    printf("%s%s-- %s --%s\n", FG_MAGENTA, BOLD, title, RESET);
}

void prompt(const char *text)
{
    printf("%s%s%s", FG_GOLD, text, RESET);
}


#ifdef _WIN32
#include <windows.h>

static void enableANSI(void)
{
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE) return;

    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode)) return;

    // ENABLE_VIRTUAL_TERMINAL_PROCESSING = 0x0004
    SetConsoleMode(hOut, dwMode | 0x0004);
}
#endif

typedef struct
{
    int month;
    int year;
} Date;



typedef struct
{
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

const char* getMonthName(int month)
{
    const char* months[] =
    {
        "January","February","March","April","May","June",
        "July","August","September","October","November","December"
    };
    if (month >= 1 && month <= 12) return months[month-1];
    return "Unknown";
}

void toLower(char *str)
{
    for (int i = 0; str[i]; i++)
    {
        str[i] = tolower(str[i]);
    }
}



int validateAccountNumber(const char *accNum)
{
    if (strlen(accNum) != 10) return 0;
    for (int i = 0; i < 10; i++)
    {
        if (!isdigit(accNum[i])) return 0;
    }
    return 1;
}

int validateEmail(const char *email)
{
    const char *at = strchr(email, '@');
    if (!at) return 0;
    const char *dot = strchr(at, '.');
    if (!dot) return 0;
    return 1;
}

int validateMobile(const char *mobile)
{
    if (strlen(mobile) != 11) return 0;
    for (int i = 0; mobile[i]; i++)
    {
        if (!isdigit(mobile[i])) return 0;
    }
    return 1;
}

int validatePositive(double value)
{
    return value >= 0;
}


int loadAccounts(Account accounts[])
{
    FILE *fp = fopen("account.txt", "r");
    if (!fp) return 0;

    int total = 0;
    char line[300];

    while (fgets(line, sizeof(line), fp) != NULL)
    {
        int len = strlen(line);
        if (len > 0 && line[len - 1] == '\n')
        {
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

void queryAccount(Account accounts[], int total)
{
    char searchNumber[20];
    printf("Enter Account Number to search: ");
    scanf("%s", searchNumber);

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
    scanf("%s", keyword);
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



void addAccount(Account accounts[], int *total)
{
    Account newAcc;

    printf("Enter Account Number: ");
    scanf("%s", newAcc.accountNumber);
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
    scanf(" %[^\n]", newAcc.name);

    printf("Enter Address (email): ");
    scanf("%s", newAcc.address);
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
    scanf("%s", newAcc.mobile);
    if (!validateMobile(newAcc.mobile))
    {
        printf("Error: Invalid mobile number! Must be 11 digits.\n");
        return;
    }

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    newAcc.dob.month = tm.tm_mon + 1;
    newAcc.dob.year = tm.tm_year + 1900;

    strcpy(newAcc.status, "active");

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
    scanf("%s", accNum);

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
    scanf("%s", accNum);

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
    scanf(" %[^\n]", accounts[index].name);

    do
    {
        printf("Enter new email (current: %s): ", accounts[index].address);
        scanf("%s", accounts[index].address);
        if (!validateEmail(accounts[index].address))
        {
            printf("Error: Invalid email format! Try again.\n");
        }
    }
    while (!validateEmail(accounts[index].address));


    do
    {
        printf("Enter new mobile (current: %s): ", accounts[index].mobile);
        scanf("%s", accounts[index].mobile);
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
    scanf("%s", accNum);

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
    scanf("%s", newStatus);
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

    // get current local time
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    int day   = tm.tm_mday;
    int month = tm.tm_mon + 1;
    int year  = tm.tm_year + 1900;

    if (acc->lastDay != day || acc->lastMonth != month || acc->lastYear != year)
    {
        acc->dailyWithdrawn = 0;
        acc->lastDay = day;
        acc->lastMonth = month;
        acc->lastYear = year;
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
    FILE *fp = fopen(filename, "a");   // renamed to fp
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
    scanf("%s", senderAcc);

    printf("Enter receiver account number: ");
    scanf("%s", receiverAcc);

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
void report()
{
    char accNum[20];
    printf("Enter account number: ");
    scanf("%s", accNum);

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
        int len = strlen(lines[count]);
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

    int start = (count >= 5 ? count - 5 : 0);
    int toShow = (count < 5 ? count : 5);
    printf("Last %d transaction(s) for account %s:\n", toShow, accNum);
    for (int i = start; i < count; i++)
    {
        printf("%s\n", lines[i]);
    }
}
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

    if (choice == 1) sortByName(accounts, total);
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
void deleteMultiple(Account accounts[], int *total)
{
    int option;
    printf("\n--- Delete Multiple Accounts ---\n");
    printf("1. By Date (YYYY-MM)\n");
    printf("2. Inactive Accounts (>3 months, balance = 0)\n");
    printf("Enter choice: ");
    scanf("%d", &option);

    int deletedCount = 0;

    if (option == 1)
    {
        int year, month;
        printf("Enter date (YYYY-MM): ");
        scanf("%d-%d", &year, &month);

        for (int i = 0; i < *total; )
        {
            if (accounts[i].dob.year == year && accounts[i].dob.month == month)
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
            printf("No accounts found for given month/year.\n");
        else
            printf("Deletion complete. %d account(s) deleted.\n", deletedCount);
    }
    else if (option == 2)
    {
        int todayMonth, todayYear;
        printf("Enter today’s date (MM YYYY): ");
        scanf("%d %d", &todayMonth, &todayYear);

        for (int i = 0; i < *total; )
        {
            if (strcmp(accounts[i].status, "inactive") == 0 && accounts[i].balance == 0)
            {
                int diffMonths = (todayYear - accounts[i].lastYear) * 12 + (todayMonth - accounts[i].lastMonth);
                if (diffMonths > 3)
                {
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
            printf("Deletion complete. %d account(s) deleted\n", deletedCount);
    }
    else
    {
        printf("Invalid choice.\n");
        return;
    }

    if (deletedCount > 0)
    {
        saveAccounts(accounts, *total);
    }
}


int getIntChoice()
{
    char buffer[50];
    int choice;
    if (scanf("%49s", buffer) != 1) return -1;
    char *endptr;
    choice = strtol(buffer, &endptr, 10);
    if (*endptr != '\0') return -1;
    return choice;
}

int main()
{
#ifdef _WIN32
    enableANSI();
#endif

    char buffer[50];
    int choice;

    do
    {
        header("Initial Menu");
        printf("%s1.%s Login\n", FG_BLUE, RESET);
        printf("%s2.%s Quit\n", FG_BLUE, RESET);
        prompt("Enter choice: ");
        scanf("%49s", buffer);

        if (strcmp(buffer, "1") == 0 || strcasecmp(buffer, "login") == 0)
        {
            FILE *fp;
            char fileUser[50], filePass[50];
            char username[50], userpass[50];
            int found = 0;

            fp = fopen("user.txt", "r");
            if (!fp)
            {
                msg_err("Error opening users file.");
                continue;
            }

            printf("Enter Username: ");
            scanf("%s", username);
            printf("Enter Password: ");
            scanf("%s", userpass);

            toLower(username);

            while (fscanf(fp, "%s %s", fileUser, filePass) != EOF)
            {
                char tempUser[50];
                strcpy(tempUser, fileUser);
                toLower(tempUser);

                if (strcmp(username, tempUser) == 0 && strcmp(userpass, filePass) == 0)
                {
                    found = 1;
                    break;
                }
            }
            fclose(fp);

            if (!found)
            {
                msg_err("Invalid username or password.");
                continue;
            }

            msg_ok("Login successful!");

            Account accounts[MAX_ACCOUNTS];
            //printf("--");
            int totalAccounts = loadAccounts(accounts);
            //printf("--");
            printf("%s%d%s accounts loaded successfully.\n", FG_GREEN, totalAccounts, RESET);

            int subChoice;
            do
            {
                header("Main Menu");
                printf("%s1.%s Add Account\n", FG_BLUE, RESET);
                printf("%s2.%s Delete Account\n", FG_BLUE, RESET);
                printf("%s3.%s Modify Account\n", FG_BLUE, RESET);
                printf("%s4.%s Search Account\n", FG_BLUE, RESET);
                printf("%s5.%s Advanced Search\n", FG_BLUE, RESET);
                printf("%s6.%s Change Account Status\n", FG_BLUE, RESET);
                printf("%s7.%s Withdraw\n", FG_BLUE, RESET);
                printf("%s8.%s Deposit\n", FG_BLUE, RESET);
                printf("%s9.%s Transfer\n", FG_BLUE, RESET);
                printf("%s10.%s Report (Last 5 Transactions)\n", FG_BLUE, RESET);
                printf("%s11.%s Print (Sorted)\n", FG_BLUE, RESET);
                printf("%s12.%s Save Accounts\n", FG_BLUE, RESET);
                printf("%s13.%s Quit\n", FG_BLUE, RESET);
                prompt("Enter choice: ");

                subChoice = getIntChoice();

                if (subChoice == 1) addAccount(accounts, &totalAccounts);
                else if (subChoice == 2) deleteAccount(accounts, &totalAccounts);
                else if (subChoice == 3) modifyAccount(accounts, totalAccounts);
                else if (subChoice == 4) queryAccount(accounts, totalAccounts);
                else if (subChoice == 5) advancedSearch(accounts, totalAccounts);
                else if (subChoice == 6) changeStatus(accounts, totalAccounts);
                else if (subChoice == 7)
                {
                    char accNum[20];
                    double amount;
                    printf("Enter account number: ");
                    scanf("%s", accNum);

                    int index = -1;
                    for (int i = 0; i < totalAccounts; i++)
                    {
                        if (strcmp(accounts[i].accountNumber, accNum) == 0)
                        {
                            index = i;
                            break;
                        }
                    }

                    if (index == -1)
                    {
                        printf("Error: Account not found.\n");
                    }
                    else
                    {
                        printf("Enter amount to withdraw: ");
                        scanf("%lf", &amount);
                        withdraw(&accounts[index], amount);
                          int choiceSave;
                        printf("Do you want to save changes? (1 = Yes, 2 = No): ");
                        scanf("%d", &choiceSave);
                        if (choiceSave == 1)
                        {
                            saveAccounts(accounts, totalAccounts);
                            printf("Changes confirmed and saved.\n");
                        }
                        else
                        {
                            printf("Changes discarded (not saved).\n");
                        }
                    }
                }

                else if (subChoice == 8)
                {

                        char accNum[20];
                        double amount;
                        printf("Enter account number: ");
                        scanf("%s", accNum);
                        int index = -1;
                        for (int i = 0; i < totalAccounts; i++)
                        {
                            if (strcmp(accounts[i].accountNumber, accNum) == 0)
                            {
                                index = i;
                                break;
                            }
                        }
                        if (index == -1) printf("Account not found.\n");
                        else
                        {
                            printf("Enter deposit amount: ");
                            scanf("%lf", &amount);
                            deposit(&accounts[index], amount);
                            int choiceSave;
                            printf("Do you want to save changes? (1 = Yes, 2 = No): ");
                            scanf("%d", &choiceSave);
                            if (choiceSave == 1)
                            {
                                saveAccounts(accounts, totalAccounts);
                                printf("Changes confirmed and saved.\n");
                            }
                            else
                            {
                                printf("Changes discarded (not saved).\n");
                            }

                    }
                }
                else if (subChoice == 9) transfer(accounts, totalAccounts);
                else if (subChoice == 10) report();
                else if (subChoice == 11) print(accounts, totalAccounts);
                else if (subChoice == 12) saveAccounts(accounts, totalAccounts);
                else if (subChoice == -1) printf("Invalid choice.\n");

            }
            while (subChoice != 13);
        }
        else if (strcmp(buffer, "2") == 0)
        {
            break;
        }
        else
        {
            printf("Invalid input. Please type 1 or login to proceed, or 2 to quit.\n");
        }

    }
    while (1);

    printf("Exiting program. Goodbye!\n");
    return 0;
}
