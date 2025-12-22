#ifndef BALANCE_H
#define BALANCE_H

#include <time.h>

// 账户状态枚举（与全局定义保持一致）
typedef enum {
    ACCOUNT_ACTIVE,
    ACCOUNT_FROZEN,
    ACCOUNT_LOCKED
} AccountStatus;

// 用户结构体（与全局定义保持一致）
typedef struct {
    int user_id;                // 用户ID
    char username[16];          // 用户名 (3-15字符)
    char password[21];          // 密码 (6-20字符)
    double balance;             // 账户余额（非负）
    AccountStatus status;       // 账户状态
    int failed_attempts;        // 登录失败次数
    time_t lock_until;          // 锁定截止时间
} User;

// 全局变量外部声明（复用主程序全局数据）
extern User users[];
extern int user_count;

// 余额查询核心函数
double query_balance(int user_id);

// 辅助函数声明
int find_user_by_id(int user_id);

// 测试相关枚举（错误码定义）
typedef enum {
    QUERY_SUCCESS = 0,          // 查询成功
    QUERY_USER_NOT_EXIST = -1,  // 用户不存在
    QUERY_ACCOUNT_INVALID = -2  // 账户状态异常（冻结/锁定）
} QueryBalanceErrorCode;

// 测试函数声明 - 各类覆盖测试
void test_equivalence_classes();          // 等价类测试
void test_all_definition_coverage();      // 全定义覆盖测试
void test_all_computation_usage();        // 全计算使用覆盖测试
void test_all_predicate_usage();          // 全谓词使用覆盖测试
void test_all_usage_coverage();           // 全使用覆盖测试
void test_all_def_use_coverage();         // 全定义-使用覆盖测试

// 辅助：初始化测试环境
void init_test_environment();

#endif // BALANCE_H