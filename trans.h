
#ifndef TRANS_H
#define TRANS_H

#include "types.h"

void withdraw(Account *acc, double amount);
void deposit(Account *acc, double amount);
void transfer(Account accounts[], int total);

#endif
