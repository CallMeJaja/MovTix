#include "conio.h"
#include "ctime"
#include "limits"
#include <cstdlib>
#include <iostream>

#include "utils.h"

using namespace std;

//============================================================
// UTILITY FUNCTION IMPLEMENTATIONS
//============================================================

void clearScreen() { system("cls"); }

void pauseScreen() {
    cout << "\nTekan Enter untuk melanjutkan...";
    while (kbhit())
        getch();
    while (true) {
        if (kbhit()) {
            char ch = getch();
            if (ch == '\r' || ch == '\n') {

                cout << endl;
                break;
            }
        }
    }
}

void showHeader(const char *title) {
    cout << "============================================================"
         << endl;
    cout << "                    " << title << endl;
    cout << "============================================================"
         << endl;
}

void getCurrentDateTime(char *dateTime) {
    time_t now = time(0);
    tm *ltm = localtime(&now);
    sprintf(dateTime, "%02d-%02d-%04d, %02d:%02d:%02d", ltm->tm_mday,
            ltm->tm_mon + 1, ltm->tm_year + 1900, ltm->tm_hour, ltm->tm_min,
            ltm->tm_sec);
}

void getCurrentTime(char *timeStr) {
    time_t now = time(0);
    tm *ltm = localtime(&now);
    sprintf(timeStr, "%02d:%02d", ltm->tm_hour, ltm->tm_min);
}

char getSingleInput() {
    char input;
    while (true) {
        if (kbhit()) {
            input = getch();
            cout << input << endl;
            return input;
        }
    }
}

int getSingletDigit() {
    char input = getSingleInput();
    return (input >= '0' && input <= '9') ? (input - '0') : -1;
}

int getNumberInput() {
    int number;
    cin >> number;
    return number;
}

const char *getStatusString(TransactionStatus status) {
    switch (status) {
    case PENDING_PAYMENT:
        return "UNPAID";
    case PAID:
        return "PAID";
    case EXPIRED:
        return "EXPIRED";
    case CANCELLED:
        return "CANCELLED";
    case USED:
        return "USED";
    default:
        return "UNKNOWN";
    }
}

int compareDateTime(const char *time1, const char *time2) {
    int day1, month1, year1, hour1, min1, sec1;
    int day2, month2, year2, hour2, min2, sec2;
    sscanf(time1, "%d-%d-%d, %d:%d:%d", &day1, &month1, &year1, &hour1, &min1,
           &sec1);
    sscanf(time2, "%d-%d-%d, %d:%d:%d", &day2, &month2, &year2, &hour2, &min2,
           &sec2);

    long long total1 = ((long long)year1 * 365 + month1 * 30 + day1) * 86400 +
                       hour1 * 3600 + min1 * 60 + sec1;
    long long total2 = ((long long)year2 * 365 + month2 * 30 + day2) * 86400 +
                       hour2 * 3600 + min2 * 60 + sec2;

    if (total1 < total2)
        return -1;
    if (total1 > total2)
        return 1;
    return 0;
}

int compareTime(const char *time1, const char *time2) {
    int hour1, min1, hour2, min2;
    sscanf(time1, "%d:%d", &hour1, &min1);
    sscanf(time2, "%d:%d", &hour2, &min2);
    int totalMin1 = hour1 * 60 + min1;
    int totalMin2 = hour2 * 60 + min2;
    if (totalMin1 < totalMin2)
        return -1;
    if (totalMin1 > totalMin2)
        return 1;
    return 0;
}