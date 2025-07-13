#include "booking.h"
#include "cstdlib"
#include "limits"
#include "ui.h"
#include "utils.h"
#include <cstring>
#include <iomanip>
#include <iostream>

using namespace std;

void initializeSeats() {
    // Inisialisasi kursi untuk auditorium 1 (80 kursi: A-J, 1-8)
    int seatIndex = 0;
    for (char row = 'A'; row <= 'J'; row++) {
        for (int col = 1; col <= 8; col++) {
            sprintf(auditoriumSeats[0][seatIndex].seatNumber, "%c%02d", row,
                    col);
            auditoriumSeats[0][seatIndex].status = AVAILABLE;
            auditoriumSeats[0][seatIndex].userId = -1;
            auditoriumSeats[0][seatIndex].transactionId = -1;
            strcpy(auditoriumSeats[0][seatIndex].reservationTime, "");
            seatIndex++;
        }
    }

    // Inisialisasi kursi untuk auditorium 2 (80 kursi: A-J, 1-8)
    seatIndex = 0;
    for (char row = 'A'; row <= 'J'; row++) {
        for (int col = 1; col <= 8; col++) {
            sprintf(auditoriumSeats[1][seatIndex].seatNumber, "%c%02d", row,
                    col);
            auditoriumSeats[1][seatIndex].status = AVAILABLE;
            auditoriumSeats[1][seatIndex].userId = -1;
            auditoriumSeats[1][seatIndex].transactionId = -1;
            strcpy(auditoriumSeats[1][seatIndex].reservationTime, "");
            seatIndex++;
        }
    }

    // Inisialisasi kursi untuk auditorium 3 (60 kursi)
    seatIndex = 0;
    for (char row = 'A'; row <= 'G'; row++) {
        for (int col = 1; col <= 8; col++) {
            sprintf(auditoriumSeats[2][seatIndex].seatNumber, "%c%02d", row,
                    col);
            auditoriumSeats[2][seatIndex].status = AVAILABLE;
            auditoriumSeats[2][seatIndex].userId = -1;
            auditoriumSeats[2][seatIndex].transactionId = -1;
            strcpy(auditoriumSeats[2][seatIndex].reservationTime, "");
            seatIndex++;
        }
    }
    // Baris H hanya 4 kursi (H01-H04)
    for (int col = 1; col <= 4; col++) {
        sprintf(auditoriumSeats[2][seatIndex].seatNumber, "H%02d", col);
        auditoriumSeats[2][seatIndex].status = AVAILABLE;
        auditoriumSeats[2][seatIndex].userId = -1;
        auditoriumSeats[2][seatIndex].transactionId = -1;
        strcpy(auditoriumSeats[2][seatIndex].reservationTime, "");
        seatIndex++;
    }

    // Simulasi beberapa kursi sudah dibayar (taken)
    auditoriumSeats[0][5].status = TAKEN; // A06
    auditoriumSeats[0][5].userId = 0;
    auditoriumSeats[0][12].status = TAKEN; // B05
    auditoriumSeats[0][12].userId = 1;
    auditoriumSeats[1][20].status = TAKEN; // C05
    auditoriumSeats[1][20].userId = 0;
}

void initializeFnB() {
    // Makanan
    foodBeverages[0] = {1, "Popcorn Regular", 25000, "Food", true};
    foodBeverages[1] = {2, "Popcorn Large", 35000, "Food", true};
    foodBeverages[2] = {3, "Nachos", 30000, "Food", true};
    foodBeverages[3] = {4, "Hot Dog", 20000, "Food", true};
    foodBeverages[4] = {5, "French Fries", 22000, "Food", true};

    // Minuman
    foodBeverages[5] = {6, "Coca Cola Regular", 15000, "Beverage", true};
    foodBeverages[6] = {7, "Coca Cola Large", 20000, "Beverage", true};
    foodBeverages[7] = {8, "Orange Juice", 18000, "Beverage", true};
    foodBeverages[8] = {9, "Mineral Water", 10000, "Beverage", true};
    foodBeverages[9] = {10, "Ice Coffee", 25000, "Beverage", true};

    fnbCount = 10;
}

void checkExpiredReservations() {
    char currentTime[20];
    getCurrentDateTime(currentTime);

    // Cek semua transaksi yang pending
    for (int i = 0; i < transactionCount; i++) {
        if (transactions[i].status == PENDING_PAYMENT &&
            transactions[i].isActive) {
            // Jika sudah kadaluarsa
            if (compareDateTime(currentTime, transactions[i].expiryTime) >= 0) {
                // Update status transaksi menjadi expired
                transactions[i].status = EXPIRED;

                // Bebaskan kursi yang di-reserve
                int auditorium = transactions[i].auditorium;
                char seatList[200];
                strcpy(seatList, transactions[i].selectedSeats);

                char *token = strtok(seatList, ",");
                while (token != NULL) {
                    // Trim whitespace
                    while (*token == ' ')
                        token++;

                    // Cari kursi dan ubah status menjadi available
                    for (int j = 0; j < seatCounts[auditorium - 1]; j++) {
                        if (strcmp(
                                auditoriumSeats[auditorium - 1][j].seatNumber,
                                token) == 0) {
                            auditoriumSeats[auditorium - 1][j].status =
                                AVAILABLE;
                            auditoriumSeats[auditorium - 1][j].userId = -1;
                            auditoriumSeats[auditorium - 1][j].transactionId =
                                -1;
                            strcpy(auditoriumSeats[auditorium - 1][j]
                                       .reservationTime,
                                   "");
                            break;
                        }
                    }
                    token = strtok(NULL, ",");
                }
            }
        }
    }
}

bool parseAndValidateSeats(const char *seatInput, int auditorium,
                           char validSeats[][4], int &validCount) {
    validCount = 0;
    char tempInput[200];
    strcpy(tempInput, seatInput);

    // Split berdasarkan koma
    char *token = strtok(tempInput, ",");
    while (token != NULL && validCount < 10) {
        // Trim whitespace
        while (*token == ' ')
            token++;

        // Konversi ke uppercase
        for (int i = 0; token[i]; i++) {
            token[i] = toupper(token[i]);
        }

        // Validasi format
        if (strlen(token) != 3 || !isalpha(token[0]) || !isdigit(token[1]) ||
            !isdigit(token[2])) {
            cout << "\nFormat kursi tidak valid: " << token
                 << " (gunakan format A01, B05, dll)" << endl;
            return false;
        }

        // Cek apakah kursi exist dan statusnya
        bool seatExists = false;
        SeatStatus seatStatus;

        for (int i = 0; i < seatCounts[auditorium - 1]; i++) {
            if (strcmp(auditoriumSeats[auditorium - 1][i].seatNumber, token) ==
                0) {
                seatExists = true;
                seatStatus = auditoriumSeats[auditorium - 1][i].status;
                break;
            }
        }

        if (!seatExists) {
            cout << "\nKursi " << token << " tidak tersedia di auditorium "
                 << auditorium << endl;
            return false;
        }

        if (seatStatus == TAKEN) {
            cout << "\nKursi " << token << " sudah terisi!" << endl;
            return false;
        }

        if (seatStatus == PENDING) {
            cout << "\nKursi " << token << " sedang dalam proses pembayaran!"
                 << endl;
            return false;
        }

        // Cek duplikasi
        for (int i = 0; i < validCount; i++) {
            if (strcmp(validSeats[i], token) == 0) {
                cout << "\nKursi " << token << " duplikat dalam input!" << endl;
                return false;
            }
        }

        strcpy(validSeats[validCount], token);
        validCount++;

        token = strtok(NULL, ",");
    }

    if (validCount == 0) {
        cout << "\nTidak ada kursi yang valid!" << endl;
        return false;
    }

    return true;
}

void reserveSeats(const char validSeats[][4], int seatCount, int auditorium,
                  int transactionId) {
    char currentTime[20];
    getCurrentDateTime(currentTime);

    for (int i = 0; i < seatCount; i++) {
        for (int j = 0; j < seatCounts[auditorium - 1]; j++) {
            if (strcmp(auditoriumSeats[auditorium - 1][j].seatNumber,
                       validSeats[i]) == 0) {
                auditoriumSeats[auditorium - 1][j].status = PENDING;
                auditoriumSeats[auditorium - 1][j].userId = currentUserIndex;
                auditoriumSeats[auditorium - 1][j].transactionId =
                    transactionId;
                strcpy(auditoriumSeats[auditorium - 1][j].reservationTime,
                       currentTime);
                break;
            }
        }
    }
}

void handleTicketBooking() {
    while (true) {
        clearScreen();
        showHeader("PESAN TIKET FILM");

        char currentDateTime[20];
        getCurrentDateTime(currentDateTime);
        cout << "\nWaktu saat ini: " << currentDateTime << endl;

        cout << "\n| No | Judul                  | Genre     | Rating | Jadwal "
                "Tersedia |"
             << endl;
        cout << "|----|------------------------|-----------|--------|----------"
                "-------|"
             << endl;

        for (int i = 0; i < movieCount; i++) {
            if (movies[i].isActive) {
                // Hitung jadwal yang masih tersedia
                char currentTime[6];
                getCurrentTime(currentTime);
                int availableShows = 0;

                for (int j = 0; j < movies[i].showtimeCount; j++) {
                    if (compareTime(currentTime, movies[i].showtimes[j].time) <
                        0) {
                        availableShows++;
                    }
                }

                printf("| %-2d | %-22s | %-9s | %-6s | %8d show |\n", i + 1,
                       movies[i].title, movies[i].genre, movies[i].rating,
                       availableShows);
            }
        }

        cout << "\n[Pilih nomor film untuk lihat jadwal, 0 untuk kembali]"
             << endl;
        cout << "\nPilihan Anda: ";

        int input = getSingletDigit();
        if (input >= 1 && input <= movieCount) {
            showMovieSchedule(input - 1);
        } else if (input == 0) {
            break; // Kembali ke menu utama
        } else {
            cout << "\nInput tidak valid!" << endl;
            pauseScreen();
        }
    }
}

void processTicketBooking(int movieIndex, int showtimeIndex) {
    Movie movie = movies[movieIndex];
    Showtime showtime = movie.showtimes[showtimeIndex];

    // Tampilkan peta kursi
    displaySeatMap(showtime.auditorium);

    // Input kursi
    char seatInput[200];
    char validSeats[10][4];
    int seatCount = 0;

    cout << "\nMasukkan nomor kursi yang diinginkan: ";
    cin >> seatInput;

    if (!parseAndValidateSeats(seatInput, showtime.auditorium, validSeats,
                               seatCount)) {
        pauseScreen();
        return;
    }

    // Tampilkan kursi yang dipilih
    cout << "\nKursi yang dipilih:" << endl;
    for (int i = 0; i < seatCount; i++) {
        cout << "- " << validSeats[i] << endl;
    }

    int ticketPrice = seatCount * showtime.price;
    cout << "\nJumlah tiket: " << seatCount << endl;
    cout << "Harga per tiket: Rp " << showtime.price << endl;
    cout << "Total harga tiket: Rp " << ticketPrice << endl;

    cout << "\nKonfirmasi pemilihan kursi? (Y/N): ";
    char confirmSeat = getSingleInput();
    if (confirmSeat != 'Y' && confirmSeat != 'y') {
        cout << "\nPemilihan kursi dibatalkan." << endl;
        pauseScreen();
        return;
    }

    // Pilih F&B (opsional)
    SelectedFnB selectedFnB[10];
    int fnbItemCount = 0;
    int fnbTotalPrice = 0;

    cout << "\nIngin menambah F&B? (Y/N): ";
    char wantFnB = getSingleInput();

    if (wantFnB == 'Y' || wantFnB == 'y') {
        fnbItemCount = handleFnBSelection(selectedFnB);
        for (int i = 0; i < fnbItemCount; i++) {
            fnbTotalPrice += selectedFnB[i].totalPrice;
        }
    }

    // Buat transaksi dengan status pending
    int currentTransactionId = transactionCount;

    transactions[currentTransactionId].id = currentTransactionId + 1;
    transactions[currentTransactionId].userId = currentUserIndex;
    transactions[currentTransactionId].movieId = movie.id;
    transactions[currentTransactionId].showtimeIndex = showtimeIndex;
    transactions[currentTransactionId].ticketCount = seatCount;

    // Format selected seats
    strcpy(transactions[currentTransactionId].selectedSeats, "");
    for (int i = 0; i < seatCount; i++) {
        strcat(transactions[currentTransactionId].selectedSeats, validSeats[i]);
        if (i < seatCount - 1)
            strcat(transactions[currentTransactionId].selectedSeats, ",");
    }

    // Simpan F&B
    transactions[currentTransactionId].fnbCount = fnbItemCount;
    for (int i = 0; i < fnbItemCount; i++) {
        transactions[currentTransactionId].selectedFnB[i] = selectedFnB[i];
    }

    transactions[currentTransactionId].ticketPrice = ticketPrice;
    transactions[currentTransactionId].fnbPrice = fnbTotalPrice;
    transactions[currentTransactionId].totalPrice = ticketPrice + fnbTotalPrice;
    transactions[currentTransactionId].auditorium = showtime.auditorium;

    getCurrentDateTime(transactions[currentTransactionId].transactionDate);
    calculateExpiryTime(transactions[currentTransactionId].expiryTime);
    transactions[currentTransactionId].status = PENDING_PAYMENT;
    transactions[currentTransactionId].isActive = true;

    strcpy(transactions[currentTransactionId].movieTitle, movie.title);
    strcpy(transactions[currentTransactionId].showtime, showtime.time);

    // Initialize booking code dan passkey
    strcpy(transactions[currentTransactionId].bookingCode, "");
    strcpy(transactions[currentTransactionId].passkey, "");

    // Reserve kursi dengan status pending
    reserveSeats(validSeats, seatCount, showtime.auditorium,
                 currentTransactionId);

    // Tampilkan ringkasan dan lanjut ke pembayaran
    processPayment(currentTransactionId);
    transactionCount++;
}

int handleFnBSelection(SelectedFnB selectedFnB[]) {
    int selectedCount = 0;

    while (true) {
        clearScreen();
        showHeader("PILIH MAKANAN & MINUMAN");

        cout << "\n| No | Nama                | Harga   | Kategori |" << endl;
        cout << "|----|---------------------|---------|----------|" << endl;

        for (int i = 0; i < fnbCount; i++) {
            if (foodBeverages[i].isAvailable) {
                printf("| %-2d | %-19s | %-7d | %-8s |\n", i + 1,
                       foodBeverages[i].name, foodBeverages[i].price,
                       foodBeverages[i].category);
            }
        }

        cout << "| 0  | Selesai memilih     |         |          |" << endl;

        if (selectedCount > 0) {
            cout << "\nItem yang sudah dipilih:" << endl;
            for (int i = 0; i < selectedCount; i++) {
                printf("- %dx %s = Rp %d\n", selectedFnB[i].quantity,
                       selectedFnB[i].name, selectedFnB[i].totalPrice);
            }
        }

        cout << "\nPilih nomor F&B (0 untuk selesai): ";
        int choice = getNumberInput();

        if (choice == 0) {
            break;
        } else if (choice >= 1 && choice <= fnbCount &&
                   foodBeverages[choice - 1].isAvailable) {
            cout << "\nJumlah yang ingin dipesan: ";
            int quantity;
            cin >> quantity;

            if (quantity > 0 && quantity <= 10) {
                selectedFnB[selectedCount].fnbId = foodBeverages[choice - 1].id;
                strcpy(selectedFnB[selectedCount].name,
                       foodBeverages[choice - 1].name);
                selectedFnB[selectedCount].quantity = quantity;
                selectedFnB[selectedCount].price =
                    foodBeverages[choice - 1].price;
                selectedFnB[selectedCount].totalPrice =
                    quantity * foodBeverages[choice - 1].price;
                selectedCount++;

                cout << "\nItem berhasil ditambahkan!" << endl;
                pauseScreen();
            } else {
                cout << "\nJumlah tidak valid!" << endl;
                pauseScreen();
            }
        } else {
            cout << "\nPilihan tidak valid!" << endl;
            pauseScreen();
        }
    }

    return selectedCount;
}

void processPayment(int transactionId) {
    while (true) {
        char currentTime[20];
        getCurrentDateTime(currentTime);

        if (compareDateTime(currentTime,
                            transactions[transactionId].expiryTime) >= 0) {
            clearScreen();
            showHeader("WAKTU PEMBAYARAN HABIS");
            cout << "\nMaaf, waktu pembayaran telah habis!" << endl;
            cout << "Reservasi kursi Anda telah dibatalkan." << endl;
            cancelReservation(transactionId);
            pauseScreen();
            return;
        }

        clearScreen();
        showHeader("PEMBAYARAN TIKET");

        Transaction trans = transactions[transactionId];

        cout << "\nID Transaksi   : MOVTIX" << setfill('0') << setw(4)
             << trans.id << endl;
        cout << "Film           : " << trans.movieTitle << endl;
        cout << "Jadwal         : " << trans.showtime << endl;
        cout << "Kursi          : " << trans.selectedSeats << endl;
        cout << "Jumlah Tiket   : " << trans.ticketCount << endl;

        if (trans.fnbCount > 0) {
            cout << "\nF&B yang dipesan:" << endl;
            for (int i = 0; i < trans.fnbCount; i++) {
                printf("- %dx %s = Rp %d\n", trans.selectedFnB[i].quantity,
                       trans.selectedFnB[i].name,
                       trans.selectedFnB[i].totalPrice);
            }
        }

        cout << "\nTotal Pembayaran: Rp " << trans.totalPrice << endl;
        cout << "Batas Waktu     : " << trans.expiryTime << endl;
        cout << "Waktu Sekarang  : " << currentTime << endl;

        int timeDiff = compareDateTime(trans.expiryTime, currentTime);
        if (timeDiff > 0) {
            cout << "Status          : Masih dalam batas waktu" << endl;
        } else {
            cout << "Status          : HAMPIR HABIS!" << endl;
        }

        cout << "\nPilihan:" << endl;
        cout << "1. Bayar Sekarang" << endl;
        cout << "2. Kembali ke Menu Utama (Reservasi tetap aktif)" << endl;
        cout << "3. Batalkan Pesanan" << endl;
        cout << "\nPilihan Anda [1-3]: ";

        char choice = getSingleInput();

        switch (choice) {
        case '1': {
            // Menu pilih metode pembayaran
            while (true) {
                clearScreen();
                showHeader("PILIH METODE PEMBAYARAN");

                cout << "\nTotal yang harus dibayar: Rp " << trans.totalPrice
                     << endl;
                cout << "\nPilih Metode Pembayaran:" << endl;
                cout << "1. Transfer Bank" << endl;
                cout << "2. Kartu (Debit/Kredit)" << endl;
                cout << "3. Poin MovTix (Saldo: "
                     << users[currentUserIndex].movtixPoints << " poin)"
                     << endl;
                cout << "0. Kembali ke menu pembayaran" << endl;
                cout << "\n[Pilih 0-3]: ";

                char paymentChoice = getSingleInput();
                PaymentInfo paymentInfo = {};
                bool paymentSuccess = false;

                switch (paymentChoice) {
                case '1':
                    paymentSuccess =
                        processBankPayment(trans.totalPrice, &paymentInfo);
                    break;
                case '2':
                    // Clear input buffer sebelum getline
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    paymentSuccess =
                        processCardPayment(trans.totalPrice, &paymentInfo);
                    break;
                case '3':
                    paymentSuccess =
                        processPointsPayment(trans.totalPrice, &paymentInfo);
                    break;
                case '0':
                    break;
                default:
                    cout << "\nPilihan tidak valid!" << endl;
                    pauseScreen();
                    continue;
                }

                if (paymentChoice == '0')
                    break;

                if (paymentSuccess) {
                    // Generate booking code dan passkey
                    generateBookingCode(
                        transactions[transactionId].bookingCode);
                    generatePasskey(transactions[transactionId].passkey);

                    // Simpan payment info
                    transactions[transactionId].paymentInfo = paymentInfo;

                    // Konfirmasi pembayaran
                    confirmPayment(transactionId);

                    // Berikan poin untuk transaksi yang berhasil (jika tidak
                    // pakai poin)
                    if (paymentInfo.method != MOVTIX_POINTS) {
                        int earnedPoints =
                            trans.totalPrice / 1000; // 1 poin per Rp 1000
                        users[currentUserIndex].movtixPoints += earnedPoints;
                    }

                    // Update popularitas
                    for (int i = 0; i < movieCount; i++) {
                        if (movies[i].id == trans.movieId) {
                            movies[i].popularity += trans.ticketCount;
                            break;
                        }
                    }

                    // Tampilkan konfirmasi dengan kode booking
                    clearScreen();
                    showHeader("PEMBAYARAN BERHASIL");

                    cout << "\nPembayaran berhasil!" << endl;
                    cout << "\nKode Booking : "
                         << transactions[transactionId].bookingCode << endl;
                    cout << "Passkey      : "
                         << transactions[transactionId].passkey << endl;

                    if (paymentInfo.method != MOVTIX_POINTS) {
                        cout << "\nAnda mendapat " << (trans.totalPrice / 1000)
                             << " poin MovTix!" << endl;
                        cout << "Total poin   : "
                             << users[currentUserIndex].movtixPoints << " poin"
                             << endl;
                    }

                    cout << "\nSimpan kode booking & passkey untuk masuk "
                            "studio dan klaim F&B."
                         << endl;
                    cout << "\n[Tekan ENTER untuk kembali ke menu utama.]"
                         << endl;
                    pauseScreen();

                    return;
                }
            }
            break;
        }
        case '2':
            cout << "\nReservasi kursi masih aktif sampai " << trans.expiryTime
                 << endl;
            cout << "Anda dapat melanjutkan pembayaran melalui menu Histori "
                    "Transaksi."
                 << endl;
            pauseScreen();
            return;

        case '3': {
            cout << "\nApakah Anda yakin ingin membatalkan pesanan? (Y/N): ";
            char confirm = getSingleInput();
            if (confirm == 'Y' || confirm == 'y') {
                cancelReservation(transactionId);
                cout << "\nPesanan dibatalkan. Kursi telah dibebaskan." << endl;
                pauseScreen();
                return;
            }
            break;
        }
        default:
            cout << "\nPilihan tidak valid!" << endl;
            pauseScreen();
            break;
        }
    }
}

bool processCardPayment(int totalAmount, PaymentInfo *paymentInfo) {
    clearScreen();
    showHeader("PEMBAYARAN KARTU");

    cout << "\nMetode pembayaran: Kartu (Debit/Kredit)" << endl;
    cout << "\nTotal belanja        : Rp " << totalAmount << endl;
    cout << "Biaya admin          : Rp 5.000" << endl;
    cout << "-------------------------------" << endl;
    cout << "Total yang harus dibayar: Rp " << (totalAmount + 5000) << endl;

    char cardNumber[50];
    char cardName[50];
    char expiry[10];
    char cvv[5];

    cout << "\nMasukkan nomor kartu (16 digit): ";
    cin.getline(cardNumber, 50);

    cout << "Masukkan nama di kartu: ";
    cin.getline(cardName, 50);

    cout << "Masa berlaku (MM/YY): ";
    cin.getline(expiry, 10);

    cout << "Kode CVV (3 digit): ";
    cin.getline(cvv, 5);

    if (!validateCard(cardNumber, expiry, cvv)) {
        pauseScreen();
        return false;
    }

    const char *cardType = detectCardType(cardNumber);
    cout << "\nJenis kartu terdeteksi: " << cardType << endl;

    if (strcmp(cardType, "Unknown") == 0) {
        cout << "\nJenis kartu tidak didukung! Hanya menerima Visa dan "
                "Mastercard."
             << endl;
        pauseScreen();
        return false;
    }

    cout << "\n[Konfirmasi pembayaran? 1. Ya  0. Batal]: ";
    char confirm = getSingleInput();

    if (confirm != '1') {
        cout << "\nPembayaran dibatalkan." << endl;
        pauseScreen();
        return false;
    }

    // Simpan info pembayaran
    paymentInfo->method = CARD_PAYMENT;
    strcpy(paymentInfo->cardNumber, cardNumber);
    strcpy(paymentInfo->cardName, cardName);
    strcpy(paymentInfo->expiry, expiry);
    strcpy(paymentInfo->cvv, cvv);

    cout << "\nPembayaran sedang diproses..." << endl;
    // Simulasi delay
    for (int i = 0; i < 3; i++) {
        cout << ".";
        // Simple delay simulation
        for (int j = 0; j < 100000000; j++)
            ;
    }
    cout << endl;

    cout << "Pembayaran berhasil!" << endl;
    pauseScreen();
    return true;
}

bool processBankPayment(int totalAmount, PaymentInfo *paymentInfo) {
    clearScreen();
    showHeader("TRANSFER BANK");

    cout << "\nPilih bank tujuan:" << endl;
    cout << "1. BCA" << endl;
    cout << "2. Mandiri" << endl;
    cout << "3. BNI" << endl;
    cout << "4. BRI" << endl;
    cout << "0. Batal" << endl;
    cout << "\nPilihan [0-4]: ";

    char choice = getSingleInput();

    const char *bankNames[] = {"", "BCA", "Mandiri", "BNI", "BRI"};
    const char *accountNumbers[] = {"", "1234567890", "9876543210",
                                    "1357924680", "2468013579"};

    if (choice < '1' || choice > '4') {
        if (choice == '0') {
            cout << "\nTransfer dibatalkan." << endl;
        } else {
            cout << "\nPilihan tidak valid!" << endl;
        }
        pauseScreen();
        return false;
    }

    int bankIndex = choice - '0';

    cout << "\n=== INFORMASI TRANSFER ===" << endl;
    cout << "Bank Tujuan    : " << bankNames[bankIndex] << endl;
    cout << "No. Rekening   : " << accountNumbers[bankIndex] << endl;
    cout << "Atas Nama     : MOVTIX INDONESIA" << endl;
    cout << "Jumlah Transfer: Rp " << totalAmount << endl;
    cout << "=========================" << endl;

    cout << "\nSilakan lakukan transfer dan tekan 1 jika sudah selesai, 0 "
            "untuk batal: ";
    char confirm = getSingleInput();

    if (confirm != '1') {
        cout << "\nTransfer dibatalkan atau belum selesai." << endl;
        pauseScreen();
        return false;
    }

    // Simpan info pembayaran
    paymentInfo->method = BANK_TRANSFER;
    strcpy(paymentInfo->bankName, bankNames[bankIndex]);

    cout << "\nTransfer berhasil dikonfirmasi!" << endl;
    pauseScreen();
    return true;
}

bool processPointsPayment(int totalAmount, PaymentInfo *paymentInfo) {
    clearScreen();
    showHeader("PEMBAYARAN POIN MOVTIX");

    int userPoints = users[currentUserIndex].movtixPoints;
    int requiredPoints = totalAmount / 100; // 1 poin = Rp 100

    cout << "\nTotal pembayaran: Rp " << totalAmount << endl;
    cout << "Poin dibutuhkan : " << requiredPoints << " poin" << endl;
    cout << "Poin Anda       : " << userPoints << " poin" << endl;

    if (userPoints < requiredPoints) {
        cout << "\nMaaf, poin Anda tidak mencukupi!" << endl;
        cout << "Kekurangan      : " << (requiredPoints - userPoints) << " poin"
             << endl;
        pauseScreen();
        return false;
    }

    cout << "\nSisa poin setelah pembayaran: " << (userPoints - requiredPoints)
         << " poin" << endl;
    cout << "\n[Konfirmasi pembayaran dengan poin? 1. Ya  0. Batal]: ";

    char confirm = getSingleInput();

    if (confirm != '1') {
        cout << "\nPembayaran dibatalkan." << endl;
        pauseScreen();
        return false;
    }

    // Kurangi poin user
    users[currentUserIndex].movtixPoints -= requiredPoints;

    // Simpan info pembayaran
    paymentInfo->method = MOVTIX_POINTS;
    paymentInfo->pointsUsed = requiredPoints;

    cout << "\nPembayaran dengan poin berhasil!" << endl;
    cout << "Poin tersisa: " << users[currentUserIndex].movtixPoints << " poin"
         << endl;
    pauseScreen();
    return true;
}

void confirmPayment(int transactionId) {
    // Update status transaksi
    transactions[transactionId].status = PAID;

    // Update status kursi menjadi taken
    int auditorium = transactions[transactionId].auditorium;
    char seatList[200];
    strcpy(seatList, transactions[transactionId].selectedSeats);

    char *token = strtok(seatList, ",");
    while (token != NULL) {
        while (*token == ' ')
            token++;

        for (int j = 0; j < seatCounts[auditorium - 1]; j++) {
            if (strcmp(auditoriumSeats[auditorium - 1][j].seatNumber, token) ==
                0) {
                auditoriumSeats[auditorium - 1][j].status = TAKEN;
                break;
            }
        }
        token = strtok(NULL, ",");
    }
}

void cancelReservation(int transactionId) {
    // Update status transaksi
    transactions[transactionId].status = CANCELLED;

    // Bebaskan kursi
    int auditorium = transactions[transactionId].auditorium;
    char seatList[200];
    strcpy(seatList, transactions[transactionId].selectedSeats);

    char *token = strtok(seatList, ",");
    while (token != NULL) {
        while (*token == ' ')
            token++;

        for (int j = 0; j < seatCounts[auditorium - 1]; j++) {
            if (strcmp(auditoriumSeats[auditorium - 1][j].seatNumber, token) ==
                0) {
                auditoriumSeats[auditorium - 1][j].status = AVAILABLE;
                auditoriumSeats[auditorium - 1][j].userId = -1;
                auditoriumSeats[auditorium - 1][j].transactionId = -1;
                strcpy(auditoriumSeats[auditorium - 1][j].reservationTime, "");
                break;
            }
        }
        token = strtok(NULL, ",");
    }
}

void generateBookingCode(char *bookingCode) {
    const char chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    for (int i = 0; i < 8; i++) {
        bookingCode[i] = chars[rand() % 36];
    }
    bookingCode[8] = '\0';
}

void generatePasskey(char *passkey) {
    for (int i = 0; i < 6; i++) {
        passkey[i] = '0' + (rand() % 10);
    }
    passkey[6] = '\0';
}

int findTransactionByCode(const char *bookingCode, const char *passkey) {
    for (int i = 0; i < transactionCount; i++) {
        if (transactions[i].status == PAID &&
            strcmp(transactions[i].bookingCode, bookingCode) == 0 &&
            strcmp(transactions[i].passkey, passkey) == 0) {
            return i;
        }
    }
    return -1; // Tidak ditemukan
}

void handleTicketExchange() {
    char bookingCode[15];
    char passkey[15];

    while (true) {
        clearScreen();
        showHeader("PENUKARAN TIKET & F&B");

        // Input kode booking
        cout << "\nMasukkan kode booking (8 karakter): ";
        cin >> bookingCode;

        // Validasi format kode booking
        if (!validateBookingCode(bookingCode)) {
            cout << "\nFormat kode booking tidak valid!" << endl;
            cout << "Kode booking harus 8 karakter (huruf kapital dan angka)"
                 << endl;
            cout << "Contoh: ZXH0DA1Y" << endl;
            pauseScreen();
            continue;
        }

        // Input passkey
        cout << "Masukkan passkey (6 digit): ";
        cin >> passkey;

        // Validasi format passkey
        if (!validatePasskey(passkey)) {
            cout << "\nFormat passkey tidak valid!" << endl;
            cout << "Passkey harus 6 digit angka" << endl;
            cout << "Contoh: 123456" << endl;
            pauseScreen();
            continue;
        }

        // Cari transaksi berdasarkan kode
        int transactionIndex = findTransactionByCode(bookingCode, passkey);

        if (transactionIndex != -1) {
            // Jika valid
            transactions[transactionIndex].status = USED;

            cout << "\n[Penukaran tiket & F&B berhasil! Selamat menonton.]"
                 << endl;
            pauseScreen();
            return;

        } else {
            // Jika tidak valid
            cout << "\n[Kode booking atau passkey salah / sudah digunakan.]"
                 << endl;

            cout << "\nCoba lagi? (Y/N): ";
            char choice = getSingleInput();

            if (choice != 'Y' && choice != 'y') {
                return;
            }
        }
    }
}
