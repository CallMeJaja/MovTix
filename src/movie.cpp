#include "movie.h"
#include "utils.h"
#include <cstring>
#include <iostream>

using namespace std;

void initializeMovies() {
    // Film 1: Deadpool & Wolverine
    movies[0].id = 1;
    strcpy(movies[0].title, "Deadpool & Wolverine");
    strcpy(movies[0].genre, "Action");
    movies[0].duration = 130;
    strcpy(movies[0].rating, "D17+");
    strcpy(movies[0].synopsis,
           "Deadpool dan Wolverine bersatu dalam petualangan penuh aksi yang "
           "menggabungkan humor khas Deadpool dengan intensitas Wolverine. "
           "Sebuah film superhero yang menghadirkan pertarungan epik dengan "
           "dialog yang menghibur.");
    movies[0].popularity = 1250;
    movies[0].showtimeCount = 3;
    strcpy(movies[0].showtimes[0].time, "12:00");
    movies[0].showtimes[0].auditorium = 1;
    movies[0].showtimes[0].capacity = 80;
    movies[0].showtimes[0].price = 50000;
    strcpy(movies[0].showtimes[1].time, "15:00");
    movies[0].showtimes[1].auditorium = 2;
    movies[0].showtimes[1].capacity = 80;
    movies[0].showtimes[1].price = 50000;
    strcpy(movies[0].showtimes[2].time, "18:15");
    movies[0].showtimes[2].auditorium = 1;
    movies[0].showtimes[2].capacity = 80;
    movies[0].showtimes[2].price = 55000;

    // Sample reviews untuk Deadpool & Wolverine
    movies[0].reviewCount = 3;
    movies[0].reviews[0].userId = 0;
    movies[0].reviews[0].rating = 5;
    strcpy(movies[0].reviews[0].comment, "Film superhero terbaik tahun ini!");
    strcpy(movies[0].reviews[0].date, "01-07-2024");
    movies[0].reviews[1].userId = 1;
    movies[0].reviews[1].rating = 4;
    strcpy(movies[0].reviews[1].comment,
           "Aksi yang spektakuler dan humor yang kocak.");
    strcpy(movies[0].reviews[1].date, "02-07-2024");
    movies[0].reviews[2].userId = 2;
    movies[0].reviews[2].rating = 5;
    strcpy(movies[0].reviews[2].comment,
           "Wajib nonton! Chemistry Deadpool-Wolverine luar biasa.");
    strcpy(movies[0].reviews[2].date, "03-07-2024");

    movies[0].isActive = true;
    calculateAverageRating(0);

    // Film 2: Inside Out 2
    movies[1].id = 2;
    strcpy(movies[1].title, "Inside Out 2");
    strcpy(movies[1].genre, "Animation");
    movies[1].duration = 100;
    strcpy(movies[1].rating, "SU");
    strcpy(movies[1].synopsis,
           "Melanjutkan petualangan emosi Riley yang kini beranjak remaja. "
           "Film animasi Pixar yang mengeksplorasi kompleksitas emosi manusia "
           "dengan cara yang menghibur dan mendidik untuk seluruh keluarga.");
    movies[1].popularity = 1050;
    movies[1].showtimeCount = 4;
    strcpy(movies[1].showtimes[0].time, "09:00");
    movies[1].showtimes[0].auditorium = 3;
    movies[1].showtimes[0].capacity = 60;
    movies[1].showtimes[0].price = 45000;
    strcpy(movies[1].showtimes[1].time, "11:00");
    movies[1].showtimes[1].auditorium = 3;
    movies[1].showtimes[1].capacity = 60;
    movies[1].showtimes[1].price = 45000;
    strcpy(movies[1].showtimes[2].time, "13:15");
    movies[1].showtimes[2].auditorium = 2;
    movies[1].showtimes[2].capacity = 80;
    movies[1].showtimes[2].price = 45000;
    strcpy(movies[1].showtimes[3].time, "15:30");
    movies[1].showtimes[3].auditorium = 3;
    movies[1].showtimes[3].capacity = 60;
    movies[1].showtimes[3].price = 45000;

    // Sample reviews untuk Inside Out 2
    movies[1].reviewCount = 4;
    movies[1].reviews[0].userId = 0;
    movies[1].reviews[0].rating = 4;
    strcpy(movies[1].reviews[0].comment,
           "Pixar selalu berhasil menyentuh hati.");
    strcpy(movies[1].reviews[0].date, "05-07-2024");
    movies[1].reviews[1].userId = 1;
    movies[1].reviews[1].rating = 5;
    strcpy(movies[1].reviews[1].comment,
           "Cocok untuk keluarga, pesan moral yang dalam.");
    strcpy(movies[1].reviews[1].date, "06-07-2024");
    movies[1].reviews[2].userId = 2;
    movies[1].reviews[2].rating = 4;
    strcpy(movies[1].reviews[2].comment,
           "Animasi yang memukau dan cerita yang menarik.");
    strcpy(movies[1].reviews[2].date, "07-07-2024");
    movies[1].reviews[3].userId = 3;
    movies[1].reviews[3].rating = 5;
    strcpy(movies[1].reviews[3].comment,
           "Masterpiece! Riley's journey sangat relatable.");
    strcpy(movies[1].reviews[3].date, "08-07-2024");

    movies[1].isActive = true;
    calculateAverageRating(1);

    // Film 3: Interstellar
    movies[2].id = 3;
    strcpy(movies[2].title, "Interstellar");
    strcpy(movies[2].genre, "Sci-Fi");
    movies[2].duration = 169;
    strcpy(movies[2].rating, "SU");
    strcpy(movies[2].synopsis,
           "Sebuah masterpiece Christopher Nolan yang mengisahkan perjalanan "
           "luar angkasa untuk mencari planet baru yang dapat dihuni manusia. "
           "Film yang menggabungkan sains kompleks dengan emosi kemanusiaan "
           "yang mendalam.");
    movies[2].popularity = 980;
    movies[2].showtimeCount = 3;
    strcpy(movies[2].showtimes[0].time, "10:00");
    movies[2].showtimes[0].auditorium = 1;
    movies[2].showtimes[0].capacity = 80;
    movies[2].showtimes[0].price = 50000;
    strcpy(movies[2].showtimes[1].time, "13:15");
    movies[2].showtimes[1].auditorium = 1;
    movies[2].showtimes[1].capacity = 80;
    movies[2].showtimes[1].price = 50000;
    strcpy(movies[2].showtimes[2].time, "16:30");
    movies[2].showtimes[2].auditorium = 1;
    movies[2].showtimes[2].capacity = 80;
    movies[2].showtimes[2].price = 50000;

    // Sample reviews untuk Interstellar
    movies[2].reviewCount = 5;
    movies[2].reviews[0].userId = 0;
    movies[2].reviews[0].rating = 5;
    strcpy(movies[2].reviews[0].comment,
           "Christopher Nolan genius! Mind-blowing sci-fi.");
    strcpy(movies[2].reviews[0].date, "10-07-2024");
    movies[2].reviews[1].userId = 1;
    movies[2].reviews[1].rating = 5;
    strcpy(movies[2].reviews[1].comment,
           "Cinematography luar biasa, cerita yang dalam.");
    strcpy(movies[2].reviews[1].date, "11-07-2024");
    movies[2].reviews[2].userId = 2;
    movies[2].reviews[2].rating = 4;
    strcpy(movies[2].reviews[2].comment,
           "Kompleks tapi worth it. Hans Zimmer score amazing!");
    strcpy(movies[2].reviews[2].date, "12-07-2024");
    movies[2].reviews[3].userId = 3;
    movies[2].reviews[3].rating = 5;
    strcpy(movies[2].reviews[3].comment,
           "Emotional journey yang tak terlupakan.");
    strcpy(movies[2].reviews[3].date, "13-07-2024");
    movies[2].reviews[4].userId = 4;
    movies[2].reviews[4].rating = 5;
    strcpy(movies[2].reviews[4].comment,
           "Masterpiece! Wajib ditonton berkali-kali.");
    strcpy(movies[2].reviews[4].date, "14-07-2024");

    movies[2].isActive = true;
    calculateAverageRating(2);

    // Film 4: Kung Fu Panda 4
    movies[3].id = 4;
    strcpy(movies[3].title, "Kung Fu Panda 4");
    strcpy(movies[3].genre, "Animation");
    movies[3].duration = 94;
    strcpy(movies[3].rating, "SU");
    strcpy(movies[3].synopsis,
           "Po kembali dalam petualangan baru sebagai Dragon Warrior. Kali ini "
           "ia harus menghadapi tantangan terbesar sambil mencari penerus yang "
           "tepat untuk melanjutkan warisan Kung Fu Panda.");
    movies[3].popularity = 900;
    movies[3].showtimeCount = 3;
    strcpy(movies[3].showtimes[0].time, "17:45");
    movies[3].showtimes[0].auditorium = 2;
    movies[3].showtimes[0].capacity = 80;
    movies[3].showtimes[0].price = 45000;
    strcpy(movies[3].showtimes[1].time, "19:45");
    movies[3].showtimes[1].auditorium = 3;
    movies[3].showtimes[1].capacity = 60;
    movies[3].showtimes[1].price = 45000;
    strcpy(movies[3].showtimes[2].time, "20:30");
    movies[3].showtimes[2].auditorium = 2;
    movies[3].showtimes[2].capacity = 80;
    movies[3].showtimes[2].price = 45000;

    // Sample reviews untuk Kung Fu Panda 4
    movies[3].reviewCount = 3;
    movies[3].reviews[0].userId = 0;
    movies[3].reviews[0].rating = 4;
    strcpy(movies[3].reviews[0].comment, "Po masih lucu seperti biasa!");
    strcpy(movies[3].reviews[0].date, "15-07-2024");
    movies[3].reviews[1].userId = 1;
    movies[3].reviews[1].rating = 4;
    strcpy(movies[3].reviews[1].comment,
           "Animasi bagus, cerita menghibur untuk anak-anak.");
    strcpy(movies[3].reviews[1].date, "16-07-2024");
    movies[3].reviews[2].userId = 2;
    movies[3].reviews[2].rating = 3;
    strcpy(movies[3].reviews[2].comment,
           "Standar franchise Kung Fu Panda, tidak mengecewakan.");
    strcpy(movies[3].reviews[2].date, "17-07-2024");

    movies[3].isActive = true;
    calculateAverageRating(3);

    movieCount = 4;
}

void calculateAverageRating(int movieIndex) {
    if (movies[movieIndex].reviewCount == 0) {
        movies[movieIndex].averageRating = 0.0f;
    }

    int totalRating = 0;
    for (int i = 0; i < movies[movieIndex].reviewCount; i++) {
        totalRating += movies[movieIndex].reviews[i].rating;
    }

    // Konversi dari skala 1-5 ke skala 0-10
    movies[movieIndex].averageRating =
        (float)(totalRating * 2.0) / movies[movieIndex].reviewCount;
}

void formatShowtimes(Movie movie, char *result) {
    strcpy(result, "");
    for (int i = 0; i < movie.showtimeCount; i++) {
        if (i > 0)
            strcat(result, ", ");
        strcat(result, movie.showtimes[i].time);
    }
}

void sortMoviesByRating() {
    for (int i = 0; i < movieCount - 1; i++) {
        for (int j = 0; j < movieCount - 1; j++) {
            if (movies[j].averageRating < movies[j + 1].averageRating) {
                Movie temp = movies[j];
                movies[j] = movies[j + 1];
                movies[j + 1] = temp;
            }
        }
    }
}

void sortMoviesByPopularity() {
    for (int i = 0; i < movieCount - 1; i++) {
        for (int j = 0; j < movieCount - 1; j++) {
            if (movies[j].popularity < movies[j + 1].popularity) {
                Movie temp = movies[j];
                movies[j] = movies[j + 1];
                movies[j + 1] = temp;
            }
        }
    }
}

void sortMoviesByTitle() {
    for (int i = 0; i < movieCount - 1; i++) {
        for (int j = 0; j < movieCount - 1; j++) {
            if (strcmp(movies[j].title, movies[j + 1].title) > 0) {
                Movie temp = movies[j];
                movies[j] = movies[j + 1];
                movies[j + 1] = temp;
            }
        }
    }
}

void sortMoviesByDuration() {
    for (int i = 0; i < movieCount - 1; i++) {
        for (int j = 0; j < movieCount - i - 1; j++) {
            if (movies[j].duration > movies[j + 1].duration) {
                Movie temp = movies[j];
                movies[j] = movies[j + 1];
                movies[j + 1] = temp;
            }
        }
    }
}

void searchMovies(const char *keyword, int results[], int &resultCount) {
    resultCount = 0;

    for (int i = 0; i < movieCount; i++) {
        if (movies[i].isActive) {
            bool found = false;

            // Konversi ke lowercase untuk pencarian case-insensitive
            char movieTitle[100];
            char movieGenre[50];
            char searchKeyword[100];

            strcpy(movieTitle, movies[i].title);
            strcpy(movieGenre, movies[i].genre);
            strcpy(searchKeyword, keyword);

            // Konversi ke lowercase
            for (int j = 0; j < strlen(movieTitle); j++) {
                movieTitle[j] = tolower(movieTitle[j]);
            }

            for (int j = 0; j < strlen(movieGenre); j++) {
                movieGenre[j] = tolower(movieGenre[j]);
            }

            for (int j = 0; j < strlen(searchKeyword); j++) {
                searchKeyword[j] = tolower(searchKeyword[j]);
            }

            // Cari di judul (substring)
            if (strstr(movieTitle, searchKeyword) != nullptr) {
                found = true;
            }

            // Cari di genre (substring)
            if (!found && strstr(movieGenre, searchKeyword) != nullptr) {
                found = true;
            }

            if (found) {
                // Simpan index movie, bukan copy seluruh struct
                results[resultCount] = i;
                resultCount++;
            }
        }
    }
}
