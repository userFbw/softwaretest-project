
#include "register.h"

int register_user(const char* username, const char* password, const char* confirm_password) {
    // 1. 检查输入参数是否为空
    if (username == NULL || password == NULL || confirm_password == NULL) {
        printf("用户名、密码或确认密码不能为空\n");
        return -1;
    }

    // 2. 检查用户名格式
    int username_len = strlen(username);
    if (username_len < 3) {
        printf("输入的用户名太短，至少需要3个字符\n");
        return -1;
    }
    if (username_len > 15) {
        printf("输入的用户名太长，不能超过15个字符\n");
        return -1;
    }
    
    // 检查用户名是否只包含合法字符
    for (int i = 0; i < username_len; i++) {
        char c = username[i];
        if (!(('a' <= c && c <= 'z') || 
              ('A' <= c && c <= 'Z') || 
              ('0' <= c && c <= '9') || 
              c == '_')) {
            printf("输入的用户名包含非法字符 '%c'，只能包含字母、数字和下划线\n", c);
            return -1;
        }
    }

    // 3. 检查密码强度
    int password_len = strlen(password);
    if (password_len < 6) {
        printf("输入的密码太短，至少需要6个字符\n");
        return -1;
    }
    if (password_len > 20) {
        printf("输入的密码太长，不能超过20个字符\n");
        return -1;
    }
    
    bool has_letter = false;
    bool has_digit = false;
    
    for (int i = 0; i < password_len; i++) {
        char c = password[i];
        if (('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z')) {
            has_letter = true;
        }
        if ('0' <= c && c <= '9') {
            has_digit = true;
        }
    }
    
    if (!has_letter) {
        printf("输入的密码必须包含至少一个字母\n");
        return -1;
    }
    if (!has_digit) {
        printf("输入的密码必须包含至少一个数字\n");
        return -1;
    }

    // 4. 检查两次输入的密码是否一致
    if (strcmp(password, confirm_password) != 0) {
        printf("两次输入的密码不一致\n");
        return -1;
    }

    // 5. 检查用户名是否已存在
    if (find_user_by_username(username) != -1) {
        printf("用户名 '%s' 已被使用\n", username);
        return -1;
    }

    // 6. 检查用户数量是否已达上限
    if (user_count >= MAX_USERS) {
        printf("用户数量已达上限 (%d)\n", MAX_USERS);
        return -1;
    }

    // 7. 创建新用户
    User* new_user = &users[user_count];
    
    // 设置用户基本信息
    new_user->user_id = next_user_id++;
    strncpy(new_user->username, username, sizeof(new_user->username) - 1);
    new_user->username[sizeof(new_user->username) - 1] = '\0'; // 确保字符串终止
    strncpy(new_user->password, password, sizeof(new_user->password) - 1);
    new_user->password[sizeof(new_user->password) - 1] = '\0';
    
    // 设置账户初始状态
    new_user->balance = 100.0;              //初始化100元起始余额
    new_user->status = ACCOUNT_ACTIVE;
    new_user->failed_attempts = 0;
    new_user->lock_until = 0;
    
    user_count++;
    
    printf("注册成功！用户 '%s' 已创建，用户ID: %d\n", username, new_user->user_id);
    return new_user->user_id; // 返回新用户的ID
}

void init_test_env() {
    // 重置用户计数器
    user_count = 0;
    // 重置下一个用户ID（从1开始）
    next_user_id = 1;
    // 清空用户数组（避免残留数据影响测试）
    memset(users, 0, sizeof(users));
    // 如果测试涉及交易，也可以重置交易相关变量
    transaction_count = 0;
    next_transaction_id = 1;
    memset(transactions, 0, sizeof(transactions));
}
// 测试函数
void test_register_user() {
    printf("=== 开始注册功能测试 ===\n");
    
    // 保存原始用户数量，用于恢复
    int original_user_count = user_count;
    
    // TC001: 正常注册测试
    printf("\n--- TC001: 正常注册测试 ---\n");
    init_test_env();
    int result = register_user("testuser", "password123", "password123");
    if (result > 0) {
        printf(" TC001 通过: 正常注册成功，用户ID=%d\n", result);
    } else {
        printf(" TC001 失败: 正常注册失败\n");
    }
    
    // TC002: 参数为空测试
    printf("\n--- TC002: 用户名为空测试 ---\n");
    init_test_env();
    result = register_user(NULL, "password123", "password123");
    if (result == -1) {
        printf(" TC002 通过: 参数为空正确处理\n");
    } else {
        printf(" TC002 失败: 参数为空未正确处理\n");
    }

    
    // TC003: 用户名太短测试
    printf("\n--- TC005: 用户名太短测试 ---\n");
    init_test_env();
    result = register_user("ab", "password123", "password123");
    if (result == -1) {
        printf(" TC005 通过: 用户名太短正确处理\n");
    } else {
        printf(" TC005 失败: 用户名太短未正确处理\n");
    }
    
    // TC004: 用户名太长测试
    printf("\n--- TC006: 用户名太长测试 ---\n");
    init_test_env();
    result = register_user("verylongusername123", "password123", "password123");
    if (result == -1) {
        printf(" TC006 通过: 用户名太长正确处理\n");
    } else {
        printf(" TC006 失败: 用户名太长未正确处理\n");
    }
    
    // TC005: 用户名非法字符测试
    printf("\n--- TC007: 用户名非法字符测试 ---\n");
    init_test_env();
    result = register_user("user@123", "password123", "password123");
    if (result == -1) {
        printf(" TC007 通过: 用户名非法字符正确处理\n");
    } else {
        printf(" TC007 失败: 用户名非法字符未正确处理\n");
    }
    
    // TC006: 密码太短测试
    printf("\n--- TC008: 密码太短测试 ---\n");
    init_test_env();
    result = register_user("testuser", "123", "123");
    if (result == -1) {
        printf(" TC008 通过: 密码太短正确处理\n");
    } else {
        printf(" TC008 失败: 密码太短未正确处理\n");
    }
    
    // TC007: 密码太长测试
    printf("\n--- TC009: 密码太长测试 ---\n");
    init_test_env();
    result = register_user("testuser", "verylongpassword123456789", "verylongpassword123456789");
    if (result == -1) {
        printf(" TC009 通过: 密码太长正确处理\n");
    } else {
        printf(" TC009 失败: 密码太长未正确处理\n");
    }
    
    // TC08: 密码无字母测试
    printf("\n--- TC010: 密码无字母测试 ---\n");
    init_test_env();
    result = register_user("testuser", "123456", "123456");
    if (result == -1) {
        printf(" TC010 通过: 密码无字母正确处理\n");
    } else {
        printf(" TC010 失败: 密码无字母未正确处理\n");
    }
    
    // TC09: 密码无数字测试
    printf("\n--- TC011: 密码无数字测试 ---\n");
    init_test_env();
    result = register_user("testuser", "abcdef", "abcdef");
    if (result == -1) {
        printf(" TC011 通过: 密码无数字正确处理\n");
    } else {
        printf(" TC011 失败: 密码无数字未正确处理\n");
    }
    
    // TC010: 密码不一致测试
    printf("\n--- TC012: 密码不一致测试 ---\n");
    init_test_env();
    result = register_user("testuser", "password123", "password124");
    if (result == -1) {
        printf(" TC012 通过: 密码不一致正确处理\n");
    } else {
        printf(" TC012 失败: 密码不一致未正确处理\n");
    }
    
    // TC011: 用户名已存在测试
    printf("\n--- TC013: 用户名已存在测试 ---\n");
    init_test_env();
    // 先注册一个用户
    register_user("existing_user", "password123", "password123");
    // 尝试用相同用户名注册
    result = register_user("existing_user", "newpassword123", "newpassword123");
    if (result == -1) {
        printf(" TC013 通过: 用户名已存在正确处理\n");
    } else {
        printf(" TC013 失败: 用户名已存在未正确处理\n");
    }
    
    // TC012: 用户数量达上限测试
    printf("\n--- TC014: 用户数量达上限测试 ---\n");
    init_test_env();
    // 填充用户直到上限
    for (int i = 0; i < MAX_USERS; i++) {
        char username[20];
        sprintf(username, "user%d", i);
        register_user(username, "password123", "password123");
    }
    // 尝试注册第MAX_USERS+1个用户
    result = register_user("extra_user", "password123", "password123");
    if (result == -1) {
        printf(" TC014 通过: 用户数量达上限正确处理\n");
    } else {
        printf(" TC014 失败: 用户数量达上限未正确处理\n");
    }
    

    
    printf("\n=== 注册功能测试完成 ===\n");
    
    // 恢复原始用户数量
    user_count = original_user_count;
}