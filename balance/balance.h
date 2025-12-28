#ifndef BALANCE_H
#define BALANCE_H

#include "../common.h" // 确保路径正确，指向包含User定义的头文件

// ======================
// 余额查询错误码
// ======================
typedef enum {
    QUERY_SUCCESS = 0,          // 查询成功
    QUERY_USER_NOT_EXIST = -1,  // 用户不存在
    QUERY_ACCOUNT_INVALID = -2  // 账户状态异常（冻结/锁定）
} QueryBalanceErrorCode;

// ======================
// 核心功能声明
// ======================
double query_balance(int user_id);

// ======================
// 测试套件声明
// ======================
void init_test_environment();
void test_equivalence_classes();          // 全量测试集合
void test_all_definition_coverage();      // 全定义覆盖
void test_all_computation_usage();        // 全计算使用覆盖
void test_all_predicate_usage();          // 全谓词使用覆盖
void test_all_usage_coverage();           // 全使用覆盖
void test_all_def_use_coverage();         // 全定义-使用覆盖

#endif // BALANCE_H