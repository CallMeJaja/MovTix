#include "utils.h"
#include "conio.h"
#include "iomanip"
#include "iostream"
#include "limits"
#include "string.h"

using namespace std;

void clearScreen() { system("cls"); }

void pauseScreen() {
    cout << "\nTekan Enter untuk melanjutkan...";

    // Bersihkan buffer dulu
    while (kbhit()) {
        getch();
    }

    // Tunggu tekanan Enter
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

void clearInputBuffer() {
    while (kbhit()) {
        getch();
    }
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
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
    if (input >= '0' && input <= '9') {
        return input - '0';
    } else {
        return -1; // Invalid input
    }
}

int getNumberInput() {
    int number;
    cin >> number;
    return number;
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

int compareDateTime(const char *time1, const char *time2) {
    int day1, month1, year1, hour1, min1, sec1;
    int day2, month2, year2, hour2, min2, sec2;

    sscanf(time1, "%d-%d-%d, %d:%d:%d", &day1, &month1, &year1, &hour1, &min1,
           &sec1);
    sscanf(time2, "%d-%d-%d, %d:%d:%d", &day2, &month2, &year2, &hour2, &min2,
           &sec2);

    // Konversi ke detik untuk perbandingan
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

void calculateExpiryTime(char *expiryTime) {
    time_t now = time(0);
    now += 15 * 60; // Tambah 15 menit
    tm *ltm = localtime(&now);

    sprintf(expiryTime, "%02d-%02d-%04d, %02d:%02d:%02d", ltm->tm_mday,
            ltm->tm_mon + 1, ltm->tm_year + 1900, ltm->tm_hour, ltm->tm_min,
            ltm->tm_sec);
}

void formatDuration(int minutes, char *result) {
    int hours = minutes / 60;
    int mins = minutes % 60;
    if (hours > 0) {
        sprintf(result, "%dj%dm", hours, mins);
    } else {
        sprintf(result, "%dm", mins);
    }
}

void formatDate(const char *fullDateTime, char *shortDate) {
    int day, month, year, hour, min, sec;
    sscanf(fullDateTime, "%d-%d-%d, %d:%d:%d", &day, &month, &year, &hour, &min,
           &sec);
    sprintf(shortDate, "%02d-%02d-%02d", day, month, year % 100);
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

bool isValidEmail(const char *email) {
    int len = strlen(email);
    bool hasAt = false;
    bool hasDot = false;

    for (int i = 0; i < len; i++) {
        if (email[i] == '@') {
            hasAt = true;
        } else if (email[i] == '.' && hasAt) {
            hasDot = true;
        }
    }

    return hasAt && hasDot && len > 5;
}

bool validateCard(const char *cardNumber, const char *expiry, const char *cvv) {
    // Validasi nomor kartu (16 digit)
    char cleanNumber[20];
    int cleanIndex = 0;

    for (int i = 0; cardNumber[i]; i++) {
        if (isdigit(cardNumber[i])) {
            cleanNumber[cleanIndex++] = cardNumber[i];
        }
    }
    cleanNumber[cleanIndex] = '\0';

    if (strlen(cleanNumber) != 16) {
        cout << "\nNomor kartu harus 16 digit!" << endl;
        return false;
    }

    // Validasi masa berlaku (MM/YY)
    if (strlen(expiry) != 5 || expiry[2] != '/') {
        cout << "\nFormat masa berlaku salah! Gunakan MM/YY" << endl;
        return false;
    }

    int month = (expiry[0] - '0') * 10 + (expiry[1] - '0');
    int year = (expiry[3] - '0') * 10 + (expiry[4] - '0');

    if (month < 1 || month > 12) {
        cout << "\nBulan tidak valid! Harus antara 01-12" << endl;
        return false;
    }

    time_t now = time(0);
    tm *ltm = localtime(&now);

    int currentYear = ltm->tm_year % 100; // 2025 -> 25
    int currentMonth = ltm->tm_mon + 1;   // 0-11 -> 1-12

    cout << "\n[INFO] Masa berlaku kartu: " << expiry << endl;
    cout << "[INFO] Bulan/Tahun saat ini: " << setfill('0') << setw(2)
         << currentMonth << "/20" << setw(2) << currentYear << endl;

    // Cek apakah kartu sudah expired
    if (year < currentYear || (year == currentYear && month < currentMonth)) {
        cout << "\nKartu sudah kadaluarsa! Masa berlaku: " << expiry << endl;
        cout << "Tahun saat ini: 20" << currentYear
             << ", Bulan: " << currentMonth << endl;
        return false;
    }

    // Validasi CVV (3 digit)
    if (strlen(cvv) != 3) {
        cout << "\nCVV harus 3 digit!" << endl;
        return false;
    }

    for (int i = 0; i < 3; i++) {
        if (!isdigit(cvv[i])) {
            cout << "\nCVV harus berupa angka!" << endl;
            return false;
        }
    }
    cout << "\nValidasi berhasil! Kartu berlaku sampai " << expiry << endl;

    return true;
}

bool validateBookingCode(const char *code) {
    // Kode booking harus 8 karakter, huruf kapital dan angka
    if (strlen(code) != 8) {
        return false;
    }

    for (int i = 0; i < 8; i++) {
        if (!isalnum(code[i]) || islower(code[i])) {
            return false;
        }
    }
    return true;
}

bool validatePasskey(const char *passkey) {
    // Passkey harus 6 digit angka
    if (strlen(passkey) != 6) {
        return false;
    }

    for (int i = 0; i < 6; i++) {
        if (!isdigit(passkey[i])) {
            return false;
        }
    }
    return true;
}

const char *detectCardType(const char *cardNumber) {
    char cleanNumber[20];
    int cleanIndex = 0;

    // Remove spaces and non-digits
    for (int i = 0; cardNumber[i]; i++) {
        if (isdigit(cardNumber[i])) {
            cleanNumber[cleanIndex++] = cardNumber[i];
        }
    }
    cleanNumber[cleanIndex] = '\0';

    if (strlen(cleanNumber) != 16)
        return "Unknown";

    // Visa: starts with 4
    if (cleanNumber[0] == '4')
        return "Visa";

    // Mastercard: starts with 5 or 2221-2720
    if (cleanNumber[0] == '5')
        return "Mastercard";
    if (cleanNumber[0] == '2' && cleanNumber[1] == '2') {
        int prefix = (cleanNumber[2] - '0') * 10 + (cleanNumber[3] - '0');
        if (prefix >= 21 && prefix <= 72)
            return "Mastercard";
    }

    return "Unknown";
}
