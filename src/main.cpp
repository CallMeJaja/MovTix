#include "auth.h"
#include "booking.h"
#include "movie.h"
#include "types.h"
#include "ui.h"
#include "utils.h"
#include <iostream>

using namespace std;

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
                pauseScreen();
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
                cout << "\nPilihan tidak valid. Silakan pilih 1-6." << endl;
                pauseScreen();
                break;
            }
        }
    }

    return 0;
}