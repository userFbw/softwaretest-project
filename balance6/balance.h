#ifndef BALANCE_H
#define BALANCE_H

// 查询余额函数声明
double query_balance(int user_id);

// --- 测试相关函数 ---
// 原有的基路径测试（单元测试）
void test_basis_path();
// 新增：跨模块集成测试（验证 注册-转账-查询 链路）
void test_balance_integration();

// 辅助工具（仅限测试使用）
void set_user_balance(int user_id, double balance);
void set_user_status(int user_id, int status);

#endif