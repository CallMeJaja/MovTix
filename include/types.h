#ifndef TYPES_H
#define TYPES_H

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
// GLOBAL VARIABLE DECLARATIONS
//============================================================

extern User users[100];
extern Movie movies[50];
extern FoodBeverage foodBeverages[50];
extern Seat auditoriumSeats[5][100];
extern Transaction transactions[1000];

extern int seatCounts[5];
extern int fnbCount;
extern int userCount;
extern int movieCount;
extern int currentUserIndex;
extern int transactionCount;

#endif // TYPES_H