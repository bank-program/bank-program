
#ifndef TYPES_H
#define TYPES_H

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

const char* getMonthName(int month);

#endif
