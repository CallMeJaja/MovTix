#include <iostream>

#include "globals.h"
#include "ui.h"
#include "utils.h"

using namespace std;

//============================================================
// UI MENU FUNCTION IMPLEMENTATIONS
//============================================================

void showAuthMenu() {
    clearScreen();
    showHeader("MOVTIX BIOSKOP");
    cout << "\nSelamat datang di MOVTIX!" << endl;
    cout << "\n1. Login" << endl;
    cout << "2. Daftar Akun Baru" << endl;
    cout << "3. Lupa Password" << endl;
    cout << "4. Keluar" << endl;
    cout << "\nSilakan masukkan pilihan [1-4]: ";
}

void showMainMenu() {
    clearScreen();
    showHeader("MENU UTAMA MOVTIX");
    cout << "\nSelamat datang, " << users[currentUserIndex].nama << "!" << endl;
    cout << "Poin MOVTIX Anda: " << users[currentUserIndex].movtixPoints
         << " poin" << endl;
    cout << "\n1. Daftar Film" << endl;
    cout << "2. Cari Film" << endl;
    cout << "3. Pesan Tiket" << endl;
    cout << "4. Histori Transaksi" << endl;
    cout << "5. Penukaran Tiket & F&B" << endl;
    cout << "6. Logout" << endl;
    cout << "\nMasukkan pilihan [1-6]: ";
}