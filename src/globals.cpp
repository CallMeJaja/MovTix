#include "globals.h"

//============================================================
// GLOBAL VARIABLE DEFINITIONS
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