#ifndef LABC_PROJ_USER_H
#define LABC_PROJ_USER_H

#include "../Common.h"

//these already include the space for the '\0' character
#define USERNAME_MAX_LENGTH 101
#define PASSWORD_MAX_LENGTH 201

typedef struct {
    unsigned long id;
    char name[USERNAME_MAX_LENGTH];
    char password[PASSWORD_MAX_LENGTH];
} User;

bool checkPassword(const User* u, const char* attempted_password);

/**
 * Changes the password of a User
 * @param u The user whose password should be changed
 * @param new_password The new password
 * @return Whether or not the password was successfully altered
 */
bool changePassword(User* u, const char* new_password);

/**
 * Gets the users from the database
 * @param n The address of the variable in which me
 * @return an array of users
 */
User* getUsers(int* n);
#endif