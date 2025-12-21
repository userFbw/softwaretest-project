#include <stdio.h>
#include <math.h> 
#include "balance.h"
#include "../common.h" 

// 查询余额函数实现 (保持不变)
double query_balance(int user_id) {
    // 1. 检查用户ID是否有效 (决策点 1)
    if (user_id <= 0) {
        return -1.0; // P2
    }

    // 4. 在用户数组中查找用户
    int user_index = find_user_by_id(user_id);
    // 5. 检查用户是否找到 (决策点 2)
    if (user_index == -1) {
        return -1.0; // P3
    }

    // 8. 检查账户状态 (决策点 3)
    if (users[user_index].status != 0) { // 假设0是正常状态
        return -2.0; // P4
    }

    // 11. 返回余额 (P1)
    return users[user_index].balance;
}

// 设置用户余额（用于测试）
void set_user_balance(int user_id, double balance) {
    int user_index = find_user_by_id(user_id);
    if (user_index != -1) {
        users[user_index].balance = balance;
    }
}

// 设置用户状态（用于测试）
void set_user_status(int user_id, int status) {
    int user_index = find_user_by_id(user_id);
    if (user_index != -1) {
        users[user_index].status = status;
    }
}

// ==============================
// 基路径测试方法实现
// ==============================

// 基路径测试
void test_basis_path() {
    printf("\n=== 基路径测试 (V(G)=4) ===\n");
    printf("测试目标：覆盖所有4条线性独立路径\n\n");

    // 路径条件对照：
    // P1: T T T -> 返回 Balance
    // P2: F - - -> 返回 -1.0
    // P3: T F - -> 返回 -1.0
    // P4: T T F -> 返回 -2.0

    printf("用例ID | 用户ID | 路径 ID | C1(ID>0) | C2(用户存在) | C3(状态=0) | 设置状态 | 预期输出 | 实际输出 | 结果\n");
    printf("-------|--------|---------|----------|--------------|------------|----------|----------|----------|-----\n");

    // 浮点数比较阈值
    const double EPSILON = 0.000001;

    // 路径测试用例：
    // { 用户ID, 设置余额, 设置状态, 预期输出, 路径 ID }
    double test_cases[][5] = {
        // P1: T T T (正常路径)
        {1, 100.50, 0, 100.50, 1},
        // P2: F - - (ID <= 0)
        {-1, 0.00, 0, -1.0, 2},
        // P3: T F - (ID > 0, 用户不存在) - 假设用户ID 6 不存在
        {6, 0.00, 0, -1.0, 3},
        // P4: T T F (ID > 0, 用户存在, 状态 != 0)
        {2, 50.00, 1, -2.0, 4},
    };

    int pass_count = 0;
    int total_count = sizeof(test_cases) / sizeof(test_cases[0]);

    for (int i = 0; i < total_count; i++) {
        int user_id = (int)test_cases[i][0];
        double balance = test_cases[i][1];
        int status = (int)test_cases[i][2];
        double expected = test_cases[i][3];
        int path_id = (int)test_cases[i][4];

        // 设置测试环境 (只对有效的用户 ID 设置状态和余额)
        if (path_id == 1 || path_id == 4) { // P1, P4 需要存在的用户
            set_user_balance(user_id, balance);
            set_user_status(user_id, status);
        }

        // 确定 C1, C2, C3 的显示值 (用于表格)
        const char* c1_desc = (user_id > 0) ? "T" : "F";
        const char* c2_desc = "-";
        const char* c3_desc = "-";

        if (path_id == 1 || path_id == 4) { // P1, P4
            c2_desc = "T";
            c3_desc = (status == 0) ? "T" : "F";
        }
        else if (path_id == 3) { // P3
            c2_desc = "F";
        }

        // 执行测试
        double actual = query_balance(user_id);

        // 验证结果 (使用浮点数安全比较)
        int result = (fabs(actual - expected) < EPSILON) ? 1 : 0;
        if (result) pass_count++;

        printf("BP%d    | %-6d | P%d      | %-8s | %-12s | %-10s | %-8s | %-8.2f | %-8.2f | %s\n",
            i + 1, user_id, path_id, c1_desc, c2_desc, c3_desc,
            (path_id == 1) ? "正常(0)" : ((path_id == 4) ? "异常(1)" : "-"),
            expected, actual,
            result ? "通过" : "失败");
    }

    printf("\n基路径测试结果: %d/%d 通过\n", pass_count, total_count);
}

// 综合测试函数 (保持原有的名称 test_equivalence_classes，但实现是基路径测试)
void test_equivalence_classes() {
    printf("\n=== 开始基路径测试套件 ===\n");

    // 保存原始状态
    double original_balances[MAX_USERS] = { 0 };
    int original_statuses[MAX_USERS] = { 0 };
    for (int i = 0; i < user_count; i++) {
        original_balances[i] = users[i].balance;
        original_statuses[i] = users[i].status;
    }

    // 执行基路径测试
    test_basis_path();

    // 恢复原始状态
    for (int i = 0; i < user_count; i++) {
        users[i].balance = original_balances[i];
        users[i].status = original_statuses[i];
    }

    printf("\n=== 基路径测试套件完成 ===\n");
}