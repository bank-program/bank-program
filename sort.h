
#ifndef SORT_H
#define SORT_H

#include "types.h"

void displayAccount(Account acc);
void sortByName(Account accounts[], int total);
void sortByBalance(Account accounts[], int total);
void sortByDate(Account accounts[], int total);
void sortByStatus(Account accounts[], int total);
void print(Account accounts[], int total);

#endif
