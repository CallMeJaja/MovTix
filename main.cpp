#include "conio.h"
#include "limits"
#include <cstdlib>
#include <cstring>
#include <iostream>

using namespace std;

// Struktur untuk menyimpan data pengguna
struct User {
    char nama[100];
    char username[50];
    char email[100];
    char password[50];
    bool isActive;
};

// Struktur untuk menyimpan jadwal tayang
struct Showtime {
    char time[6]; // Format: HH:MM
    int auditorium;
    int capacity;
    int price;
};

// Struktur untuk menyimpan data review
struct Review {
    int userId;
    int rating; // Rating dari 1 sampai 5
    char comment[200];
    char date[12]; // Format: YYYY-MM-DD
};

// Struktur untuk menyimpan data film
struct Movie {
    int id;
    char title[100];
    char genre[50];
    int duration;    // Durasi dalam menit
    char rating[10]; // Rating film (misal: PG-13, R)
    char synopsis[500];
    int popularity;
    Showtime showtimes[10];
    int showtimeCount;
    Review reviews[500];
    int reviewCount;
    float averageRating;
    bool isActive;
};

// Array untuk menyimpan data pengguna (maksimal 100 pengguna)
User users[100];
Movie movies[50];
int userCount = 0;
int movieCount = 0;
int currentUserIndex = -1;

// Fungsi untuk membersihkan layar
void clearScreen() { system("cls"); }

// Fungsi untuk menampilkan header
void showHeader(const char *title) {
    cout << "============================================================"
         << endl;
    cout << "                    " << title << endl;
    cout << "============================================================"
         << endl;
}

// Fungsi untuk mencari pengguna berdasarkan username
int findUserByUsername(const char *username) {
    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].username, username) == 0) {
            return i;
        }
    }
    return -1;
}

// Fungsi untuk mencari pengguna berdasarkan email
int findUserByEmail(const char *email) {
    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].email, email) == 0) {
            return i;
        }
    }
    return -1;
}

// Fungsi untuk validasi email sederhana
bool isValidEmail(const char *email) {
    int len = strlen(email);
    bool hasAt = false;
    bool hasDot = false;

    for (int i = 0; i < len; i++) {
        if (email[i] == '@') {
            hasAt = true;
        } else if (email[i] == '.' && hasAt) {
            hasDot = true;
        }
    }

    return hasAt && hasDot && len > 5;
}

void clearInputBuffer() {
    while (kbhit()) {
        getch();
    }
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void pauseScreen() {
    cout << "\nTekan Enter untuk melanjutkan...";

    // Bersihkan buffer dulu
    while (kbhit()) {
        getch();
    }

    // Tunggu tekanan Enter
    while (true) {
        if (kbhit()) {
            char ch = getch();
            if (ch == '\r' || ch == '\n') {
                cout << endl;
                break;
            }
        }
    }
}

// Fungsi untuk login
bool login() {
    clearScreen();
    showHeader("LOGIN MOVTIX");

    char username[50];
    char password[50];

    cout << "\nMasukkan Username : ";
    cin >> username;
    cout << "Masukkan Password : ";
    cin >> password;

    int userIndex = findUserByUsername(username);

    if (userIndex != -1 && strcmp(users[userIndex].password, password) == 0) {
        currentUserIndex = userIndex;
        cout << "\n[Login berhasil! Selamat datang, " << users[userIndex].nama
             << ".]" << endl;
        pauseScreen();
        return true;
    } else {
        cout << "\n[Login gagal. Silakan periksa username dan password Anda.]"
             << endl;
        pauseScreen();
        return false;
    }
}

// Fungsi untuk registrasi
bool registerUser() {
    clearScreen();
    showHeader("PENDAFTARAN AKUN BARU");

    if (userCount >= 100) {
        cout << "\n[Maaf, database pengguna penuh!]" << endl;
        pauseScreen();
        return false;
    }

    char nama[100];
    char username[50];
    char email[100];
    char password[50];
    char confirmPassword[50];

    cout << "\nNama Lengkap      : ";
    cin >> nama;

    cout << "Username          : ";
    cin >> username;

    // Validasi username unik
    if (findUserByUsername(username) != -1) {
        cout << "\n[Username sudah digunakan. Silakan pilih username lain.]"
             << endl;
        pauseScreen();
        return false;
    }

    cout << "Email             : ";
    cin >> email;

    // Validasi email
    if (!isValidEmail(email)) {
        cout << "\n[Format email tidak valid.]" << endl;
        pauseScreen();
        return false;
    }

    // Validasi email unik
    if (findUserByEmail(email) != -1) {
        cout << "\n[Email sudah terdaftar. Silakan gunakan email lain.]"
             << endl;
        pauseScreen();
        return false;
    }

    cout << "Password          : ";
    cin >> password;

    cout << "Konfirmasi Pass   : ";
    cin >> confirmPassword;

    // Validasi password match
    if (strcmp(password, confirmPassword) != 0) {
        cout << "\n[Password tidak cocok. Silakan coba lagi.]" << endl;
        pauseScreen();
        return false;
    }

    // Simpan data pengguna baru
    strcpy(users[userCount].nama, nama);
    strcpy(users[userCount].username, username);
    strcpy(users[userCount].email, email);
    strcpy(users[userCount].password, password);
    users[userCount].isActive = true;
    userCount++;

    cout << "\n[Registrasi berhasil! Silakan login untuk melanjutkan.]" << endl;
    pauseScreen();
    return true;
}

// Fungsi untuk reset password
bool resetPassword() {
    clearScreen();
    showHeader("RESET PASSWORD");

    char usernameOrEmail[100];
    char correctCode[] = "123456";
    char inputCode[10];
    char newPassword[50];
    char confirmPassword[50];

    cout << "\nMasukkan username/email Anda : ";
    cin >> usernameOrEmail;

    int userIndex = findUserByUsername(usernameOrEmail);
    if (userIndex == -1) {
        userIndex = findUserByEmail(usernameOrEmail);
    }

    if (userIndex == -1) {
        cout << "\n[Username/email tidak ditemukan.]" << endl;
        pauseScreen();
        return false;
    }

    // Simulasi pengiriman kode verifikasi
    cout << "\n[Kode verifikasi telah dikirim ke email Anda.]" << endl;

    cout << "Masukkan kode verifikasi    : ";
    cin >> inputCode;

    if (strcmp(inputCode, correctCode) != 0) {
        cout << "\n[Kode verifikasi salah.]" << endl;
        pauseScreen();
        return false;
    }

    cout << "Masukkan password baru      : ";
    cin >> newPassword;

    cout << "Konfirmasi password baru    : ";
    cin >> confirmPassword;

    if (strcmp(newPassword, confirmPassword) != 0) {
        cout << "\n[Password tidak cocok. Silakan coba lagi.]" << endl;
        pauseScreen();
        return false;
    }

    // Update password
    strcpy(users[userIndex].password, newPassword);

    cout << "\n[Password berhasil diubah. Silakan login.]" << endl;
    pauseScreen();
    return true;
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

// Fungsi untuk format durasi dari menit ke jam:menit
void formatDuration(int minutes, char *result) {
    int hours = minutes / 60;
    int mins = minutes % 60;
    if (hours > 0) {
        sprintf(result, "%dj%dm", hours, mins);
    } else {
        sprintf(result, "%dm", mins);
    }
}

// Fungsi untuk format jadwal tayang
void formatShowtimes(Movie movie, char *result) {
    strcpy(result, "");
    for (int i = 0; i < movie.showtimeCount; i++) {
        if (i > 0)
            strcat(result, ", ");
        strcat(result, movie.showtimes[i].time);
    }
}

// Fungsi menampilkan bintang berdasarkan rating
void displayStars(int rating) {
    for (int i = 0; i < 5; i++) {
        if (i < rating) {
            cout << "*";
        } else {
            cout << "X";
        }
    }
}

// Fungsi untuk sorting berdasarkan rating (descending)
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

// Fungsi untuk menampilkan daftar film
void showMovieList() {
    clearScreen();
    showHeader("DAFTAR FILM SEDANG TAYANG");
    cout << "\n| No | Judul                  | Genre     | Durasi | Rating | "
            "Jadwal Tayang               | Review Score | Popularitas |"
         << endl;
    cout << "|----|------------------------|-----------|--------|--------|-----"
            "------------------------|--------------|-------------|"
         << endl;

    for (int i = 0; i < movieCount; i++) {
        if (movies[i].isActive) {
            char duration[10];
            char showtimes[100];
            formatDuration(movies[i].duration, duration);
            formatShowtimes(movies[i], showtimes);

            printf("| %-2d | %-22s | %-9s | %-6s | %-6s | %-27s | %4.1f/10 "
                   "(%3d) | %7d tiket |\n",
                   i + 1, movies[i].title, movies[i].genre, duration,
                   movies[i].rating, showtimes, movies[i].averageRating,
                   movies[i].reviewCount, movies[i].popularity);
        }
    }
    cout << "\n[Tips: Tekan S untuk sortir, ketik nomor film untuk detail, 0 "
            "untuk kembali]"
         << endl;
    cout << "\nPilihan Anda: ";
}

// Fungsi untuk menampilkan detail film
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
    cout << "Popularitas: " << movie.popularity << " tiket terjual" << endl;
    printf("Review     : %.1f/10 dari %d review\n", movie.averageRating,
           movie.reviewCount);
    cout << "\nSinopsis   :" << endl;
    cout << movie.synopsis << endl;

    cout << "\n--- Jadwal Tayang ---" << endl;
    cout << "| No | Waktu | Audi | Kapasitas | Harga     |" << endl;
    cout << "|----|-------|------|-----------|-----------|" << endl;
    for (int i = 0; i < movie.showtimeCount; i++) {
        printf("| %-2d | %-5s | %-4d | %-9d | Rp %-6d |\n", i + 1,
               movie.showtimes[i].time, movie.showtimes[i].auditorium,
               movie.showtimes[i].capacity, movie.showtimes[i].price);
    }

    // Tampilkan beberapa review terakhir
    if (movie.reviewCount > 0) {
        cout << "\n--- Review Pengguna ---" << endl;
        int reviewsToShow = movie.reviewCount > 5 ? 5 : movie.reviewCount;
        for (int i = movie.reviewCount - reviewsToShow; i < movie.reviewCount;
             i++) {
            cout << "> ";
            displayStars(movie.reviews[i].rating);
            cout << " (" << movie.reviews[i].rating << "/5) - "
                 << movie.reviews[i].comment << endl;
            cout << "  Tanggal: " << movie.reviews[i].date << endl << endl;
        }
    }
    pauseScreen();
}

void showSortMenu() {
    clearScreen();
    showHeader("PILIH METODE SORTIR");

    cout << "\n1. Sortir berdasarkan Rating (Tertinggi)" << endl;
    cout << "2. Sortir berdasarkan Popularitas (Terpopuler)" << endl;
    cout << "3. Sortir berdasarkan Judul (A-Z)" << endl;
    cout << "4. Sortir berdasarkan Durasi (Terpendek)" << endl;
    cout << "0. Kembali tanpa sortir" << endl;
    cout << "\nPilihan Anda [0-4]: ";
}

char getSingleInput() {
    char input;
    while (true) {
        if (kbhit()) {
            input = getch();
            cout << input << endl;
            return input;
        }
    }
}

int getSingletDigit() {
    char input = getSingleInput();
    if (input >= '0' && input <= '9') {
        return input - '0';
    } else {
        return -1; // Invalid input
    }
}

// Fungsi untuk menangani menu daftar film
void handleMovieListMenu() {
    char input;

    while (true) {
        showMovieList();
        input = getSingleInput();

        // Cek apakah input adalah 'S' untuk sorting
        if (input == 'S' || input == 's') {
            showSortMenu();
            int choice = getSingletDigit();

            switch (choice) {
            case 1:
                sortMoviesByRating();
                cout << "\n[Film berhasil diurutkan berdasarkan rating "
                        "tertinggi]"
                     << endl;
                pauseScreen();
                break;
            case 2:
                sortMoviesByPopularity();
                cout << "\n[Film berhasil diurutkan berdasarkan popularitas]"
                     << endl;
                pauseScreen();
                break;
            case 3:
                sortMoviesByTitle();
                cout << "\n[Film berhasil diurutkan berdasarkan judul A-Z]"
                     << endl;
                pauseScreen();
                break;
            case 4:
                sortMoviesByDuration();
                cout << "\n[Film berhasil diurutkan berdasarkan durasi "
                        "terpendek]"
                     << endl;
                pauseScreen();
                break;
            case 0:
                break;
            default:
                cout << "\nPilihan tidak valid! Silahkan pilih 0-4" << endl;
                pauseScreen();
                break;
            }
        } else if (input >= '0' && input <= '9') {
            int choice = input - '0';

            if (choice == 0) {
                break; // Kembali ke menu utama
            } else if (choice >= 1 && choice <= movieCount) {
                showMovieDetail(choice - 1);
            } else {
                cout << "\nPilihan tidak valid! Silakan pilih nomor film yang "
                        "tersedia."
                     << endl;
                pauseScreen();
            }
        } else {
            cout << "\nInput tidak valid! Gunakan S untuk sorting, 0-"
                 << movieCount << " untuk memilih film." << endl;
            pauseScreen();
        }
    }
}

// Fungsi untuk menampilkan menu autentikasi
void showAuthMenu() {
    clearScreen();
    showHeader("MOVTIX BIOSKOP");

    cout << "\nSelamat datang di MOVTIX!" << endl;
    cout << "Pilih salah satu opsi berikut:" << endl;
    cout << "\n1. Login" << endl;
    cout << "2. Daftar Akun Baru" << endl;
    cout << "3. Lupa Password" << endl;
    cout << "4. Keluar" << endl;
    cout << "\nSilakan masukkan pilihan [1-4]: ";
}

// Fungsi untuk menampilkan menu utama (sementara)
void showMainMenu() {
    clearScreen();
    showHeader("MENU UTAMA MOVTIX");

    cout << "\n1. Daftar Film Sedang Tayang" << endl;
    cout << "2. Cari Film" << endl;
    cout << "3. Pesan Tiket" << endl;
    cout << "4. Histori Transaksi" << endl;
    cout << "5. Penukaran Tiket & F&B" << endl;
    cout << "6. Keluar / Logout" << endl;
    cout << "\nMasukkan pilihan [1-6]: ";
}

// Replace the current searchMovies function with this optimized version
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

// Update displaySearchResults to work with indices instead of full Movie
// objects
void displaySearchResults(int results[], int resultCount, const char *keyword) {
    clearScreen();
    showHeader("CARI FILM MOVTIX");

    if (resultCount == 0) {
        cout << "\nTidak ada film yang ditemukan untuk kata kunci : " << keyword
             << endl;
        cout << "\nSaran:" << endl;
        cout << "- Pastikan ejaan kata kunci." << endl;
        cout << "- Coba kata kunci lain." << endl;
        pauseScreen();
        return;
    }

    cout << "\nHasil pencarian untuk: " << keyword << endl;

    cout << "\nHasil pencarian:" << endl;
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

    cout << "\n[Pilih nomor film untuk detail, 0 untuk kembali]" << endl;
    cout << "\nPilihan Anda: ";
    int input = getSingletDigit();

    if (input >= 1 && input <= resultCount) {
        // Gunakan index yang sudah disimpan
        showMovieDetail(results[input - 1]);
    } else if (input != 0) {
        cout << "\nPilihan tidak valid! Silakan pilih nomor film yang "
                "tersedia."
             << endl;
        pauseScreen();
    }
}

void handleSearchMovie() {
    int searchResult[50];
    int resultCount = 0;
    char keyword[100];

    while (true) {
        clearScreen();
        showHeader("CARI FILM MOVTIX");
        cout << "\nMasukkan kata kunci pencarian (judul/genre): ";

        cin.getline(keyword, sizeof(keyword));

        // Lakukan pencarian
        searchMovies(keyword, searchResult, resultCount);

        // Tampilkan hasil
        displaySearchResults(searchResult, resultCount, keyword);

        // Tanyakan apakah ingin mencari lagi
        cout << "\nIngin mencari lagi? (Y/N): ";
        char choice = getSingleInput();
        if (choice == 'N' || choice == 'n') {
            break; // Kembali ke menu utama
        } else if (choice != 'Y' && choice != 'y') {
            cout << "\nPilihan tidak valid! Kembali ke menu utama." << endl;
            pauseScreen();
            break;
        }
    }
}

// Fungsi utama program
int main() {
    int choice;
    bool isLoggedIn = false;

    // Inisialisasi data pengguna demo
    strcpy(users[0].nama, "Admin MOVTIX");
    strcpy(users[0].username, "admin");
    strcpy(users[0].email, "admin@movtix.com");
    strcpy(users[0].password, "admin123");

    strcpy(users[1].nama, "Reza Asriano Maulana");
    strcpy(users[1].username, "reza");
    strcpy(users[1].email, "reza@movtix.com");
    strcpy(users[1].password, "reza");

    users[0].isActive = true;
    users[1].isActive = true;
    userCount = 2;

    // Inisialisasi data film
    initializeMovies();

    while (true) {
        if (!isLoggedIn) {
            showAuthMenu();
            choice = getSingletDigit();

            if (choice == -1) {
                cout << "\nPilihan tidak valid. Silakan pilih 1-4." << endl;
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
                cout << "\n[Fitur Pesan Tiket belum diimplementasikan]" << endl;
                pauseScreen();
                break;
            case 4:
                cout << "\n[Fitur Histori Transaksi belum diimplementasikan]"
                     << endl;
                pauseScreen();
                break;
            case 5:
                cout
                    << "\n[Fitur Penukaran Tiket & F&B belum diimplementasikan]"
                    << endl;
                pauseScreen();
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