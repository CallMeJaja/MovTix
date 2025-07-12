#ifndef BOOKING_H
#define BOOKING_H

#include "structures.h"

//============================================================
// BOOKING & PAYMENT FUNCTION PROTOTYPES
//============================================================

// Initialization
void initializeSeats();
void initializeFnB();

// Seat Management
void displaySeatMap(int auditorium);
void checkExpiredReservations();

// Booking Process
void handleTicketBooking();
void processTicketBooking(int movieIndex, int showtimeIndex);

// F&B
int handleFnBSelection(SelectedFnB selectedFnB[]);

// Payment
void processPayment(int transactionId);
void showTicketConfirmation(int transactionId);
void generateBookingCode(char *bookingCode);
void generatePasskey(char *passkey);

// History & Transaction Management
void handleTransactionHistory();
void handleTicketExchange();

#endif // BOOKING_H