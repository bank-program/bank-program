
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include "config.h"
#include "types.h"
#include "console.h"
#include "validate.h"
#include "storage.h"
#include "search.h"
#include "ops.h"
#include "trans.h"
#include "sort.h"

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
            scanf("%49s", username);
            printf("Enter Password: ");
            scanf("%49s", userpass);

            toLower(username);

            while (fscanf(fp, "%49s %49s", fileUser, filePass) != EOF)
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
            int totalAccounts = loadAccounts(accounts);
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
                printf("%s13.%s Delete Multiple Accounts\n", FG_BLUE, RESET);
                printf("%s14.%s Quit\n", FG_BLUE, RESET);
                prompt("Enter choice: ");

                subChoice = getIntChoice();

                if      (subChoice == 1) addAccount(accounts, &totalAccounts);
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
                    scanf("%19s", accNum);

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
                    scanf("%19s", accNum);
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
                else if (subChoice == 9)  transfer(accounts, totalAccounts);
                else if (subChoice == 10) report();
                else if (subChoice == 11) print(accounts, totalAccounts);
                else if (subChoice == 12) saveAccounts(accounts, totalAccounts);
                else if (subChoice == 13) deleteMultiple(accounts, &totalAccounts);
                else if (subChoice == -1) printf("Invalid choice.\n");

            }
            while (subChoice != 14);
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
