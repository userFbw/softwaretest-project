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
// ======================
// 自底向上集成测试的驱动模块
// ======================

/**
 * 第一层：测试底层验证函数（独立单元测试）
 * 驱动模块：模拟调用验证函数
 */
void test_layer1_validation() {
    printf("\n[层1] 测试底层验证函数\n");
    printf("测试驱动模块: test_layer1_validation()\n");
    printf("测试目标: 独立验证函数，不依赖其他模块\n");
    
    int passed = 0, total = 0;
    
    // 测试6: 用户名太短
    total++;
    printf("测试6: is_valid_username(\"ab\") -> ");
    if (!is_valid_username("ab")) {
        printf("PASS (预期: false)\n");
        passed++;
    } else {
        printf("FAIL (预期: false, 实际: true)\n");
    }
    
    // 测试7: 用户名太长
    total++;
    printf("测试7: is_valid_username(\"abcdefghijklmno\") -> ");
    if (!is_valid_username("abcdefghijklmno")) {
        printf("PASS (预期: false)\n");
        passed++;
    } else {
        printf("FAIL (预期: false, 实际: true)\n");
    }
    
    // 测试8: 非法字符@
    total++;
    printf("测试8: is_valid_username(\"user@123\") -> ");
    if (!is_valid_username("user@123")) {
        printf("PASS (预期: false)\n");
        passed++;
    } else {
        printf("FAIL (预期: false, 实际: true)\n");
    }
    
    // 测试9: 有效用户名
    total++;
    printf("测试9: is_valid_username(\"user_123\") -> ");
    if (is_valid_username("user_123")) {
        printf("PASS (预期: true)\n");
        passed++;
    } else {
        printf("FAIL (预期: true, 实际: false)\n");
    }
    
    // 测试10: 空用户名
    total++;
    printf("测试10: is_valid_username(\"\") -> ");
    if (!is_valid_username("")) {
        printf("PASS (预期: false)\n");
        passed++;
    } else {
        printf("FAIL (预期: false, 实际: true)\n");
    }
    
    // 测试11: 密码太短
    total++;
    printf("测试11: is_valid_password(\"123\") -> ");
    if (!is_valid_password("123")) {
        printf("PASS (预期: false)\n");
        passed++;
    } else {
        printf("FAIL (预期: false, 实际: true)\n");
    }
    
    // 测试12: 密码太长
    total++;
    printf("测试12: is_valid_password(\"123456789012345678901\") -> ");
    if (!is_valid_password("123456789012345678901")) {
        printf("PASS (预期: false)\n");
        passed++;
    } else {
        printf("FAIL (预期: false, 实际: true)\n");
    }
    
    // 测试13: 密码缺数字
    total++;
    printf("测试13: is_valid_password(\"abcdef\") -> ");
    if (!is_valid_password("abcdef")) {
        printf("PASS (预期: false)\n");
        passed++;
    } else {
        printf("FAIL (预期: false, 实际: true)\n");
    }
    
    // 测试14: 密码缺字母
    total++;
    printf("测试14: is_valid_password(\"123456\") -> ");
    if (!is_valid_password("123456")) {
        printf("PASS (预期: false)\n");
        passed++;
    } else {
        printf("FAIL (预期: false, 实际: true)\n");
    }
    
    // 测试15: 有效密码
    total++;
    printf("测试15: is_valid_password(\"pass123\") -> ");
    if (is_valid_password("pass123")) {
        printf("PASS (预期: true)\n");
        passed++;
    } else {
        printf("FAIL (预期: true, 实际: false)\n");
    }
    
    printf("\n[层1总结] 验证函数测试: %d/%d 通过\n", passed, total);
    printf("--------------------------------\n");
}

/**
 * 第二层：测试查找函数（使用层1已测试函数）
 * 驱动模块：模拟调用查找函数，依赖全局数据结构
 */
void test_layer2_find_functions() {
    printf("\n[层2] 测试查找函数\n");
    printf("测试驱动模块: test_layer2_find_functions()\n");
    printf("测试目标: 使用层1验证函数，测试查找功能\n");
    
    // 初始化测试环境（驱动模块准备数据）
    init_test_env();
    
    // 添加测试数据到全局数组（模拟数据库）
    User test_user1 = {1001, "test_user", "pass123", 100.0, ACCOUNT_ACTIVE, 0, 0};
    User test_user2 = {1002, "user_123", "pass456", 200.0, ACCOUNT_ACTIVE, 0, 0};
    
    users[0] = test_user1;
    users[1] = test_user2;
    user_count = 2;
    next_user_id = 1003; // 设置下一个可用ID
    
    int passed = 0, total = 0;
    
    // 测试16: 查找已存在用户
    total++;
    printf("测试16: find_user_by_username(\"test_user\") -> ");
    int result = find_user_by_username("test_user");
    if (result == 1001) {
        printf("PASS (用户ID: %d)\n", result);
        passed++;
    } else {
        printf("FAIL (预期: 1001, 实际: %d)\n", result);
    }
    
    // 测试17: 查找不存在用户
    total++;
    printf("测试17: find_user_by_username(\"nonexistent\") -> ");
    result = find_user_by_username("nonexistent");
    if (result == -1) {
        printf("PASS (返回值: %d)\n", result);
        passed++;
    } else {
        printf("FAIL (预期: -1, 实际: %d)\n", result);
    }
    
    // 测试18: 通过ID查找已存在用户
    total++;
    printf("测试18: find_user_by_id(1001) -> ");
    int index = find_user_by_id(1001);
    if (index == 0) {
        printf("PASS (数组索引: %d)\n", index);
        passed++;
    } else {
        printf("FAIL (预期索引: 0, 实际: %d)\n", index);
    }
    
    // 测试19: 通过ID查找不存在用户
    total++;
    printf("测试19: find_user_by_id(9999) -> ");
    index = find_user_by_id(9999);
    if (index == -1) {
        printf("PASS (返回值: %d)\n", index);
        passed++;
    } else {
        printf("FAIL (预期: -1, 实际: %d)\n", index);
    }
    
    printf("\n[层2总结] 查找函数测试: %d/%d 通过\n", passed, total);
    printf("--------------------------------\n");
}

/**
 * 第三层：测试register_user函数（使用层1和层2已测试函数）
 * 驱动模块：模拟调用注册业务逻辑
 */
void test_layer3_register_user() {
    printf("\n[层3] 测试register_user业务函数\n");
    printf("测试驱动模块: test_layer3_register_user()\n");
    printf("测试目标: 使用层1验证函数和层2查找函数，测试完整注册业务逻辑\n");
    
    int passed = 0, total = 0;
    
    // 测试20: 用户名太短
    total++;
    printf("测试20: 用户名太短 (ab) -> ");
    init_test_env();
    int result = register_user("ab", "pass123", "pass123");
    if (result == -1) {
        printf("PASS (返回值: %d)\n", result);
        passed++;
    } else {
        printf("FAIL (预期: -1, 实际: %d)\n", result);
    }
    
    // 测试21: 用户名太长
    total++;
    printf("测试21: 用户名太长 -> ");
    init_test_env();
    result = register_user("abcdefghijklmnop", "pass123", "pass123");
    if (result == -1) {
        printf("PASS (返回值: %d)\n", result);
        passed++;
    } else {
        printf("FAIL (预期: -1, 实际: %d)\n", result);
    }
    
    // 测试22: 非法字符
    total++;
    printf("测试22: 用户名包含非法字符 (#) -> ");
    init_test_env();
    result = register_user("user#123", "pass123", "pass123");
    if (result == -1) {
        printf("PASS (返回值: %d)\n", result);
        passed++;
    } else {
        printf("FAIL (预期: -1, 实际: %d)\n", result);
    }
    
    // 测试23: 密码太短
    total++;
    printf("测试23: 密码太短 -> ");
    init_test_env();
    result = register_user("user123", "123", "123");
    if (result == -1) {
        printf("PASS (返回值: %d)\n", result);
        passed++;
    } else {
        printf("FAIL (预期: -1, 实际: %d)\n", result);
    }
    
    // 测试24: 密码太长
    total++;
    printf("测试24: 密码太长 -> ");
    init_test_env();
    result = register_user("user123", "password1234567890123", "password1234567890123");
    if (result == -1) {
        printf("PASS (返回值: %d)\n", result);
        passed++;
    } else {
        printf("FAIL (预期: -1, 实际: %d)\n", result);
    }
    
    // 测试25: 密码缺数字
    total++;
    printf("测试25: 密码缺数字 -> ");
    init_test_env();
    result = register_user("user123", "abcdefg", "abcdefg");
    if (result == -1) {
        printf("PASS (返回值: %d)\n", result);
        passed++;
    } else {
        printf("FAIL (预期: -1, 实际: %d)\n", result);
    }
    
    // 测试26: 密码缺字母
    total++;
    printf("测试26: 密码缺字母 -> ");
    init_test_env();
    result = register_user("user123", "1234567", "1234567");
    if (result == -1) {
        printf("PASS (返回值: %d)\n", result);
        passed++;
    } else {
        printf("FAIL (预期: -1, 实际: %d)\n", result);
    }
    
    // 测试27: 密码不一致
    total++;
    printf("测试27: 密码不一致 -> ");
    init_test_env();
    result = register_user("user123", "pass123", "pass456");
    if (result == -1) {
        printf("PASS (返回值: %d)\n", result);
        passed++;
    } else {
        printf("FAIL (预期: -1, 实际: %d)\n", result);
    }
    
    // 测试28: 用户名已存在
    total++;
    printf("测试28: 用户名已存在 -> ");
    init_test_env();
    // 先注册一个用户
    register_user("existing_user", "pass123", "pass123");
    // 尝试注册同名用户
    result = register_user("existing_user", "pass123", "pass123");
    if (result == -1) {
        printf("PASS (返回值: %d)\n", result);
        passed++;
    } else {
        printf("FAIL (预期: -1, 实际: %d)\n", result);
    }
    
    // 测试29: 成功注册
    total++;
    printf("测试29: 成功注册新用户 -> ");
    init_test_env();
    result = register_user("new_user", "pass123", "pass123");
    if (result > 0) {
        printf("PASS (用户ID: %d)\n", result);
        // 验证用户数据是否正确
        int idx = find_user_by_id(result);
        if (idx != -1 && users[idx].balance == 100.0 && 
            users[idx].status == ACCOUNT_ACTIVE && 
            strcmp(users[idx].username, "new_user") == 0) {
            printf("    用户数据验证: PASS\n");
            passed++;
        } else {
            printf("    用户数据验证: FAIL\n");
        }
    } else {
        printf("FAIL (返回值: %d)\n", result);
    }
    
    // 测试30: 用户数已达上限
    total++;
    printf("测试30: 用户数已达上限 -> ");
    init_test_env();
    // 注册MAX_USERS个用户
    for (int i = 0; i < MAX_USERS; i++) {
        char username[16];
        sprintf(username, "user%d", i);
        register_user(username, "pass123", "pass123");
    }
    // 尝试注册第MAX_USERS+1个用户
    result = register_user("extra_user", "pass123", "pass123");
    if (result == -1 && user_count == MAX_USERS) {
        printf("PASS (返回值: %d, 用户数: %d)\n", result, user_count);
        passed++;
    } else {
        printf("FAIL (预期: -1, 实际: %d, 用户数: %d)\n", result, user_count);
    }
    
    // 测试31: 空用户名指针
    total++;
    printf("测试31: 空用户名指针 -> ");
    init_test_env();
    result = register_user(NULL, "pass123", "pass123");
    if (result == -1) {
        printf("PASS (返回值: %d)\n", result);
        passed++;
    } else {
        printf("FAIL (预期: -1, 实际: %d)\n", result);
    }
    
    // 测试32: 空密码指针
    total++;
    printf("测试32: 空密码指针 -> ");
    init_test_env();
    result = register_user("user123", NULL, "pass123");
    if (result == -1) {
        printf("PASS (返回值: %d)\n", result);
        passed++;
    } else {
        printf("FAIL (预期: -1, 实际: %d)\n", result);
    }
    
    printf("\n[层3总结] 注册业务函数测试: %d/%d 通过\n", passed, total);
    printf("--------------------------------\n");
}

/**
 * 第四层：整体集成测试（模拟用户界面层）
 * 最终集成测试，所有模块已组合成子系统
 */
void test_layer4_integration() {
    printf("\n[层4] 整体集成测试（模拟用户界面层）\n");
    printf("测试驱动模块: test_layer4_integration()\n");
    printf("测试目标: 模拟用户注册流程，测试完整子系统\n");
    
    int passed = 0, total = 0;
    
    // 测试33: 正常注册流程
    total++;
    printf("测试33: 模拟正常注册流程 -> ");
    init_test_env();
    int result = register_user("valid_user", "valid123", "valid123");
    if (result > 0) {
        printf("PASS (注册成功，用户ID: %d)\n", result);
        
        // 验证完整用户信息
        int idx = find_user_by_id(result);
        if (idx != -1) {
            User* user = &users[idx];
            if (user->balance == 100.0 && 
                user->status == ACCOUNT_ACTIVE && 
                user->failed_attempts == 0 && 
                strcmp(user->username, "valid_user") == 0) {
                printf("    完整用户信息验证: PASS\n");
                passed++;
            } else {
                printf("    完整用户信息验证: FAIL\n");
            }
        } else {
            printf("    用户查找失败\n");
        }
    } else {
        printf("FAIL (注册失败，返回值: %d)\n", result);
    }
    
    // 测试34: 非法用户名处理
    total++;
    printf("测试34: 非法用户名处理 -> ");
    init_test_env();
    result = register_user("user@test", "pass123", "pass123");
    if (result == -1) {
        printf("PASS (正确拒绝非法用户名)\n");
        passed++;
    } else {
        printf("FAIL (预期拒绝但注册成功)\n");
    }
    
    // 测试35: 密码不一致处理
    total++;
    printf("测试35: 密码不一致处理 -> ");
    init_test_env();
    result = register_user("test_user", "password123", "password456");
    if (result == -1) {
        printf("PASS (正确拒绝密码不一致)\n");
        passed++;
    } else {
        printf("FAIL (预期拒绝但注册成功)\n");
    }
    
    // 测试36: 重复用户处理
    total++;
    printf("测试36: 重复用户处理 -> ");
    init_test_env();
    // 先注册一个用户
    register_user("duplicate_user", "pass123", "pass123");
    int first_id = find_user_by_username("duplicate_user");
    // 尝试注册同名用户
    result = register_user("duplicate_user", "pass123", "pass123");
    if (result == -1 && first_id != -1) {
        printf("PASS (正确拒绝重复用户，首个用户ID: %d)\n", first_id);
        passed++;
    } else {
        printf("FAIL (预期拒绝但注册成功或未找到首个用户)\n");
    }
    
    printf("\n[层4总结] 整体集成测试: %d/%d 通过\n", passed, total);
    printf("--------------------------------\n");
}

/**
 * 完整的自底向上集成测试入口
 * 严格按照PPT理论模型实现：
 * 1. 从最底层模块开始测试
 * 2. 逐步向上集成
 * 3. 每个层次使用专门的驱动模块
 * 4. 用实际模块替换驱动模块，形成更大的子系统
 */
void test_register_user() {
    printf("\n===========================================\n");
    printf("    用户注册功能 - 自底向上集成测试\n");
    printf("===========================================\n");
    
    // 第一步：测试最底层验证函数
    printf("\n【步骤1】测试底层验证函数（不依赖其他模块）\n");
    test_layer1_validation();
    
    // 第二步：测试中间层查找函数（使用层1函数，需要驱动模块准备数据）
    printf("\n【步骤2】测试查找函数（依赖层1，需要全局数据）\n");
    printf("驱动模块功能: 初始化测试环境，准备测试数据\n");
    test_layer2_find_functions();
    
    // 第三步：测试业务逻辑层（使用层1和层2函数）
    printf("\n【步骤3】测试注册业务函数（依赖层1和层2）\n");
    printf("驱动模块功能: 调用register_user，验证业务规则\n");
    test_layer3_register_user();
    
    // 第四步：整体集成测试（模拟用户界面层）
    printf("\n【步骤4】整体集成测试（完整子系统）\n");
    printf("驱动模块功能: 模拟用户注册流程，测试完整功能\n");
    test_layer4_integration();
    
    
    // 等待用户确认
    printf("\n按回车键返回主菜单...");
    getchar(); // 等待回车，确保测试结果可见
}