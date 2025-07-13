#ifndef UI_H
#define UI_H

// General UI
void showHeader(const char *title);
void displayStars(int rating);

// Auth Menus
void showAuthMenu();

// Main Menus
void showMainMenu();

// Movie Menus
void handleMovieListMenu();
void showMovieList();
void showMovieDetail(int movieIndex);
void showSortMenu();
void handleSearchMovie();
void displaySearchResults(int results[], int resultCount, const char *keyword);
void showMovieSchedule(int movieIndex);

// Booking & Transaction Menus
void displaySeatMap(int auditorium);
void showTicketConfirmation(int transactionId);
void handleTransactionHistory();
void showTransactionHistory();
void showTransactionDetail(int transactionIndex);
void showTransactionSummary();

#endif // UI_H