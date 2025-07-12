#include "cstdlib"
#include "ctime"
#include <iostream>

#include "auth.h"
#include "booking.h"
#include "globals.h"
#include "movie.h"
#include "ui.h"
#include "utils.h"

using namespace std;

//============================================================
// MAIN APPLICATION LOGIC
//============================================================

void initializeSystem() {
    srand(time(0)); // Seed for random number generation
    initializeUsers();
    initializeMovies();
    initializeSeats();
    initializeFnB();
}

void runAuthFlow() {
    while (true) {
        showAuthMenu();
        int choice = getSingletDigit();
        switch (choice) {
        case 1:
            if (login())
                return;
            break;
        case 2:
            registerUser();
            break;
        case 3:
            resetPassword();
            break;
        case 4:
            exit(0);
        default:
            cout << "\nPilihan tidak valid." << endl;
            pauseScreen();
            break;
        }
    }
}

void runMainApp() {
    while (true) {
        showMainMenu();
        int choice = getSingletDigit();
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
            cout << "\n[Logout berhasil.]" << endl;
            currentUserIndex = -1;
            pauseScreen();
            return; // Exit to auth flow
        default:
            cout << "\nPilihan tidak valid." << endl;
            pauseScreen();
            break;
        }
    }
}

int main() {
    initializeSystem();

    while (true) {
        if (currentUserIndex == -1) {
            runAuthFlow();
        } else {
            runMainApp();
        }
    }

    return 0;
}