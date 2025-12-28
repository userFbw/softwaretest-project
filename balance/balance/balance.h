#ifndef BALANCE_H
#define BALANCE_H

// 引用公共头文件，这样就不需要在这里重复定义 User 结构体了
#include "../common.h" 

// ======================
// 余额查询模块 - 函数声明
// ======================

/**
 * 余额查询核心函数
 * @param user_id 待查询的用户ID
 * @return 成功返回余额(>=0)，失败返回错误码(-1.0 或 -2.0)
 */
double query_balance(int user_id);

// ======================
// 测试套件 - 函数声明
// ======================

// 初始化测试数据（向全局数组 users 中填入模拟数据）
void init_test_environment();

// 执行所有覆盖测试用例
void test_equivalence_classes();

// 具体的路径/覆盖测试函数
void test_all_definition_coverage();      // 全定义覆盖
void test_all_computation_usage();        // 全计算使用覆盖
void test_all_predicate_usage();          // 全谓词使用覆盖
void test_all_def_use_coverage();         // 全定义-使用覆盖

#endif // BALANCE_H