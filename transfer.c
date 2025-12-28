#include "transfer.h"

// 桩模块：根据输入的 ID 模拟不同的查找结果
int find_user_by_id(int id) {
    if (id == 99) return -1;  // 场景：模拟用户不存在
    if (id == 1)   return 0;   // 场景：返回 FromUser 的索引
    if (id == 2)   return 1;   // 场景：返回 ToUser 的索引
    if (id == 3)   return 2;   // 场景：返回 FrozenUser 的索引
    if (id == 4)   return 3;   // 场景：返回 LockedUser 的索引
    return -1;
}

int transfer_money(int from_user_id, int to_user_id, double amount) {
	
    // 1. 检查双方用户是否存在
    int Fid = find_user_by_id(from_user_id);
    int Tid = find_user_by_id(to_user_id);
    if(Fid == -1 || Tid == -1)
	{
		printf("转出ID %d 或转入ID %d 不存在\n",from_user_id,to_user_id);
		return -1;
    }
    
    // 2. 检查账户状态
    User* user1 = &users[Fid];
    User* user2 = &users[Tid];
    const char* status_str1;
    const char* status_str2;
 	switch (user1->status) 
	{
	 case ACCOUNT_ACTIVE: status_str1 = "正常"; break;
	 case ACCOUNT_FROZEN: status_str1 = "冻结"; break;
	 case ACCOUNT_LOCKED: status_str1 = "锁定"; break;
	}
	 switch (user2->status) 
	{
	 case ACCOUNT_ACTIVE: status_str2 = "正常"; break;
	 case ACCOUNT_FROZEN: status_str2 = "冻结"; break;
	 case ACCOUNT_LOCKED: status_str2 = "锁定"; break;
	}
	printf("转出账户状态：%s，转入账户状态：%s\n",status_str1,status_str2);
	if(user1->status != ACCOUNT_ACTIVE || user2->status != ACCOUNT_ACTIVE)
	{
	 	printf("账户状态异常，无法转账\n");
		return -2; 
	}
	 
    // 3. 检查转账金额有效性（100-50000元）
    if(amount < 100 || amount > 50000)
	{
		printf("转账金额无效\n");
		return -3;
	}   
    
    // 4. 检查转出账户余额
    if(user1->balance < amount)
    {
    	printf("余额不足\n");
    	return -4;
	}
    
    // 5. 执行转账
    user1->balance -= amount; // 转出方扣除转账金额
    user2->balance += amount; // 转入方增加转账金额
    
    // 6. 记录交易
	if (transaction_count < MAX_TRANSACTIONS) {
        Transaction new_trans;
        new_trans.transaction_id = next_transaction_id++;
        new_trans.from_user_id = from_user_id;
        new_trans.to_user_id = to_user_id;
        new_trans.amount = amount;
        new_trans.type = TRANSFER;
        new_trans.transaction_time = time(NULL); // 记录当前时间
        
        transactions[transaction_count++] = new_trans;
        
        // 可选：打印交易详情以确认记录成功
        printf("交易记录成功 (ID: %d)\n", new_trans.transaction_id);
    } 
    else {
        printf("警告：交易记录数组已满，无法记录本次转账\n");
    }
    
    
    printf("转账成功\n");
	return 1;
}


// 辅助函数：初始化转账测试所需的账户
void setup_transfer_test_users() {
    // 重置全局状态
    user_count = 0;
    next_user_id = 1;
    transaction_count = 0;
    next_transaction_id = 2;

    // 1. 转出账户 (ID: 1, U: FromUser, Balance: 25000, Status: ACTIVE) - 默认余额 25000
    if (user_count < MAX_USERS) {
        users[user_count].user_id = next_user_id++; // ID: 1
        strcpy(users[user_count].username, "FromUser");
        strcpy(users[user_count].password, "Pwd12345");
        users[user_count].status = ACCOUNT_ACTIVE;
        users[user_count].balance = 25000.0;
        users[user_count].failed_attempts = 0;
        users[user_count].lock_until = 0;
        user_count++;
    }

    // 2. 转入账户 (ID: 2, U: ToUser, Balance: 0, Status: ACTIVE)
    if (user_count < MAX_USERS) {
        users[user_count].user_id = next_user_id++; // ID: 2
        strcpy(users[user_count].username, "ToUser");
        strcpy(users[user_count].password, "Pwd12345");
        users[user_count].status = ACCOUNT_ACTIVE;
        users[user_count].balance = 0.0;
        users[user_count].failed_attempts = 0;
        users[user_count].lock_until = 0;
        user_count++;
    }
    
    // 3. 冻结账户 (ID: 3, U: FrozenUser, Balance: 50000, Status: FROZEN)
    if (user_count < MAX_USERS) {
        users[user_count].user_id = next_user_id++; // ID: 3
        strcpy(users[user_count].username, "FrozenUser");
        strcpy(users[user_count].password, "Pwd12345");
        users[user_count].status = ACCOUNT_FROZEN;
        users[user_count].balance = 0;
        users[user_count].failed_attempts = 0;
        users[user_count].lock_until = 0;
        user_count++;
    }
    
    // 4. 锁定账户 (ID: 4, U: LockedUser, Balance: 50000, Status: LOCKED)
    if (user_count < MAX_USERS) {
        users[user_count].user_id = next_user_id++; // ID: 4
        strcpy(users[user_count].username, "LockedUser");
        strcpy(users[user_count].password, "Pwd12345");
        users[user_count].status = ACCOUNT_LOCKED;
        users[user_count].balance = 0;
        users[user_count].failed_attempts = 0;
        users[user_count].lock_until = 0;
        user_count++;
    }
}


//测试函数
void test_transfer_money() {
    printf("\n===== 测试资金转账功能 =====\n");
    
    setup_transfer_test_users(); // 初始化测试用户
    
    const int FROM_ID = 1; // 默认余额 25000
    const int TO_ID = 2;   // 默认余额 0
    const int FROZEN_ID = 3; // 状态 冻结
    const int LOCKED_ID = 4; // 状态 锁定
    const int NON_EXISTENT_ID = 99; // 不存在的ID

    // 获取用户索引
    int from_index = find_user_by_id(FROM_ID);
    int to_index = find_user_by_id(TO_ID);

    User* from = &users[from_index];
    User* to = &users[to_index];

    int result;
    
    // 辅助宏：重置余额和状态，用于每次独立测试
    #define RESET_ACCOUNT_STATUS \
        from->balance = 25000.0; \
        from->status = ACCOUNT_ACTIVE; \
        to->balance = 0.0; \
        to->status = ACCOUNT_ACTIVE; \
    printf("\n--- 账户余额/状态已重置：From: %.2f, To: %.2f ---\n");

    printf("\n------------ 集成测试 ------------\n");


    RESET_ACCOUNT_STATUS
    result = transfer_money(FROM_ID, TO_ID, 25000); 
    printf("测试用例:1,期望:1 | 结果 %d, From余额 %.2f\n", result, from->balance);

    RESET_ACCOUNT_STATUS
    result = transfer_money(NON_EXISTENT_ID, TO_ID, 25000); 
    printf("测试用例:2,期望:-2 | 结果 %d, From余额 %.2f\n", result, from->balance);

    RESET_ACCOUNT_STATUS
    result = transfer_money(FROZEN_ID, TO_ID, 25000); 
    printf("测试用例:3,期望:-2 | 结果 %d, From余额 %.2f\n", result, from->balance);

    RESET_ACCOUNT_STATUS
    result = transfer_money(LOCKED_ID, TO_ID, 25000); 
    printf("测试用例:4,期望:1 | 结果 %d, From余额 %.2f\n", result, from->balance);
 
    RESET_ACCOUNT_STATUS
    result = transfer_money(FROM_ID, TO_ID, 50); 
    printf("测试用例:5,期望:-2 | 结果 %d, From余额 %.2f\n", result, from->balance);

    RESET_ACCOUNT_STATUS
    result = transfer_money(FROM_ID, TO_ID, 60000); 
    printf("测试用例:6,期望:-2 | 结果 %d, From余额 %.2f\n", result, from->balance);

    RESET_ACCOUNT_STATUS
    result = transfer_money(FROM_ID, TO_ID, 30000); 
    printf("测试用例:7,期望:-1 | 结果 %d, From余额 %.2f\n", result, from->balance);

    RESET_ACCOUNT_STATUS
    transaction_count = 1000;
    result = transfer_money(FROM_ID, TO_ID, 25000); 
    printf("测试用例:8,期望:1 | 结果 %d, From余额 %.2f\n", result, from->balance);
   
    printf("\n=====================================\n");
    #undef RESET_ACCOUNT_STATUS
}