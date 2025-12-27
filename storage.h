
#ifndef STORAGE_H
#define STORAGE_H

#include "types.h"

int loadAccounts(Account accounts[]);
void saveAccounts(Account accounts[], int total);
void report(void);

#endif
