#ifndef UTILS_H
#define UTILS_H

#include "types.h"

// System & IO
void clearScreen();
void pauseScreen();
void clearInputBuffer();
char getSingleInput();
int getSingletDigit();
int getNumberInput();

// Time
void getCurrentDateTime(char *dateTime);
void getCurrentTime(char *timeStr);
int compareDateTime(const char *time1, const char *time2);
int compareTime(const char *time1, const char *time2);
void calculateExpiryTime(char *expiryTime);

// Formatting
void formatDuration(int minutes, char *result);
void formatDate(const char *fullDateTime, char *shortDate);
const char *getStatusString(TransactionStatus status);

// Validation
bool isValidEmail(const char *email);
bool validateCard(const char *cardNumber, const char *expiry, const char *cvv);
bool validateBookingCode(const char *code);
bool validatePasskey(const char *passkey);
const char *detectCardType(const char *cardNumber);

#endif // UTILS_H