#ifndef LOGIN_H
#define LOGIN_H

#include "../common.h"

int login_user(const char* username, const char* password);
void test_login_user();

void setup_equivalence_test_users();
void test_equivalence_cases();

void setup_decision_table_users();
void test_decision_table_cases();

void setup_path_test_users();
void test_path_cases();

#endif
