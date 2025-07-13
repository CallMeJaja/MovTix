#include "auth.h"
#include "types.h"
#include "ui.h"
#include "utils.h"
#include <cstring>
#include <iostream>

using namespace std;

// Fungsi untuk inisialisasi user
void initializeUsers() {
    strcpy(users[0].nama, "Admin MOVTIX");
    strcpy(users[0].username, "admin");
    strcpy(users[0].email, "admin@movtix.com");
    strcpy(users[0].password, "admin123");
    users[0].movtixPoints = 10000;
    users[0].isActive = true;

    strcpy(users[1].nama, "Reza Asriano Maulana");
    strcpy(users[1].username, "reza");
    strcpy(users[1].email, "reza@movtix.com");
    strcpy(users[1].password, "reza");
    users[1].movtixPoints = 5000;
    users[1].isActive = true;

    userCount = 2;
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

    if (findUserByUsername(username) != -1) {
        cout << "\n[Username sudah digunakan. Silakan pilih username lain.]"
             << endl;
        pauseScreen();
        return false;
    }

    cout << "Email             : ";
    cin >> email;

    if (!isValidEmail(email)) {
        cout << "\n[Format email tidak valid.]" << endl;
        pauseScreen();
        return false;
    }

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

    if (strcmp(password, confirmPassword) != 0) {
        cout << "\n[Password tidak cocok. Silakan coba lagi.]" << endl;
        pauseScreen();
        return false;
    }

    strcpy(users[userCount].nama, nama);
    strcpy(users[userCount].username, username);
    strcpy(users[userCount].email, email);
    strcpy(users[userCount].password, password);
    users[userCount].isActive = true;
    users[userCount].movtixPoints = 0; // Poin awal
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

    strcpy(users[userIndex].password, newPassword);

    cout << "\n[Password berhasil diubah. Silakan login.]" << endl;
    pauseScreen();
    return true;
}