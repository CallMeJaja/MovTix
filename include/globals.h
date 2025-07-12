#ifndef GLOBALS_H
#define GLOBALS_H

#include "structures.h"

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

#endif // GLOBALS_H