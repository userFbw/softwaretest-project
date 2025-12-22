#include <stdio.h>
#include <math.h> 
#include "balance.h"
#include "../common.h" 
#include "../register/register.h"  // 引入成员A的接口
#include "../transfer/transfer.h"  // 引入成员D的接口

// 查询余额函数实现
double query_balance(int user_id) {
    if (user_id <= 0) return -1.0;

    int user_index = find_user_by_id(user_id);
    if (user_index == -1) return -1.0;

    // 关键集成点：状态 0 必须是各模块公认的“正常”状态
    if (users[user_index].status != 0) {
        return -2.0;
    }

    return users[user_index].balance;
}

// ==============================
// 余额查询 - 跨模块集成测试函数
// ==============================
void test_balance_integration() {
    printf("\n======= 银行系统功能集成测试 (业务流) =======\n");

    // 1. 调用成员A的注册接口：模拟真实用户产生
    // 验证点：注册模块写入的数据，查询模块能否通过 ID 查到
    int userA = register_user("Integrate_A", "pass123", "pass123");
    int userB = register_user("Integrate_B", "pass123", "pass123");

    if (userA <= 0 || userB <= 0) {
        printf("[失败] 环境初始化错误：注册模块接口调用异常。\n");
        return;
    }

    // 初始注资（模拟系统级操作）
    set_user_balance(userA, 1000.0);
    set_user_balance(userB, 10.0);
    printf("[步骤1] 注册成功。用户A(ID:%d) 余额:1000, 用户B(ID:%d) 余额:10\n", userA, userB);

    // 2. 调用成员D的转账接口：执行跨模块数据变动
    // 验证点：成员D扣钱后，你的查询函数能否立刻读到最新的 balance 字段
    double amount = 300.0;
    printf("[步骤2] 动作：用户A 向 用户B 转账 %.2f 元...\n", amount);

    int status = transfer_money(userA, userB, amount);

    // 根据成员D的确认：返回 1 代表成功
    if (status == 1) {
        printf("[步骤3] 转账接口返回 1 (成功)，开始校验余额...\n");

        // 3. 调用你的查询接口：验证数据一致性
        double balA = query_balance(userA);
        double balB = query_balance(userB);

        printf("\n--- 最终账目校验 ---\n");
        printf("用户A (转出方) 余额: %.2f (预期: 700.00) -> %s\n",
            balA, (fabs(balA - 700.0) < 0.01) ? "PASS" : "FAIL");
        printf("用户B (转入方) 余额: %.2f (预期: 310.00) -> %s\n",
            balB, (fabs(balB - 310.0) < 0.01) ? "PASS" : "FAIL");

        if (fabs(balA - 700.0) < 0.01 && fabs(balB - 310.0) < 0.01) {
            printf("\n[结论] 集成测试成功：注册、转账、查询模块链路畅通！\n");
        }
        else {
            printf("\n[结论] 集成测试失败：模块间存在数据同步问题。\n");
        }
    }
    else {
        printf("[失败] 转账接口返回 %d，业务链路中断，请检查成员D的逻辑。\n", status);
    }

    printf("============================================\n");
}

// main.c 调用的那个“入口”函数
void test_equivalence_classes() {
    test_balance_integration();  // 跑集成测试
}