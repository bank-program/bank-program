
#ifndef VALIDATE_H
#define VALIDATE_H

int validateAccountNumber(const char *accNum);
int validateEmail(const char *email);
int validateMobile(const char *mobile);
int validatePositive(double value);

void toLower(char *str);
int getIntChoice(void);

#endif
