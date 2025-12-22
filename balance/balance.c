#include "balance.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

// 全局变量（复用主程序定义，此处仅声明）
extern User users[];
extern int user_count;

// 辅助函数：通过ID查找用户索引
int find_user_by_id(int user_id) {
    for (int i = 0; i < user_count; i++) {
        if (users[i].user_id == user_id) {
            return i;
        }
    }
    return -1; // 未找到
}

// 余额查询核心实现
double query_balance(int user_id) {
    // 1. 定义1：user_index（用户索引）
    int user_index = find_user_by_id(user_id);

    // 谓词1：用户是否存在
    if (user_index == -1) {
        printf("错误：用户ID %d 不存在\n", user_id);
        return QUERY_USER_NOT_EXIST; // 定义使用1：返回不存在错误
    }

    // 定义2：当前用户指针
    User* current_user = &users[user_index];

    // 谓词2：账户是否激活
    if (current_user->status != ACCOUNT_ACTIVE) {
        const char* status_str = "未知";
        // 谓词3：账户状态分支（计算使用：status值）
        switch (current_user->status) {
        case ACCOUNT_FROZEN: status_str = "冻结"; break;
        case ACCOUNT_LOCKED: status_str = "锁定"; break;
        }
        printf("错误：账户状态为 %s，无法查询余额\n", status_str);
        return QUERY_ACCOUNT_INVALID; // 定义使用2：返回账户异常错误
    }

    // 谓词4：余额是否为负（计算使用：balance值）
    if (current_user->balance < 0) {
        printf("警告：账户余额异常 (%.2f)\n", current_user->balance);
    }

    // 定义使用3：返回有效余额（计算使用：balance值）
    printf("用户 %d 的余额查询成功: %.2f\n", user_id, current_user->balance);
    return current_user->balance;
}

// 辅助：初始化测试环境（创建测试用户）
void init_test_environment() {
    user_count = 0; // 清空原有数据

    // 测试用户1：正常激活，余额正数（ID=1001）
    users[user_count++] = (User){
        .user_id = 1001, .username = "test_active_pos",
        .status = ACCOUNT_ACTIVE, .balance = 500.00, .failed_attempts = 0
    };

    // 测试用户2：正常激活，余额负数（ID=1002）
    users[user_count++] = (User){
        .user_id = 1002, .username = "test_active_neg",
        .status = ACCOUNT_ACTIVE, .balance = -200.00, .failed_attempts = 0
    };

    // 测试用户3：冻结状态（ID=1003）
    users[user_count++] = (User){
        .user_id = 1003, .username = "test_frozen",
        .status = ACCOUNT_FROZEN, .balance = 800.00, .failed_attempts = 0
    };

    // 测试用户4：锁定状态（ID=1004）
    users[user_count++] = (User){
        .user_id = 1004, .username = "test_locked",
        .status = ACCOUNT_LOCKED, .balance = 1000.00, .failed_attempts = 3
    };

    // 测试用户5：正常激活，余额0（ID=1005）
    users[user_count++] = (User){
        .user_id = 1005, .username = "test_active_zero",
        .status = ACCOUNT_ACTIVE, .balance = 0.00, .failed_attempts = 0
    };

    printf("测试环境初始化完成，创建 %d 个测试用户\n", user_count);
}

// 1. 全定义覆盖测试（覆盖所有变量定义点）
void test_all_definition_coverage() {
    printf("\n=====================================\n");
    printf("          全定义覆盖测试\n");
    printf("=====================================\n");
    init_test_environment();

    double result;
    // 目标：覆盖query_balance中所有变量定义点
    // 定义1：user_index（用户索引）- 覆盖"存在"和"不存在"两种定义
    // 定义2：current_user（用户指针）- 覆盖所有状态的用户
    // 定义3：balance（余额）- 覆盖正/负/零值

    // TC1：覆盖user_index=-1（用户不存在）
    printf("\n[TC1] 定义点1：user_index=-1（用户ID=9999）\n");
    result = query_balance(9999);
    printf("结果：%.2f (期望：-1) → %s\n", result, result == -1 ? "通过" : "失败");

    // TC2：覆盖current_user（激活+余额正）
    printf("\n[TC2] 定义点2：current_user（激活+余额正，ID=1001）\n");
    result = query_balance(1001);
    printf("结果：%.2f (期望：500.00) → %s\n", result, result == 500.00 ? "通过" : "失败");

    // TC3：覆盖current_user->balance（负数）
    printf("\n[TC3] 定义点3：balance=-200（激活+余额负，ID=1002）\n");
    result = query_balance(1002);
    printf("结果：%.2f (期望：-200.00) → %s\n", result, result == -200.00 ? "通过" : "失败");

    // TC4：覆盖current_user->status（冻结）
    printf("\n[TC4] 定义点2：current_user（冻结状态，ID=1003）\n");
    result = query_balance(1003);
    printf("结果：%.2f (期望：-2) → %s\n", result, result == -2 ? "通过" : "失败");

    // TC5：覆盖current_user->balance（零值）
    printf("\n[TC5] 定义点3：balance=0（激活+余额零，ID=1005）\n");
    result = query_balance(1005);
    printf("结果：%.2f (期望：0.00) → %s\n", result, result == 0.00 ? "通过" : "失败");

    printf("\n全定义覆盖测试完成\n");
}

// 2. 全计算使用覆盖测试（覆盖所有变量的计算使用场景）
void test_all_computation_usage() {
    printf("\n=====================================\n");
    printf("        全计算使用覆盖测试\n");
    printf("=====================================\n");
    init_test_environment();

    double result;
    // 目标：覆盖所有变量的"计算使用"（参与运算/判断/输出）
    // 计算使用1：user_index（参与==判断）
    // 计算使用2：current_user->status（参与!=判断、switch分支）
    // 计算使用3：current_user->balance（参与<判断、输出、返回）

    // TC1：user_index计算使用（不存在）
    printf("\n[TC1] 计算使用1：user_index=-1（ID=9999）\n");
    result = query_balance(9999);
    printf("覆盖：user_index==-1 判断 → %s\n", result == -1 ? "通过" : "失败");

    // TC2：status计算使用（冻结）
    printf("\n[TC2] 计算使用2：status=FROZEN（ID=1003）\n");
    result = query_balance(1003);
    printf("覆盖：status!=ACTIVE + switch(FROZEN) → %s\n", result == -2 ? "通过" : "失败");

    // TC3：status计算使用（锁定）
    printf("\n[TC3] 计算使用2：status=LOCKED（ID=1004）\n");
    result = query_balance(1004);
    printf("覆盖：status!=ACTIVE + switch(LOCKED) → %s\n", result == -2 ? "通过" : "失败");

    // TC4：balance计算使用（负数）
    printf("\n[TC4] 计算使用3：balance<0（ID=1002）\n");
    result = query_balance(1002);
    printf("覆盖：balance<0 判断 + 输出 + 返回 → %s\n", result == -200.00 ? "通过" : "失败");

    // TC5：balance计算使用（正数）
    printf("\n[TC5] 计算使用3：balance>0（ID=1001）\n");
    result = query_balance(1001);
    printf("覆盖：balance>=0 + 输出 + 返回 → %s\n", result == 500.00 ? "通过" : "失败");

    // TC6：balance计算使用（零值）
    printf("\n[TC6] 计算使用3：balance=0（ID=1005）\n");
    result = query_balance(1005);
    printf("覆盖：balance==0 + 输出 + 返回 → %s\n", result == 0.00 ? "通过" : "失败");

    printf("\n全计算使用覆盖测试完成\n");
}

// 3. 全谓词使用覆盖测试（覆盖所有谓词的真/假分支）
void test_all_predicate_usage() {
    printf("\n=====================================\n");
    printf("        全谓词使用覆盖测试\n");
    printf("=====================================\n");
    init_test_environment();

    double result;
    // 目标：覆盖所有谓词的TRUE/FALSE分支
    // 谓词1：user_index == -1 （T/F）
    // 谓词2：current_user->status != ACCOUNT_ACTIVE （T/F）
    // 谓词3：current_user->balance < 0 （T/F）

    // TC1：谓词1=T（用户不存在）
    printf("\n[TC1] 谓词1=T：user_index==-1（ID=9999）\n");
    result = query_balance(9999);
    printf("覆盖：user_index==-1 → %s\n", result == -1 ? "通过" : "失败");

    // TC2：谓词1=F + 谓词2=F + 谓词3=F（激活+余额正）
    printf("\n[TC2] 谓词1=F + 谓词2=F + 谓词3=F（ID=1001）\n");
    result = query_balance(1001);
    printf("覆盖：status=ACTIVE + balance>=0 → %s\n", result == 500.00 ? "通过" : "失败");

    // TC3：谓词1=F + 谓词2=T（冻结）
    printf("\n[TC3] 谓词1=F + 谓词2=T（冻结，ID=1003）\n");
    result = query_balance(1003);
    printf("覆盖：status!=ACTIVE → %s\n", result == -2 ? "通过" : "失败");

    // TC4：谓词1=F + 谓词2=F + 谓词3=T（激活+余额负）
    printf("\n[TC4] 谓词1=F + 谓词2=F + 谓词3=T（ID=1002）\n");
    result = query_balance(1002);
    printf("覆盖：balance<0 → %s\n", result == -200.00 ? "通过" : "失败");

    printf("\n全谓词使用覆盖测试完成\n");
}

// 4. 全使用覆盖测试（覆盖定义+谓词+计算使用）
void test_all_usage_coverage() {
    printf("\n=====================================\n");
    printf("          全使用覆盖测试\n");
    printf("=====================================\n");
    init_test_environment();

    double result;
    // 目标：覆盖所有"定义-使用"对 + 所有谓词/计算使用
    // 覆盖维度：定义使用（返回值）、谓词使用（判断）、计算使用（运算/输出）

    // TC1：定义1使用（user_index=-1）+ 谓词1=T
    printf("\n[TC1] 定义1使用 + 谓词1=T（ID=9999）\n");
    result = query_balance(9999);
    printf("覆盖：user_index定义+谓词判断+返回值使用 → %s\n", result == -1 ? "通过" : "失败");

    // TC2：定义2使用（激活+余额正）+ 谓词2=F + 谓词3=F + 计算使用（返回余额）
    printf("\n[TC2] 定义2使用 + 谓词2/F + 谓词3/F（ID=1001）\n");
    result = query_balance(1001);
    printf("覆盖：current_user定义+status判断+balance计算 → %s\n", result == 500.00 ? "通过" : "失败");

    // TC3：定义2使用（冻结）+ 谓词2=T + 计算使用（switch分支）
    printf("\n[TC3] 定义2使用 + 谓词2=T（冻结，ID=1003）\n");
    result = query_balance(1003);
    printf("覆盖：current_user定义+status判断+switch计算 → %s\n", result == -2 ? "通过" : "失败");

    // TC4：定义2使用（激活+余额负）+ 谓词2=F + 谓词3=T + 计算使用（警告输出）
    printf("\n[TC4] 定义2使用 + 谓词3=T（ID=1002）\n");
    result = query_balance(1002);
    printf("覆盖：current_user定义+balance判断+警告输出 → %s\n", result == -200.00 ? "通过" : "失败");

    // TC5：定义2使用（锁定）+ 谓词2=T + 计算使用（switch分支）
    printf("\n[TC5] 定义2使用 + 谓词2=T（锁定，ID=1004）\n");
    result = query_balance(1004);
    printf("覆盖：current_user定义+status判断+switch计算 → %s\n", result == -2 ? "通过" : "失败");

    printf("\n全使用覆盖测试完成\n");
}

// 5. 全定义-使用覆盖测试（覆盖所有定义点到使用点的路径）
void test_all_def_use_coverage() {
    printf("\n=====================================\n");
    printf("        全定义-使用覆盖测试\n");
    printf("=====================================\n");
    init_test_environment();

    double result;
    // 目标：覆盖所有"定义点→使用点"的路径
    // 定义1（user_index）→ 使用点：谓词1判断、返回值
    // 定义2（current_user）→ 使用点：谓词2判断、switch分支、谓词3判断、输出、返回值
    // 定义3（balance）→ 使用点：谓词3判断、输出、返回值

    // 路径1：定义1 → 谓词1=T → 返回-1
    printf("\n[TC1] 路径1：定义1→谓词1=T→返回-1（ID=9999）\n");
    result = query_balance(9999);
    printf("覆盖：user_index定义→判断→返回 → %s\n", result == -1 ? "通过" : "失败");

    // 路径2：定义1→谓词1=F → 定义2→谓词2=T（冻结）→ 返回-2
    printf("\n[TC2] 路径2：定义1→F→定义2→谓词2=T（冻结）→返回-2（ID=1003）\n");
    result = query_balance(1003);
    printf("覆盖：user_index→current_user→status判断→返回 → %s\n", result == -2 ? "通过" : "失败");

    // 路径3：定义1→F → 定义2→谓词2=T（锁定）→ 返回-2
    printf("\n[TC3] 路径3：定义1→F→定义2→谓词2=T（锁定）→返回-2（ID=1004）\n");
    result = query_balance(1004);
    printf("覆盖：user_index→current_user→status判断→返回 → %s\n", result == -2 ? "通过" : "失败");

    // 路径4：定义1→F → 定义2→谓词2=F → 定义3→谓词3=T → 返回balance
    printf("\n[TC4] 路径4：定义1→F→定义2→F→定义3→T→返回balance（ID=1002）\n");
    result = query_balance(1002);
    printf("覆盖：user_index→current_user→balance判断→返回 → %s\n", result == -200.00 ? "通过" : "失败");

    // 路径5：定义1→F → 定义2→谓词2=F → 定义3→谓词3=F → 返回balance
    printf("\n[TC5] 路径5：定义1→F→定义2→F→定义3→F→返回balance（ID=1001）\n");
    result = query_balance(1001);
    printf("覆盖：user_index→current_user→balance判断→返回 → %s\n", result == 500.00 ? "通过" : "失败");

    // 路径6：定义1→F → 定义2→谓词2=F → 定义3（零值）→ 谓词3=F → 返回balance
    printf("\n[TC6] 路径6：定义1→F→定义2→F→定义3（零）→F→返回balance（ID=1005）\n");
    result = query_balance(1005);
    printf("覆盖：user_index→current_user→balance(0)→返回 → %s\n", result == 0.00 ? "通过" : "失败");

    printf("\n全定义-使用覆盖测试完成\n");
}

// 等价类测试（扩展为批量调用所有测试函数）
void test_equivalence_classes() {
    printf("\n=====================================\n");
    printf("        余额查询全量测试套件\n");
    printf("=====================================\n");

    // 2. 全定义覆盖测试
    printf("\n---------- 1. 全定义覆盖测试 ----------\n");
    test_all_definition_coverage();

    // 3. 全计算使用覆盖测试
    printf("\n---------- 2. 全计算使用覆盖测试 ----------\n");
    test_all_computation_usage();

    // 4. 全谓词使用覆盖测试
    printf("\n---------- 3. 全谓词使用覆盖测试 ----------\n");
    test_all_predicate_usage();

    // 5. 全使用覆盖测试
    printf("\n---------- 4. 全使用覆盖测试 ----------\n");
    test_all_usage_coverage();

    // 6. 全定义-使用覆盖测试
    printf("\n---------- 5. 全定义-使用覆盖测试 ----------\n");
    test_all_def_use_coverage();

    printf("\n=====================================\n");
    printf("        所有测试执行完成\n");
    printf("=====================================\n");
}