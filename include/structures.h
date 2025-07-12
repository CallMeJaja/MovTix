#ifndef STRUCTURES_H
#define STRUCTURES_H

//============================================================
// ENUM DEFINITIONS
//============================================================

enum SeatStatus { AVAILABLE, PENDING, TAKEN };

enum PaymentMethod { BANK_TRANSFER, CARD_PAYMENT, MOVTIX_POINTS };

enum TransactionStatus {
    PENDING_PAYMENT = 0,
    PAID = 1,
    EXPIRED = 2,
    CANCELLED = 3,
    USED = 4
};

//============================================================
// STRUCT DEFINITIONS
//============================================================

struct Seat {
    char seatNumber[4];
    SeatStatus status;
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
    TransactionStatus status;
    char expiryTime[20];
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
    char time[6];
    int auditorium;
    int capacity;
    int price;
};

struct Review {
    int userId;
    int rating;
    char comment[200];
    char date[12];
};

struct Movie {
    int id;
    char title[100];
    char genre[50];
    int duration;
    char rating[10];
    char synopsis[500];
    int popularity;
    Showtime showtimes[10];
    int showtimeCount;
    Review reviews[500];
    int reviewCount;
    float averageRating;
    bool isActive;
};

#endif // STRUCTURES_H