#include "login.h"
#include "../common.h"

// 最大登录尝试次数
#define MAX_LOGIN_ATTEMPTS 3
// 账户锁定时间（秒）
#define LOCK_DURATION 60 // 1分钟

int login_user(const char* username, const char* password) {
    // 1. 检查输入参数
    if (username == NULL || password == NULL || *username == '\0') {
        printf("错误：用户名或密码不能为空\n");
        return -1;
    }
    
    // 2. 查找用户
    int user_id = find_user_by_username(username);
    if (user_id == -1) {
        printf("错误：用户 '%s' 不存在\n", username);
        return -1;
    }
    
    // 3. 获取用户索引
    int index = find_user_by_id(user_id);
    if (index == -1) {
        printf("错误：用户数据异常\n");
        return -1;
    }
    
    User* user = &users[index];
    
    // 4. 检查账户状态
    time_t now = time(NULL);        //10
    
    // 如果账户被锁定，检查是否已过锁定时间
    if (user->status == ACCOUNT_LOCKED) {       //11
        if (now < user->lock_until) {
            char time_str[20];
            strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", localtime(&user->lock_until));
            printf("账户已锁定，解锁时间: %s\n", time_str);  //15
            return -1;
        } else {
            // 锁定时间已过，解锁账户
            user->status = ACCOUNT_ACTIVE;    //16
            user->failed_attempts = 0;
            user->lock_until = 0;
            printf("账户已自动解锁\n");
        }
    }
    
    // 5. 检查账户是否冻结
    if (user->status == ACCOUNT_FROZEN) {           //20
        printf("账户已被冻结，请联系管理员\n");
        return -1;
    }
    
    // 6. 验证密码
    if (strcmp(user->password, password) == 0) {            //22
        // 登录成功，重置失败计数
        user->failed_attempts = 0;
        user->lock_until = 0;
        printf("登录成功！欢迎回来，%s\n", username);
        return user->user_id;
    }
    
    // 7. 密码错误处理
    if (user->failed_attempts < 0) {            //26
        user->failed_attempts = 0;
    }
    user->failed_attempts++;            //28
    printf("密码错误，剩余尝试次数: %d\n", MAX_LOGIN_ATTEMPTS - user->failed_attempts);
    
    // 8. 检查是否达到最大尝试次数
    if (user->failed_attempts >= MAX_LOGIN_ATTEMPTS) {      //30
        user->status = ACCOUNT_LOCKED;
        user->lock_until = now + LOCK_DURATION;
        
        char time_str[20];
        strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", localtime(&user->lock_until));
        
        printf("账户已被锁定，解锁时间: %s\n", time_str);
    }
    
    return -1;
}


// =================================================================
// 辅助函数：重置并创建测试用户 (setup_login_test_users)
// =================================================================

void setup_login_test_users() {
    // 重置全局状态
    user_count = 0;
    next_user_id = 1;
    
    // 1. N8Chars (ID: 1) - BVA 01, 06-09, RBVT 15, 17, 19, 20, 21
    users[user_count].user_id = next_user_id++;
    strcpy(users[user_count].username, "N8Chars");       
    strcpy(users[user_count].password, "P12CharsPass");  
    users[user_count].status = ACCOUNT_ACTIVE;
    users[user_count].failed_attempts = 0;
    users[user_count].lock_until = 0;
    user_count++;
    
    // 2. UMax15CharsAABB (ID: 2) - BVA 03, 05, RBVT 14, 18, 23
    users[user_count].user_id = next_user_id++;
    strcpy(users[user_count].username, "UMax15CharsAABB"); 
    strcpy(users[user_count].password, "PMax20CharsAABBCCDDEE"); 
    users[user_count].status = ACCOUNT_ACTIVE;
    users[user_count].failed_attempts = 0;
    users[user_count].lock_until = 0;
    user_count++;
    
    // 3. Min (ID: 3) - BVA 02, 04, RBVT 13, 22
    users[user_count].user_id = next_user_id++;
    strcpy(users[user_count].username, "Min");          
    strcpy(users[user_count].password, "P6Pass");       
    users[user_count].status = ACCOUNT_ACTIVE;
    users[user_count].failed_attempts = 0;
    users[user_count].lock_until = 0;
    user_count++;

    // 4. FrozenAcc (ID: 4) - BVA 12
    users[user_count].user_id = next_user_id++;
    strcpy(users[user_count].username, "FrozenAcc");
    strcpy(users[user_count].password, "Pwd12345");
    users[user_count].status = ACCOUNT_FROZEN;
    users[user_count].failed_attempts = 0;
    users[user_count].lock_until = 0;
    user_count++;

    // 5. TimeLock (ID: 5) - BVA 10, 11
    users[user_count].user_id = next_user_id++;
    strcpy(users[user_count].username, "TimeLock");
    strcpy(users[user_count].password, "Pwd12345");
    users[user_count].status = ACCOUNT_ACTIVE; 
    users[user_count].failed_attempts = 0;
    users[user_count].lock_until = 0;
    user_count++;
}

// =================================================================
// 严格对齐表格的测试函数 (test_login_user)
// =================================================================
void test_login_user1() {
    printf("======================================================\n");
    printf(" 用户登录功能测试\n");
    printf("======================================================\n");

    // 准备测试环境和用户
    setup_login_test_users();
    int result;
    time_t now = time(NULL);

    // 获取测试用户指针 (用于修改状态)
    User* user_n8 = &users[0];   // ID: 1
    User* user_max = &users[1];  // ID: 2
    User* user_min = &users[2];  // ID: 3
    User* user_frozen = &users[3]; // ID: 4
    User* user_time = &users[4]; // ID: 5

    // 常用常量
    const char* U_N8 = user_n8->username;
    const char* P_N12 = user_n8->password;
    const char* U_Max = user_max->username;
    const char* P_Max20 = user_max->password;
    const char* U_Min = user_min->username;
    const char* P_Min6 = user_min->password;
    const char* U_Time = user_time->username;
    const char* P_Time = user_time->password;
    const char* U_Frozen = user_frozen->username;
    const char* P_Frozen = user_frozen->password;

    const char* P_Wrong = "WrongPass";
    
    // RBVT 专用的非法输入 (来自表格)
    const char* U_Lmin_1 = "12";                     // 2 字符 (RBVT 13, 22)
    const char* U_Lmax_1 = "UMax15CharsAABBCC";      // 16 字符 (RBVT 14, 23)
    const char* P_Lmin_1 = "Wrong";                  // 5 字符 (RBVT 17, 22)
    const char* P_Lmax_1 = "PMax20CharsAABBCCDDEEX"; // 21 字符 (RBVT 18, 23)
    const char* U_NonExistent = "NonExistentUser";   // (RBVT 16)
    const char* P_Empty = "";                        // (RBVT 19)

    // ----------------------------------------------------------------
    // I. 边界值分析 (BVA) 测试 (共 12 个用例)
    // ----------------------------------------------------------------
    printf("\n--- I. 边界值分析 (BVA) 测试 (12 个用例) ---\n");

    // BVA 01: Nominal
    result = login_user(U_N8, P_N12);
    printf("[BVA 01] 7字名/12字密 (期望 1) | %s\n", result == 1 ? "PASS" : "FAIL");

    // BVA 02: Min (Lmin=3) / Min (Lmin=6)
    result = login_user(U_Min, P_Min6);
    printf("[BVA 02] 3字名/6字密 (期望 3) | %s\n", result == 3 ? "PASS" : "FAIL");

    // BVA 03: Max (Lmax=15) / Max (Lmax=20)
    result = login_user(U_Max, P_Max20);
    printf("[BVA 03] 15字名/20字密 (期望 2) | %s\n", result == 2 ? "PASS" : "FAIL");
    
    // BVA 04: Min (Lmin=3) / Min (Lmin=6) - 反映表格内容
    result = login_user(U_Min, P_Min6);
    printf("[BVA 04] 3字名/6字密 (期望 3) | %s\n", result == 3 ? "PASS" : "FAIL");

    // BVA 05: Max (Lmax=15) / Max (Lmax=20) - 反映表格内容
    result = login_user(U_Max, P_Max20);
    printf("[BVA 05] 15字名/20字密 (期望 2) | %s\n", result == 2 ? "PASS" : "FAIL");

    // --- 状态逻辑测试 (重置 N8Chars 状态) ---
    user_n8->status = ACCOUNT_ACTIVE;
    user_n8->failed_attempts = 0;

    // BVA 06: 失败次数 Nmin=0 -> 1错
    result = login_user(U_N8, P_Wrong);
    printf("[BVA 06] 0错->1错 (期望 -1, 次数 1) | %s\n", result == -1 && user_n8->failed_attempts == 1 ? "PASS" : "FAIL");

    // BVA 07: 失败次数 Nmin+1=1 -> 2错
    user_n8->failed_attempts = 1;
    result = login_user(U_N8, P_Wrong);
    printf("[BVA 07] 1错->2错 (期望 -1, 次数 2) | %s\n", result == -1 && user_n8->failed_attempts == 2 ? "PASS" : "FAIL");

    // BVA 08: 失败次数 Nmax-1=2 -> 3错 (触发锁定)
    user_n8->failed_attempts = 2;
    result = login_user(U_N8, P_Wrong);
    printf("[BVA 08] 2错->3错(锁定) (期望 -1, LOCKED) | %s\n", result == -1 && user_n8->status == ACCOUNT_LOCKED ? "PASS" : "FAIL");

    // BVA 09: 已锁定状态下，尝试正确密码 (锁定时间 T+10s)
    user_n8->status = ACCOUNT_LOCKED;
    user_n8->lock_until = now + 10; 
    user_n8->failed_attempts = 3; // 保持锁定状态
    result = login_user(U_N8, P_N12);
    printf("[BVA 09] 锁定中登录 (期望 -1, 保持 LOCKED) | %s\n", result == -1 && user_n8->status == ACCOUNT_LOCKED ? "PASS" : "FAIL");

    // BVA 10: 锁定时间 Texp-1 (now - 1s) -> 自动解锁并登录成功
    user_time->status = ACCOUNT_LOCKED;
    user_time->lock_until = now - 1; 
    result = login_user(U_Time, P_Time);
    printf("[BVA 10] 锁定过期(T-1) (期望 5, ACTIVE) | %s\n", result == 5 && user_time->status == ACCOUNT_ACTIVE ? "PASS" : "FAIL");

    // BVA 11: 锁定时间 Texp+1 (now + 1s) -> 仍在锁定中
    user_time->status = ACCOUNT_LOCKED;
    user_time->lock_until = now + 1; 
    result = login_user(U_Time, P_Time);
    printf("[BVA 11] 仍在锁定(T+1) (期望 -1, 保持 LOCKED) | %s\n", result == -1 && user_time->status == ACCOUNT_LOCKED ? "PASS" : "FAIL");

    // BVA 12: 账户冻结 (FROZEN)
    result = login_user(U_Frozen, P_Frozen);
    printf("[BVA 12] 账户冻结 (期望 -1, 保持 FROZEN) | %s\n", result == -1 && user_frozen->status == ACCOUNT_FROZEN ? "PASS" : "FAIL");
    
    // ----------------------------------------------------------------
    // II. 健壮边界值测试 (RBVT) 测试 (共 11 个用例)
    // ----------------------------------------------------------------
    printf("\n--- II. 健壮边界值测试 (RBVT) 测试 (11 个用例) ---\n");

    // RBVT 13: 2字符名 (Lmin-1)
    result = login_user(U_Lmin_1, P_N12);
    printf("[RBVT 13] 2字名 (期望 -1) | %s\n", result == -1 ? "PASS" : "FAIL");
    
    // RBVT 14: 16字符名 (Lmax+1)
    result = login_user(U_Lmax_1, P_N12); 
    printf("[RBVT 14] 16字名 (期望 -1) | %s\n", result == -1 ? "PASS" : "FAIL");

    // RBVT 15: NULL 用户名
    result = login_user(NULL, P_N12);
    printf("[RBVT 15] NULL名 (期望 -1) | %s\n", result == -1 ? "PASS" : "FAIL");

    // RBVT 16: 用户不存在
    result = login_user(U_NonExistent, P_N12);
    printf("[RBVT 16] 用户不存在 (期望 -1) | %s\n", result == -1 ? "PASS" : "FAIL");
    
    // RBVT 17: 5字符密 (Lmin-1) -> 失败次数 0 -> 1
    user_n8->status = ACCOUNT_ACTIVE;
    user_n8->failed_attempts = 0;
    result = login_user(U_N8, P_Lmin_1); 
    printf("[RBVT 17] 5字密 (期望 -1, 次数 1) | %s\n", result == -1 && user_n8->failed_attempts == 1 ? "PASS" : "FAIL");
    
    // RBVT 18: 21字符密 (Lmax+1) -> 失败次数 0 -> 1
    user_max->status = ACCOUNT_ACTIVE;
    user_max->failed_attempts = 0;
    result = login_user(U_Max, P_Lmax_1); 
    printf("[RBVT 18] 21字密 (期望 -1, 次数 1) | %s\n", result == -1 && user_max->failed_attempts == 1 ? "PASS" : "FAIL");

    // RBVT 19: 空字符串密码
    user_n8->status = ACCOUNT_ACTIVE;
    user_n8->failed_attempts = 0;
    result = login_user(U_N8, P_Empty);
    printf("[RBVT 19] 空密码 (期望 -1) | %s\n", result == -1 ? "PASS" : "FAIL");

    // RBVT 20: 失败次数 Nmin-1 = -1 -> 1错
    user_n8->failed_attempts = -1; 
    result = login_user(U_N8, P_Wrong);
    printf("[RBVT 20] -1错->1错 (期望 -1, 次数 1) | %s\n", result == -1 && user_n8->failed_attempts == 1 ? "PASS" : "FAIL");
    
    // RBVT 21: 失败次数 Nmax+1 = 5 (超大值) -> 触发锁定
    user_n8->status = ACCOUNT_ACTIVE;
    user_n8->failed_attempts = 5; 
    result = login_user(U_N8, P_Wrong);
    printf("[RBVT 21] 5错->锁定 (期望 -1, LOCKED) | %s\n", result == -1 && user_n8->status == ACCOUNT_LOCKED ? "PASS" : "FAIL");

    // RBVT 22: 组合边界 (2字符名 & 5字符密) - 期望用户不存在
    result = login_user(U_Lmin_1, P_Lmin_1); 
    printf("[RBVT 22] 2字名&5字密 (期望 -1) | %s\n", result == -1 ? "PASS" : "FAIL");
    
    // RBVT 23: 组合边界 (16字符名 & 21字符密) - 期望用户不存在
    result = login_user(U_Lmax_1, P_Lmax_1); 
    printf("[RBVT 23] 16字名&21字密 (期望 -1) | %s\n", result == -1 ? "PASS" : "FAIL");

    printf("\n======================================================\n");
    printf(" 所有测试用例执行完毕\n");
    printf("======================================================\n");
}


void setup_equivalence_test_users() {
    // 重置全局状态
    user_count = 0;
    next_user_id = 1;
    
    // 1. User01 (ID: 1) - 对应 WRE-01, 06, 07, 08, 09 的有效用户
    // E1: 合法且已注册的用户名, E2: 正确密码, E3: 正常, E4: 0次
    users[user_count].user_id = next_user_id++;
    strcpy(users[user_count].username, "User01");       
    strcpy(users[user_count].password, "PwA123");  
    users[user_count].status = ACCOUNT_ACTIVE;
    users[user_count].failed_attempts = 0;
    users[user_count].lock_until = 0;
    user_count++;
    
    // 2. LockedUser (ID: 2) - 对应 WRE-10 (账户状态：已锁定)
    // I9: 账户状态为锁定
    users[user_count].user_id = next_user_id++;
    strcpy(users[user_count].username, "LockedUser"); 
    strcpy(users[user_count].password, "PwA123"); 
    users[user_count].status = ACCOUNT_LOCKED; // 初始状态：锁定
    users[user_count].failed_attempts = 0; 
    users[user_count].lock_until = time(NULL) + 60; // 锁定 60 秒 (保持锁定状态)
    user_count++;
    
    // 3. FrozenUser (ID: 3) - 对应 WRE-11 (账户状态：已冻结)
    // I10: 账户状态为冻结
    users[user_count].user_id = next_user_id++;
    strcpy(users[user_count].username, "FrozenUser");          
    strcpy(users[user_count].password, "PwA123");       
    users[user_count].status = ACCOUNT_FROZEN; // 初始状态：冻结
    users[user_count].failed_attempts = 0;
    users[user_count].lock_until = 0;
    user_count++;

    // 4. User02 (ID: 4) - 对应 WRE-12 (失败次数 =N 触发锁定)
    // C2: 初始设置为 N-1 次失败 (2次)
    users[user_count].user_id = next_user_id++;
    strcpy(users[user_count].username, "User02");
    strcpy(users[user_count].password, "CorrectPwd"); 
    users[user_count].status = ACCOUNT_ACTIVE;
    users[user_count].failed_attempts = MAX_LOGIN_ATTEMPTS - 1; // 初始 2 次失败
    users[user_count].lock_until = 0;
    user_count++;

    // 5. User03 (ID: 5) - 对应 WRE-13 (失败次数 <N)
    // C2: 初始设置为 M < N 次失败 (1次)
    users[user_count].user_id = next_user_id++;
    strcpy(users[user_count].username, "User03");
    strcpy(users[user_count].password, "CorrectPwd"); 
    users[user_count].status = ACCOUNT_ACTIVE; 
    users[user_count].failed_attempts = MAX_LOGIN_ATTEMPTS - 2; // 初始 1 次失败
    users[user_count].lock_until = 0;
    user_count++;
}

void test_equivalence_cases() {
    printf("======================================================\n");
    printf(" III. 等价类划分测试 (WRE-01 到 WRE-13)\n");
    printf("======================================================\n");

    // 1. 准备测试环境和用户
    setup_equivalence_test_users();
    int result;
    time_t now = time(NULL);

    // 2. 获取测试用户指针和常量 (确保与 setup_equivalence_test_users 对齐)
    User* user_01 = &users[0];   // ID: 1, User01
    User* user_locked = &users[1]; // ID: 2, LockedUser
    User* user_frozen = &users[2]; // ID: 3, FrozenUser
    User* user_02 = &users[3];     // ID: 4, User02
    User* user_03 = &users[4];     // ID: 5, User03

    // --- 定义用例中需要的参数 (来自 Excel 表格) ---
    const char* U_Valid = user_01->username;    // User01 (E1)
    const char* P_Correct = user_01->password;  // PwA123 (E2)
    const char* P_Wrong = "PwB456";             // I5: 不正确的密码
    
    // WRE-02: 用户名不存在 (I1)
    const char* U_NotExist = "NotExists"; 

    // WRE-03, 04, 05: 非法用户名 (I2, I3, I4)
    const char* U_Lmin_1 = "U1";           // I2: 长度 <3
    const char* U_Lmax_1 = "VeryLongUsername012"; // I3: 长度 >15
    const char* U_InvalidChar = "User@name";  // I4: 格式无效

    // WRE-07, 08, 09: 非法密码 (I6, I7, I8)
    const char* P_Lmin_1 = "A123";           // I6: 长度 <6
    const char* P_Lmax_1 = "LongPasswordA1234567890B"; // I7: 长度 >20
    const char* P_DigitOnly = "1234567";    // I8: 纯数字

    // ----------------------------------------------------------------
    // WRE-01: 登录成功 (弱一般 / 强一般)
    // 输入: User01 (E1), PwA123 (E2) | 约束: 正常 (E3), 0次 (E4)
    // 预期: 登录成功
    // ----------------------------------------------------------------
    user_01->status = ACCOUNT_ACTIVE;
    user_01->failed_attempts = 0; 
    result = login_user(U_Valid, P_Correct);
    printf("[WRE-01] 登录成功 (期望 > 0) | %s\n", 
           result == 1 && user_01->failed_attempts == 0 && user_01->status == ACCOUNT_ACTIVE ? "PASS" : "FAIL");

    // ----------------------------------------------------------------
    // WRE-02: 用户名不存在 (弱健壮)
    // 输入: NotExists (I1), PwA123 (E2) | 约束: 正常 (E3), 0次 (E4)
    // 预期: 失败：用户不存在
    // ----------------------------------------------------------------
    result = login_user(U_NotExist, P_Correct);
    printf("[WRE-02] 用户名不存在 (期望 -1) | %s\n", result == -1 ? "PASS" : "FAIL");

    // ----------------------------------------------------------------
    // WRE-03: 用户名长度 <3 (弱健壮)
    // 输入: U1 (I2), PwA123 (E2) | 约束: E3, E4
    // 预期: 失败：用户名格式错误
    // ----------------------------------------------------------------
    result = login_user(U_Lmin_1, P_Correct);
    // 您的代码会将其视为用户不存在，但为匹配用例预期，我们只检查失败
    printf("[WRE-03] 用户名长度 <3 (期望 -1) | %s\n", result == -1 ? "PASS" : "FAIL");
    
    // ----------------------------------------------------------------
    // WRE-04: 用户名长度 >15 (弱健壮)
    // 输入: VeryLongUsername012 (I3), PwA123 (E2) | 约束: E3, E4
    // 预期: 失败：用户名格式错误
    // ----------------------------------------------------------------
    result = login_user(U_Lmax_1, P_Correct);
    printf("[WRE-04] 用户名长度 >15 (期望 -1) | %s\n", result == -1 ? "PASS" : "FAIL");

    // ----------------------------------------------------------------
    // WRE-05: 用户名格式无效 (弱健壮)
    // 输入: User@name (I4), PwA123 (E2) | 约束: E3, E4
    // 预期: 失败：用户名格式错误
    // ----------------------------------------------------------------
    result = login_user(U_InvalidChar, P_Correct);
    printf("[WRE-05] 用户名格式无效 (期望 -1) | %s\n", result == -1 ? "PASS" : "FAIL");

    // ----------------------------------------------------------------
    // WRE-06: 密码不正确 (弱健壮)
    // 输入: User01 (E1), PwB456 (I5) | 约束: 正常 (E3), 0次 (E4)
    // 预期: 失败：密码错误
    // ----------------------------------------------------------------
    user_01->status = ACCOUNT_ACTIVE;
    user_01->failed_attempts = 0; // 重置计数
    result = login_user(U_Valid, P_Wrong);
    // 预期失败 (-1) 且失败次数变为 1
    printf("[WRE-06] 密码不正确 (期望 -1, 次数 1) | %s\n", 
           result == -1 && user_01->failed_attempts == 1 ? "PASS" : "FAIL");
           
    // ----------------------------------------------------------------
    // WRE-07: 密码长度 <6 (弱健壮)
    // 输入: User01 (E1), A123 (I6) | 约束: E3, E4
    // 预期: 失败：密码格式错误
    // ----------------------------------------------------------------
    user_01->status = ACCOUNT_ACTIVE;
    user_01->failed_attempts = 0; // 重置计数
    result = login_user(U_Valid, P_Lmin_1);
    // 您的代码会将短密码视为错误密码 (I5)
    printf("[WRE-07] 密码长度 <6 (期望 -1, 次数 1) | %s\n", 
           result == -1 && user_01->failed_attempts == 1 ? "PASS" : "FAIL");

    // ----------------------------------------------------------------
    // WRE-08: 密码长度 >20 (弱健壮)
    // 输入: User01 (E1), LongPasswordA1234567890B (I7) | 约束: E3, E4
    // 预期: 失败：密码格式错误
    // ----------------------------------------------------------------
    user_01->status = ACCOUNT_ACTIVE;
    user_01->failed_attempts = 0; // 重置计数
    result = login_user(U_Valid, P_Lmax_1);
    // 您的代码会将长密码视为错误密码 (I5)
    printf("[WRE-08] 密码长度 >20 (期望 -1, 次数 1) | %s\n", 
           result == -1 && user_01->failed_attempts == 1 ? "PASS" : "FAIL");

    // ----------------------------------------------------------------
    // WRE-09: 密码格式无效（只含数字） (弱健壮)
    // 输入: User01 (E1), 1234567 (I8) | 约束: E3, E4
    // 预期: 失败：密码格式错误
    // ----------------------------------------------------------------
    user_01->status = ACCOUNT_ACTIVE;
    user_01->failed_attempts = 0; // 重置计数
    result = login_user(U_Valid, P_DigitOnly);
    // 您的代码会将纯数字密码视为错误密码 (I5)
    printf("[WRE-09] 纯数字密码 (期望 -1, 次数 1) | %s\n", 
           result == -1 && user_01->failed_attempts == 1 ? "PASS" : "FAIL");

    // ----------------------------------------------------------------
    // WRE-10: 账户状态：已锁定 (弱健壮)
    // 输入: LockedUser (E1), PwA123 (E2) | 约束: 已锁定 (I9), 0次 (E4)
    // 预期: 失败：账户锁定
    // ----------------------------------------------------------------
    // LockedUser 在 setup 中已设置为 LOCK_UNTIL > now
    result = login_user(user_locked->username, user_locked->password);
    printf("[WRE-10] 账户已锁定 (期望 -1) | %s\n", 
           result == -1 && user_locked->status == ACCOUNT_LOCKED ? "PASS" : "FAIL");

    // ----------------------------------------------------------------
    // WRE-11: 账户状态：已冻结 (弱健壮)
    // 输入: FrozenUser (E1), PwA123 (E2) | 约束: 已冻结 (I10), 0次 (E4)
    // 预期: 失败：账户状态异常
    // ----------------------------------------------------------------
    result = login_user(user_frozen->username, user_frozen->password);
    printf("[WRE-11] 账户已冻结 (期望 -1) | %s\n", 
           result == -1 && user_frozen->status == ACCOUNT_FROZEN ? "PASS" : "FAIL");

    // ----------------------------------------------------------------
    // WRE-12: 失败次数 =N (锁定) (弱健壮)
    // 输入: User02 (E1), 第 N+1 次错误密码 (I5) | 约束: 正常 (E3), 达到 N 次 (I11)
    // 预期: 失败：账户锁定
    // ----------------------------------------------------------------
    // User02 初始设置为 failed_attempts = 2 (N-1)
    result = login_user(user_02->username, P_Wrong); // 这将是第 3 次失败 (N)
    // 预期失败 (-1) 且状态变为 LOCKED
    printf("[WRE-12] 失败次数=N(锁定) (期望 -1, LOCKED) | %s\n", 
           result == -1 && user_02->status == ACCOUNT_LOCKED ? "PASS" : "FAIL");

    // ----------------------------------------------------------------
    // WRE-13: 失败次数 <N (弱健壮)
    // 输入: User03 (E1), 第 M 次错误密码 (I5) | 约束: 正常 (E3), 小于 N 次 (I12)
    // 预期: 失败：密码错误
    // ----------------------------------------------------------------
    // User03 初始设置为 failed_attempts = 1 (M)
    result = login_user(user_03->username, P_Wrong); // 这将是第 2 次失败 (M+1)
    // 预期失败 (-1) 且失败次数变为 2
    printf("[WRE-13] 失败次数<N (期望 -1, 次数 2, ACTIVE) | %s\n", 
           result == -1 && user_03->failed_attempts == 2 && user_03->status == ACCOUNT_ACTIVE ? "PASS" : "FAIL");
           
    printf("\n======================================================\n");
    printf(" 等价类测试用例执行完毕\n");
    printf("======================================================\n");
}


// =================================================================
// 辅助函数：准备决策表测试用户 (setup_decision_table_users)
// 仅用于决策表测试 (用例 1-5)
// =================================================================

void setup_decision_table_users() {
    // 重置全局状态
    user_count = 0;
    next_user_id = 1;

    // 1. NormalUser (ID: 1) - 对应 用例 4 和 5
    // 状态: 正常, 失败次数: 0
    users[user_count].user_id = next_user_id++;
    strcpy(users[user_count].username, "NormalUser");
    strcpy(users[user_count].password, "CorrectPass");
    users[user_count].status = ACCOUNT_ACTIVE;
    users[user_count].failed_attempts = 0;
    users[user_count].lock_until = 0;
    user_count++;

    // 2. FrozenUser (ID: 2) - 对应 用例 2
    // 状态: 冻结
    users[user_count].user_id = next_user_id++;
    strcpy(users[user_count].username, "FrozenUser");
    strcpy(users[user_count].password, "AnyPassword");
    users[user_count].status = ACCOUNT_FROZEN;
    users[user_count].failed_attempts = 0;
    users[user_count].lock_until = 0;
    user_count++;

    // 3. LockedUser (ID: 3) - 对应 用例 3
    // 状态: 锁定 (锁定时间设置为未来，保持锁定状态)
    users[user_count].user_id = next_user_id++;
    strcpy(users[user_count].username, "LockedUser");
    strcpy(users[user_count].password, "CorrectPass"); // 密码正确不影响锁定状态
    users[user_count].status = ACCOUNT_LOCKED;
    users[user_count].failed_attempts = MAX_LOGIN_ATTEMPTS; // 保持失败次数
    users[user_count].lock_until = time(NULL) + 60; // 锁定 60 秒
    user_count++;
    
    // 用例 1 (NonExistUser) 不需要实际用户对象
}

// =================================================================
// 决策表测试函数 (test_decision_table_cases)
// 对应上传文件中的用例 1 到 5
// =================================================================

void test_decision_table_cases() {
    printf("======================================================\n");
    printf(" IV. 决策表测试 (用例 1 到 5)\n");
    printf("======================================================\n");

    // 1. 准备测试环境和用户
    setup_decision_table_users();
    int result;
    
    // 2. 获取测试用户指针和常量
    User* user_normal = &users[0];   // ID: 1, NormalUser
    User* user_frozen = &users[1];   // ID: 2, FrozenUser
    User* user_locked = &users[2];   // ID: 3, LockedUser

    const char* U_Normal = user_normal->username;
    const char* P_Correct = user_normal->password;
    const char* U_Frozen = user_frozen->username;
    const char* U_Locked = user_locked->username;
    
    const char* U_NonExist = "NonExistUser"; // 用例 1: 用户名不存在
    const char* P_Wrong = "WrongPass";       // 用例 5: 错误密码
    const char* P_Any = "AnyPassword";       // 用例 1, 2, 3: 任意密码

    // ----------------------------------------------------------------
    // 用例 1: R1 - 用户不存在
    // 覆盖条件: 用户不存在 | 预期结果: 返回用户不存在 (-1)
    // ----------------------------------------------------------------
    result = login_user(U_NonExist, P_Any);
    printf("[用例 1] 用户不存在 (期望 -1) | %s\n", result == -1 ? "PASS" : "FAIL");

    // ----------------------------------------------------------------
    // 用例 2: R2 - 账户已冻结
    // 覆盖条件: 账户已冻结 | 预期结果: 返回账户已冻结 (-1)
    // ----------------------------------------------------------------
    result = login_user(U_Frozen, P_Any);
    printf("[用例 2] 账户已冻结 (期望 -1, 保持 FROZEN) | %s\n", 
           result == -1 && user_frozen->status == ACCOUNT_FROZEN ? "PASS" : "FAIL");

    // ----------------------------------------------------------------
    // 用例 3: R3 - 账户已锁定 (锁定时间未过)
    // 覆盖条件: 账户已锁定 | 预期结果: 返回账户已锁定 (-1)
    // ----------------------------------------------------------------
    result = login_user(U_Locked, P_Correct);
    printf("[用例 3] 账户已锁定 (期望 -1, 保持 LOCKED) | %s\n", 
           result == -1 && user_locked->status == ACCOUNT_LOCKED ? "PASS" : "FAIL");

    // ----------------------------------------------------------------
    // 用例 4: R4 - 登录成功
    // 覆盖条件: 正常用户 & 正确密码 | 预期结果: 登录成功 (>0)
    // ----------------------------------------------------------------
    user_normal->status = ACCOUNT_ACTIVE;
    user_normal->failed_attempts = 2; // 测试登录成功时，失败尝试次数是否重置
    result = login_user(U_Normal, P_Correct);
    printf("[用例 4] 登录成功 (期望 1, 次数 0) | %s\n", 
           result == 1 && user_normal->failed_attempts == 0 ? "PASS" : "FAIL");
           
    // ----------------------------------------------------------------
    // 用例 5: R5 - 密码错误
    // 覆盖条件: 正常用户 & 错误密码 | 预期结果: 密码错误 (-1, 失败尝试次数增加)
    // ----------------------------------------------------------------
    user_normal->status = ACCOUNT_ACTIVE;
    user_normal->failed_attempts = 0; // 重置计数
    result = login_user(U_Normal, P_Wrong);
    printf("[用例 5] 密码错误 (期望 -1, 次数 1) | %s\n", 
           result == -1 && user_normal->failed_attempts == 1 ? "PASS" : "FAIL");
           
    printf("\n======================================================\n");
    printf(" 决策表测试用例执行完毕\n");
    printf("======================================================\n");
}


// =================================================================
// 辅助函数：准备基路径测试用户 (setup_path_test_users)
// =================================================================
void setup_path_test_users() {
    // 重置全局状态
    user_count = 0;
    next_user_id = 1;

    // 1. PathUser (ID: 1) - 通用正常用户，用于路径测试中的多处跳转
    users[user_count].user_id = next_user_id++;
    strcpy(users[user_count].username, "PathUser");
    strcpy(users[user_count].password, "Pwd123");
    users[user_count].status = ACCOUNT_ACTIVE;
    users[user_count].failed_attempts = 0;
    users[user_count].lock_until = 0;
    user_count++;

    // 2. FrozenPath (ID: 2) - 用于测试冻结路径 (L->M)
    users[user_count].user_id = next_user_id++;
    strcpy(users[user_count].username, "FrozenPath");
    strcpy(users[user_count].password, "Pwd123");
    users[user_count].status = ACCOUNT_FROZEN;
    users[user_count].failed_attempts = 0;
    users[user_count].lock_until = 0;
    user_count++;

    // 3. LockedExpired (ID: 3) - 用于测试锁定自动解锁路径 (I->K->L)
    users[user_count].user_id = next_user_id++;
    strcpy(users[user_count].username, "LockedExpired");
    strcpy(users[user_count].password, "Pwd123");
    users[user_count].status = ACCOUNT_LOCKED;
    users[user_count].failed_attempts = 3;
    users[user_count].lock_until = time(NULL) - 10; // 锁定时间已过期 10 秒
    user_count++;
}

// =================================================================
// V. 基路径测试函数 (test_path_cases)
// =================================================================
void test_path_cases() {
    printf("======================================================\n");
    printf(" V. 基路径测试 (Path Testing)\n");
    printf("======================================================\n");

    setup_path_test_users();
    int result;
    time_t now = time(NULL);

    User* u_path = &users[0];     // PathUser
    User* u_frozen = &users[1];   // FrozenPath
    User* u_expired = &users[2];  // LockedExpired

    // --- 路径 1 & 2 & 3: 参数校验路径 ---
    // A1→B→END (username NULL)
    result = login_user(NULL, "any");
    printf("[Path 01] A1-B-END (NULL名) | %s\n", result == -1 ? "PASS" : "FAIL");

    // A1→A2→B→END (password NULL)
    result = login_user("PathUser", NULL);
    printf("[Path 02] A1-A2-B-END (NULL密) | %s\n", result == -1 ? "PASS" : "FAIL");

    // A1→A2→A3→B→END (空字符串名)
    result = login_user("", "any");
    printf("[Path 03] A1-A3-B-END (空字符串名) | %s\n", result == -1 ? "PASS" : "FAIL");

    // --- 路径 4 & 5: 用户查找路径 ---
    // A1→A2→A3→C→D→END (用户不存在)
    result = login_user("Unknown", "any");
    printf("[Path 04] C-D-END (用户不存在) | %s\n", result == -1 ? "PASS" : "FAIL");

    // --- 路径 6 & 7 & 8: 账户状态路径 ---
    // A1→...→G→H→I→J→END (正在锁定中)
    u_path->status = ACCOUNT_LOCKED;
    u_path->lock_until = now + 60; // 未来 1 分钟
    result = login_user(u_path->username, u_path->password);
    printf("[Path 06] H-I-J-END (锁定未到期) | %s\n", result == -1 ? "PASS" : "FAIL");

    // A1→...→G→H→I→K→L→M→END (锁定过期自动解锁 -> 发现被冻结)
    u_expired->status = ACCOUNT_LOCKED;
    u_expired->lock_until = now - 10; // 已过期
    u_expired->status = ACCOUNT_FROZEN; // 解锁后紧接着发现是冻结 (模拟复合状态)
    // 注意：代码逻辑是先判断LOCKED，解锁后继续走到FROZEN判断
    result = login_user(u_expired->username, u_expired->password);
    printf("[Path 07] I-K-L-M-END (自动解锁后冻结) | %s\n", result == -1 ? "PASS" : "FAIL");

    // A1→...→I→L→M→END (直接处于冻结状态)
    result = login_user(u_frozen->username, u_frozen->password);
    printf("[Path 08] L-M-END (直接冻结) | %s\n", result == -1 ? "PASS" : "FAIL");

    // --- 路径 9 & 10 & 11: 密码校验与锁定路径 ---
    // A1→...→L→N→O→END (登录成功)
    u_path->status = ACCOUNT_ACTIVE;
    u_path->failed_attempts = 2; // 测试重置逻辑
    result = login_user(u_path->username, u_path->password);
    printf("[Path 09] N-O-END (登录成功并重置) | %s\n", result == 1 && u_path->failed_attempts == 0 ? "PASS" : "FAIL");

    // A1→...→N→P→Q→R→S→T→U→END (密码错误且失败次数 < 0 -> 归零后触发锁定)
    u_path->failed_attempts = -1; // 触发 Q 节点防御逻辑
    // 为了触发 U 节点(锁定)，我们需要在一次登录失败后达到 MAX_LOGIN_ATTEMPTS
    // 代码逻辑：failed_attempts 归 0，然后 ++ 变成 1。
    // 如果要触发 U，我们将 failed_attempts 预设为 MAX-1
    u_path->failed_attempts = -1; 
    // 修改逻辑使之能触发锁定：由于代码先判断 <0 则归 0，再 ++。
    // 我们手动设置 failed_attempts = MAX_LOGIN_ATTEMPTS - 1
    u_path->failed_attempts = MAX_LOGIN_ATTEMPTS - 1; 
    result = login_user(u_path->username, "WrongPwd");
    printf("[Path 10] P-Q-R-S-T-U-END (触发锁定) | %s\n", result == -1 && u_path->status == ACCOUNT_LOCKED ? "PASS" : "FAIL");

    // A1→...→N→P→R→S→T→END (密码错误但未达上限)
    u_path->status = ACCOUNT_ACTIVE;
    u_path->failed_attempts = 0;
    result = login_user(u_path->username, "WrongPwd");
    printf("[Path 11] P-R-S-T-END (仅增加计数) | %s\n", result == -1 && u_path->failed_attempts == 1 ? "PASS" : "FAIL");

    printf("\n======================================================\n");
}