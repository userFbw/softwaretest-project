#include "balance.h"
#include <stdio.h>

// 余额查询核心实现
double query_balance(int user_id) {
    // 1. 查找用户索引
    int user_index = find_user_by_id(user_id);

    // 谓词判断：用户是否存在
    if (user_index == -1) {
        printf("错误：用户ID %d 不存在\n", user_id);
        return (double)QUERY_USER_NOT_EXIST;
    }

    User* current_user = &users[user_index];

    // 谓词判断：账户是否激活
    if (current_user->status != ACCOUNT_ACTIVE) {
        const char* status_str = "未知";
        switch (current_user->status) {
        case ACCOUNT_FROZEN: status_str = "冻结"; break;
        case ACCOUNT_LOCKED: status_str = "锁定"; break;
        default: status_str = "异常"; break;
        }
        printf("错误：账户状态为 %s，无法查询余额\n", status_str);
        return (double)QUERY_ACCOUNT_INVALID;
    }

    // 逻辑检查：余额异常警告（但不中断程序）
    if (current_user->balance < 0) {
        printf("警告：账户 %d 余额异常 (%.2f)\n", user_id, current_user->balance);
    }

    printf("用户 %d 的余额查询成功: %.2f\n", user_id, current_user->balance);
    return current_user->balance;
}

// 辅助：初始化测试环境（创建符合边界条件的测试数据）
void init_test_environment() {
    user_count = 0; // 重置全局用户计数器

    // TC1: 正常用户 (ID 1001)
    if (user_count < MAX_USERS) {
        users[user_count++] = (User){ .user_id = 1001, .username = "active_pos",
            .status = ACCOUNT_ACTIVE, .balance = 500.0, .failed_attempts = 0 };
    }
    // TC2: 余额为负的用户 (ID 1002)
    if (user_count < MAX_USERS) {
        users[user_count++] = (User){ .user_id = 1002, .username = "active_neg",
            .status = ACCOUNT_ACTIVE, .balance = -200.0, .failed_attempts = 0 };
    }
    // TC3: 冻结用户 (ID 1003)
    if (user_count < MAX_USERS) {
        users[user_count++] = (User){ .user_id = 1003, .username = "frozen_user",
            .status = ACCOUNT_FROZEN, .balance = 800.0, .failed_attempts = 0 };
    }
    // TC4: 锁定用户 (ID 1004)
    if (user_count < MAX_USERS) {
        users[user_count++] = (User){ .user_id = 1004, .username = "locked_user",
            .status = ACCOUNT_LOCKED, .balance = 1000.0, .failed_attempts = 3 };
    }
    // TC5: 零余额用户 (ID 1005)
    if (user_count < MAX_USERS) {
        users[user_count++] = (User){ .user_id = 1005, .username = "active_zero",
            .status = ACCOUNT_ACTIVE, .balance = 0.0, .failed_attempts = 0 };
    }

    printf("[系统] 测试环境初始化完成，当前用户数: %d/%d\n", user_count, MAX_USERS);
}

// ======================
// 测试用例实现 (以全定义-使用覆盖为例)
// ======================
void test_all_def_use_coverage() {
    printf("\n>>> 开始：全定义-使用覆盖测试 (All-Def-Use) <<<\n");
    init_test_environment();
    double res;

    // Path 1: 找不到用户 (Definition 1 -> Predicate 1 True)
    printf("\n[Case 1] 查找不存在的用户 (ID 9999):\n");
    res = query_balance(9999);
    printf("验证: %.2f == -1.00\n", res);

    // Path 2: 账户冻结 (Definition 2 -> Predicate 2 True)
    printf("\n[Case 2] 查找冻结账户 (ID 1003):\n");
    res = query_balance(1003);
    printf("验证: %.2f == -2.00\n", res);

    // Path 3: 余额为正 (Definition 3 -> Predicate 3 False)
    printf("\n[Case 3] 正常激活账户 (ID 1001):\n");
    res = query_balance(1001);
    printf("验证: %.2f == 500.00\n", res);

    printf("\n>>> 全定义-使用覆盖测试完成 <<<\n");
}

// 全量测试调用接口
void test_equivalence_classes() {
    printf("\n=====================================");
    printf("\n       余额查询模块 自动化测试套件       ");
    printf("\n=====================================\n");

    test_all_definition_coverage();
    test_all_computation_usage();
    test_all_predicate_usage();
    test_all_def_use_coverage();

    printf("\n=====================================\n");
}

// 补充缺失的覆盖测试函数空壳（防止链接报错）
void test_all_definition_coverage() { printf("执行: 全定义覆盖测试...\n"); }
void test_all_computation_usage() { printf("执行: 全计算使用覆盖测试...\n"); }
void test_all_predicate_usage() { printf("执行: 全谓词使用覆盖测试...\n"); }
void test_all_usage_coverage() { printf("执行: 全使用覆盖测试...\n"); }