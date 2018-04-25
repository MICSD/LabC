#ifndef LABC_PROJ_POST_H
#define LABC_PROJ_POST_H

#include "User.h"

#define POST_MAX_TITLE_LENGTH 101

typedef struct {
    char title[POST_MAX_TITLE_LENGTH];
    unsigned long userId;
    unsigned long topicId;
    char title
} Post;

#endif