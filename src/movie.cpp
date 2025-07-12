#include "cstring"
#include <iostream>

#include "booking.h" // For processTicketBooking
#include "globals.h"
#include "movie.h"
#include "utils.h"

using namespace std;

//============================================================
// HELPER FUNCTIONS
//============================================================

void calculateAverageRating(int movieIndex) {
    if (movies[movieIndex].reviewCount == 0) {
        movies[movieIndex].averageRating = 0.0f;
        return;
    }
    int totalRating = 0;
    for (int i = 0; i < movies[movieIndex].reviewCount; i++) {
        totalRating += movies[movieIndex].reviews[i].rating;
    }
    movies[movieIndex].averageRating =
        (float)(totalRating * 2.0) / movies[movieIndex].reviewCount;
}

void formatDuration(int minutes, char *result) {
    sprintf(result, "%dj%dm", minutes / 60, minutes % 60);
}

void formatShowtimes(Movie movie, char *result) {
    strcpy(result, "");
    for (int i = 0; i < movie.showtimeCount; i++) {
        if (i > 0)
            strcat(result, ", ");
        strcat(result, movie.showtimes[i].time);
    }
}

void displayStars(int rating) {
    for (int i = 0; i < 5; i++) {
        cout << (i < rating ? "*" : ".");
    }
}

void sortMoviesBy(
    int criteria) { // 1: Rating, 2: Popularity, 3: Title, 4: Duration
    for (int i = 0; i < movieCount - 1; i++) {
        for (int j = 0; j < movieCount - 1 - i; j++) {
            bool shouldSwap = false;
            if (criteria == 1 &&
                movies[j].averageRating < movies[j + 1].averageRating)
                shouldSwap = true;
            if (criteria == 2 &&
                movies[j].popularity < movies[j + 1].popularity)
                shouldSwap = true;
            if (criteria == 3 &&
                strcmp(movies[j].title, movies[j + 1].title) > 0)
                shouldSwap = true;
            if (criteria == 4 && movies[j].duration > movies[j + 1].duration)
                shouldSwap = true;

            if (shouldSwap) {
                Movie temp = movies[j];
                movies[j] = movies[j + 1];
                movies[j + 1] = temp;
            }
        }
    }
}

void searchMovies(const char *keyword, int results[], int &resultCount) {
    resultCount = 0;
    char lowerKeyword[100];
    strcpy(lowerKeyword, keyword);
    for (int i = 0; lowerKeyword[i]; i++)
        lowerKeyword[i] = tolower(lowerKeyword[i]);

    for (int i = 0; i < movieCount; i++) {
        if (movies[i].isActive) {
            char lowerTitle[100], lowerGenre[50];
            strcpy(lowerTitle, movies[i].title);
            strcpy(lowerGenre, movies[i].genre);
            for (int j = 0; lowerTitle[j]; j++)
                lowerTitle[j] = tolower(lowerTitle[j]);
            for (int j = 0; lowerGenre[j]; j++)
                lowerGenre[j] = tolower(lowerGenre[j]);

            if (strstr(lowerTitle, lowerKeyword) ||
                strstr(lowerGenre, lowerKeyword)) {
                results[resultCount++] = i;
            }
        }
    }
}

void displaySearchResults(int results[], int resultCount, const char *keyword) {
    clearScreen();
    showHeader("HASIL PENCARIAN FILM");

    if (resultCount == 0) {
        cout << "\nTidak ada film yang ditemukan untuk kata kunci: " << keyword
             << endl;
        pauseScreen();
        return;
    }

    cout << "\nHasil pencarian untuk: " << keyword << endl;
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

    cout << "\n[Pilih nomor film untuk detail, 0 untuk kembali]: ";
    int input = getSingletDigit();
    if (input >= 1 && input <= resultCount) {
        showMovieDetail(results[input - 1]);
    }
}

void showMovieSchedule(int movieIndex) {
    clearScreen();
    Movie movie = movies[movieIndex];
    char headerTitle[100];
    sprintf(headerTitle, "JADWAL TAYANG: %s", movie.title);
    showHeader(headerTitle);

    char currentTime[6];
    getCurrentTime(currentTime);
    cout << "\nWaktu saat ini: " << currentTime << endl;
    cout << "\n| No | Waktu | Audi | Harga     | Status    |" << endl;
    cout << "|----|-------|------|-----------|-----------|" << endl;

    bool hasAvailableShow = false;
    for (int i = 0; i < movie.showtimeCount; i++) {
        bool canBook = compareTime(currentTime, movie.showtimes[i].time) < 0;
        if (canBook)
            hasAvailableShow = true;
        printf("| %-2d | %-5s | %-4d | Rp %-6d | %-9s |\n", i + 1,
               movie.showtimes[i].time, movie.showtimes[i].auditorium,
               movie.showtimes[i].price, (canBook ? "Tersedia" : "Berakhir"));
    }

    if (!hasAvailableShow) {
        cout << "\n[Tidak ada jadwal tersedia untuk hari ini]" << endl;
        pauseScreen();
        return;
    }

    cout << "\n[Pilih jadwal untuk pesan tiket, 0 untuk kembali]: ";
    int input = getSingletDigit();
    if (input >= 1 && input <= movie.showtimeCount) {
        if (compareTime(currentTime, movie.showtimes[input - 1].time) < 0) {
            processTicketBooking(movieIndex, input - 1);
        } else {
            cout << "\n[Jadwal sudah berakhir!]" << endl;
            pauseScreen();
        }
    }
}

//============================================================
// MOVIE MANAGEMENT FUNCTION IMPLEMENTATIONS
//============================================================

void initializeMovies() {
    // Movie 1
    movies[0] = {1,        "Deadpool & Wolverine",
                 "Action", 130,
                 "D17+",   "Synopsis...",
                 1250,     {},
                 3,        {},
                 2,        0.0f,
                 true};
    movies[0].showtimes[0] = {"12:00", 1, 80, 50000};
    movies[0].showtimes[1] = {"15:00", 2, 80, 50000};
    movies[0].showtimes[2] = {"18:15", 1, 80, 55000};
    movies[0].reviews[0] = {0, 5, "Film terbaik!", "01-07-2024"};
    movies[0].reviews[1] = {1, 4, "Aksi spektakuler.", "02-07-2024"};
    calculateAverageRating(0);

    // Movie 2
    movies[1] = {2,           "Inside Out 2",
                 "Animation", 100,
                 "SU",        "Synopsis...",
                 1050,        {},
                 4,           {},
                 2,           0.0f,
                 true};
    movies[1].showtimes[0] = {"09:00", 3, 60, 45000};
    movies[1].showtimes[1] = {"11:00", 3, 60, 45000};
    movies[1].showtimes[2] = {"13:15", 2, 80, 45000};
    movies[1].showtimes[3] = {"15:30", 3, 60, 45000};
    movies[1].reviews[0] = {0, 4, "Menyentuh hati.", "05-07-2024"};
    movies[1].reviews[1] = {1, 5, "Cocok untuk keluarga.", "06-07-2024"};
    calculateAverageRating(1);

    // Movie 3
    movies[2] = {
        3, "Interstellar", "Sci-Fi", 169, "SU", "Synopsis...", 980, {}, 3, {},
        2, 0.0f,           true};
    movies[2].showtimes[0] = {"10:00", 1, 80, 50000};
    movies[2].showtimes[1] = {"13:15", 1, 80, 50000};
    movies[2].showtimes[2] = {"16:30", 1, 80, 50000};
    movies[2].reviews[0] = {0, 5, "Mind-blowing!", "10-07-2024"};
    movies[2].reviews[1] = {1, 5, "Cinematography luar biasa.", "11-07-2024"};
    calculateAverageRating(2);

    // Movie 4
    movies[3] = {4,           "Kung Fu Panda 4",
                 "Animation", 94,
                 "SU",        "Synopsis...",
                 900,         {},
                 3,           {},
                 2,           0.0f,
                 true};
    movies[3].showtimes[0] = {"17:45", 2, 80, 45000};
    movies[3].showtimes[1] = {"19:45", 3, 60, 45000};
    movies[3].showtimes[2] = {"20:30", 2, 80, 45000};
    movies[3].reviews[0] = {0, 4, "Po masih lucu!", "15-07-2024"};
    movies[3].reviews[1] = {1, 4, "Menghibur.", "16-07-2024"};
    calculateAverageRating(3);

    movieCount = 4;
}

void showMovieList() {
    clearScreen();
    showHeader("DAFTAR FILM SEDANG TAYANG");
    cout << "| No | Judul                  | Genre     | Durasi | Rating | "
            "Review | Popularitas |"
         << endl;
    cout << "|----|------------------------|-----------|--------|--------|-----"
            "---|-------------|"
         << endl;

    for (int i = 0; i < movieCount; i++) {
        if (movies[i].isActive) {
            char duration[10];
            formatDuration(movies[i].duration, duration);
            printf("| %-2d | %-22s | %-9s | %-6s | %-6s | %4.1f/10| %7d   |\n",
                   i + 1, movies[i].title, movies[i].genre, duration,
                   movies[i].rating, movies[i].averageRating,
                   movies[i].popularity);
        }
    }
    cout << "\n[Tips: Tekan S untuk sortir, nomor untuk detail, 0 kembali]"
         << endl;
    cout << "Pilihan Anda: ";
}

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
    printf("Review     : %.1f/10 dari %d review\n", movie.averageRating,
           movie.reviewCount);
    cout << "Popularitas: " << movie.popularity << " tiket terjual" << endl;
    cout << "\nSinopsis   :\n" << movie.synopsis << endl;

    cout << "\n--- Jadwal Tayang ---" << endl;
    cout << "| No | Waktu | Audi | Harga     |" << endl;
    cout << "|----|-------|------|-----------|" << endl;
    for (int i = 0; i < movie.showtimeCount; i++) {
        printf("| %-2d | %-5s | %-4d | Rp %-6d |\n", i + 1,
               movie.showtimes[i].time, movie.showtimes[i].auditorium,
               movie.showtimes[i].price);
    }
    pauseScreen();
}

void handleMovieListMenu() {
    while (true) {
        showMovieList();
        char input = getSingleInput();

        if (input == 'S' || input == 's') {
            clearScreen();
            showHeader("PILIH METODE SORTIR");
            cout << "\n1. Rating (Tertinggi)\n2. Popularitas\n3. Judul "
                    "(A-Z)\n4. Durasi (Terpendek)\n0. Kembali"
                 << endl;
            cout << "\nPilihan Anda [0-4]: ";
            int choice = getSingletDigit();
            if (choice > 0 && choice < 5)
                sortMoviesBy(choice);
        } else if (input >= '1' && input <= '9') {
            int choice = input - '0';
            if (choice <= movieCount)
                showMovieDetail(choice - 1);
        } else if (input == '0') {
            break;
        }
    }
}

void handleSearchMovie() {
    while (true) {
        clearScreen();
        showHeader("CARI FILM");
        char keyword[100];
        cout << "\nMasukkan kata kunci pencarian (judul/genre): ";
        cin >> keyword;

        int searchResult[50], resultCount = 0;
        searchMovies(keyword, searchResult, resultCount);
        displaySearchResults(searchResult, resultCount, keyword);

        cout << "\nCari lagi? (Y/N): ";
        char choice = getSingleInput();
        if (choice != 'Y' && choice != 'y')
            break;
    }
}