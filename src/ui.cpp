#include "ui.h"
#include "booking.h"
#include "conio.h"
#include "iomanip"
#include "movie.h"
#include "utils.h"
#include <cstring>
#include <iostream>
using namespace std;

void showHeader(const char *title) {
    cout << "============================================================"
         << endl;
    cout << "                    " << title << endl;
    cout << "============================================================"
         << endl;
}

void displayStars(int rating) {
    for (int i = 0; i < 5; i++) {
        if (i < rating) {
            cout << "*";
        } else {
            cout << "X";
        }
    }
}

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

void showMainMenu() {
    clearScreen();
    showHeader("MENU UTAMA MOVTIX");

    // Tampilkan waktu saat ini
    char currentDateTime[20];
    getCurrentDateTime(currentDateTime);
    cout << "\nWaktu saat ini: " << currentDateTime << endl;
    cout << "\nSelamat datang, " << users[currentUserIndex].nama << "!" << endl;
    cout << "Poin MOVTIX Anda: " << users[currentUserIndex].movtixPoints
         << " poin" << endl;

    cout << "\n1. Daftar Film Sedang Tayang" << endl;
    cout << "2. Cari Film" << endl;
    cout << "3. Pesan Tiket" << endl;
    cout << "4. Histori Transaksi" << endl;
    cout << "5. Penukaran Tiket & F&B" << endl;
    cout << "6. Keluar" << endl;
    cout << "\nMasukkan pilihan [1-6]: ";
}

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

void handleSearchMovie() {
    int searchResult[50];
    int resultCount = 0;
    char keyword[100];

    while (true) {
        clearScreen();
        showHeader("CARI FILM MOVTIX");
        cout << "\nMasukkan kata kunci pencarian (judul/genre): ";

        cin >> keyword;

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

void showMovieSchedule(int movieIndex) {
    clearScreen();
    showHeader("JADWAL TAYANG FILM");

    Movie movie = movies[movieIndex];
    char currentTime[6];
    getCurrentTime(currentTime);

    cout << "\nFilm: " << movie.title << endl;
    cout << "Waktu saat ini: " << currentTime << endl;

    cout << "\n| No | Waktu | Audi | Kapasitas | Harga     | Status    |"
         << endl;
    cout << "|----|-------|------|-----------|-----------|-----------|" << endl;

    bool hasAvailableShow = false;
    for (int i = 0; i < movie.showtimeCount; i++) {
        char status[15];
        bool canBook = compareTime(currentTime, movie.showtimes[i].time) < 0;

        if (canBook) {
            strcpy(status, "Tersedia");
            hasAvailableShow = true;
        } else {
            strcpy(status, "Berakhir");
        }

        printf("| %-2d | %-5s | %-4d | %-9d | Rp %-6d | %-9s |\n", i + 1,
               movie.showtimes[i].time, movie.showtimes[i].auditorium,
               movie.showtimes[i].capacity, movie.showtimes[i].price, status);
    }

    if (!hasAvailableShow) {
        cout << "\n[Maaf, tidak ada jadwal yang tersedia untuk hari ini]"
             << endl;
        pauseScreen();
        return;
    }

    cout << "\n[Pilih nomor jadwal untuk pesan tiket, 0 untuk kembali]" << endl;
    cout << "\nPilihan Anda: ";

    int input = getSingletDigit();
    if (input >= 1 && input <= movie.showtimeCount) {
        // Validasi jadwal masih tersedia
        if (compareTime(currentTime, movie.showtimes[input - 1].time) < 0) {
            processTicketBooking(movieIndex, input - 1);
        } else {
            cout << "\n[Jadwal sudah berakhir! Silakan pilih jadwal lain.]"
                 << endl;
            pauseScreen();
            showMovieSchedule(movieIndex); // Kembali ke pemilihan jadwal
        }
    } else if (input != 0) {
        cout << "\nPilihan tidak valid!" << endl;
        pauseScreen();
        showMovieSchedule(movieIndex);
    }
}

void displaySeatMap(int auditorium) {
    // Cek kursi yang kadaluarsa sebelum menampilkan
    checkExpiredReservations();

    clearScreen();
    // Fix: Gunakan sprintf atau cara lain tanpa to_string
    char headerTitle[100];
    sprintf(headerTitle, "PETA KURSI AUDITORIUM %d", auditorium);
    showHeader(headerTitle);

    cout << "\nNote: [  ] = Tersedia  [PP] = Pending  [TI] = Terisi\n" << endl;

    // Header kolom
    cout << "     ";
    for (int col = 1; col <= 8; col++) {
        printf("%02d   ", col);
    }
    cout << endl << endl;

    int seatIndex = 0;
    char currentRow = 'A';

    while (seatIndex < seatCounts[auditorium - 1]) {
        cout << currentRow << " | ";

        // Tampilkan kursi untuk baris ini
        int colCount = 0;
        while (seatIndex < seatCounts[auditorium - 1] &&
               auditoriumSeats[auditorium - 1][seatIndex].seatNumber[0] ==
                   currentRow) {

            switch (auditoriumSeats[auditorium - 1][seatIndex].status) {
            case AVAILABLE:
                cout << "[  ] ";
                break;
            case PENDING:
                cout << "[PD] ";
                break;
            case TAKEN:
                cout << "[TI] ";
                break;
            }

            seatIndex++;
            colCount++;
        }

        // Untuk auditorium 3, baris H hanya 4 kursi
        if (auditorium == 3 && currentRow == 'H') {
            for (int i = colCount; i < 8; i++) {
                cout << "     ";
            }
        }

        cout << endl;
        currentRow++;
    }

    cout << "\n                 [ LAYAR ]" << endl;
    cout << "\nContoh input kursi: A01,B05,F10 (pisahkan dengan koma)" << endl;
}

void showTicketConfirmation(int transactionId) {
    clearScreen();
    showHeader("KONFIRMASI TIKET");

    Transaction trans = transactions[transactionId];

    cout << "\n==================== E-TICKET MOVTIX ===================="
         << endl;
    cout << "Kode Booking   : MOVTIX" << setfill('0') << setw(4) << trans.id
         << endl;
    cout << "Film           : " << trans.movieTitle << endl;
    cout << "Tanggal        : " << trans.transactionDate << endl;
    cout << "Jam Tayang     : " << trans.showtime << endl;
    cout << "Auditorium     : " << trans.auditorium << endl;
    cout << "Kursi          : " << trans.selectedSeats << endl;
    cout << "Jumlah Tiket   : " << trans.ticketCount << endl;

    if (trans.fnbCount > 0) {
        cout << "\nF&B yang dipesan:" << endl;
        for (int i = 0; i < trans.fnbCount; i++) {
            printf("- %dx %s = Rp %d\n", trans.selectedFnB[i].quantity,
                   trans.selectedFnB[i].name, trans.selectedFnB[i].totalPrice);
        }
    }

    cout << "\nTotal Pembayaran: Rp " << trans.totalPrice << endl;
    cout << "Status          : LUNAS" << endl;
    cout << "=========================================================" << endl;

    cout << "\nTiket berhasil dibeli!" << endl;
    cout << "Simpan kode booking untuk penukaran tiket di bioskop." << endl;
    pauseScreen();
}

void handleTransactionHistory() {
    while (true) {
        clearScreen();
        showHeader("HISTORI TRANSAKSI MOVTIX");

        char currentDateTime[20];
        getCurrentDateTime(currentDateTime);
        cout << "\nWaktu saat ini: " << currentDateTime << endl;
        cout << "\nSelamat datang, " << users[currentUserIndex].nama << "!"
             << endl;

        cout << "\n1. Lihat Histori Transaksi" << endl;
        cout << "2. Ringkasan Aktivitas" << endl;
        cout << "0. Kembali ke Menu Utama" << endl;
        cout << "\nPilihan Anda [0-2]: ";

        char choice = getSingleInput();

        switch (choice) {
        case '1':
            showTransactionHistory();
            break;

        case '2':
            showTransactionSummary();
            break;

        case '0':
            return;

        default:
            cout << "\nPilihan tidak valid!" << endl;
            pauseScreen();
            break;
        }
    }
}

void showTransactionHistory() {
    clearScreen();
    showHeader("HISTORI TRANSAKSI MOVTIX");

    // Cek expired reservations terlebih dahulu
    checkExpiredReservations();

    // Filter transaksi berdasarkan user yang sedang login
    int userTransactions[100];
    int userTransactionCount = 0;

    for (int i = 0; i < transactionCount; i++) {
        if (transactions[i].userId == currentUserIndex &&
            transactions[i].isActive) {
            userTransactions[userTransactionCount] = i;
            userTransactionCount++;
        }
    }

    if (userTransactionCount == 0) {
        cout << "\nAnda belum memiliki riwayat transaksi." << endl;
        cout << "Silakan pesan tiket terlebih dahulu." << endl;
        pauseScreen();
        return;
    }

    cout << "\nNama: " << users[currentUserIndex].nama << endl;
    cout << "\n| No | Tanggal   | Film         | Jadwal | Audi | Kursi   | "
            "Status    | Total    |"
         << endl;
    cout << "|----|-----------|--------------|--------|------|---------|-------"
            "----|----------|"
         << endl;

    for (int i = 0; i < userTransactionCount; i++) {
        int transIndex = userTransactions[i];
        Transaction trans = transactions[transIndex];

        char shortDate[10];
        formatDate(trans.transactionDate, shortDate);

        // Truncate title jika terlalu panjang
        char shortTitle[13];
        if (strlen(trans.movieTitle) > 12) {
            strncpy(shortTitle, trans.movieTitle, 12);
            shortTitle[12] = '\0';
        } else {
            strcpy(shortTitle, trans.movieTitle);
        }

        // Truncate kursi jika terlalu panjang
        char shortSeats[8];
        if (strlen(trans.selectedSeats) > 7) {
            strncpy(shortSeats, trans.selectedSeats, 7);
            shortSeats[7] = '\0';
        } else {
            strcpy(shortSeats, trans.selectedSeats);
        }

        printf("| %-2d | %-9s | %-12s | %-6s | %-4d | %-7s | %-9s | %-8d |\n",
               i + 1, shortDate, shortTitle, trans.showtime, trans.auditorium,
               shortSeats, getStatusString(trans.status), trans.totalPrice);
    }

    cout << "\n[Pilih nomor transaksi untuk detail/pembayaran ulang (jika "
            "UNPAID), atau 0 untuk kembali]"
         << endl;
    cout << "\nPilihan Anda: ";

    int choice = getSingletDigit();

    if (choice >= 1 && choice <= userTransactionCount) {
        int selectedTransIndex = userTransactions[choice - 1];
        showTransactionDetail(selectedTransIndex);
    } else if (choice != 0) {
        cout << "\nPilihan tidak valid!" << endl;
        pauseScreen();
        showTransactionHistory(); // Kembali ke histori
    }
}

void showTransactionDetail(int transactionIndex) {
    clearScreen();
    showHeader("DETAIL TRANSAKSI");

    Transaction trans = transactions[transactionIndex];

    cout << "ID Transaksi   : MOVTIX" << setfill('0') << setw(4) << trans.id
         << endl;
    cout << "Status         : " << getStatusString(trans.status) << endl;
    cout << "Tanggal Pesan  : " << trans.transactionDate << endl;

    if (trans.status == PAID && strlen(trans.bookingCode) > 0) {
        cout << "Kode Booking   : " << trans.bookingCode << endl;
        cout << "Passkey        : " << trans.passkey << endl;
    }

    if (trans.status == PENDING_PAYMENT) {
        cout << "Batas Bayar    : " << trans.expiryTime << endl;

        // Cek apakah masih dalam batas waktu
        char currentTime[20];
        getCurrentDateTime(currentTime);
        int timeDiff = compareDateTime(trans.expiryTime, currentTime);

        if (timeDiff > 0) {
            cout << "Status Waktu   : Masih dalam batas waktu" << endl;
        } else {
            cout << "Status Waktu   : KADALUARSA" << endl;
        }
    }

    cout << "\n--- Detail Film ---" << endl;
    cout << "Film           : " << trans.movieTitle << endl;
    cout << "Jadwal         : " << trans.showtime << endl;
    cout << "Auditorium     : " << trans.auditorium << endl;
    cout << "Kursi          : " << trans.selectedSeats << endl;
    cout << "Jumlah Tiket   : " << trans.ticketCount << " tiket" << endl;
    cout << "Harga Tiket    : Rp " << trans.ticketPrice << endl;

    if (trans.fnbCount > 0) {
        cout << "\n--- Detail F&B ---" << endl;
        for (int i = 0; i < trans.fnbCount; i++) {
            printf("- %dx %s = Rp %d\n", trans.selectedFnB[i].quantity,
                   trans.selectedFnB[i].name, trans.selectedFnB[i].totalPrice);
        }
        cout << "Total F&B      : Rp " << trans.fnbPrice << endl;
    }

    cout << "\nTOTAL BAYAR    : Rp " << trans.totalPrice << endl;
    cout << "=========================================================="
         << endl;

    // Menu aksi berdasarkan status
    if (trans.status == PENDING_PAYMENT) {
        char currentTime[20];
        getCurrentDateTime(currentTime);

        if (compareDateTime(currentTime, trans.expiryTime) >= 0) {
            cout << "\n[Transaksi ini telah kadaluarsa dan dibatalkan otomatis]"
                 << endl;
            pauseScreen();
            return;
        }

        cout << "\nPilihan:" << endl;
        cout << "1. Lanjutkan Pembayaran" << endl;
        cout << "2. Batalkan Transaksi" << endl;
        cout << "0. Kembali" << endl;
        cout << "\nPilihan Anda [0-2]: ";

        char choice = getSingleInput();

        switch (choice) {
        case '1':
            // Lanjut ke pembayaran
            processPayment(transactionIndex);
            break;

        case '2': {
            cout << "\nApakah Anda yakin ingin membatalkan transaksi ini? "
                    "(Y/N): ";
            char confirm = getSingleInput();
            if (confirm == 'Y' || confirm == 'y') {
                cancelReservation(transactionIndex);
                cout << "\nTransaksi berhasil dibatalkan. Kursi telah "
                        "dibebaskan."
                     << endl;
                pauseScreen();
            }
            break;
        }

        case '0':
            return;

        default:
            cout << "\nPilihan tidak valid!" << endl;
            pauseScreen();
            showTransactionDetail(transactionIndex);
            break;
        }
    } else if (trans.status == PAID) {
        cout << "\n[Transaksi ini telah dibayar. E-Ticket dapat digunakan "
                "untuk masuk bioskop]"
             << endl;

        // Tampilkan informasi lengkap untuk transaksi yang sudah dibayar
        cout << "\n=== INFORMASI PENUKARAN TIKET ===" << endl;
        cout << "Kode Booking : " << trans.bookingCode << endl;
        cout << "Passkey      : " << trans.passkey << endl;
        cout << "Gunakan kode ini di Menu 5 untuk penukaran tiket dan F&B"
             << endl;
        cout << "=========================================" << endl;

        cout << "\nPilihan:" << endl;
        cout << "1. Tampilkan E-Ticket" << endl;
        cout << "2. Salin Kode" << endl;
        cout << "0. Kembali" << endl;
        cout << "\nPilihan Anda [0-2]: ";

        char choice = getSingleInput();

        switch (choice) {
        case '1':
            showTicketConfirmation(transactionIndex);
            break;
        case '2':
            clearScreen();
            showHeader("KODE UNTUK PENUKARAN TIKET");
            cout << "\n=================== SIMPAN KODE INI ==================="
                 << endl;
            cout << "Kode Booking : " << trans.bookingCode << endl;
            cout << "Passkey      : " << trans.passkey << endl;
            cout << "Film         : " << trans.movieTitle << endl;
            cout << "Jadwal       : " << trans.showtime << endl;
            cout << "Auditorium   : " << trans.auditorium << endl;
            cout << "Kursi        : " << trans.selectedSeats << endl;
            if (trans.fnbCount > 0) {
                cout << "\nF&B untuk diklaim di Menu 5:" << endl;
                for (int i = 0; i < trans.fnbCount; i++) {
                    printf("- %dx %s\n", trans.selectedFnB[i].quantity,
                           trans.selectedFnB[i].name);
                }
            }
            cout << "======================================================="
                 << endl;
            cout << "\nMasuk ke Menu Utama Nomor 5 dengan kode ini untuk "
                    "penukaran tiket."
                 << endl;
            pauseScreen();
            break;
        case '0':
            return;
        default:
            cout << "\nPilihan tidak valid!" << endl;
            pauseScreen();
            showTransactionDetail(transactionIndex);
            break;
        }
    } else if (trans.status == EXPIRED) {
        cout << "\n[Transaksi ini telah kadaluarsa karena tidak dibayar dalam "
                "batas waktu]"
             << endl;
        pauseScreen();
    } else if (trans.status == CANCELLED) {
        cout << "\n[Transaksi ini telah dibatalkan]" << endl;
        pauseScreen();
    } else if (trans.status == USED) {
        cout << "\n[Transaksi ini telah digunakan untuk masuk bioskop]" << endl;
        cout << "Kursi dan F&B sudah diklaim." << endl;
        pauseScreen();
    } else {
        cout << "\nStatus transaksi tidak diketahui!" << endl;
        pauseScreen();
    }
}

void showTransactionSummary() {
    clearScreen();
    showHeader("RINGKASAN TRANSAKSI");

    int totalTransactions = 0;
    int paidTransactions = 0;
    int unpaidTransactions = 0;
    int expiredTransactions = 0;
    int cancelledTransactions = 0;
    int usedTransactions = 0;
    int totalSpent = 0;
    int totalTickets = 0;

    for (int i = 0; i < transactionCount; i++) {
        if (transactions[i].userId == currentUserIndex &&
            transactions[i].isActive) {
            totalTransactions++;

            switch (transactions[i].status) {
            case PAID:
                paidTransactions++;
                totalSpent += transactions[i].totalPrice;
                totalTickets += transactions[i].ticketCount;
                break;
            case PENDING_PAYMENT:
                unpaidTransactions++;
                break;
            case EXPIRED:
                expiredTransactions++;
                break;
            case CANCELLED:
                cancelledTransactions++;
                break;
            case USED:
                usedTransactions++;
                totalSpent += transactions[i].totalPrice;
                totalTickets += transactions[i].ticketCount;
                break;
            }
        }
    }

    int successfulTransactions = paidTransactions + usedTransactions;

    cout << "\nNama: " << users[currentUserIndex].nama << endl;
    cout << "\n==================== RINGKASAN AKTIVITAS ==================="
         << endl;
    cout << "Total Transaksi            : " << totalTransactions << " transaksi"
         << endl;
    cout << "- Lunas (Belum Digunakan)  : " << paidTransactions << " transaksi"
         << endl;
    cout << "- Telah Digunakan          : " << usedTransactions << " transaksi"
         << endl;
    cout << "- Menunggu Pembayaran      : " << unpaidTransactions
         << " transaksi" << endl;
    cout << "- Kadaluarsa               : " << expiredTransactions
         << " transaksi" << endl;
    cout << "- Dibatalkan               : " << cancelledTransactions
         << " transaksi" << endl;
    cout << "\n===================== STATISTIK PEMBELIAN =================="
         << endl;
    cout << "Total Tiket Dibeli      : " << totalTickets << " tiket" << endl;
    cout << "Total Pengeluaran       : Rp " << totalSpent << endl;

    if (successfulTransactions > 0) {
        cout << "Rata-rata per Transaksi : Rp "
             << (totalSpent / successfulTransactions) << endl;
    }

    cout << "============================================================"
         << endl;

    pauseScreen();
}
