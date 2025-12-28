#include "balance.h"
#include <stdio.h>

// ======================
// 1. 核心功能实现
// ======================

double query_balance(int user_id) {
    // 定义点1: user_index
    int user_index = find_user_by_id(user_id);

    // 谓词使用(P-Use) 1: user_index == -1
    if (user_index == -1) {
        printf("错误：用户ID %d 不存在\n", user_id);
        return -1.0;
    }

    // 定义点2: current_user
    User* current_user = &users[user_index];

    // 谓词使用(P-Use) 2: current_user->status != ACCOUNT_ACTIVE
    if (current_user->status != ACCOUNT_ACTIVE) {
        const char* status_str = "未知";
        // 计算使用(C-Use) 1: status
        switch (current_user->status) {
        case ACCOUNT_FROZEN: status_str = "冻结"; break;
        case ACCOUNT_LOCKED: status_str = "锁定"; break;
        default: status_str = "异常"; break;
        }
        printf("错误：账户状态为 %s，无法查询余额\n", status_str);
        return -2.0;
    }

    // 谓词使用(P-Use) 3: current_user->balance < 0
    if (current_user->balance < 0) {
        // 计算使用(C-Use) 2: balance (用于打印)
        printf("警告：账户 %d 余额异常 (%.2f)\n", user_id, current_user->balance);
    }

    // 计算使用(C-Use) 3: balance (用于返回)
    printf("用户 %d 的余额查询成功: %.2f\n", user_id, current_user->balance);
    return current_user->balance;
}

// ======================
// 2. 测试环境初始化
// ======================

void init_test_environment() {
    user_count = 0;
    // ID 1001: 激活且余额为正 (用于覆盖 P-Use 3 False)
    users[user_count++] = (User){ .user_id = 1001, .status = ACCOUNT_ACTIVE, .balance = 500.0 };
    // ID 1002: 激活且余额为负 (用于覆盖 P-Use 3 True)
    users[user_count++] = (User){ .user_id = 1002, .status = ACCOUNT_ACTIVE, .balance = -50.0 };
    // ID 1003: 冻结 (用于覆盖 P-Use 2 True -> Case Frozen)
    users[user_count++] = (User){ .user_id = 1003, .status = ACCOUNT_FROZEN, .balance = 100.0 };
    // ID 1004: 锁定 (用于覆盖 P-Use 2 True -> Case Locked)
    users[user_count++] = (User){ .user_id = 1004, .status = ACCOUNT_LOCKED, .balance = 200.0 };
}

// ======================
// 3. 数据流覆盖测试跑全
// ======================

// 全定义覆盖 (All-Defs): 确保每个定义点至少到达一个使用点
void test_all_definition_coverage() {
    printf("\n--- 执行: 全定义覆盖测试 (All-Defs) ---\n");
    init_test_environment();
    query_balance(9999); // 覆盖 user_index 定义
    query_balance(1001); // 覆盖 current_user 和 balance 定义
}

// 全计算使用覆盖 (All-C-Uses): 覆盖所有计算使用点 (Status在switch中, Balance在输出/返回中)
void test_all_computation_usage() {
    printf("\n--- 执行: 全计算使用覆盖测试 (All-C-Uses) ---\n");
    init_test_environment();
    query_balance(1003); // 覆盖 status 在 switch 中的使用
    query_balance(1001); // 覆盖 balance 在返回中的使用
}

// 全谓词使用覆盖 (All-P-Uses): 覆盖所有 if 判断的真/假分支
void test_all_predicate_usage() {
    printf("\n--- 执行: 全谓词使用覆盖测试 (All-P-Uses) ---\n");
    init_test_environment();
    query_balance(9999); // user_index == -1 (T)
    query_balance(1001); // user_index == -1 (F), status != ACTIVE (F), balance < 0 (F)
    query_balance(1002); // balance < 0 (T)
    query_balance(1003); // status != ACTIVE (T)
}

// 全使用覆盖 (All-Usages): 包含所有 C-Use 和 P-Use
void test_all_usage_coverage() {
    printf("\n--- 执行: 全使用覆盖测试 (All-Usages) ---\n");
    test_all_predicate_usage();
    test_all_computation_usage();
}

// 全定义-使用覆盖 (All-Def-Use): 覆盖从每个定义点到每个可达使用点的所有路径
void test_all_def_use_coverage() {
    printf("\n>>> 开始：全定义-使用覆盖测试 (All-Def-Use) <<<\n");
    init_test_environment();
    double res;

    printf("\n[Case 1] 路径: user_index定义 -> 谓词1为真 (ID 9999)\n");
    res = query_balance(9999);
    printf("验证: %.2f == -1.00\n", res);

    printf("\n[Case 2] 路径: status定义 -> 谓词2为真 (ID 1003)\n");
    res = query_balance(1003);
    printf("验证: %.2f == -2.00\n", res);

    printf("\n[Case 3] 路径: balance定义 -> 谓词3为假 -> 返回balance (ID 1001)\n");
    res = query_balance(1001);
    printf("验证: %.2f == 500.00\n", res);

    printf("\n[Case 4] 路径: balance定义 -> 谓词3为真 -> 返回balance (ID 1002)\n");
    res = query_balance(1002);
    printf("验证: %.2f == -50.00\n", res);

    printf("\n>>> 全定义-使用覆盖测试完成 <<<\n");
}

// ======================
// 4. 全量测试调用接口
// ======================

void test_equivalence_classes() {
    printf("\n=====================================");
    printf("\n       余额查询模块 数据流自动化测试套件       ");
    printf("\n=====================================\n");

    test_all_definition_coverage();
    test_all_computation_usage();
    test_all_predicate_usage();
    test_all_usage_coverage();
    test_all_def_use_coverage();

    printf("\n=====================================\n");
}