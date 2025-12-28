#include <stdio.h>
#include <math.h> 
#include "balance.h"
#include "../common.h" 
#include "../register/register.h" 
#include "../transfer/transfer.h" 

// 1. 查询余额函数实现
double query_balance(int user_id) {
    if (user_id <= 0) return -1.0;

    int user_index = find_user_by_id(user_id);
    if (user_index == -1) return -1.0;

    // 状态 0 为正常
    if (users[user_index].status != 0) {
        return -2.0;
    }

    return users[user_index].balance;
}

// 2. 必须实现的辅助工具函数（之前报错的主因）
void set_user_balance(int user_id, double balance) {
    int idx = find_user_by_id(user_id);
    if (idx != -1) {
        users[idx].balance = balance;
    }
}

void set_user_status(int user_id, int status) {
    int idx = find_user_by_id(user_id);
    if (idx != -1) {
        users[idx].status = status;
    }
}

// 3. 跨模块集成测试函数
void test_balance_integration() {
    printf("\n======= 银行系统功能集成测试 (业务流) =======\n");

    // 调用 A 模块接口
    int userA = register_user("Integrate_A", "pass123", "pass123");
    int userB = register_user("Integrate_B", "pass123", "pass123");

    if (userA <= 0 || userB <= 0) {
        printf("[失败] 环境初始化错误：注册模块接口调用异常。\n");
        return;
    }

    // 初始注资
    set_user_balance(userA, 1000.0);
    set_user_balance(userB, 10.0);
    printf("[步骤1] 注册成功。用户A(ID:%d) 余额:1000, 用户B(ID:%d) 余额:10\n", userA, userB);

    // 调用 D 模块接口
    double amount = 300.0;
    printf("[步骤2] 动作：用户A 向 用户B 转账 %.2f 元...\n", amount);
    int status = transfer_money(userA, userB, amount);

    if (status == 1) {
        printf("[步骤3] 转账接口返回 1 (成功)，开始校验余额...\n");

        double balA = query_balance(userA);
        double balB = query_balance(userB);

        printf("\n--- 最终账目校验 ---\n");
        printf("用户A 余额: %.2f (预期: 700.00) -> %s\n", balA, (fabs(balA - 700.0) < 0.01) ? "PASS" : "FAIL");
        printf("用户B 余额: %.2f (预期: 310.00) -> %s\n", balB, (fabs(balB - 310.0) < 0.01) ? "PASS" : "FAIL");

        if (fabs(balA - 700.0) < 0.01 && fabs(balB - 310.0) < 0.01) {
            printf("\n[结论] 集成测试成功：模块链路畅通！\n");
        }
        else {
            printf("\n[结论] 集成测试失败：数据不一致。\n");
        }
    }
    else {
        printf("[失败] 转账接口返回异常，请检查成员D代码。\n");
    }
}

void test_equivalence_classes() {
    test_balance_integration();
}