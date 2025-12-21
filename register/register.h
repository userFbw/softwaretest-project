#ifndef REGISTER_H
#define REGISTER_H

#include "../common.h"  


int register_user(const char* username, const char* password, const char* confirm_password);
void test_register_user();
void init_test_env();

#endif
