#ifndef TRANSFER_H
#define TRANSFER_H

#include "../common.h"

int transfer_money(int from_user_id, int to_user_id, double amount);
void test_transfer_money();
void setup_transfer_test_users();
void test_transfer_money();

#endif
