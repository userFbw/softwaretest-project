#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

// ======================
// 结构体定义
// ======================

typedef enum {
    ACCOUNT_ACTIVE,
    ACCOUNT_FROZEN,
    ACCOUNT_LOCKED
} AccountStatus;

typedef struct {
    int user_id;
    char username[16];
    char password[21];
    double balance;
    AccountStatus status;
    int failed_attempts;
    time_t lock_until;
} User;

typedef enum {
    TRANSFER
} TransactionType;

typedef struct {
    int transaction_id;
    int from_user_id;
    int to_user_id;
    double amount;
    TransactionType type;
    time_t transaction_time;
} Transaction;

// ======================
// 全局变量声明
// ======================
#define MAX_USERS 5
#define MAX_TRANSACTIONS 1000

extern User users[MAX_USERS];
extern Transaction transactions[MAX_TRANSACTIONS];
extern int user_count;
extern int transaction_count;
extern int next_user_id;
extern int next_transaction_id;

// ======================
// 辅助函数实现
// ======================
void print_user(const User* user);
void print_transaction(const Transaction* trans);
int find_user_by_username(const char* username);
int find_user_by_id(int user_id);
bool is_valid_username(const char* username);
bool is_valid_password(const char* password);

#endif

