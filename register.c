#include "register.h"
int register_user(const char* username, const char* password, const char* confirm_password) {
    if (username == NULL || password == NULL || confirm_password == NULL) {
        printf("用户名、密码或确认密码不能为空\n");
        return -1;
    }
    int username_len = strlen(username);
    if (username_len < 3) {
        printf("输入的用户名太短，至少需要3个字符\n");
        return -1;
    }
    if (username_len > 15) {
        printf("输入的用户名太长，不能超过15个字符\n");
        return -1;
    }
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
    if (strcmp(password, confirm_password) != 0) {
        printf("两次输入的密码不一致\n");
        return -1;
    }
    if (find_user_by_username(username) != -1) {
        printf("用户名 '%s' 已被使用\n", username);
        return -1;
    }
    if (user_count >= MAX_USERS) {
        printf("用户数量已达上限 (%d)\n", MAX_USERS);
        return -1;
    }
    User* new_user = &users[user_count];
    new_user->user_id = next_user_id++;
    strncpy(new_user->username, username, sizeof(new_user->username) - 1);
    new_user->username[sizeof(new_user->username) - 1] = '\0'; 
    strncpy(new_user->password, password, sizeof(new_user->password) - 1);
    new_user->password[sizeof(new_user->password) - 1] = '\0';
    new_user->balance = 100.0;            
    new_user->status = ACCOUNT_ACTIVE;
    new_user->failed_attempts = 0;
    new_user->lock_until = 0;
    user_count++;
    printf("注册成功！用户 '%s' 已创建，用户ID: %d\n", username, new_user->user_id);
    return new_user->user_id; 
}
void init_test_env() {
    user_count = 0;
    next_user_id = 1;
    memset(users, 0, sizeof(users));
    transaction_count = 0;
    next_transaction_id = 1;
    memset(transactions, 0, sizeof(transactions));
}
// 测试函数
void test_register_user() {
    // ==================== 全定义测试（方案1） ====================
    printf("===================== 启动 register_user 全定义测试 =====================\n");
    init_test_env(); // 初始化测试环境：重置user_count、next_user_id等全局变量，确保测试独立性
    int ret;         // 存储register_user函数返回值，用于断言验证

    // 测试用例1：合法输入（注册成功）- 覆盖多变量正常流程的定义-使用对
    ret = register_user("test_123", "Test123456", "Test123456");
    assert(ret == 1); // 预期返回首个用户ID=1（next_user_id初始为1）
    printf("测试用例1（合法注册）执行通过 | 输入：username=\"test_123\", password=\"Test123456\", confirm_password=\"Test123456\" | 预期返回1，实际返回%d\n", ret);

    // 测试用例2：用户名长度<3 - 覆盖 TCOVER4（username_len定义→7行P-use）
    ret = register_user("us", "Test123456", "Test123456");
    assert(ret == -1); // 用户名仅2字符，触发"太短"提示，预期返回-1
    printf("测试用例2（用户名太短）执行通过 | 输入：username=\"us\", password=\"Test123456\", confirm_password=\"Test123456\" | 预期返回-1，实际返回%d\n", ret);

    // 测试用例3：用户名含非法字符 - 覆盖 TCOVER5（i1定义→16行C-use）、TCOVER6（c1定义→17行P-use）
    ret = register_user("test@user", "Test123456", "Test123456");
    assert(ret == -1); // 含非法字符'@'，触发"非法字符"提示，预期返回-1
    printf("测试用例3（用户名含非法字符）执行通过 | 输入：username=\"test@user\", password=\"Test123456\", confirm_password=\"Test123456\" | 预期返回-1，实际返回%d\n", ret);

    // 测试用例4：密码长度<6 - 覆盖 TCOVER7（password_len定义→26行P-use）
    ret = register_user("valid_user", "12345", "12345");
    assert(ret == -1); // 密码仅5字符，触发"太短"提示，预期返回-1
    printf("测试用例4（密码太短）执行通过 | 输入：username=\"valid_user\", password=\"12345\", confirm_password=\"12345\" | 预期返回-1，实际返回%d\n", ret);

    // 测试用例5：密码无字母 - 覆盖 TCOVER8（has_letter定义→35行P-use）
    ret = register_user("valid_user", "12345678", "12345678");
    assert(ret == -1); // 密码仅数字，无字母，触发"需含字母"提示，预期返回-1
    printf("测试用例5（密码无字母）执行通过 | 输入：username=\"valid_user\", password=\"12345678\", confirm_password=\"12345678\" | 预期返回-1，实际返回%d\n", ret);

    // 测试用例6：密码无数字 - 覆盖 TCOVER9（has_digit定义→38行P-use）
    ret = register_user("valid_user", "Abcdefgh", "Abcdefgh");
    assert(ret == -1); // 密码仅字母，无数字，触发"需含数字"提示，预期返回-1
    printf("测试用例6（密码无数字）执行通过 | 输入：username=\"valid_user\", password=\"Abcdefgh\", confirm_password=\"Abcdefgh\" | 预期返回-1，实际返回%d\n", ret);

    // 测试用例7：用户数量达上限 - 覆盖 TCOVER13（user_count定义→46行P-use）（假设MAX_USERS=2）
    init_test_env(); // 重置环境，重新计数
    // 先注册2个合法用户（达到MAX_USERS上限）
    register_user("user1", "Test123456", "Test123456"); 
    register_user("user2", "Test123456", "Test123456"); 
    // 注册第3个用户，触发数量上限
    ret = register_user("user3", "Test123456", "Test123456");
    assert(ret == -1); // 预期返回-1
    printf("测试用例7（用户数量达上限）执行通过 | 输入：username=\"user3\", password=\"Test123456\", confirm_password=\"Test123456\" | 预期返回-1，实际返回%d\n", ret);

    printf("===================== 全定义测试完成：所有变量定义均覆盖至少一条使用路径 =====================\n\n");


    // ==================== 全计算使用测试（方案2） ====================
    printf("=== 开始全计算使用测试 ===\n\n");
    
    // 测试用例1: 用户名太短
    printf("测试用例1: 用户名太短\n");
    init_test_env();
    int result1 = register_user("ab", "123", "123");
    printf("结果: %d (预期: -1)\n", result1);
    assert(result1 == -1);
    printf("覆盖: TCOVER1(username:1→6), TCOVER6(password:1→25), TCOVER10(confirm_password:1→53), TCOVER11(username_len:6→7)\n\n");
    
    // 测试用例2: 用户名太长
    printf("测试用例2: 用户名太长\n");
    init_test_env();
    char long_username[20];
    memset(long_username, 'a', 19);
    long_username[19] = '\0';
    int result2 = register_user(long_username, "123", "123");
    printf("结果: %d (预期: -1)\n", result2);
    assert(result2 == -1);
    printf("覆盖: TCOVER1(username:1→6), TCOVER6(password:1→25), TCOVER10(confirm_password:1→53), TCOVER13(username_len:6→15)\n\n");
    
    // 测试用例3: 密码太短
    printf("测试用例3: 密码太短\n");
    init_test_env();
    int result3 = register_user("abc", "123", "123");
    printf("结果: %d (预期: -1)\n", result3);
    assert(result3 == -1);
    printf("覆盖: TCOVER1(username:1→6), TCOVER2(username:1→16), TCOVER6(password:1→25), TCOVER10(confirm_password:1→53), ");
    printf("TCOVER11(username_len:6→7), TCOVER12(username_len:6→11), TCOVER13(username_len:6→15), TCOVER16(password_len:25→26)\n\n");
    
    // 测试用例4: 密码太长
    printf("测试用例4: 密码太长\n");
    init_test_env();
    char long_password[30];
    memset(long_password, 'a', 25);
    long_password[25] = '\0';
    int result4 = register_user("abc", long_password, long_password);
    printf("结果: %d (预期: -1)\n", result4);
    assert(result4 == -1);
    printf("覆盖: TCOVER1(username:1→6), TCOVER2(username:1→16), TCOVER6(password:1→25), TCOVER7(password:1→37), TCOVER10(confirm_password:1→53), ");
    printf("TCOVER11(username_len:6→7), TCOVER12(username_len:6→11), TCOVER13(username_len:6→15), TCOVER17(password_len:25→30)\n\n");
    
    // 测试用例5: 用户名包含非法字符
    printf("测试用例5: 用户名包含非法字符\n");
    init_test_env();
    int result5 = register_user("abc!", "pass123", "pass123");
    printf("结果: %d (预期: -1)\n", result5);
    assert(result5 == -1);
    printf("覆盖: TCOVER1(username:1→6), TCOVER2(username:1→16), TCOVER3(username:1→57), ");
    printf("TCOVER6(password:1→25), TCOVER7(password:1→37), TCOVER10(confirm_password:1→53), ");
    printf("TCOVER14(i1:15→16), TCOVER15(c1:16→21)\n\n");
    
    // 测试用例6: 密码无字母
    printf("测试用例6: 密码无字母\n");
    init_test_env();
    int result6 = register_user("abc", "123456", "123456");
    printf("结果: %d (预期: -1)\n", result6);
    assert(result6 == -1);
    printf("覆盖: TCOVER1(username:1→6), TCOVER2(username:1→16), TCOVER3(username:1→57), ");
    printf("TCOVER6(password:1→25), TCOVER7(password:1→37), TCOVER10(confirm_password:1→53), ");
    printf("TCOVER16(password_len:25→26), TCOVER18(password_len:25→36), TCOVER19(has_letter:34→45), ");
    printf("TCOVER21(has_digit:35→49), TCOVER23(i2:36→37), TCOVER24(c2:37→38), TCOVER25(c2:37→41)\n\n");
    
    // 测试用例7: 密码无数字
    printf("测试用例7: 密码无数字\n");
    init_test_env();
    int result7 = register_user("abc", "abcdef", "abcdef");
    printf("结果: %d (预期: -1)\n", result7);
    assert(result7 == -1);
    printf("覆盖: TCOVER1(username:1→6), TCOVER2(username:1→16), TCOVER3(username:1→57), ");
    printf("TCOVER6(password:1→25), TCOVER7(password:1→37), TCOVER10(confirm_password:1→53), ");
    printf("TCOVER16(password_len:25→26), TCOVER18(password_len:25→36), TCOVER20(has_letter:39→45), ");
    printf("TCOVER22(has_digit:42→49), TCOVER23(i2:36→37), TCOVER24(c2:37→38), TCOVER25(c2:37→41)\n\n");
    
    // 测试用例8: 密码不一致
    printf("测试用例8: 密码不一致\n");
    init_test_env();
    int result8 = register_user("abc", "pass123", "pass456");
    printf("结果: %d (预期: -1)\n", result8);
    assert(result8 == -1);
    printf("覆盖: TCOVER1(username:1→6), TCOVER2(username:1→16), TCOVER3(username:1→57), TCOVER4(username:1→67), ");
    printf("TCOVER6(password:1→25), TCOVER7(password:1→37), TCOVER8(password:1→53), TCOVER10(confirm_password:1→53), ");
    printf("TCOVER16(password_len:25→26), TCOVER18(password_len:25→36), TCOVER19(has_letter:34→45), TCOVER20(has_letter:39→45), ");
    printf("TCOVER21(has_digit:35→49), TCOVER22(has_digit:42→49), TCOVER23(i2:36→37), TCOVER24(c2:37→38), TCOVER25(c2:37→41)\n\n");
    
    // 测试用例9: 用户名已存在
    printf("测试用例9: 用户名已存在\n");
    init_test_env();
    // 先注册一个用户
    register_user("existing_user", "pass123", "pass123");
    // 尝试注册同名用户
    int result9 = register_user("existing_user", "pass123", "pass123");
    printf("结果: %d (预期: -1)\n", result9);
    assert(result9 == -1);
    printf("覆盖: TCOVER1(username:1→6), TCOVER2(username:1→16), TCOVER3(username:1→57), TCOVER4(username:1→67), ");
    printf("TCOVER5(username:1→76), TCOVER6(password:1→25), TCOVER7(password:1→37), TCOVER8(password:1→53), ");
    printf("TCOVER9(password:1→69), TCOVER10(confirm_password:1→53), ");
    printf("TCOVER16(password_len:25→26), TCOVER18(password_len:25→36), TCOVER19(has_letter:34→45), TCOVER20(has_letter:39→45), ");
    printf("TCOVER21(has_digit:35→49), TCOVER22(has_digit:42→49), TCOVER23(i2:36→37), TCOVER24(c2:37→38), TCOVER25(c2:37→41)\n\n");
    
    // 测试用例10: 综合测试 - 包括用户上限和成功注册
    printf("测试用例10: 综合测试 - 覆盖剩余计算使用节点\n");
    
    // 10.1 测试用户数量已达上限
    printf("\n子测试10.1: 用户数量已达上限\n");
    init_test_env();
    // 创建足够多的用户达到上限
    for (int i = 0; i < MAX_USERS; i++) {
        char username[20];
        sprintf(username, "testuser%d", i);
        register_user(username, "pass123", "pass123");
    }
    // 尝试注册第MAX_USERS+1个用户
    int result10_1 = register_user("overlimituser", "pass123", "pass123");
    printf("结果: %d (预期: -1)\n", result10_1);
    assert(result10_1 == -1);
    printf("覆盖: 用户上限检查路径\n");
    
    // 10.2 测试成功注册 - 覆盖剩余所有计算使用节点
    printf("\n子测试10.2: 成功注册\n");
    init_test_env();
    int result10_2 = register_user("newuser", "pass123", "pass123");
    printf("结果: %d (预期: 大于0)\n", result10_2);
    assert(result10_2 > 0);
    printf("覆盖所有剩余计算使用节点:\n");
    printf("TCOVER26-35(new_user的所有计算使用:65→66,67,68,69,70,71,72,73,74,76)\n");
    printf("TCOVER36(user_count:全局→77)\n");
    printf("TCOVER37(next_user_id:全局→66)\n");
    
    // 验证用户是否成功添加
    assert(user_count == 1);
    assert(strcmp(users[0].username, "newuser") == 0);
    printf("用户成功添加: user_count=%d, username=%s\n", user_count, users[0].username);
    
    printf("\n=== 全计算使用测试完成 ===\n");
    printf("所有37个测试覆盖项均已覆盖！\n");
    printf("测试用例总数: 10个\n\n");


    // ==================== 全谓语使用测试（方案3） ====================
    printf("========== 开始全谓语使用测试 ==========\n\n");
    
    // 初始化测试环境
    init_test_env();
    
    // ==================== 测试用例 1 ====================
    // 测试覆盖项：TP1, TP3, TP5 (用户名、密码、确认密码为空)
    printf("测试用例 1: 所有参数为空\n");
    printf("期望结果: 返回 -1，提示\"用户名、密码或确认密码不能为空\"\n");
    int result = register_user(NULL, NULL, NULL);
    printf("实际结果: ");
    if (result == -1) {
        printf("测试通过！\n");
    } else {
        printf("测试失败！返回值: %d\n", result);
    }
    printf("----------------------------------------\n\n");
    
    // ==================== 测试用例 2 ====================
    // 测试覆盖项：TP6 (用户名长度太短)
    printf("测试用例 2: 用户名长度 < 3\n");
    printf("期望结果: 返回 -1，提示\"输入的用户名太短，至少需要3个字符\"\n");
    init_test_env();
    result = register_user("ab", "12345a", "12345a");
    printf("实际结果: ");
    if (result == -1) {
        printf("测试通过！\n");
    } else {
        printf("测试失败！返回值: %d\n", result);
    }
    printf("----------------------------------------\n\n");
    
    // ==================== 测试用例 3 ====================
    // 测试覆盖项：TP7 (用户名长度太长)
    printf("测试用例 3: 用户名长度 > 15\n");
    printf("期望结果: 返回 -1，提示\"输入的用户名太长，不能超过15个字符\"\n");
    init_test_env();
    result = register_user("1234567890123456", "12345a", "12345a");
    printf("实际结果: ");
    if (result == -1) {
        printf("测试通过！\n");
    } else {
        printf("测试失败！返回值: %d\n", result);
    }
    printf("----------------------------------------\n\n");
    
    // ==================== 测试用例 4 ====================
    // 测试覆盖项：TP8, TP9, TP10 (用户名包含非法字符)
    printf("测试用例 4: 用户名包含非法字符\n");
    printf("期望结果: 返回 -1，提示\"输入的用户名包含非法字符，只能包含字母、数字和下划线\"\n");
    init_test_env();
    result = register_user("user@name", "12345a", "12345a");
    printf("实际结果: ");
    if (result == -1) {
        printf("测试通过！\n");
    } else {
        printf("测试失败！返回值: %d\n", result);
    }
    printf("----------------------------------------\n\n");
    
    // ==================== 测试用例 5 ====================
    // 测试覆盖项：TP11 (密码太短)
    printf("测试用例 5: 密码长度 < 6\n");
    printf("期望结果: 返回 -1，提示\"输入的密码太短，至少需要6个字符\"\n");
    init_test_env();
    result = register_user("user", "123", "123");
    printf("实际结果: ");
    if (result == -1) {
        printf("测试通过！\n");
    } else {
        printf("测试失败！返回值: %d\n", result);
    }
    printf("----------------------------------------\n\n");
    
    // ==================== 测试用例 6 ====================
    // 测试覆盖项：TP12 (密码太长)
    printf("测试用例 6: 密码长度 > 20\n");
    printf("期望结果: 返回 -1，提示\"输入的密码太长，不能超过20个字符\"\n");
    init_test_env();
    result = register_user("user", "123456789012345678901", "123456789012345678901");
    printf("实际结果: ");
    if (result == -1) {
        printf("测试通过！\n");
    } else {
        printf("测试失败！返回值: %d\n", result);
    }
    printf("----------------------------------------\n\n");
    
    // ==================== 测试用例 7 ====================
    // 测试覆盖项：TP14 (密码无字母)
    printf("测试用例 7: 密码无字母\n");
    printf("期望结果: 返回 -1，提示\"输入的密码必须包含至少一个字母\"\n");
    init_test_env();
    result = register_user("user", "123456", "123456");
    printf("实际结果: ");
    if (result == -1) {
        printf("测试通过！\n");
    } else {
        printf("测试失败！返回值: %d\n", result);
    }
    printf("----------------------------------------\n\n");
    
    // ==================== 测试用例 8 ====================
    // 测试覆盖项：TP16 (密码无数字)
    printf("测试用例 8: 密码无数字\n");
    printf("期望结果: 返回 -1，提示\"输入的密码必须包含至少一个数字\"\n");
    init_test_env();
    result = register_user("user", "abcdef", "abcdef");
    printf("实际结果: ");
    if (result == -1) {
        printf("测试通过！\n");
    } else {
        printf("测试失败！返回值: %d\n", result);
    }
    printf("----------------------------------------\n\n");
    
    // ==================== 测试用例 9 ====================
    // 测试覆盖项：TP15, TP17, TP18, TP19, TP20 (密码合法，继续后续检查)
    printf("测试用例 9: 密码包含字母和数字\n");
    printf("期望结果: 继续执行后续检查，可能成功注册或失败于其他条件\n");
    init_test_env();
    result = register_user("user", "12345a", "12345a");
    printf("实际结果: ");
    if (result > 0) {
        printf("测试通过！成功注册用户，用户ID: %d\n", result);
    } else {
        printf("测试失败！返回值: %d\n", result);
    }
    printf("----------------------------------------\n\n");
    
    // ==================== 测试用例 10 ====================
    // 测试覆盖项：TP4 (密码与确认密码不一致)
    printf("测试用例 10: 密码与确认密码不一致\n");
    printf("期望结果: 返回 -1，提示\"两次输入的密码不一致\"\n");
    init_test_env();
    result = register_user("user", "12345a", "12345b");
    printf("实际结果: ");
    if (result == -1) {
        printf("测试通过！\n");
    } else {
        printf("测试失败！返回值: %d\n", result);
    }
    printf("----------------------------------------\n\n");
    
    // ==================== 测试用例 11 ====================
    // 测试覆盖项：TP2 (用户名已存在)
    printf("测试用例 11: 用户名已存在\n");
    printf("期望结果: 返回 -1，提示\"用户名 'existing_user' 已被使用\"\n");
    init_test_env();
    // 先注册一个用户
    int first_result = register_user("existing_user", "12345a", "12345a");
    if (first_result > 0) {
        // 尝试用相同的用户名注册
        result = register_user("existing_user", "12345b", "12345b");
        printf("实际结果: ");
        if (result == -1) {
            printf("测试通过！\n");
        } else {
            printf("测试失败！返回值: %d\n", result);
        }
    } else {
        printf("实际结果: 第一次注册失败，无法进行此测试\n");
    }
    printf("----------------------------------------\n\n");
    
    // ==================== 测试用例 12 ====================
    // 测试覆盖项：TP13 (用户数量已达上限)
    printf("测试用例 12: 用户数量已达上限\n");
    printf("期望结果: 返回 -1，提示\"用户数量已达上限\"\n");
    init_test_env();
    
    // 根据MAX_USERS的值注册多个用户直到达到上限
    int max_users = MAX_USERS; // 假设MAX_USERS已定义
    int i;
    int success_count = 0;
    
    for (i = 0; i < max_users; i++) {
        char username[20];
        sprintf(username, "user%d", i);
        result = register_user(username, "12345a", "12345a");
        if (result > 0) {
            success_count++;
        } else {
            break;
        }
    }
    
    // 尝试注册最后一个用户（应该失败）
    char username[20];
    sprintf(username, "extra_user");
    result = register_user(username, "12345a", "12345a");
    printf("实际结果: ");
    if (result == -1 && success_count == max_users) {
        printf("测试通过！成功注册 %d 个用户后达到上限\n", success_count);
    } else {
        printf("测试失败！成功注册数: %d, 最后一次返回值: %d\n", success_count, result);
    }
    
    printf("\n========== 全谓语使用测试完成 ==========\n\n");


    // ==================== 全使用测试（方案4） ====================
    printf("========== 开始全使用测试 ==========\n\n");
    
    int test_case_count = 0;
    int passed_count = 0;
    
    // 测试用例1: TCOVER1, TCOVER8, TCOVER14 - 空指针测试
    printf("测试用例1: username=NULL, password=\"123\", confirm_password=\"123\"\n");
    printf("预期结果: 返回-1，输出\"用户名、密码或确认密码不能为空\"\n");
    init_test_env();
    result = register_user(NULL, "123", "123");
    if (result == -1) {
        printf("✓ 测试通过\n");
        passed_count++;
    } else {
        printf("✗ 测试失败\n");
    }
    test_case_count++;
    printf("覆盖: TCOVER1, TCOVER8, TCOVER14\n\n");
    
    // 测试用例2: TCOVER3, TCOVER10, TCOVER12, TCOVER15, TCOVER16 - 用户名太短
    printf("测试用例2: username=\"ab\", password=\"123456\", confirm_password=\"123456\"\n");
    printf("预期结果: 返回-1，输出\"输入的用户名太短\"\n");
    init_test_env();
    result = register_user("ab", "123456", "123456");
    if (result == -1) {
        printf("✓ 测试通过\n");
        passed_count++;
    } else {
        printf("✗ 测试失败\n");
    }
    test_case_count++;
    printf("覆盖: TCOVER3, TCOVER10, TCOVER12, TCOVER15, TCOVER16\n\n");
    
    // 测试用例3: TCOVER3, TCOVER10, TCOVER12, TCOVER15, TCOVER17 - 用户名太长
    printf("测试用例3: username=\"abcdefghijklmnop\", password=\"123456\", confirm_password=\"123456\"\n");
    printf("预期结果: 返回-1，输出\"输入的用户名太长\"\n");
    init_test_env();
    result = register_user("abcdefghijklmnop", "123456", "123456");
    if (result == -1) {
        printf("✓ 测试通过\n");
        passed_count++;
    } else {
        printf("✗ 测试失败\n");
    }
    test_case_count++;
    printf("覆盖: TCOVER3, TCOVER10, TCOVER12, TCOVER15, TCOVER17\n\n");
    
    // 测试用例4: TCOVER2, TCOVER3, TCOVER4, TCOVER10, TCOVER12, TCOVER15, TCOVER16, TCOVER17, 
    // TCOVER18, TCOVER19, TCOVER20, TCOVER21, TCOVER22, TCOVER23, TCOVER24, TCOVER25, TCOVER26 - 用户名包含非法字符
    printf("测试用例4: username=\"test@user\", password=\"123456\", confirm_password=\"123456\"\n");
    printf("预期结果: 返回-1，输出\"输入的用户名包含非法字符\"\n");
    init_test_env();
    result = register_user("test@user", "123456", "123456");
    if (result == -1) {
        printf("✓ 测试通过\n");
        passed_count++;
    } else {
        printf("✗ 测试失败\n");
    }
    test_case_count++;
    printf("覆盖: TCOVER2, TCOVER3, TCOVER4, TCOVER10, TCOVER12, TCOVER15, TCOVER16, TCOVER17, ");
    printf("TCOVER18, TCOVER19, TCOVER20, TCOVER21, TCOVER22, TCOVER23, TCOVER24, TCOVER25, TCOVER26\n\n");
    
    // 测试用例5: TCOVER3, TCOVER10, TCOVER12, TCOVER15, TCOVER16, TCOVER17, TCOVER27 - 密码太短
    printf("测试用例5: username=\"validuser\", password=\"123\", confirm_password=\"123\"\n");
    printf("预期结果: 返回-1，输出\"输入的密码太短\"\n");
    init_test_env();
    result = register_user("validuser", "123", "123");
    if (result == -1) {
        printf("✓ 测试通过\n");
        passed_count++;
    } else {
        printf("✗ 测试失败\n");
    }
    test_case_count++;
    printf("覆盖: TCOVER3, TCOVER10, TCOVER12, TCOVER15, TCOVER16, TCOVER17, TCOVER27\n\n");
    
    // 测试用例6: TCOVER3, TCOVER10, TCOVER12, TCOVER15, TCOVER16, TCOVER17, TCOVER28 - 密码太长
    printf("测试用例6: username=\"validuser\", password=\"123456789012345678901\", confirm_password=\"123456789012345678901\"\n");
    printf("预期结果: 返回-1，输出\"输入的密码太长\"\n");
    init_test_env();
    result = register_user("validuser", "123456789012345678901", "123456789012345678901");
    if (result == -1) {
        printf("✓ 测试通过\n");
        passed_count++;
    } else {
        printf("✗ 测试失败\n");
    }
    test_case_count++;
    printf("覆盖: TCOVER3, TCOVER10, TCOVER12, TCOVER15, TCOVER16, TCOVER17, TCOVER28\n\n");
    
    // 测试用例7: TCOVER3, TCOVER9, TCOVER10, TCOVER11, TCOVER12, TCOVER15, TCOVER16, TCOVER17, 
    // TCOVER29, TCOVER30, TCOVER34, TCOVER35, TCOVER36, TCOVER37, TCOVER38 - 密码缺少字母
    printf("测试用例7: username=\"validuser\", password=\"123456\", confirm_password=\"123456\"\n");
    printf("预期结果: 返回-1，输出\"输入的密码必须包含至少一个字母\"\n");
    init_test_env();
    result = register_user("validuser", "123456", "123456");
    if (result == -1) {
        printf("✓ 测试通过\n");
        passed_count++;
    } else {
        printf("✗ 测试失败\n");
    }
    test_case_count++;
    printf("覆盖: TCOVER3, TCOVER9, TCOVER10, TCOVER11, TCOVER12, TCOVER15, TCOVER16, TCOVER17, ");
    printf("TCOVER29, TCOVER30, TCOVER34, TCOVER35, TCOVER36, TCOVER37, TCOVER38\n\n");
    
    // 测试用例8: TCOVER3, TCOVER9, TCOVER10, TCOVER11, TCOVER12, TCOVER15, TCOVER16, TCOVER17, 
    // TCOVER29, TCOVER31, TCOVER32, TCOVER34, TCOVER35, TCOVER36, TCOVER37, TCOVER38 - 密码缺少数字
    printf("测试用例8: username=\"validuser\", password=\"abcdef\", confirm_password=\"abcdef\"\n");
    printf("预期结果: 返回-1，输出\"输入的密码必须包含至少一个数字\"\n");
    init_test_env();
    result = register_user("validuser", "abcdef", "abcdef");
    if (result == -1) {
        printf("✓ 测试通过\n");
        passed_count++;
    } else {
        printf("✗ 测试失败\n");
    }
    test_case_count++;
    printf("覆盖: TCOVER3, TCOVER9, TCOVER10, TCOVER11, TCOVER12, TCOVER15, TCOVER16, TCOVER17, ");
    printf("TCOVER29, TCOVER31, TCOVER32, TCOVER34, TCOVER35, TCOVER36, TCOVER37, TCOVER38\n\n");
    
    // 测试用例9: TCOVER3, TCOVER9, TCOVER10, TCOVER11, TCOVER12, TCOVER15, TCOVER16, TCOVER17, 
    // TCOVER29, TCOVER31, TCOVER33, TCOVER34, TCOVER35, TCOVER36, TCOVER37, TCOVER38 - 密码不一致
    printf("测试用例9: username=\"validuser\", password=\"abc123\", confirm_password=\"abc124\"\n");
    printf("预期结果: 返回-1，输出\"两次输入的密码不一致\"\n");
    init_test_env();
    result = register_user("validuser", "abc123", "abc124");
    if (result == -1) {
        printf("✓ 测试通过\n");
        passed_count++;
    } else {
        printf("✗ 测试失败\n");
    }
    test_case_count++;
    printf("覆盖: TCOVER3, TCOVER9, TCOVER10, TCOVER11, TCOVER12, TCOVER15, TCOVER16, TCOVER17, ");
    printf("TCOVER29, TCOVER31, TCOVER33, TCOVER34, TCOVER35, TCOVER36, TCOVER37, TCOVER38\n\n");
    
    // 测试用例10: TCOVER3, TCOVER5, TCOVER9, TCOVER10, TCOVER11, TCOVER12, TCOVER15, TCOVER16, 
    // TCOVER17, TCOVER29, TCOVER31, TCOVER33, TCOVER34, TCOVER35, TCOVER36, TCOVER37, TCOVER38 - 用户名已存在
    printf("测试用例10: username=\"existinguser\", password=\"abc123\", confirm_password=\"abc123\"\n");
    printf("预期结果: 返回-1，输出\"用户名 'existinguser' 已被使用\"\n");
    init_test_env();
    // 先注册一个用户
    register_user("existinguser", "abc123", "abc123");
    // 尝试用相同用户名注册
    result = register_user("existinguser", "abc123", "abc123");
    if (result == -1) {
        printf("✓ 测试通过\n");
        passed_count++;
    } else {
        printf("✗ 测试失败\n");
    }
    test_case_count++;
    printf("覆盖: TCOVER3, TCOVER5, TCOVER9, TCOVER10, TCOVER11, TCOVER12, TCOVER15, TCOVER16, ");
    printf("TCOVER17, TCOVER29, TCOVER31, TCOVER33, TCOVER34, TCOVER35, TCOVER36, TCOVER37, TCOVER38\n\n");
    
    // 测试用例11: TCOVER3, TCOVER5, TCOVER9, TCOVER10, TCOVER11, TCOVER12, TCOVER15, TCOVER16, 
    // TCOVER17, TCOVER29, TCOVER31, TCOVER33, TCOVER34, TCOVER35, TCOVER36, TCOVER37, TCOVER38, TCOVER49 - 用户数达到上限
    printf("测试用例11: username=\"newuser\", password=\"abc123\", confirm_password=\"abc123\" (假设用户数已达上限)\n");
    printf("预期结果: 返回-1，输出\"用户数量已达上限\"\n");
    init_test_env();
    // 将用户数设置为上限-1，然后注册一个用户，使其达到上限
    user_count = MAX_USERS - 1;
    next_user_id = MAX_USERS + 100; // 设置一个较高的ID避免冲突
    // 尝试注册，此时应该达到上限
    result = register_user("newuser", "abc123", "abc123");
    if (result == -1) {
        printf("✓ 测试通过\n");
        passed_count++;
    } else {
        printf("✗ 测试失败\n");
    }
    test_case_count++;
    printf("覆盖: TCOVER3, TCOVER5, TCOVER9, TCOVER10, TCOVER11, TCOVER12, TCOVER15, TCOVER16, ");
    printf("TCOVER17, TCOVER29, TCOVER31, TCOVER33, TCOVER34, TCOVER35, TCOVER36, TCOVER37, TCOVER38, TCOVER49\n\n");
    
    // 测试用例12: TCOVER6, TCOVER7, TCOVER13, TCOVER39, TCOVER40, TCOVER41, TCOVER42, TCOVER43, 
    // TCOVER44, TCOVER45, TCOVER46, TCOVER47, TCOVER48, TCOVER50, TCOVER51 - 正常注册
    printf("测试用例12: username=\"newuser\", password=\"abc123\", confirm_password=\"abc123\" (正常注册)\n");
    printf("预期结果: 注册成功，返回用户ID\n");
    init_test_env();
    result = register_user("newuser", "abc123", "abc123");
    if (result > 0) {
        printf("✓ 测试通过，返回用户ID: %d\n", result);
        passed_count++;
    } else {
        printf("✗ 测试失败，返回: %d\n", result);
    }
    test_case_count++;
    printf("覆盖: TCOVER6, TCOVER7, TCOVER13, TCOVER39, TCOVER40, TCOVER41, TCOVER42, TCOVER43, ");
    printf("TCOVER44, TCOVER45, TCOVER46, TCOVER47, TCOVER48, TCOVER50, TCOVER51\n\n");
    
    // 测试总结
    printf("========== 全使用测试完成 ==========\n\n");


    // ==================== 全定义-使用测试（方案5） ====================
    printf("开始全定义-使用测试...\n\n");
    
    // 测试用例1: 所有参数为NULL
    printf("测试用例1: 所有参数为NULL (覆盖TCOVER1,8,14)\n");
    init_test_env();
    result = register_user(NULL, NULL, NULL);
    printf("返回值: %d (预期: -1)\n\n", result);
    
    // 测试用例2: 用户名太短
    printf("测试用例2: 用户名太短 (覆盖TCOVER2,16,1,8,14)\n");
    init_test_env();
    result = register_user("ab", "pass12", "pass12");
    printf("返回值: %d (预期: -1)\n\n", result);
    
    // 测试用例3: 用户名太长
    printf("测试用例3: 用户名太长 (覆盖TCOVER2,17,1,8,14)\n");
    init_test_env();
    result = register_user("abcdefghijklmnop", "pass12", "pass12");
    printf("返回值: %d (预期: -1)\n\n", result);
    
    // 测试用例4: 用户名包含非法字符
    printf("测试用例4: 用户名包含非法字符 (覆盖TCOVER2,3,4,18,19,20,21,22,23,24,25,1,8,14)\n");
    init_test_env();
    result = register_user("user@name", "pass12", "pass12");
    printf("返回值: %d (预期: -1)\n\n", result);
    
    // 测试用例5: 密码无字母
    printf("测试用例5: 密码无字母 (覆盖TCOVER2,9,26,28,31,1,8,14,部分29)\n");
    init_test_env();
    result = register_user("username", "123456", "123456");
    printf("返回值: %d (预期: -1)\n\n", result);
    
    // 测试用例6: 密码无数字
    printf("测试用例6: 密码无数字 (覆盖TCOVER2,9,26,28,29,1,8,14,部分31)\n");
    init_test_env();
    result = register_user("username", "abcdef", "abcdef");
    printf("返回值: %d (预期: -1)\n\n", result);
    
    // 测试用例7: 密码不匹配
    printf("测试用例7: 密码不匹配 (覆盖TCOVER2,9,12,15,1,8,14)\n");
    init_test_env();
    result = register_user("username", "pass12", "pass123");
    printf("返回值: %d (预期: -1)\n\n", result);
    
    // 测试用例8: 用户名已存在
    printf("测试用例8: 用户名已存在 (覆盖TCOVER2,5,1,8,14)\n");
    init_test_env();
    // 先注册一个用户
    register_user("existing_user", "pass12", "pass12");
    // 尝试注册相同用户名的用户
    result = register_user("existing_user", "pass12", "pass12");
    printf("返回值: %d (预期: -1)\n\n", result);
    
    // 测试用例9: 用户数量已达上限
    printf("测试用例9: 用户数量已达上限 (覆盖TCOVER47,1,8,14)\n");
    init_test_env();
    // 将用户数量设置为最大值
    user_count = MAX_USERS;
    result = register_user("newuser", "pass12", "pass12");
    printf("返回值: %d (预期: -1)\n\n", result);
    
    // 测试用例10: 有效注册
    printf("测试用例10: 有效注册 (覆盖TCOVER2,3,4,5,6,7,9,10,11,12,13,18,19,20,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,48,49,1,8,14)\n");
    init_test_env();
    printf("初始用户数: %d\n", user_count);
    result = register_user("newuser", "pass12", "pass12");
    printf("注册成功返回值: %d (预期: 1)\n", result);
    printf("注册后用户数: %d (预期: 1)\n", user_count);
    
    // 验证新用户信息
    if (user_count > 0) {
        printf("验证新用户信息:\n");
        printf("  用户名: %s\n", users[0].username);
        printf("  用户ID: %d\n", users[0].user_id);
        printf("  余额: %.2f\n", users[0].balance);
    }
    
    // 测试用例11: 测试用户名的循环检查（覆盖更多i1循环路径）
    printf("\n测试用例11: 测试用户名循环检查 (覆盖TCOVER19,20,21,22,23,24,25的多种组合)\n");
    init_test_env();
    // 用户名包含多个字符，其中有一个非法字符
    result = register_user("user@name#test", "pass12", "pass12");
    printf("返回值: %d (预期: -1)\n\n", result);
    
    // 测试用例12: 测试密码的循环检查（覆盖更多i2循环路径）
    printf("测试用例12: 测试密码循环检查 (覆盖TCOVER33,34,35,36的多种组合)\n");
    init_test_env();
    // 密码包含字母和数字
    result = register_user("testuser", "pass1234", "pass1234");
    printf("返回值: %d (预期: 大于0)\n\n", result);
    
    // 测试用例13: 测试多用户注册
    printf("测试用例13: 测试多用户注册\n");
    init_test_env();
    printf("初始用户数: %d\n", user_count);
    register_user("user1", "pass123", "pass123");
    register_user("user2", "pass456", "pass456");
    register_user("user3", "pass789", "pass789");
    printf("最终用户数: %d (预期: 3)\n", user_count);
    
    printf("\n全定义-使用测试完成！\n");
}