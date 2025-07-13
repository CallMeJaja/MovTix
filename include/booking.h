#ifndef BOOKING_H
#define BOOKING_H

#include "types.h"

// Initialization
void initializeSeats();
void initializeFnB();

// Seat Management
void checkExpiredReservations();
bool parseAndValidateSeats(const char *seatInput, int auditorium,
                           char validSeats[][4], int &validCount);
void reserveSeats(const char validSeats[][4], int seatCount, int auditorium,
                  int transactionId);

// Booking Process
void handleTicketBooking();
void processTicketBooking(int movieIndex, int showtimeIndex);
int handleFnBSelection(SelectedFnB selectedFnB[]);

// Payment
void processPayment(int transactionId);
bool processCardPayment(int totalAmount, PaymentInfo *paymentInfo);
bool processBankPayment(int totalAmount, PaymentInfo *paymentInfo);
bool processPointsPayment(int totalAmount, PaymentInfo *paymentInfo);

// Transaction Management
void confirmPayment(int transactionId);
void cancelReservation(int transactionId);
void generateBookingCode(char *bookingCode);
void generatePasskey(char *passkey);
int findTransactionByCode(const char *bookingCode, const char *passkey);
void handleTicketExchange();

#endif // BOOKING_H