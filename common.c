#include "common.h"
#include <ctype.h>
#include <time.h>

// ======================
// 辅助函数实现
// ======================

/**
 * 打印用户详细信息
 * @param user 指向User结构的指针
 */
void print_user(const User* user) {
    if (user == NULL) {
        printf("错误：无效的用户指针\n");
        return;
    }
    printf("=============用户基本信息============\n");
    printf("用户ID: %d\n", user->user_id);
    printf("用户名: %s\n", user->username);
    
    // 账户状态描述
    const char* status_str;
    switch (user->status) {
        case ACCOUNT_ACTIVE: status_str = "正常"; break;
        case ACCOUNT_FROZEN: status_str = "冻结"; break;
        case ACCOUNT_LOCKED: status_str = "锁定"; break;
        default: status_str = "未知状态";
    }
    printf("账户状态: %s\n", status_str);
    
    printf("账户余额: %.2f\n", user->balance);
    
    // 如果账户被锁定，显示解锁时间
    if (user->status == ACCOUNT_LOCKED) {
        char time_str[20];
        strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", 
                 localtime(&user->lock_until));
        printf("锁定直到: %s\n", time_str);
    }
    
    printf("登录失败次数: %d\n", user->failed_attempts);
    printf("--------------------------------\n");
}

/**
 * 打印交易详细信息
 * @param trans 指向Transaction结构的指针
 */
void print_transaction(const Transaction* trans) {
    if (trans == NULL) {
        printf("错误：无效的交易指针\n");
        return;
    }
    
    printf("交易ID: %d\n", trans->transaction_id);
    
    // 交易类型描述
    const char* type_str;
    switch (trans->type) {
        case TRANSFER: type_str = "转账"; break;
        default: type_str = "未知类型";
    }
    printf("交易类型: %s\n", type_str);
    
    printf("发起方ID: %d\n", trans->from_user_id);
    printf("接收方ID: %d\n", trans->to_user_id);
    printf("交易金额: %.2f\n", trans->amount);
    
    // 格式化时间
    char time_str[20];
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", 
             localtime(&trans->transaction_time));
    printf("交易时间: %s\n", time_str);
    printf("--------------------------------\n");
}

/**
 * 通过用户名查找用户
 * @param username 要查找的用户名
 * @return 用户ID（找到）或 -1（未找到）
 */
int find_user_by_username(const char* username) {
    if (username == NULL || *username == '\0') {
        return -1;
    }
    
    for (int i = 0; i < user_count; i++) {
        if (strcmp(users[i].username, username) == 0) {
            return users[i].user_id;
        }
    }
    
    return -1;
}

/**
 * 通过用户ID查找用户
 * @param user_id 要查找的用户ID
 * @return 用户数组索引（找到）或 -1（未找到）
 */
int find_user_by_id(int user_id) {
    for (int i = 0; i < user_count; i++) {
        if (users[i].user_id == user_id) {
            return i;
        }
    }
    return -1;
}

/**
 * 验证用户名格式
 * @param username 要验证的用户名
 * @return true-有效，false-无效
 */
bool is_valid_username(const char* username) {
    if (username == NULL) {
        return false;
    }
    
    size_t len = strlen(username);
    
    // 检查长度
    if (len < 3 || len > 15) {
        return false;
    }
    
    // 检查字符是否合法
    for (size_t i = 0; i < len; i++) {
        char c = username[i];
        if (!(('a' <= c && c <= 'z') || 
              ('A' <= c && c <= 'Z') || 
              ('0' <= c && c <= '9') || 
              c == '_')) {
            return false;
        }
    }
    
    return true;
}

/**
 * 验证密码强度
 * @param password 要验证的密码
 * @return true-有效，false-无效
 */
bool is_valid_password(const char* password) {
    if (password == NULL) {
        return false;
    }
    
    size_t len = strlen(password);
    
    // 检查长度
    if (len < 6 || len > 20) {
        return false;
    }
    
    bool has_letter = false;
    bool has_digit = false;
    
    // 检查字符组成
    for (size_t i = 0; i < len; i++) {
        char c = password[i];
        if (('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z')) {
            has_letter = true;
        } else if ('0' <= c && c <= '9') {
            has_digit = true;
        }
    }
    
    return has_letter && has_digit;
}