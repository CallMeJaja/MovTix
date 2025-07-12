#ifndef UTILS_H
#define UTILS_H

#include "structures.h"

//============================================================
// UTILITY FUNCTION PROTOTYPES
//============================================================

void clearScreen();
void pauseScreen();
void showHeader(const char *title);
void getCurrentDateTime(char *dateTime);
void getCurrentTime(char *timeStr);
char getSingleInput();
int getSingletDigit();
int getNumberInput();
const char *getStatusString(TransactionStatus status);
int compareDateTime(const char *time1, const char *time2);
int compareTime(const char *time1, const char *time2);

#endif // UTILS_H