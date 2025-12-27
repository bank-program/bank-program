
#ifndef OPS_H
#define OPS_H

#include "types.h"

void addAccount(Account accounts[], int *total);
void deleteAccount(Account accounts[], int *total);
void modifyAccount(Account accounts[], int total);
void changeStatus(Account accounts[], int total);
void deleteMultiple(Account accounts[], int *total);

#endif
