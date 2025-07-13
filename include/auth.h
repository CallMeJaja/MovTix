#ifndef AUTH_H
#define AUTH_H

void initializeUsers();
bool login();
bool registerUser();
bool resetPassword();
int findUserByUsername(const char *username);
int findUserByEmail(const char *email);

#endif // AUTH_H