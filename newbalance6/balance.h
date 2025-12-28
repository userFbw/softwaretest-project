#ifndef BALANCE_H
#define BALANCE_H

#include "../common.h" // 必须包含，否则 User 结构体未定义

// 查询余额核心函数
double query_balance(int user_id);

// --- 集成测试相关 ---
void test_balance_integration();
void test_equivalence_classes();

// 辅助工具实现（必须在 balance.c 中定义）
void set_user_balance(int user_id, double balance);
void set_user_status(int user_id, int status);

#endif