
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include "validate.h"

int validateAccountNumber(const char *accNum)
{
    if (strlen(accNum) != 10) return 0;
    for (int i = 0; i < 10; i++)
    {
        if (!isdigit((unsigned char)accNum[i])) return 0;
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
        if (!isdigit((unsigned char)mobile[i])) return 0;
    }
    return 1;
}

int validatePositive(double value)
{
    return value >= 0;
}

void toLower(char *str)
{
    for (int i = 0; str[i]; i++)
    {
        str[i] = (char)tolower((unsigned char)str[i]);
    }
}

int getIntChoice()
{
    char buffer[50];
    int choice;
    if (scanf("%49s", buffer) != 1) return -1;
    char *endptr;
    choice = (int)strtol(buffer, &endptr, 10);
    if (*endptr != '\0') return -1;
    return choice;
}
