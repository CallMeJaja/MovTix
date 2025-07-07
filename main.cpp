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

// Array untuk menyimpan data pengguna (maksimal 100 pengguna)
User users[100];
int userCount = 0;
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
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void pauseScreen() {
    cout << "\nTekan Enter untuk melanjutkan...";
    clearInputBuffer();
    cin.get();
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
    cin.ignore();
    cin.getline(nama, 100);

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

    // Kode verifikasi sederhana (dalam implementasi nyata, ini harus random)
    char correctCode[] = "123456";
    char inputCode[10];

    cout << "Masukkan kode verifikasi    : ";
    cin >> inputCode;

    if (strcmp(inputCode, correctCode) != 0) {
        cout << "\n[Kode verifikasi salah.]" << endl;
        pauseScreen();
        return false;
    }

    char newPassword[50];
    char confirmPassword[50];

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

// Fungsi utama program
int main() {
    int choice;
    bool isLoggedIn = false;

    // Inisialisasi data pengguna demo
    strcpy(users[0].nama, "Admin MOVTIX");
    strcpy(users[0].username, "admin");
    strcpy(users[0].email, "admin@movtix.com");
    strcpy(users[0].password, "admin123");
    users[0].isActive = true;
    userCount = 1;

    while (true) {
        if (!isLoggedIn) {
            showAuthMenu();
            cin >> choice;

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
            cin >> choice;

            switch (choice) {
            case 1:
                cout << "\n[Fitur Daftar Film belum diimplementasikan]" << endl;
                pauseScreen();
                break;
            case 2:
                cout << "\n[Fitur Cari Film belum diimplementasikan]" << endl;
                pauseScreen();
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