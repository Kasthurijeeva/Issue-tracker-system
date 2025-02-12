#ifndef MAIN_H
#define MAIN_H

#include "login.h"
#include "issue.h"
int is_valid_password(const char *password);
int is_valid_email(const char email[]);
int input(char string[]);
#endif