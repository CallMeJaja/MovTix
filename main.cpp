#include "conio.h"
#include "ctime"
#include "limits"
#include <cstdlib>
#include <cstring>
#include <iomanip>
#include <iostream>

using namespace std;

//============================================================
// ENUM & STRUCT DEFINITIONS
//============================================================

enum SeatStatus {
    AVAILABLE, // [ ] - Tersedia
    PENDING,   // [P] - Menunggu Pembayaran
    TAKEN      // [T] - Sudah Dibayar
};

enum PaymentMethod {
    BANK_TRANSFER, // Bank Transfer Local
    CARD_PAYMENT,  // VISA & MASTERCARD
    MOVTIX_POINTS  // POINT MOVTIX
};

enum TransactionStatus {
    PENDING_PAYMENT = 0, // Menunggu Pembayaran
    PAID = 1,            // Sudah Dibayar
    EXPIRED = 2,         // Transaksi Kadaluarsa
    CANCELLED = 3,
    USED = 4 // Transaksi Dibatalkan
};

struct Seat {
    char seatNumber[4];
    SeatStatus status; // Status kursi
    int userId;
    char reservationTime[20];
    int transactionId;
};

struct FoodBeverage {
    int id;
    char name[50];
    int price;
    char category[20];
    bool isAvailable;
};

struct SelectedFnB {
    int fnbId;
    char name[50];
    int quantity;
    int price;
    int totalPrice;
};

struct PaymentInfo {
    PaymentMethod method;
    char cardNumber[20];
    char cardName[50];
    char expiry[6];
    char cvv[4];
    char bankName[50];
    int pointsUsed;
    bool isSuccess;
};

struct Transaction {
    int id;
    int userId;
    int movieId;
    int showtimeIndex;
    int ticketCount;
    char selectedSeats[200];
    SelectedFnB selectedFnB[10];
    int fnbCount;
    int ticketPrice;
    int fnbPrice;
    int totalPrice;
    char transactionDate[25];
    char movieTitle[100];
    char showtime[6];
    int auditorium;
    TransactionStatus status; // Status transaksi
    char expiryTime[20];      // Waktu kadaluarsa pembayaran
    bool isActive;
    char bookingCode[10];
    char passkey[10];
    PaymentInfo paymentInfo;
};

struct User {
    char nama[100];
    char username[50];
    char email[100];
    char password[50];
    bool isActive;
    int movtixPoints;
};

struct Showtime {
    char time[6]; // Format: HH:MM
    int auditorium;
    int capacity;
    int price;
};

struct Review {
    int userId;
    int rating; // Rating dari 1 sampai 5
    char comment[200];
    char date[12]; // Format: YYYY-MM-DD
};

struct Movie {
    int id;
    char title[100];
    char genre[50];
    int duration;    // Durasi dalam menit
    char rating[10]; // Rating film (misal: PG-13, R)
    char synopsis[500];
    int popularity;
    Showtime showtimes[10];
    int showtimeCount;
    Review reviews[500];
    int reviewCount;
    float averageRating;
    bool isActive;
};

//============================================================
// GLOBAL VARIABLES
//============================================================

User users[100];
Movie movies[50];
FoodBeverage foodBeverages[50];
Seat auditoriumSeats[5][100];
Transaction transactions[1000];

int seatCounts[5] = {80, 80, 60, 0, 0};
int fnbCount = 0;
int userCount = 0;
int movieCount = 0;
int currentUserIndex = -1;
int transactionCount = 0;

//============================================================
// FUNCTION PROTOTYPES (DEKLARASI FUNGSI)
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
void processPayment(int transactionId);
void cancelReservation(int transactionId);
void generationBookingCode(char *bookingCode);
void generatePasskey(char *passkey);
const char *detectCardType(const char *cardNumber);
bool validateCard(const char *cardNumber, const char *expiry, const char *cvv);
bool processCardPayment(int totalAmount, PaymentInfo *paymentInfo);
bool processBankPayment(int totalAmount, PaymentInfo *paymentInfo);
bool processPointsPayment(int totalAmount, PaymentInfo *paymentInfo);
void processTicketExchange(int transactionIndex);
int findTransactionByCode(const char *bookingCode, const char *passkey);
bool validateBookingCode(const char *code);
bool validatePasskey(const char *passkey);

//============================================================
// FUNCTION DEFINITIONS
//============================================================

// Fungsi untuk menghitung waktu kadaluarsa (15 menit dari sekarang)
void calculateExpiryTime(char *expiryTime) {
    time_t now = time(0);
    now += 15 * 60; // Tambah 15 menit
    tm *ltm = localtime(&now);

    sprintf(expiryTime, "%02d-%02d-%04d, %02d:%02d:%02d", ltm->tm_mday,
            ltm->tm_mon + 1, ltm->tm_year + 1900, ltm->tm_hour, ltm->tm_min,
            ltm->tm_sec);
}

// Fungsi untuk membandingkan waktu (return: -1 jika time1 < time2, 0 jika sama,
// 1 jika time1 > time2)
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

// Fungsi untuk mengecek dan mengupdate kursi yang kadaluarsa
void checkExpiredReservations() {
    char currentTime[20];
    getCurrentDateTime(currentTime);

    // Cek semua transaksi yang pending
    for (int i = 0; i < transactionCount; i++) {
        if (transactions[i].status == PENDING_PAYMENT &&
            transactions[i].isActive) {
            // Jika sudah kadaluarsa
            if (compareDateTime(currentTime, transactions[i].expiryTime) >= 0) {
                // Update status transaksi menjadi expired
                transactions[i].status = EXPIRED;

                // Bebaskan kursi yang di-reserve
                int auditorium = transactions[i].auditorium;
                char seatList[200];
                strcpy(seatList, transactions[i].selectedSeats);

                char *token = strtok(seatList, ",");
                while (token != NULL) {
                    // Trim whitespace
                    while (*token == ' ')
                        token++;

                    // Cari kursi dan ubah status menjadi available
                    for (int j = 0; j < seatCounts[auditorium - 1]; j++) {
                        if (strcmp(
                                auditoriumSeats[auditorium - 1][j].seatNumber,
                                token) == 0) {
                            auditoriumSeats[auditorium - 1][j].status =
                                AVAILABLE;
                            auditoriumSeats[auditorium - 1][j].userId = -1;
                            auditoriumSeats[auditorium - 1][j].transactionId =
                                -1;
                            strcpy(auditoriumSeats[auditorium - 1][j]
                                       .reservationTime,
                                   "");
                            break;
                        }
                    }
                    token = strtok(NULL, ",");
                }
            }
        }
    }
}

// Update fungsi inisialisasi kursi
void initializeSeats() {
    // Inisialisasi kursi untuk auditorium 1 (80 kursi: A-J, 1-8)
    int seatIndex = 0;
    for (char row = 'A'; row <= 'J'; row++) {
        for (int col = 1; col <= 8; col++) {
            sprintf(auditoriumSeats[0][seatIndex].seatNumber, "%c%02d", row,
                    col);
            auditoriumSeats[0][seatIndex].status = AVAILABLE;
            auditoriumSeats[0][seatIndex].userId = -1;
            auditoriumSeats[0][seatIndex].transactionId = -1;
            strcpy(auditoriumSeats[0][seatIndex].reservationTime, "");
            seatIndex++;
        }
    }

    // Inisialisasi kursi untuk auditorium 2 (80 kursi: A-J, 1-8)
    seatIndex = 0;
    for (char row = 'A'; row <= 'J'; row++) {
        for (int col = 1; col <= 8; col++) {
            sprintf(auditoriumSeats[1][seatIndex].seatNumber, "%c%02d", row,
                    col);
            auditoriumSeats[1][seatIndex].status = AVAILABLE;
            auditoriumSeats[1][seatIndex].userId = -1;
            auditoriumSeats[1][seatIndex].transactionId = -1;
            strcpy(auditoriumSeats[1][seatIndex].reservationTime, "");
            seatIndex++;
        }
    }

    // Inisialisasi kursi untuk auditorium 3 (60 kursi)
    seatIndex = 0;
    for (char row = 'A'; row <= 'G'; row++) {
        for (int col = 1; col <= 8; col++) {
            sprintf(auditoriumSeats[2][seatIndex].seatNumber, "%c%02d", row,
                    col);
            auditoriumSeats[2][seatIndex].status = AVAILABLE;
            auditoriumSeats[2][seatIndex].userId = -1;
            auditoriumSeats[2][seatIndex].transactionId = -1;
            strcpy(auditoriumSeats[2][seatIndex].reservationTime, "");
            seatIndex++;
        }
    }
    // Baris H hanya 4 kursi (H01-H04)
    for (int col = 1; col <= 4; col++) {
        sprintf(auditoriumSeats[2][seatIndex].seatNumber, "H%02d", col);
        auditoriumSeats[2][seatIndex].status = AVAILABLE;
        auditoriumSeats[2][seatIndex].userId = -1;
        auditoriumSeats[2][seatIndex].transactionId = -1;
        strcpy(auditoriumSeats[2][seatIndex].reservationTime, "");
        seatIndex++;
    }

    // Simulasi beberapa kursi sudah dibayar (taken)
    auditoriumSeats[0][5].status = TAKEN; // A06
    auditoriumSeats[0][5].userId = 0;
    auditoriumSeats[0][12].status = TAKEN; // B05
    auditoriumSeats[0][12].userId = 1;
    auditoriumSeats[1][20].status = TAKEN; // C05
    auditoriumSeats[1][20].userId = 0;
}

// Update fungsi display peta kursi
void displaySeatMap(int auditorium) {
    // Cek kursi yang kadaluarsa sebelum menampilkan
    checkExpiredReservations();

    clearScreen();
    // Fix: Gunakan sprintf atau cara lain tanpa to_string
    char headerTitle[100];
    sprintf(headerTitle, "PETA KURSI AUDITORIUM %d", auditorium);
    showHeader(headerTitle);

    cout << "\nNote: [  ] = Tersedia  [PP] = Pending  [TI] = Terisi\n" << endl;

    // Header kolom
    cout << "     ";
    for (int col = 1; col <= 8; col++) {
        printf("%02d   ", col);
    }
    cout << endl << endl;

    int seatIndex = 0;
    char currentRow = 'A';

    while (seatIndex < seatCounts[auditorium - 1]) {
        cout << currentRow << " | ";

        // Tampilkan kursi untuk baris ini
        int colCount = 0;
        while (seatIndex < seatCounts[auditorium - 1] &&
               auditoriumSeats[auditorium - 1][seatIndex].seatNumber[0] ==
                   currentRow) {

            switch (auditoriumSeats[auditorium - 1][seatIndex].status) {
            case AVAILABLE:
                cout << "[  ] ";
                break;
            case PENDING:
                cout << "[PD] ";
                break;
            case TAKEN:
                cout << "[TI] ";
                break;
            }

            seatIndex++;
            colCount++;
        }

        // Untuk auditorium 3, baris H hanya 4 kursi
        if (auditorium == 3 && currentRow == 'H') {
            for (int i = colCount; i < 8; i++) {
                cout << "     ";
            }
        }

        cout << endl;
        currentRow++;
    }

    cout << "\n                 [ LAYAR ]" << endl;
    cout << "\nContoh input kursi: A01,B05,F10 (pisahkan dengan koma)" << endl;
}

// Update fungsi validasi kursi
bool parseAndValidateSeats(const char *seatInput, int auditorium,
                           char validSeats[][4], int &validCount) {
    validCount = 0;
    char tempInput[200];
    strcpy(tempInput, seatInput);

    // Split berdasarkan koma
    char *token = strtok(tempInput, ",");
    while (token != NULL && validCount < 10) {
        // Trim whitespace
        while (*token == ' ')
            token++;

        // Konversi ke uppercase
        for (int i = 0; token[i]; i++) {
            token[i] = toupper(token[i]);
        }

        // Validasi format
        if (strlen(token) != 3 || !isalpha(token[0]) || !isdigit(token[1]) ||
            !isdigit(token[2])) {
            cout << "\nFormat kursi tidak valid: " << token
                 << " (gunakan format A01, B05, dll)" << endl;
            return false;
        }

        // Cek apakah kursi exist dan statusnya
        bool seatExists = false;
        SeatStatus seatStatus;

        for (int i = 0; i < seatCounts[auditorium - 1]; i++) {
            if (strcmp(auditoriumSeats[auditorium - 1][i].seatNumber, token) ==
                0) {
                seatExists = true;
                seatStatus = auditoriumSeats[auditorium - 1][i].status;
                break;
            }
        }

        if (!seatExists) {
            cout << "\nKursi " << token << " tidak tersedia di auditorium "
                 << auditorium << endl;
            return false;
        }

        if (seatStatus == TAKEN) {
            cout << "\nKursi " << token << " sudah terisi!" << endl;
            return false;
        }

        if (seatStatus == PENDING) {
            cout << "\nKursi " << token << " sedang dalam proses pembayaran!"
                 << endl;
            return false;
        }

        // Cek duplikasi
        for (int i = 0; i < validCount; i++) {
            if (strcmp(validSeats[i], token) == 0) {
                cout << "\nKursi " << token << " duplikat dalam input!" << endl;
                return false;
            }
        }

        strcpy(validSeats[validCount], token);
        validCount++;

        token = strtok(NULL, ",");
    }

    if (validCount == 0) {
        cout << "\nTidak ada kursi yang valid!" << endl;
        return false;
    }

    return true;
}

// Fungsi untuk membersihkan layar
void clearScreen() { system("cls"); }

// Fungsi untuk menampilkan header
void showHeader(const char *title) {
    cout << "============================================================"
         << endl;
    cout << "                    " << title << endl;
    cout << "============================================================"
         << endl;
}

// Fungsi untuk reserve kursi (status pending)
void reserveSeats(const char validSeats[][4], int seatCount, int auditorium,
                  int transactionId) {
    char currentTime[20];
    getCurrentDateTime(currentTime);

    for (int i = 0; i < seatCount; i++) {
        for (int j = 0; j < seatCounts[auditorium - 1]; j++) {
            if (strcmp(auditoriumSeats[auditorium - 1][j].seatNumber,
                       validSeats[i]) == 0) {
                auditoriumSeats[auditorium - 1][j].status = PENDING;
                auditoriumSeats[auditorium - 1][j].userId = currentUserIndex;
                auditoriumSeats[auditorium - 1][j].transactionId =
                    transactionId;
                strcpy(auditoriumSeats[auditorium - 1][j].reservationTime,
                       currentTime);
                break;
            }
        }
    }
}

// Fungsi untuk konfirmasi pembayaran
void confirmPayment(int transactionId) {
    // Update status transaksi
    transactions[transactionId].status = PAID;

    // Update status kursi menjadi taken
    int auditorium = transactions[transactionId].auditorium;
    char seatList[200];
    strcpy(seatList, transactions[transactionId].selectedSeats);

    char *token = strtok(seatList, ",");
    while (token != NULL) {
        while (*token == ' ')
            token++;

        for (int j = 0; j < seatCounts[auditorium - 1]; j++) {
            if (strcmp(auditoriumSeats[auditorium - 1][j].seatNumber, token) ==
                0) {
                auditoriumSeats[auditorium - 1][j].status = TAKEN;
                break;
            }
        }
        token = strtok(NULL, ",");
    }
}

// Fungsi untuk membatalkan reservasi
void cancelReservation(int transactionId) {
    // Update status transaksi
    transactions[transactionId].status = CANCELLED;

    // Bebaskan kursi
    int auditorium = transactions[transactionId].auditorium;
    char seatList[200];
    strcpy(seatList, transactions[transactionId].selectedSeats);

    char *token = strtok(seatList, ",");
    while (token != NULL) {
        while (*token == ' ')
            token++;

        for (int j = 0; j < seatCounts[auditorium - 1]; j++) {
            if (strcmp(auditoriumSeats[auditorium - 1][j].seatNumber, token) ==
                0) {
                auditoriumSeats[auditorium - 1][j].status = AVAILABLE;
                auditoriumSeats[auditorium - 1][j].userId = -1;
                auditoriumSeats[auditorium - 1][j].transactionId = -1;
                strcpy(auditoriumSeats[auditorium - 1][j].reservationTime, "");
                break;
            }
        }
        token = strtok(NULL, ",");
    }
}

// Fungsi untuk menangani pemilihan F&B
int handleFnBSelection(SelectedFnB selectedFnB[]) {
    int selectedCount = 0;

    while (true) {
        clearScreen();
        showHeader("PILIH MAKANAN & MINUMAN");

        cout << "\n| No | Nama                | Harga   | Kategori |" << endl;
        cout << "|----|---------------------|---------|----------|" << endl;

        for (int i = 0; i < fnbCount; i++) {
            if (foodBeverages[i].isAvailable) {
                printf("| %-2d | %-19s | %-7d | %-8s |\n", i + 1,
                       foodBeverages[i].name, foodBeverages[i].price,
                       foodBeverages[i].category);
            }
        }

        cout << "| 0  | Selesai memilih     |         |          |" << endl;

        if (selectedCount > 0) {
            cout << "\nItem yang sudah dipilih:" << endl;
            for (int i = 0; i < selectedCount; i++) {
                printf("- %dx %s = Rp %d\n", selectedFnB[i].quantity,
                       selectedFnB[i].name, selectedFnB[i].totalPrice);
            }
        }

        cout << "\nPilih nomor F&B (0 untuk selesai): ";
        int choice = getNumberInput();

        if (choice == 0) {
            break;
        } else if (choice >= 1 && choice <= fnbCount &&
                   foodBeverages[choice - 1].isAvailable) {
            cout << "\nJumlah yang ingin dipesan: ";
            int quantity;
            cin >> quantity;

            if (quantity > 0 && quantity <= 10) {
                selectedFnB[selectedCount].fnbId = foodBeverages[choice - 1].id;
                strcpy(selectedFnB[selectedCount].name,
                       foodBeverages[choice - 1].name);
                selectedFnB[selectedCount].quantity = quantity;
                selectedFnB[selectedCount].price =
                    foodBeverages[choice - 1].price;
                selectedFnB[selectedCount].totalPrice =
                    quantity * foodBeverages[choice - 1].price;
                selectedCount++;

                cout << "\nItem berhasil ditambahkan!" << endl;
                pauseScreen();
            } else {
                cout << "\nJumlah tidak valid!" << endl;
                pauseScreen();
            }
        } else {
            cout << "\nPilihan tidak valid!" << endl;
            pauseScreen();
        }
    }

    return selectedCount;
}

// Update fungsi processTicketBooking dengan sistem pending
void processTicketBooking(int movieIndex, int showtimeIndex) {
    Movie movie = movies[movieIndex];
    Showtime showtime = movie.showtimes[showtimeIndex];

    // Tampilkan peta kursi
    displaySeatMap(showtime.auditorium);

    // Input kursi
    char seatInput[200];
    char validSeats[10][4];
    int seatCount = 0;

    cout << "\nMasukkan nomor kursi yang diinginkan: ";
    cin >> seatInput;

    if (!parseAndValidateSeats(seatInput, showtime.auditorium, validSeats,
                               seatCount)) {
        pauseScreen();
        return;
    }

    // Tampilkan kursi yang dipilih
    cout << "\nKursi yang dipilih:" << endl;
    for (int i = 0; i < seatCount; i++) {
        cout << "- " << validSeats[i] << endl;
    }

    int ticketPrice = seatCount * showtime.price;
    cout << "\nJumlah tiket: " << seatCount << endl;
    cout << "Harga per tiket: Rp " << showtime.price << endl;
    cout << "Total harga tiket: Rp " << ticketPrice << endl;

    cout << "\nKonfirmasi pemilihan kursi? (Y/N): ";
    char confirmSeat = getSingleInput();
    if (confirmSeat != 'Y' && confirmSeat != 'y') {
        cout << "\nPemilihan kursi dibatalkan." << endl;
        pauseScreen();
        return;
    }

    // Pilih F&B (opsional)
    SelectedFnB selectedFnB[10];
    int fnbItemCount = 0;
    int fnbTotalPrice = 0;

    cout << "\nIngin menambah F&B? (Y/N): ";
    char wantFnB = getSingleInput();

    if (wantFnB == 'Y' || wantFnB == 'y') {
        fnbItemCount = handleFnBSelection(selectedFnB);
        for (int i = 0; i < fnbItemCount; i++) {
            fnbTotalPrice += selectedFnB[i].totalPrice;
        }
    }

    // Buat transaksi dengan status pending
    int currentTransactionId = transactionCount;

    transactions[currentTransactionId].id = currentTransactionId + 1;
    transactions[currentTransactionId].userId = currentUserIndex;
    transactions[currentTransactionId].movieId = movie.id;
    transactions[currentTransactionId].showtimeIndex = showtimeIndex;
    transactions[currentTransactionId].ticketCount = seatCount;

    // Format selected seats
    strcpy(transactions[currentTransactionId].selectedSeats, "");
    for (int i = 0; i < seatCount; i++) {
        strcat(transactions[currentTransactionId].selectedSeats, validSeats[i]);
        if (i < seatCount - 1)
            strcat(transactions[currentTransactionId].selectedSeats, ",");
    }

    // Simpan F&B
    transactions[currentTransactionId].fnbCount = fnbItemCount;
    for (int i = 0; i < fnbItemCount; i++) {
        transactions[currentTransactionId].selectedFnB[i] = selectedFnB[i];
    }

    transactions[currentTransactionId].ticketPrice = ticketPrice;
    transactions[currentTransactionId].fnbPrice = fnbTotalPrice;
    transactions[currentTransactionId].totalPrice = ticketPrice + fnbTotalPrice;
    transactions[currentTransactionId].auditorium = showtime.auditorium;

    getCurrentDateTime(transactions[currentTransactionId].transactionDate);
    calculateExpiryTime(transactions[currentTransactionId].expiryTime);
    transactions[currentTransactionId].status = PENDING_PAYMENT;
    transactions[currentTransactionId].isActive = true;

    strcpy(transactions[currentTransactionId].movieTitle, movie.title);
    strcpy(transactions[currentTransactionId].showtime, showtime.time);

    // Initialize booking code dan passkey
    strcpy(transactions[currentTransactionId].bookingCode, "");
    strcpy(transactions[currentTransactionId].passkey, "");

    // Reserve kursi dengan status pending
    reserveSeats(validSeats, seatCount, showtime.auditorium,
                 currentTransactionId);

    // Tampilkan ringkasan dan lanjut ke pembayaran
    processPayment(currentTransactionId);
    transactionCount++;
}

// Fungsi untuk menampilkan konfirmasi tiket
void showTicketConfirmation(int transactionId) {
    clearScreen();
    showHeader("KONFIRMASI TIKET");

    Transaction trans = transactions[transactionId];

    cout << "\n==================== E-TICKET MOVTIX ===================="
         << endl;
    cout << "Kode Booking   : MOVTIX" << setfill('0') << setw(4) << trans.id
         << endl;
    cout << "Film           : " << trans.movieTitle << endl;
    cout << "Tanggal        : " << trans.transactionDate << endl;
    cout << "Jam Tayang     : " << trans.showtime << endl;
    cout << "Auditorium     : " << trans.auditorium << endl;
    cout << "Kursi          : " << trans.selectedSeats << endl;
    cout << "Jumlah Tiket   : " << trans.ticketCount << endl;

    if (trans.fnbCount > 0) {
        cout << "\nF&B yang dipesan:" << endl;
        for (int i = 0; i < trans.fnbCount; i++) {
            printf("- %dx %s = Rp %d\n", trans.selectedFnB[i].quantity,
                   trans.selectedFnB[i].name, trans.selectedFnB[i].totalPrice);
        }
    }

    cout << "\nTotal Pembayaran: Rp " << trans.totalPrice << endl;
    cout << "Status          : LUNAS" << endl;
    cout << "=========================================================" << endl;

    cout << "\nTiket berhasil dibeli!" << endl;
    cout << "Simpan kode booking untuk penukaran tiket di bioskop." << endl;
    pauseScreen();
}

// Fungsi untuk generate kode booking random
void generateBookingCode(char *bookingCode) {
    const char chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    for (int i = 0; i < 8; i++) {
        bookingCode[i] = chars[rand() % 36];
    }
    bookingCode[8] = '\0';
}

// Fungsi untuk generate passkey random
void generatePasskey(char *passkey) {
    for (int i = 0; i < 6; i++) {
        passkey[i] = '0' + (rand() % 10);
    }
    passkey[6] = '\0';
}

// Fungsi untuk mendeteksi jenis kartu
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

// Fungsi untuk validasi kartu kredit
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

// Fungsi untuk mencari transaksi berdasarkan booking code dan passkey
int findTransactionByCode(const char *bookingCode, const char *passkey) {
    for (int i = 0; i < transactionCount; i++) {
        if (transactions[i].status == PAID &&
            strcmp(transactions[i].bookingCode, bookingCode) == 0 &&
            strcmp(transactions[i].passkey, passkey) == 0) {
            return i;
        }
    }
    return -1; // Tidak ditemukan
}

// Fungsi untuk validasi input kode booking
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

// Fungsi untuk validasi input passkey
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

// Fungsi utama untuk penukaran tiket dan F&B
void handleTicketExchange() {
    char bookingCode[15];
    char passkey[15];

    while (true) {
        clearScreen();
        showHeader("PENUKARAN TIKET & F&B");

        // Input kode booking
        cout << "\nMasukkan kode booking (8 karakter): ";
        cin >> bookingCode;

        // Validasi format kode booking
        if (!validateBookingCode(bookingCode)) {
            cout << "\nFormat kode booking tidak valid!" << endl;
            cout << "Kode booking harus 8 karakter (huruf kapital dan angka)"
                 << endl;
            cout << "Contoh: ZXH0DA1Y" << endl;
            pauseScreen();
            continue;
        }

        // Input passkey
        cout << "Masukkan passkey (6 digit): ";
        cin >> passkey;

        // Validasi format passkey
        if (!validatePasskey(passkey)) {
            cout << "\nFormat passkey tidak valid!" << endl;
            cout << "Passkey harus 6 digit angka" << endl;
            cout << "Contoh: 123456" << endl;
            pauseScreen();
            continue;
        }

        // Cari transaksi berdasarkan kode
        int transactionIndex = findTransactionByCode(bookingCode, passkey);

        if (transactionIndex != -1) {
            // Jika valid
            transactions[transactionIndex].status = USED;

            cout << "\n[Penukaran tiket & F&B berhasil! Selamat menonton.]"
                 << endl;
            pauseScreen();
            return;

        } else {
            // Jika tidak valid
            cout << "\n[Kode booking atau passkey salah / sudah digunakan.]"
                 << endl;

            cout << "\nCoba lagi? (Y/N): ";
            char choice = getSingleInput();

            if (choice != 'Y' && choice != 'y') {
                return;
            }
        }
    }
}

// Fungsi untuk proses pembayaran kartu
bool processCardPayment(int totalAmount, PaymentInfo *paymentInfo) {
    clearScreen();
    showHeader("PEMBAYARAN KARTU");

    cout << "\nMetode pembayaran: Kartu (Debit/Kredit)" << endl;
    cout << "\nTotal belanja        : Rp " << totalAmount << endl;
    cout << "Biaya admin          : Rp 5.000" << endl;
    cout << "-------------------------------" << endl;
    cout << "Total yang harus dibayar: Rp " << (totalAmount + 5000) << endl;

    char cardNumber[50];
    char cardName[50];
    char expiry[10];
    char cvv[5];

    cout << "\nMasukkan nomor kartu (16 digit): ";
    cin.getline(cardNumber, 50);

    cout << "Masukkan nama di kartu: ";
    cin.getline(cardName, 50);

    cout << "Masa berlaku (MM/YY): ";
    cin.getline(expiry, 10);

    cout << "Kode CVV (3 digit): ";
    cin.getline(cvv, 5);

    if (!validateCard(cardNumber, expiry, cvv)) {
        pauseScreen();
        return false;
    }

    const char *cardType = detectCardType(cardNumber);
    cout << "\nJenis kartu terdeteksi: " << cardType << endl;

    if (strcmp(cardType, "Unknown") == 0) {
        cout << "\nJenis kartu tidak didukung! Hanya menerima Visa dan "
                "Mastercard."
             << endl;
        pauseScreen();
        return false;
    }

    cout << "\n[Konfirmasi pembayaran? 1. Ya  0. Batal]: ";
    char confirm = getSingleInput();

    if (confirm != '1') {
        cout << "\nPembayaran dibatalkan." << endl;
        pauseScreen();
        return false;
    }

    // Simpan info pembayaran
    paymentInfo->method = CARD_PAYMENT;
    strcpy(paymentInfo->cardNumber, cardNumber);
    strcpy(paymentInfo->cardName, cardName);
    strcpy(paymentInfo->expiry, expiry);
    strcpy(paymentInfo->cvv, cvv);

    cout << "\nPembayaran sedang diproses..." << endl;
    // Simulasi delay
    for (int i = 0; i < 3; i++) {
        cout << ".";
        // Simple delay simulation
        for (int j = 0; j < 100000000; j++)
            ;
    }
    cout << endl;

    cout << "Pembayaran berhasil!" << endl;
    pauseScreen();
    return true;
}

// Fungsi untuk proses pembayaran bank transfer
bool processBankPayment(int totalAmount, PaymentInfo *paymentInfo) {
    clearScreen();
    showHeader("TRANSFER BANK");

    cout << "\nPilih bank tujuan:" << endl;
    cout << "1. BCA" << endl;
    cout << "2. Mandiri" << endl;
    cout << "3. BNI" << endl;
    cout << "4. BRI" << endl;
    cout << "0. Batal" << endl;
    cout << "\nPilihan [0-4]: ";

    char choice = getSingleInput();

    const char *bankNames[] = {"", "BCA", "Mandiri", "BNI", "BRI"};
    const char *accountNumbers[] = {"", "1234567890", "9876543210",
                                    "1357924680", "2468013579"};

    if (choice < '1' || choice > '4') {
        if (choice == '0') {
            cout << "\nTransfer dibatalkan." << endl;
        } else {
            cout << "\nPilihan tidak valid!" << endl;
        }
        pauseScreen();
        return false;
    }

    int bankIndex = choice - '0';

    cout << "\n=== INFORMASI TRANSFER ===" << endl;
    cout << "Bank Tujuan    : " << bankNames[bankIndex] << endl;
    cout << "No. Rekening   : " << accountNumbers[bankIndex] << endl;
    cout << "Atas Nama     : MOVTIX INDONESIA" << endl;
    cout << "Jumlah Transfer: Rp " << totalAmount << endl;
    cout << "=========================" << endl;

    cout << "\nSilakan lakukan transfer dan tekan 1 jika sudah selesai, 0 "
            "untuk batal: ";
    char confirm = getSingleInput();

    if (confirm != '1') {
        cout << "\nTransfer dibatalkan atau belum selesai." << endl;
        pauseScreen();
        return false;
    }

    // Simpan info pembayaran
    paymentInfo->method = BANK_TRANSFER;
    strcpy(paymentInfo->bankName, bankNames[bankIndex]);

    cout << "\nTransfer berhasil dikonfirmasi!" << endl;
    pauseScreen();
    return true;
}

// Fungsi untuk proses pembayaran poin
bool processPointsPayment(int totalAmount, PaymentInfo *paymentInfo) {
    clearScreen();
    showHeader("PEMBAYARAN POIN MOVTIX");

    int userPoints = users[currentUserIndex].movtixPoints;
    int requiredPoints = totalAmount / 100; // 1 poin = Rp 100

    cout << "\nTotal pembayaran: Rp " << totalAmount << endl;
    cout << "Poin dibutuhkan : " << requiredPoints << " poin" << endl;
    cout << "Poin Anda       : " << userPoints << " poin" << endl;

    if (userPoints < requiredPoints) {
        cout << "\nMaaf, poin Anda tidak mencukupi!" << endl;
        cout << "Kekurangan      : " << (requiredPoints - userPoints) << " poin"
             << endl;
        pauseScreen();
        return false;
    }

    cout << "\nSisa poin setelah pembayaran: " << (userPoints - requiredPoints)
         << " poin" << endl;
    cout << "\n[Konfirmasi pembayaran dengan poin? 1. Ya  0. Batal]: ";

    char confirm = getSingleInput();

    if (confirm != '1') {
        cout << "\nPembayaran dibatalkan." << endl;
        pauseScreen();
        return false;
    }

    // Kurangi poin user
    users[currentUserIndex].movtixPoints -= requiredPoints;

    // Simpan info pembayaran
    paymentInfo->method = MOVTIX_POINTS;
    paymentInfo->pointsUsed = requiredPoints;

    cout << "\nPembayaran dengan poin berhasil!" << endl;
    cout << "Poin tersisa: " << users[currentUserIndex].movtixPoints << " poin"
         << endl;
    pauseScreen();
    return true;
}

// Fungsi untuk proses pembayaran
void processPayment(int transactionId) {
    while (true) {
        char currentTime[20];
        getCurrentDateTime(currentTime);

        if (compareDateTime(currentTime,
                            transactions[transactionId].expiryTime) >= 0) {
            clearScreen();
            showHeader("WAKTU PEMBAYARAN HABIS");
            cout << "\nMaaf, waktu pembayaran telah habis!" << endl;
            cout << "Reservasi kursi Anda telah dibatalkan." << endl;
            cancelReservation(transactionId);
            pauseScreen();
            return;
        }

        clearScreen();
        showHeader("PEMBAYARAN TIKET");

        Transaction trans = transactions[transactionId];

        cout << "\nID Transaksi   : MOVTIX" << setfill('0') << setw(4)
             << trans.id << endl;
        cout << "Film           : " << trans.movieTitle << endl;
        cout << "Jadwal         : " << trans.showtime << endl;
        cout << "Kursi          : " << trans.selectedSeats << endl;
        cout << "Jumlah Tiket   : " << trans.ticketCount << endl;

        if (trans.fnbCount > 0) {
            cout << "\nF&B yang dipesan:" << endl;
            for (int i = 0; i < trans.fnbCount; i++) {
                printf("- %dx %s = Rp %d\n", trans.selectedFnB[i].quantity,
                       trans.selectedFnB[i].name,
                       trans.selectedFnB[i].totalPrice);
            }
        }

        cout << "\nTotal Pembayaran: Rp " << trans.totalPrice << endl;
        cout << "Batas Waktu     : " << trans.expiryTime << endl;
        cout << "Waktu Sekarang  : " << currentTime << endl;

        int timeDiff = compareDateTime(trans.expiryTime, currentTime);
        if (timeDiff > 0) {
            cout << "Status          : Masih dalam batas waktu" << endl;
        } else {
            cout << "Status          : HAMPIR HABIS!" << endl;
        }

        cout << "\nPilihan:" << endl;
        cout << "1. Bayar Sekarang" << endl;
        cout << "2. Kembali ke Menu Utama (Reservasi tetap aktif)" << endl;
        cout << "3. Batalkan Pesanan" << endl;
        cout << "\nPilihan Anda [1-3]: ";

        char choice = getSingleInput();

        switch (choice) {
        case '1': {
            // Menu pilih metode pembayaran
            while (true) {
                clearScreen();
                showHeader("PILIH METODE PEMBAYARAN");

                cout << "\nTotal yang harus dibayar: Rp " << trans.totalPrice
                     << endl;
                cout << "\nPilih Metode Pembayaran:" << endl;
                cout << "1. Transfer Bank" << endl;
                cout << "2. Kartu (Debit/Kredit)" << endl;
                cout << "3. Poin MovTix (Saldo: "
                     << users[currentUserIndex].movtixPoints << " poin)"
                     << endl;
                cout << "0. Kembali ke menu pembayaran" << endl;
                cout << "\n[Pilih 0-3]: ";

                char paymentChoice = getSingleInput();
                PaymentInfo paymentInfo = {};
                bool paymentSuccess = false;

                switch (paymentChoice) {
                case '1':
                    paymentSuccess =
                        processBankPayment(trans.totalPrice, &paymentInfo);
                    break;
                case '2':
                    // Clear input buffer sebelum getline
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    paymentSuccess =
                        processCardPayment(trans.totalPrice, &paymentInfo);
                    break;
                case '3':
                    paymentSuccess =
                        processPointsPayment(trans.totalPrice, &paymentInfo);
                    break;
                case '0':
                    break;
                default:
                    cout << "\nPilihan tidak valid!" << endl;
                    pauseScreen();
                    continue;
                }

                if (paymentChoice == '0')
                    break;

                if (paymentSuccess) {
                    // Generate booking code dan passkey
                    generateBookingCode(
                        transactions[transactionId].bookingCode);
                    generatePasskey(transactions[transactionId].passkey);

                    // Simpan payment info
                    transactions[transactionId].paymentInfo = paymentInfo;

                    // Konfirmasi pembayaran
                    confirmPayment(transactionId);

                    // Berikan poin untuk transaksi yang berhasil (jika tidak
                    // pakai poin)
                    if (paymentInfo.method != MOVTIX_POINTS) {
                        int earnedPoints =
                            trans.totalPrice / 1000; // 1 poin per Rp 1000
                        users[currentUserIndex].movtixPoints += earnedPoints;
                    }

                    // Update popularitas
                    for (int i = 0; i < movieCount; i++) {
                        if (movies[i].id == trans.movieId) {
                            movies[i].popularity += trans.ticketCount;
                            break;
                        }
                    }

                    // Tampilkan konfirmasi dengan kode booking
                    clearScreen();
                    showHeader("PEMBAYARAN BERHASIL");

                    cout << "\nPembayaran berhasil!" << endl;
                    cout << "\nKode Booking : "
                         << transactions[transactionId].bookingCode << endl;
                    cout << "Passkey      : "
                         << transactions[transactionId].passkey << endl;

                    if (paymentInfo.method != MOVTIX_POINTS) {
                        cout << "\nAnda mendapat " << (trans.totalPrice / 1000)
                             << " poin MovTix!" << endl;
                        cout << "Total poin   : "
                             << users[currentUserIndex].movtixPoints << " poin"
                             << endl;
                    }

                    cout << "\nSimpan kode booking & passkey untuk masuk "
                            "studio dan klaim F&B."
                         << endl;
                    cout << "\n[Tekan ENTER untuk kembali ke menu utama.]"
                         << endl;
                    pauseScreen();

                    return;
                }
            }
            break;
        }
        case '2':
            cout << "\nReservasi kursi masih aktif sampai " << trans.expiryTime
                 << endl;
            cout << "Anda dapat melanjutkan pembayaran melalui menu Histori "
                    "Transaksi."
                 << endl;
            pauseScreen();
            return;

        case '3': {
            cout << "\nApakah Anda yakin ingin membatalkan pesanan? (Y/N): ";
            char confirm = getSingleInput();
            if (confirm == 'Y' || confirm == 'y') {
                cancelReservation(transactionId);
                cout << "\nPesanan dibatalkan. Kursi telah dibebaskan." << endl;
                pauseScreen();
                return;
            }
            break;
        }
        default:
            cout << "\nPilihan tidak valid!" << endl;
            pauseScreen();
            break;
        }
    }
}

// Update inisialisasi user dengan poin
void initializeUsers() {
    strcpy(users[0].nama, "Admin MOVTIX");
    strcpy(users[0].username, "admin");
    strcpy(users[0].email, "admin@movtix.com");
    strcpy(users[0].password, "admin123");
    users[0].movtixPoints = 10000;

    strcpy(users[1].nama, "Reza Asriano Maulana");
    strcpy(users[1].username, "reza");
    strcpy(users[1].email, "reza@movtix.com");
    strcpy(users[1].password, "reza");
    users[1].movtixPoints = 5000;

    users[0].isActive = true;
    users[1].isActive = true;
    userCount = 2;
}

// Fungsi untuk inisialisasi F&B
void initializeFnB() {
    // Makanan
    foodBeverages[0] = {1, "Popcorn Regular", 25000, "Food", true};
    foodBeverages[1] = {2, "Popcorn Large", 35000, "Food", true};
    foodBeverages[2] = {3, "Nachos", 30000, "Food", true};
    foodBeverages[3] = {4, "Hot Dog", 20000, "Food", true};
    foodBeverages[4] = {5, "French Fries", 22000, "Food", true};

    // Minuman
    foodBeverages[5] = {6, "Coca Cola Regular", 15000, "Beverage", true};
    foodBeverages[6] = {7, "Coca Cola Large", 20000, "Beverage", true};
    foodBeverages[7] = {8, "Orange Juice", 18000, "Beverage", true};
    foodBeverages[8] = {9, "Mineral Water", 10000, "Beverage", true};
    foodBeverages[9] = {10, "Ice Coffee", 25000, "Beverage", true};

    fnbCount = 10;
}

void getCurrentDateTime(char *dateTime) {
    time_t now = time(0);
    tm *ltm = localtime(&now);

    sprintf(dateTime, "%02d-%02d-%04d, %02d:%02d:%02d", ltm->tm_mday,
            ltm->tm_mon + 1, ltm->tm_year + 1900, ltm->tm_hour, ltm->tm_min,
            ltm->tm_sec);
}

// Tambahkan fungsi untuk mendapatkan waktu saja (untuk validasi jadwal)
void getCurrentTime(char *timeStr) {
    time_t now = time(0);
    tm *ltm = localtime(&now);

    sprintf(timeStr, "%02d:%02d", ltm->tm_hour, ltm->tm_min);
}

// Fungsi untuk membandingkan waktu (format HH:MM)
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

// Fungsi untuk menampilkan jadwal film yang tersedia
void showMovieSchedule(int movieIndex) {
    clearScreen();
    showHeader("JADWAL TAYANG FILM");

    Movie movie = movies[movieIndex];
    char currentTime[6];
    getCurrentTime(currentTime);

    cout << "\nFilm: " << movie.title << endl;
    cout << "Waktu saat ini: " << currentTime << endl;

    cout << "\n| No | Waktu | Audi | Kapasitas | Harga     | Status    |"
         << endl;
    cout << "|----|-------|------|-----------|-----------|-----------|" << endl;

    bool hasAvailableShow = false;
    for (int i = 0; i < movie.showtimeCount; i++) {
        char status[15];
        bool canBook = compareTime(currentTime, movie.showtimes[i].time) < 0;

        if (canBook) {
            strcpy(status, "Tersedia");
            hasAvailableShow = true;
        } else {
            strcpy(status, "Berakhir");
        }

        printf("| %-2d | %-5s | %-4d | %-9d | Rp %-6d | %-9s |\n", i + 1,
               movie.showtimes[i].time, movie.showtimes[i].auditorium,
               movie.showtimes[i].capacity, movie.showtimes[i].price, status);
    }

    if (!hasAvailableShow) {
        cout << "\n[Maaf, tidak ada jadwal yang tersedia untuk hari ini]"
             << endl;
        pauseScreen();
        return;
    }

    cout << "\n[Pilih nomor jadwal untuk pesan tiket, 0 untuk kembali]" << endl;
    cout << "\nPilihan Anda: ";

    int input = getSingletDigit();
    if (input >= 1 && input <= movie.showtimeCount) {
        // Validasi jadwal masih tersedia
        if (compareTime(currentTime, movie.showtimes[input - 1].time) < 0) {
            processTicketBooking(movieIndex, input - 1);
        } else {
            cout << "\n[Jadwal sudah berakhir! Silakan pilih jadwal lain.]"
                 << endl;
            pauseScreen();
            showMovieSchedule(movieIndex); // Kembali ke pemilihan jadwal
        }
    } else if (input != 0) {
        cout << "\nPilihan tidak valid!" << endl;
        pauseScreen();
        showMovieSchedule(movieIndex);
    }
}

// Fungsi utama untuk handle pemesanan tiket
void handleTicketBooking() {
    while (true) {
        clearScreen();
        showHeader("PESAN TIKET FILM");

        char currentDateTime[20];
        getCurrentDateTime(currentDateTime);
        cout << "\nWaktu saat ini: " << currentDateTime << endl;

        cout << "\n| No | Judul                  | Genre     | Rating | Jadwal "
                "Tersedia |"
             << endl;
        cout << "|----|------------------------|-----------|--------|----------"
                "-------|"
             << endl;

        for (int i = 0; i < movieCount; i++) {
            if (movies[i].isActive) {
                // Hitung jadwal yang masih tersedia
                char currentTime[6];
                getCurrentTime(currentTime);
                int availableShows = 0;

                for (int j = 0; j < movies[i].showtimeCount; j++) {
                    if (compareTime(currentTime, movies[i].showtimes[j].time) <
                        0) {
                        availableShows++;
                    }
                }

                printf("| %-2d | %-22s | %-9s | %-6s | %8d show |\n", i + 1,
                       movies[i].title, movies[i].genre, movies[i].rating,
                       availableShows);
            }
        }

        cout << "\n[Pilih nomor film untuk lihat jadwal, 0 untuk kembali]"
             << endl;
        cout << "\nPilihan Anda: ";

        int input = getSingletDigit();
        if (input >= 1 && input <= movieCount) {
            showMovieSchedule(input - 1);
        } else if (input == 0) {
            break; // Kembali ke menu utama
        } else {
            cout << "\nInput tidak valid!" << endl;
            pauseScreen();
        }
    }
}

// Fungsi untuk mencari pengguna berdasarkan username
int findUserByUsername(const char *username) {
    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].username, username) == 0) {
            return i;
        }
    }
    return -1;
}

// Fungsi untuk mencari pengguna berdasarkan email
int findUserByEmail(const char *email) {
    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].email, email) == 0) {
            return i;
        }
    }
    return -1;
}

// Fungsi untuk validasi email sederhana
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

void clearInputBuffer() {
    while (kbhit()) {
        getch();
    }
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

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

// Fungsi untuk login
bool login() {
    clearScreen();
    showHeader("LOGIN MOVTIX");

    char username[50];
    char password[50];

    cout << "\nMasukkan Username : ";
    cin >> username;
    cout << "Masukkan Password : ";
    cin >> password;

    int userIndex = findUserByUsername(username);

    if (userIndex != -1 && strcmp(users[userIndex].password, password) == 0) {
        currentUserIndex = userIndex;
        cout << "\n[Login berhasil! Selamat datang, " << users[userIndex].nama
             << ".]" << endl;
        pauseScreen();
        return true;
    } else {
        cout << "\n[Login gagal. Silakan periksa username dan password Anda.]"
             << endl;
        pauseScreen();
        return false;
    }
}

// Fungsi untuk registrasi
bool registerUser() {
    clearScreen();
    showHeader("PENDAFTARAN AKUN BARU");

    if (userCount >= 100) {
        cout << "\n[Maaf, database pengguna penuh!]" << endl;
        pauseScreen();
        return false;
    }

    char nama[100];
    char username[50];
    char email[100];
    char password[50];
    char confirmPassword[50];

    cout << "\nNama Lengkap      : ";
    cin >> nama;

    cout << "Username          : ";
    cin >> username;

    // Validasi username unik
    if (findUserByUsername(username) != -1) {
        cout << "\n[Username sudah digunakan. Silakan pilih username lain.]"
             << endl;
        pauseScreen();
        return false;
    }

    cout << "Email             : ";
    cin >> email;

    // Validasi email
    if (!isValidEmail(email)) {
        cout << "\n[Format email tidak valid.]" << endl;
        pauseScreen();
        return false;
    }

    // Validasi email unik
    if (findUserByEmail(email) != -1) {
        cout << "\n[Email sudah terdaftar. Silakan gunakan email lain.]"
             << endl;
        pauseScreen();
        return false;
    }

    cout << "Password          : ";
    cin >> password;

    cout << "Konfirmasi Pass   : ";
    cin >> confirmPassword;

    // Validasi password match
    if (strcmp(password, confirmPassword) != 0) {
        cout << "\n[Password tidak cocok. Silakan coba lagi.]" << endl;
        pauseScreen();
        return false;
    }

    // Simpan data pengguna baru
    strcpy(users[userCount].nama, nama);
    strcpy(users[userCount].username, username);
    strcpy(users[userCount].email, email);
    strcpy(users[userCount].password, password);
    users[userCount].isActive = true;
    userCount++;

    cout << "\n[Registrasi berhasil! Silakan login untuk melanjutkan.]" << endl;
    pauseScreen();
    return true;
}

// Fungsi untuk reset password
bool resetPassword() {
    clearScreen();
    showHeader("RESET PASSWORD");

    char usernameOrEmail[100];
    char correctCode[] = "123456";
    char inputCode[10];
    char newPassword[50];
    char confirmPassword[50];

    cout << "\nMasukkan username/email Anda : ";
    cin >> usernameOrEmail;

    int userIndex = findUserByUsername(usernameOrEmail);
    if (userIndex == -1) {
        userIndex = findUserByEmail(usernameOrEmail);
    }

    if (userIndex == -1) {
        cout << "\n[Username/email tidak ditemukan.]" << endl;
        pauseScreen();
        return false;
    }

    // Simulasi pengiriman kode verifikasi
    cout << "\n[Kode verifikasi telah dikirim ke email Anda.]" << endl;

    cout << "Masukkan kode verifikasi    : ";
    cin >> inputCode;

    if (strcmp(inputCode, correctCode) != 0) {
        cout << "\n[Kode verifikasi salah.]" << endl;
        pauseScreen();
        return false;
    }

    cout << "Masukkan password baru      : ";
    cin >> newPassword;

    cout << "Konfirmasi password baru    : ";
    cin >> confirmPassword;

    if (strcmp(newPassword, confirmPassword) != 0) {
        cout << "\n[Password tidak cocok. Silakan coba lagi.]" << endl;
        pauseScreen();
        return false;
    }

    // Update password
    strcpy(users[userIndex].password, newPassword);

    cout << "\n[Password berhasil diubah. Silakan login.]" << endl;
    pauseScreen();
    return true;
}

void calculateAverageRating(int movieIndex) {
    if (movies[movieIndex].reviewCount == 0) {
        movies[movieIndex].averageRating = 0.0f;
    }

    int totalRating = 0;
    for (int i = 0; i < movies[movieIndex].reviewCount; i++) {
        totalRating += movies[movieIndex].reviews[i].rating;
    }

    // Konversi dari skala 1-5 ke skala 0-10
    movies[movieIndex].averageRating =
        (float)(totalRating * 2.0) / movies[movieIndex].reviewCount;
}

void initializeMovies() {
    // Film 1: Deadpool & Wolverine
    movies[0].id = 1;
    strcpy(movies[0].title, "Deadpool & Wolverine");
    strcpy(movies[0].genre, "Action");
    movies[0].duration = 130;
    strcpy(movies[0].rating, "D17+");
    strcpy(movies[0].synopsis,
           "Deadpool dan Wolverine bersatu dalam petualangan penuh aksi yang "
           "menggabungkan humor khas Deadpool dengan intensitas Wolverine. "
           "Sebuah film superhero yang menghadirkan pertarungan epik dengan "
           "dialog yang menghibur.");
    movies[0].popularity = 1250;
    movies[0].showtimeCount = 3;
    strcpy(movies[0].showtimes[0].time, "12:00");
    movies[0].showtimes[0].auditorium = 1;
    movies[0].showtimes[0].capacity = 80;
    movies[0].showtimes[0].price = 50000;
    strcpy(movies[0].showtimes[1].time, "15:00");
    movies[0].showtimes[1].auditorium = 2;
    movies[0].showtimes[1].capacity = 80;
    movies[0].showtimes[1].price = 50000;
    strcpy(movies[0].showtimes[2].time, "18:15");
    movies[0].showtimes[2].auditorium = 1;
    movies[0].showtimes[2].capacity = 80;
    movies[0].showtimes[2].price = 55000;

    // Sample reviews untuk Deadpool & Wolverine
    movies[0].reviewCount = 3;
    movies[0].reviews[0].userId = 0;
    movies[0].reviews[0].rating = 5;
    strcpy(movies[0].reviews[0].comment, "Film superhero terbaik tahun ini!");
    strcpy(movies[0].reviews[0].date, "01-07-2024");
    movies[0].reviews[1].userId = 1;
    movies[0].reviews[1].rating = 4;
    strcpy(movies[0].reviews[1].comment,
           "Aksi yang spektakuler dan humor yang kocak.");
    strcpy(movies[0].reviews[1].date, "02-07-2024");
    movies[0].reviews[2].userId = 2;
    movies[0].reviews[2].rating = 5;
    strcpy(movies[0].reviews[2].comment,
           "Wajib nonton! Chemistry Deadpool-Wolverine luar biasa.");
    strcpy(movies[0].reviews[2].date, "03-07-2024");

    movies[0].isActive = true;
    calculateAverageRating(0);

    // Film 2: Inside Out 2
    movies[1].id = 2;
    strcpy(movies[1].title, "Inside Out 2");
    strcpy(movies[1].genre, "Animation");
    movies[1].duration = 100;
    strcpy(movies[1].rating, "SU");
    strcpy(movies[1].synopsis,
           "Melanjutkan petualangan emosi Riley yang kini beranjak remaja. "
           "Film animasi Pixar yang mengeksplorasi kompleksitas emosi manusia "
           "dengan cara yang menghibur dan mendidik untuk seluruh keluarga.");
    movies[1].popularity = 1050;
    movies[1].showtimeCount = 4;
    strcpy(movies[1].showtimes[0].time, "09:00");
    movies[1].showtimes[0].auditorium = 3;
    movies[1].showtimes[0].capacity = 60;
    movies[1].showtimes[0].price = 45000;
    strcpy(movies[1].showtimes[1].time, "11:00");
    movies[1].showtimes[1].auditorium = 3;
    movies[1].showtimes[1].capacity = 60;
    movies[1].showtimes[1].price = 45000;
    strcpy(movies[1].showtimes[2].time, "13:15");
    movies[1].showtimes[2].auditorium = 2;
    movies[1].showtimes[2].capacity = 80;
    movies[1].showtimes[2].price = 45000;
    strcpy(movies[1].showtimes[3].time, "15:30");
    movies[1].showtimes[3].auditorium = 3;
    movies[1].showtimes[3].capacity = 60;
    movies[1].showtimes[3].price = 45000;

    // Sample reviews untuk Inside Out 2
    movies[1].reviewCount = 4;
    movies[1].reviews[0].userId = 0;
    movies[1].reviews[0].rating = 4;
    strcpy(movies[1].reviews[0].comment,
           "Pixar selalu berhasil menyentuh hati.");
    strcpy(movies[1].reviews[0].date, "05-07-2024");
    movies[1].reviews[1].userId = 1;
    movies[1].reviews[1].rating = 5;
    strcpy(movies[1].reviews[1].comment,
           "Cocok untuk keluarga, pesan moral yang dalam.");
    strcpy(movies[1].reviews[1].date, "06-07-2024");
    movies[1].reviews[2].userId = 2;
    movies[1].reviews[2].rating = 4;
    strcpy(movies[1].reviews[2].comment,
           "Animasi yang memukau dan cerita yang menarik.");
    strcpy(movies[1].reviews[2].date, "07-07-2024");
    movies[1].reviews[3].userId = 3;
    movies[1].reviews[3].rating = 5;
    strcpy(movies[1].reviews[3].comment,
           "Masterpiece! Riley's journey sangat relatable.");
    strcpy(movies[1].reviews[3].date, "08-07-2024");

    movies[1].isActive = true;
    calculateAverageRating(1);

    // Film 3: Interstellar
    movies[2].id = 3;
    strcpy(movies[2].title, "Interstellar");
    strcpy(movies[2].genre, "Sci-Fi");
    movies[2].duration = 169;
    strcpy(movies[2].rating, "SU");
    strcpy(movies[2].synopsis,
           "Sebuah masterpiece Christopher Nolan yang mengisahkan perjalanan "
           "luar angkasa untuk mencari planet baru yang dapat dihuni manusia. "
           "Film yang menggabungkan sains kompleks dengan emosi kemanusiaan "
           "yang mendalam.");
    movies[2].popularity = 980;
    movies[2].showtimeCount = 3;
    strcpy(movies[2].showtimes[0].time, "10:00");
    movies[2].showtimes[0].auditorium = 1;
    movies[2].showtimes[0].capacity = 80;
    movies[2].showtimes[0].price = 50000;
    strcpy(movies[2].showtimes[1].time, "13:15");
    movies[2].showtimes[1].auditorium = 1;
    movies[2].showtimes[1].capacity = 80;
    movies[2].showtimes[1].price = 50000;
    strcpy(movies[2].showtimes[2].time, "16:30");
    movies[2].showtimes[2].auditorium = 1;
    movies[2].showtimes[2].capacity = 80;
    movies[2].showtimes[2].price = 50000;

    // Sample reviews untuk Interstellar
    movies[2].reviewCount = 5;
    movies[2].reviews[0].userId = 0;
    movies[2].reviews[0].rating = 5;
    strcpy(movies[2].reviews[0].comment,
           "Christopher Nolan genius! Mind-blowing sci-fi.");
    strcpy(movies[2].reviews[0].date, "10-07-2024");
    movies[2].reviews[1].userId = 1;
    movies[2].reviews[1].rating = 5;
    strcpy(movies[2].reviews[1].comment,
           "Cinematography luar biasa, cerita yang dalam.");
    strcpy(movies[2].reviews[1].date, "11-07-2024");
    movies[2].reviews[2].userId = 2;
    movies[2].reviews[2].rating = 4;
    strcpy(movies[2].reviews[2].comment,
           "Kompleks tapi worth it. Hans Zimmer score amazing!");
    strcpy(movies[2].reviews[2].date, "12-07-2024");
    movies[2].reviews[3].userId = 3;
    movies[2].reviews[3].rating = 5;
    strcpy(movies[2].reviews[3].comment,
           "Emotional journey yang tak terlupakan.");
    strcpy(movies[2].reviews[3].date, "13-07-2024");
    movies[2].reviews[4].userId = 4;
    movies[2].reviews[4].rating = 5;
    strcpy(movies[2].reviews[4].comment,
           "Masterpiece! Wajib ditonton berkali-kali.");
    strcpy(movies[2].reviews[4].date, "14-07-2024");

    movies[2].isActive = true;
    calculateAverageRating(2);

    // Film 4: Kung Fu Panda 4
    movies[3].id = 4;
    strcpy(movies[3].title, "Kung Fu Panda 4");
    strcpy(movies[3].genre, "Animation");
    movies[3].duration = 94;
    strcpy(movies[3].rating, "SU");
    strcpy(movies[3].synopsis,
           "Po kembali dalam petualangan baru sebagai Dragon Warrior. Kali ini "
           "ia harus menghadapi tantangan terbesar sambil mencari penerus yang "
           "tepat untuk melanjutkan warisan Kung Fu Panda.");
    movies[3].popularity = 900;
    movies[3].showtimeCount = 3;
    strcpy(movies[3].showtimes[0].time, "17:45");
    movies[3].showtimes[0].auditorium = 2;
    movies[3].showtimes[0].capacity = 80;
    movies[3].showtimes[0].price = 45000;
    strcpy(movies[3].showtimes[1].time, "19:45");
    movies[3].showtimes[1].auditorium = 3;
    movies[3].showtimes[1].capacity = 60;
    movies[3].showtimes[1].price = 45000;
    strcpy(movies[3].showtimes[2].time, "20:30");
    movies[3].showtimes[2].auditorium = 2;
    movies[3].showtimes[2].capacity = 80;
    movies[3].showtimes[2].price = 45000;

    // Sample reviews untuk Kung Fu Panda 4
    movies[3].reviewCount = 3;
    movies[3].reviews[0].userId = 0;
    movies[3].reviews[0].rating = 4;
    strcpy(movies[3].reviews[0].comment, "Po masih lucu seperti biasa!");
    strcpy(movies[3].reviews[0].date, "15-07-2024");
    movies[3].reviews[1].userId = 1;
    movies[3].reviews[1].rating = 4;
    strcpy(movies[3].reviews[1].comment,
           "Animasi bagus, cerita menghibur untuk anak-anak.");
    strcpy(movies[3].reviews[1].date, "16-07-2024");
    movies[3].reviews[2].userId = 2;
    movies[3].reviews[2].rating = 3;
    strcpy(movies[3].reviews[2].comment,
           "Standar franchise Kung Fu Panda, tidak mengecewakan.");
    strcpy(movies[3].reviews[2].date, "17-07-2024");

    movies[3].isActive = true;
    calculateAverageRating(3);

    movieCount = 4;
}

// Fungsi untuk format durasi dari menit ke jam:menit
void formatDuration(int minutes, char *result) {
    int hours = minutes / 60;
    int mins = minutes % 60;
    if (hours > 0) {
        sprintf(result, "%dj%dm", hours, mins);
    } else {
        sprintf(result, "%dm", mins);
    }
}

// Fungsi untuk format jadwal tayang
void formatShowtimes(Movie movie, char *result) {
    strcpy(result, "");
    for (int i = 0; i < movie.showtimeCount; i++) {
        if (i > 0)
            strcat(result, ", ");
        strcat(result, movie.showtimes[i].time);
    }
}

// Fungsi menampilkan bintang berdasarkan rating
void displayStars(int rating) {
    for (int i = 0; i < 5; i++) {
        if (i < rating) {
            cout << "*";
        } else {
            cout << "X";
        }
    }
}

// Fungsi untuk sorting berdasarkan rating (descending)
void sortMoviesByRating() {
    for (int i = 0; i < movieCount - 1; i++) {
        for (int j = 0; j < movieCount - 1; j++) {
            if (movies[j].averageRating < movies[j + 1].averageRating) {
                Movie temp = movies[j];
                movies[j] = movies[j + 1];
                movies[j + 1] = temp;
            }
        }
    }
}

void sortMoviesByPopularity() {
    for (int i = 0; i < movieCount - 1; i++) {
        for (int j = 0; j < movieCount - 1; j++) {
            if (movies[j].popularity < movies[j + 1].popularity) {
                Movie temp = movies[j];
                movies[j] = movies[j + 1];
                movies[j + 1] = temp;
            }
        }
    }
}

void sortMoviesByTitle() {
    for (int i = 0; i < movieCount - 1; i++) {
        for (int j = 0; j < movieCount - 1; j++) {
            if (strcmp(movies[j].title, movies[j + 1].title) > 0) {
                Movie temp = movies[j];
                movies[j] = movies[j + 1];
                movies[j + 1] = temp;
            }
        }
    }
}

void sortMoviesByDuration() {
    for (int i = 0; i < movieCount - 1; i++) {
        for (int j = 0; j < movieCount - i - 1; j++) {
            if (movies[j].duration > movies[j + 1].duration) {
                Movie temp = movies[j];
                movies[j] = movies[j + 1];
                movies[j + 1] = temp;
            }
        }
    }
}

// Fungsi untuk menampilkan daftar film
void showMovieList() {
    clearScreen();
    showHeader("DAFTAR FILM SEDANG TAYANG");
    cout << "\n| No | Judul                  | Genre     | Durasi | Rating | "
            "Jadwal Tayang               | Review Score | Popularitas |"
         << endl;
    cout << "|----|------------------------|-----------|--------|--------|-----"
            "------------------------|--------------|-------------|"
         << endl;

    for (int i = 0; i < movieCount; i++) {
        if (movies[i].isActive) {
            char duration[10];
            char showtimes[100];
            formatDuration(movies[i].duration, duration);
            formatShowtimes(movies[i], showtimes);

            printf("| %-2d | %-22s | %-9s | %-6s | %-6s | %-27s | %4.1f/10 "
                   "(%3d) | %7d tiket |\n",
                   i + 1, movies[i].title, movies[i].genre, duration,
                   movies[i].rating, showtimes, movies[i].averageRating,
                   movies[i].reviewCount, movies[i].popularity);
        }
    }
    cout << "\n[Tips: Tekan S untuk sortir, ketik nomor film untuk detail, 0 "
            "untuk kembali]"
         << endl;
    cout << "\nPilihan Anda: ";
}

// Fungsi untuk menampilkan detail film
void showMovieDetail(int movieIndex) {
    clearScreen();
    showHeader("DETAIL FILM");

    Movie movie = movies[movieIndex];
    char duration[10];
    formatDuration(movie.duration, duration);

    cout << "\nJudul      : " << movie.title << endl;
    cout << "Genre      : " << movie.genre << endl;
    cout << "Durasi     : " << duration << endl;
    cout << "Rating     : " << movie.rating << endl;
    cout << "Popularitas: " << movie.popularity << " tiket terjual" << endl;
    printf("Review     : %.1f/10 dari %d review\n", movie.averageRating,
           movie.reviewCount);
    cout << "\nSinopsis   :" << endl;
    cout << movie.synopsis << endl;

    cout << "\n--- Jadwal Tayang ---" << endl;
    cout << "| No | Waktu | Audi | Kapasitas | Harga     |" << endl;
    cout << "|----|-------|------|-----------|-----------|" << endl;
    for (int i = 0; i < movie.showtimeCount; i++) {
        printf("| %-2d | %-5s | %-4d | %-9d | Rp %-6d |\n", i + 1,
               movie.showtimes[i].time, movie.showtimes[i].auditorium,
               movie.showtimes[i].capacity, movie.showtimes[i].price);
    }

    // Tampilkan beberapa review terakhir
    if (movie.reviewCount > 0) {
        cout << "\n--- Review Pengguna ---" << endl;
        int reviewsToShow = movie.reviewCount > 5 ? 5 : movie.reviewCount;
        for (int i = movie.reviewCount - reviewsToShow; i < movie.reviewCount;
             i++) {
            cout << "> ";
            displayStars(movie.reviews[i].rating);
            cout << " (" << movie.reviews[i].rating << "/5) - "
                 << movie.reviews[i].comment << endl;
            cout << "  Tanggal: " << movie.reviews[i].date << endl << endl;
        }
    }
    pauseScreen();
}

void showSortMenu() {
    clearScreen();
    showHeader("PILIH METODE SORTIR");

    cout << "\n1. Sortir berdasarkan Rating (Tertinggi)" << endl;
    cout << "2. Sortir berdasarkan Popularitas (Terpopuler)" << endl;
    cout << "3. Sortir berdasarkan Judul (A-Z)" << endl;
    cout << "4. Sortir berdasarkan Durasi (Terpendek)" << endl;
    cout << "0. Kembali tanpa sortir" << endl;
    cout << "\nPilihan Anda [0-4]: ";
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

// Fungsi untuk menangani menu daftar film
void handleMovieListMenu() {
    char input;

    while (true) {
        showMovieList();
        input = getSingleInput();

        // Cek apakah input adalah 'S' untuk sorting
        if (input == 'S' || input == 's') {
            showSortMenu();
            int choice = getSingletDigit();

            switch (choice) {
            case 1:
                sortMoviesByRating();
                cout << "\n[Film berhasil diurutkan berdasarkan rating "
                        "tertinggi]"
                     << endl;
                pauseScreen();
                break;
            case 2:
                sortMoviesByPopularity();
                cout << "\n[Film berhasil diurutkan berdasarkan popularitas]"
                     << endl;
                pauseScreen();
                break;
            case 3:
                sortMoviesByTitle();
                cout << "\n[Film berhasil diurutkan berdasarkan judul A-Z]"
                     << endl;
                pauseScreen();
                break;
            case 4:
                sortMoviesByDuration();
                cout << "\n[Film berhasil diurutkan berdasarkan durasi "
                        "terpendek]"
                     << endl;
                pauseScreen();
                break;
            case 0:
                break;
            default:
                cout << "\nPilihan tidak valid! Silahkan pilih 0-4" << endl;
                pauseScreen();
                break;
            }
        } else if (input >= '0' && input <= '9') {
            int choice = input - '0';

            if (choice == 0) {
                break; // Kembali ke menu utama
            } else if (choice >= 1 && choice <= movieCount) {
                showMovieDetail(choice - 1);
            } else {
                cout << "\nPilihan tidak valid! Silakan pilih nomor film yang "
                        "tersedia."
                     << endl;
                pauseScreen();
            }
        } else {
            cout << "\nInput tidak valid! Gunakan S untuk sorting, 0-"
                 << movieCount << " untuk memilih film." << endl;
            pauseScreen();
        }
    }
}

// Fungsi untuk menampilkan menu autentikasi
void showAuthMenu() {
    clearScreen();
    showHeader("MOVTIX BIOSKOP");

    cout << "\nSelamat datang di MOVTIX!" << endl;
    cout << "Pilih salah satu opsi berikut:" << endl;
    cout << "\n1. Login" << endl;
    cout << "2. Daftar Akun Baru" << endl;
    cout << "3. Lupa Password" << endl;
    cout << "4. Keluar" << endl;
    cout << "\nSilakan masukkan pilihan [1-4]: ";
}

// Fungsi untuk menampilkan menu utama (sementara)
void showMainMenu() {
    clearScreen();
    showHeader("MENU UTAMA MOVTIX");

    // Tampilkan waktu saat ini
    char currentDateTime[20];
    getCurrentDateTime(currentDateTime);
    cout << "\nWaktu saat ini: " << currentDateTime << endl;
    cout << "\nSelamat datang, " << users[currentUserIndex].nama << "!" << endl;
    cout << "Poin MOVTIX Anda: " << users[currentUserIndex].movtixPoints
         << " poin" << endl;

    cout << "\n1. Daftar Film Sedang Tayang" << endl;
    cout << "2. Cari Film" << endl;
    cout << "3. Pesan Tiket" << endl;
    cout << "4. Histori Transaksi" << endl;
    cout << "5. Penukaran Tiket & F&B" << endl;
    cout << "6. Keluar / Logout" << endl;
    cout << "\nMasukkan pilihan [1-6]: ";
}

// Replace the current searchMovies function with this optimized version
void searchMovies(const char *keyword, int results[], int &resultCount) {
    resultCount = 0;

    for (int i = 0; i < movieCount; i++) {
        if (movies[i].isActive) {
            bool found = false;

            // Konversi ke lowercase untuk pencarian case-insensitive
            char movieTitle[100];
            char movieGenre[50];
            char searchKeyword[100];

            strcpy(movieTitle, movies[i].title);
            strcpy(movieGenre, movies[i].genre);
            strcpy(searchKeyword, keyword);

            // Konversi ke lowercase
            for (int j = 0; j < strlen(movieTitle); j++) {
                movieTitle[j] = tolower(movieTitle[j]);
            }

            for (int j = 0; j < strlen(movieGenre); j++) {
                movieGenre[j] = tolower(movieGenre[j]);
            }

            for (int j = 0; j < strlen(searchKeyword); j++) {
                searchKeyword[j] = tolower(searchKeyword[j]);
            }

            // Cari di judul (substring)
            if (strstr(movieTitle, searchKeyword) != nullptr) {
                found = true;
            }

            // Cari di genre (substring)
            if (!found && strstr(movieGenre, searchKeyword) != nullptr) {
                found = true;
            }

            if (found) {
                // Simpan index movie, bukan copy seluruh struct
                results[resultCount] = i;
                resultCount++;
            }
        }
    }
}

// Update displaySearchResults to work with indices instead of full Movie
// objects
void displaySearchResults(int results[], int resultCount, const char *keyword) {
    clearScreen();
    showHeader("CARI FILM MOVTIX");

    if (resultCount == 0) {
        cout << "\nTidak ada film yang ditemukan untuk kata kunci : " << keyword
             << endl;
        cout << "\nSaran:" << endl;
        cout << "- Pastikan ejaan kata kunci." << endl;
        cout << "- Coba kata kunci lain." << endl;
        pauseScreen();
        return;
    }

    cout << "\nHasil pencarian untuk: " << keyword << endl;

    cout << "\nHasil pencarian:" << endl;
    cout << "| No | Judul               | Genre     | Jadwal              |"
         << endl;
    cout << "|----|---------------------|-----------|---------------------|"
         << endl;

    for (int i = 0; i < resultCount; i++) {
        char showtimes[100];
        formatShowtimes(movies[results[i]], showtimes);
        printf("| %-2d | %-19s | %-9s | %-19s |\n", i + 1,
               movies[results[i]].title, movies[results[i]].genre, showtimes);
    }

    cout << "\n[Pilih nomor film untuk detail, 0 untuk kembali]" << endl;
    cout << "\nPilihan Anda: ";
    int input = getSingletDigit();

    if (input >= 1 && input <= resultCount) {
        // Gunakan index yang sudah disimpan
        showMovieDetail(results[input - 1]);
    } else if (input != 0) {
        cout << "\nPilihan tidak valid! Silakan pilih nomor film yang "
                "tersedia."
             << endl;
        pauseScreen();
    }
}

void handleSearchMovie() {
    int searchResult[50];
    int resultCount = 0;
    char keyword[100];

    while (true) {
        clearScreen();
        showHeader("CARI FILM MOVTIX");
        cout << "\nMasukkan kata kunci pencarian (judul/genre): ";

        cin >> keyword;

        // Lakukan pencarian
        searchMovies(keyword, searchResult, resultCount);

        // Tampilkan hasil
        displaySearchResults(searchResult, resultCount, keyword);

        // Tanyakan apakah ingin mencari lagi
        cout << "\nIngin mencari lagi? (Y/N): ";
        char choice = getSingleInput();
        if (choice == 'N' || choice == 'n') {
            break; // Kembali ke menu utama
        } else if (choice != 'Y' && choice != 'y') {
            cout << "\nPilihan tidak valid! Kembali ke menu utama." << endl;
            pauseScreen();
            break;
        }
    }
}

// Tambahkan fungsi untuk format tanggal yang lebih pendek
void formatDate(const char *fullDateTime, char *shortDate) {
    int day, month, year, hour, min, sec;
    sscanf(fullDateTime, "%d-%d-%d, %d:%d:%d", &day, &month, &year, &hour, &min,
           &sec);
    sprintf(shortDate, "%02d-%02d-%02d", day, month, year % 100);
}

// Fungsi untuk mendapatkan string status transaksi
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

// Fungsi untuk menampilkan detail transaksi
void showTransactionDetail(int transactionIndex) {
    clearScreen();
    showHeader("DETAIL TRANSAKSI");

    Transaction trans = transactions[transactionIndex];

    cout << "ID Transaksi   : MOVTIX" << setfill('0') << setw(4) << trans.id
         << endl;
    cout << "Status         : " << getStatusString(trans.status) << endl;
    cout << "Tanggal Pesan  : " << trans.transactionDate << endl;

    if (trans.status == PAID && strlen(trans.bookingCode) > 0) {
        cout << "Kode Booking   : " << trans.bookingCode << endl;
        cout << "Passkey        : " << trans.passkey << endl;
    }

    if (trans.status == PENDING_PAYMENT) {
        cout << "Batas Bayar    : " << trans.expiryTime << endl;

        // Cek apakah masih dalam batas waktu
        char currentTime[20];
        getCurrentDateTime(currentTime);
        int timeDiff = compareDateTime(trans.expiryTime, currentTime);

        if (timeDiff > 0) {
            cout << "Status Waktu   : Masih dalam batas waktu" << endl;
        } else {
            cout << "Status Waktu   : KADALUARSA" << endl;
        }
    }

    cout << "\n--- Detail Film ---" << endl;
    cout << "Film           : " << trans.movieTitle << endl;
    cout << "Jadwal         : " << trans.showtime << endl;
    cout << "Auditorium     : " << trans.auditorium << endl;
    cout << "Kursi          : " << trans.selectedSeats << endl;
    cout << "Jumlah Tiket   : " << trans.ticketCount << " tiket" << endl;
    cout << "Harga Tiket    : Rp " << trans.ticketPrice << endl;

    if (trans.fnbCount > 0) {
        cout << "\n--- Detail F&B ---" << endl;
        for (int i = 0; i < trans.fnbCount; i++) {
            printf("- %dx %s = Rp %d\n", trans.selectedFnB[i].quantity,
                   trans.selectedFnB[i].name, trans.selectedFnB[i].totalPrice);
        }
        cout << "Total F&B      : Rp " << trans.fnbPrice << endl;
    }

    cout << "\nTOTAL BAYAR    : Rp " << trans.totalPrice << endl;
    cout << "=========================================================="
         << endl;

    // Menu aksi berdasarkan status
    if (trans.status == PENDING_PAYMENT) {
        char currentTime[20];
        getCurrentDateTime(currentTime);

        if (compareDateTime(currentTime, trans.expiryTime) >= 0) {
            cout << "\n[Transaksi ini telah kadaluarsa dan dibatalkan otomatis]"
                 << endl;
            pauseScreen();
            return;
        }

        cout << "\nPilihan:" << endl;
        cout << "1. Lanjutkan Pembayaran" << endl;
        cout << "2. Batalkan Transaksi" << endl;
        cout << "0. Kembali" << endl;
        cout << "\nPilihan Anda [0-2]: ";

        char choice = getSingleInput();

        switch (choice) {
        case '1':
            // Lanjut ke pembayaran
            processPayment(transactionIndex);
            break;

        case '2': {
            cout << "\nApakah Anda yakin ingin membatalkan transaksi ini? "
                    "(Y/N): ";
            char confirm = getSingleInput();
            if (confirm == 'Y' || confirm == 'y') {
                cancelReservation(transactionIndex);
                cout << "\nTransaksi berhasil dibatalkan. Kursi telah "
                        "dibebaskan."
                     << endl;
                pauseScreen();
            }
            break;
        }

        case '0':
            return;

        default:
            cout << "\nPilihan tidak valid!" << endl;
            pauseScreen();
            showTransactionDetail(transactionIndex);
            break;
        }
    } else if (trans.status == PAID) {
        cout << "\n[Transaksi ini telah dibayar. E-Ticket dapat digunakan "
                "untuk masuk bioskop]"
             << endl;

        // Tampilkan informasi lengkap untuk transaksi yang sudah dibayar
        cout << "\n=== INFORMASI PENUKARAN TIKET ===" << endl;
        cout << "Kode Booking : " << trans.bookingCode << endl;
        cout << "Passkey      : " << trans.passkey << endl;
        cout << "Gunakan kode ini di Menu 5 untuk penukaran tiket dan F&B"
             << endl;
        cout << "=========================================" << endl;

        cout << "\nPilihan:" << endl;
        cout << "1. Tampilkan E-Ticket" << endl;
        cout << "2. Salin Kode" << endl;
        cout << "0. Kembali" << endl;
        cout << "\nPilihan Anda [0-2]: ";

        char choice = getSingleInput();

        switch (choice) {
        case '1':
            showTicketConfirmation(transactionIndex);
            break;
        case '2':
            clearScreen();
            showHeader("KODE UNTUK PENUKARAN TIKET");
            cout << "\n=================== SIMPAN KODE INI ==================="
                 << endl;
            cout << "Kode Booking : " << trans.bookingCode << endl;
            cout << "Passkey      : " << trans.passkey << endl;
            cout << "Film         : " << trans.movieTitle << endl;
            cout << "Jadwal       : " << trans.showtime << endl;
            cout << "Auditorium   : " << trans.auditorium << endl;
            cout << "Kursi        : " << trans.selectedSeats << endl;
            if (trans.fnbCount > 0) {
                cout << "\nF&B untuk diklaim di Menu 5:" << endl;
                for (int i = 0; i < trans.fnbCount; i++) {
                    printf("- %dx %s\n", trans.selectedFnB[i].quantity,
                           trans.selectedFnB[i].name);
                }
            }
            cout << "======================================================="
                 << endl;
            cout << "\nMasuk ke Menu Utama Nomor 5 dengan kode ini untuk "
                    "penukaran tiket."
                 << endl;
            pauseScreen();
            break;
        case '0':
            return;
        default:
            cout << "\nPilihan tidak valid!" << endl;
            pauseScreen();
            showTransactionDetail(transactionIndex);
            break;
        }
    } else if (trans.status == EXPIRED) {
        cout << "\n[Transaksi ini telah kadaluarsa karena tidak dibayar dalam "
                "batas waktu]"
             << endl;
        pauseScreen();
    } else if (trans.status == CANCELLED) {
        cout << "\n[Transaksi ini telah dibatalkan]" << endl;
        pauseScreen();
    }
}

// Fungsi untuk menampilkan histori transaksi user
void showTransactionHistory() {
    clearScreen();
    showHeader("HISTORI TRANSAKSI MOVTIX");

    // Cek expired reservations terlebih dahulu
    checkExpiredReservations();

    // Filter transaksi berdasarkan user yang sedang login
    int userTransactions[100];
    int userTransactionCount = 0;

    for (int i = 0; i < transactionCount; i++) {
        if (transactions[i].userId == currentUserIndex &&
            transactions[i].isActive) {
            userTransactions[userTransactionCount] = i;
            userTransactionCount++;
        }
    }

    if (userTransactionCount == 0) {
        cout << "\nAnda belum memiliki riwayat transaksi." << endl;
        cout << "Silakan pesan tiket terlebih dahulu." << endl;
        pauseScreen();
        return;
    }

    cout << "\nNama: " << users[currentUserIndex].nama << endl;
    cout << "\n| No | Tanggal   | Film         | Jadwal | Audi | Kursi   | "
            "Status   | Total    |"
         << endl;
    cout << "|----|-----------|--------------|--------|------|---------|-------"
            "---|----------|"
         << endl;

    for (int i = 0; i < userTransactionCount; i++) {
        int transIndex = userTransactions[i];
        Transaction trans = transactions[transIndex];

        char shortDate[10];
        formatDate(trans.transactionDate, shortDate);

        // Truncate title jika terlalu panjang
        char shortTitle[13];
        if (strlen(trans.movieTitle) > 12) {
            strncpy(shortTitle, trans.movieTitle, 12);
            shortTitle[12] = '\0';
        } else {
            strcpy(shortTitle, trans.movieTitle);
        }

        // Truncate kursi jika terlalu panjang
        char shortSeats[8];
        if (strlen(trans.selectedSeats) > 7) {
            strncpy(shortSeats, trans.selectedSeats, 7);
            shortSeats[7] = '\0';
        } else {
            strcpy(shortSeats, trans.selectedSeats);
        }

        printf("| %-2d | %-9s | %-12s | %-6s | %-4d | %-7s | %-8s | %-8d |\n",
               i + 1, shortDate, shortTitle, trans.showtime, trans.auditorium,
               shortSeats, getStatusString(trans.status), trans.totalPrice);
    }

    cout << "\n[Pilih nomor transaksi untuk detail/pembayaran ulang (jika "
            "UNPAID), atau 0 untuk kembali]"
         << endl;
    cout << "\nPilihan Anda: ";

    int choice = getSingletDigit();

    if (choice >= 1 && choice <= userTransactionCount) {
        int selectedTransIndex = userTransactions[choice - 1];
        showTransactionDetail(selectedTransIndex);
    } else if (choice != 0) {
        cout << "\nPilihan tidak valid!" << endl;
        pauseScreen();
        showTransactionHistory(); // Kembali ke histori
    }
}

// Fungsi untuk menampilkan statistik transaksi user
void showTransactionSummary() {
    clearScreen();
    showHeader("RINGKASAN TRANSAKSI");

    int totalTransactions = 0;
    int paidTransactions = 0;
    int unpaidTransactions = 0;
    int expiredTransactions = 0;
    int cancelledTransactions = 0;
    int totalSpent = 0;
    int totalTickets = 0;

    for (int i = 0; i < transactionCount; i++) {
        if (transactions[i].userId == currentUserIndex &&
            transactions[i].isActive) {
            totalTransactions++;

            switch (transactions[i].status) {
            case PAID:
                paidTransactions++;
                totalSpent += transactions[i].totalPrice;
                totalTickets += transactions[i].ticketCount;
                break;
            case PENDING_PAYMENT:
                unpaidTransactions++;
                break;
            case EXPIRED:
                expiredTransactions++;
                break;
            case CANCELLED:
                cancelledTransactions++;
                break;
            }
        }
    }

    cout << "\nNama: " << users[currentUserIndex].nama << endl;
    cout << "\n==================== RINGKASAN AKTIVITAS ===================="
         << endl;
    cout << "Total Transaksi      : " << totalTransactions << " transaksi"
         << endl;
    cout << "Transaksi Berhasil   : " << paidTransactions << " transaksi"
         << endl;
    cout << "Menunggu Pembayaran  : " << unpaidTransactions << " transaksi"
         << endl;
    cout << "Transaksi Kadaluarsa : " << expiredTransactions << " transaksi"
         << endl;
    cout << "Transaksi Dibatalkan : " << cancelledTransactions << " transaksi"
         << endl;
    cout << "\n===================== STATISTIK PEMBELIAN ====================="
         << endl;
    cout << "Total Tiket Dibeli   : " << totalTickets << " tiket" << endl;
    cout << "Total Pengeluaran    : Rp " << totalSpent << endl;

    if (paidTransactions > 0) {
        cout << "Rata-rata per Transaksi : Rp "
             << (totalSpent / paidTransactions) << endl;
    }

    cout << "============================================================="
         << endl;

    pauseScreen();
}

// Fungsi utama untuk handle menu histori transaksi
void handleTransactionHistory() {
    while (true) {
        clearScreen();
        showHeader("HISTORI TRANSAKSI MOVTIX");

        char currentDateTime[20];
        getCurrentDateTime(currentDateTime);
        cout << "\nWaktu saat ini: " << currentDateTime << endl;
        cout << "\nSelamat datang, " << users[currentUserIndex].nama << "!"
             << endl;

        cout << "\n1. Lihat Histori Transaksi" << endl;
        cout << "2. Ringkasan Aktivitas" << endl;
        cout << "0. Kembali ke Menu Utama" << endl;
        cout << "\nPilihan Anda [0-2]: ";

        char choice = getSingleInput();

        switch (choice) {
        case '1':
            showTransactionHistory();
            break;

        case '2':
            showTransactionSummary();
            break;

        case '0':
            return;

        default:
            cout << "\nPilihan tidak valid!" << endl;
            pauseScreen();
            break;
        }
    }
}

// Fungsi utama program
int main() {
    int choice;
    bool isLoggedIn = false;

    // Inisialisasi data
    initializeUsers();
    initializeMovies();
    initializeSeats();
    initializeFnB();

    while (true) {
        if (!isLoggedIn) {
            showAuthMenu();
            choice = getSingletDigit();

            if (choice == -1) {
                cout << "\nPilihan tidak valid. Silakan pilih 1-4." << endl;
                continue;
            }

            switch (choice) {
            case 1:
                if (login()) {
                    isLoggedIn = true;
                }
                break;
            case 2:
                registerUser();
                break;
            case 3:
                resetPassword();
                break;
            case 4:
                cout << "\nTerima kasih telah menggunakan MOVTIX!" << endl;
                return 0;
            default:
                cout << "\nPilihan tidak valid. Silakan pilih 1-4." << endl;
                pauseScreen();
                break;
            }
        } else {
            showMainMenu();
            choice = getSingletDigit();

            if (choice == -1) {
                cout << "\nPilihan tidak valid. Silakan pilih 1-6." << endl;
                pauseScreen();
                continue;
            }

            switch (choice) {
            case 1:
                handleMovieListMenu();
                break;
            case 2:
                handleSearchMovie();
                break;
            case 3:
                handleTicketBooking();
                break;
            case 4:
                handleTransactionHistory();
                break;
            case 5:
                handleTicketExchange();
                break;
            case 6:
                cout << "\n[Logout berhasil. Sampai jumpa, "
                     << users[currentUserIndex].nama << "!]" << endl;
                pauseScreen();
                isLoggedIn = false;
                currentUserIndex = -1;
                break;
            default:
                cout << "\nPilihan tidak valid. Silakan pilih 1 - 6." << endl;
                pauseScreen();
                break;
            }
        }
    }

    return 0;
}