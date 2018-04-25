#include "User.h"
#include <string.h>

bool checkPassword(const User* u, const char* attempted_password) {
    return u && attempted_password && u->password && !strcmp(u->password,attempted_password);
}

bool changePassword(User* u, const char* new_password) {
    if(!u || !new_password || strlen(new_password)>=PASSWORD_MAX_LENGTH)
        return false;
    bzero(u->password,PASSWORD_MAX_LENGTH);
    strcpy(u->password,new_password);
    return true;
}