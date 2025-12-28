#ifndef BALANCE_H
#define BALANCE_H

// 必须包含公共头文件以获取 User 结构体定义
#include "../common.h" 

// ======================
// 1. 核心功能声明
// ======================

/**
 * 余额查询函数
 * 返回值说明：
 * 成功：返回余额 (double)
 * 失败：-1.0 (用户不存在), -2.0 (账户被冻结或锁定)
 */
double query_balance(int user_id);

// ======================
// 2. 数据流测试套件声明
// ======================

// 初始化测试环境
void init_test_environment();

// 余额查询全量测试入口
void test_equivalence_classes();

// 数据流覆盖指标子项
void test_all_definition_coverage();      // 全定义覆盖 (All-Defs)
void test_all_computation_usage();        // 全计算使用覆盖 (All-C-Uses)
void test_all_predicate_usage();          // 全谓词使用覆盖 (All-P-Uses)
void test_all_usage_coverage();           // 全使用覆盖 (All-Usages)
void test_all_def_use_coverage();         // 全定义-使用覆盖 (All-Def-Use)

#endif // BALANCE_H