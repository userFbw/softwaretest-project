#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "register/register.h"
#include "login/login.h"
#include "balance/balance.h"
#include "transfer/transfer.h"
#include "common.h"

// 全局变量定义
User users[MAX_USERS];
Transaction transactions[MAX_TRANSACTIONS];
int user_count = 0;
int transaction_count = 0;
int next_user_id = 1;
int next_transaction_id = 1;

// 当前登录用户信息
static int current_user_id = -1;

// 函数声明
void display_main_menu();
void register_new_user();
void login_user_menu();
void display_user_menu();
void query_balance_menu();
void transfer_money_menu();
void logout_user();
void wait_for_enter();
void clear_screen();
int get_int_input(const char* prompt);

int main() {
    int choice;
    
    
    clear_screen();
    
    printf("\n=================================\n");
    printf("   银行账户管理系统 v1.0\n");
    printf("=================================\n");
    
    while (1) {
        display_main_menu();
        choice = get_int_input("请选择操作");
        
        switch (choice) {
            case 1:
                register_new_user();
                wait_for_enter();
                clear_screen();
                break;
            case 2:
                login_user_menu();
                wait_for_enter();
                clear_screen();
                break;
            case 3:
                printf("感谢使用银行账户管理系统，再见！\n");
                exit(0);
            case 4:
                printf("开始调用系统内置测试函数\n");
                int key = get_int_input("请输入测试函数编号 (1-注册用户, 2-用户登录, 3-查询余额, 4-转账)");
                switch (key){
                    case 1:
                        test_register_user();
                        wait_for_enter();
                        clear_screen();
                        break;
                    case 2:
                        //test_login_user();
                        //test_equivalence_cases();
                        //test_decision_table_cases();
                        test_path_cases();
                        wait_for_enter();
                        clear_screen();
                        break;
                    case 3:
                        //test_query_balance();
                        test_equivalence_classes();
                        wait_for_enter();
                        clear_screen();
                        break;
                    case 4:
                        test_transfer_money();
                        wait_for_enter();
                        clear_screen();
                        break;
                }
                break;
            case 5:
                printf("银行系统基本信息:\n");
                printf("当前注册用户数: %d\n", user_count);
                printf("当前交易记录数: %d\n", transaction_count);
                break;
            default:
                printf("无效选择，请重新输入！\n");
                wait_for_enter();
                clear_screen();
        }
    }
    
    return 0;
}

// 简单清屏：打印空行
void clear_screen() {
    for (int i = 0; i < 30; i++) {
        printf("\n");
    }
}

// 等待回车键
void wait_for_enter() {
    printf("\n按回车键继续...");
    // 清除缓冲区
    int c;
    //while ((c = getchar()) != '\n' && c != EOF) {}
    getchar(); // 等待用户按回车
}

// 安全的整数输入函数
int get_int_input(const char* prompt) {
    char input[100];
    int result;
    
    printf("\n%s: ", prompt);
    fflush(stdout);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("输入错误！\n");
        return -1;
    }
    
    // 移除换行符
    input[strcspn(input, "\n")] = '\0';
    
    if (sscanf(input, "%d", &result) == 1) {
        return result;
    } else {
        printf("无效输入，请输入数字！\n");
        return -1;
    }
}

// 显示主菜单
void display_main_menu() {
    printf("\n============ 主菜单 ============\n");
    printf("1. 注册新用户\n");
    printf("2. 用户登录\n");
    printf("3. 退出系统\n");
    printf("4. 运行测试函数\n");
    printf("5. 银行系统基本信息\n");
    printf("=================================\n");
}

// 注册新用户
void register_new_user() {
    char username[16];
    char password[21];
    char confirm_password[21];
    
    printf("\n========== 用户注册 ==========\n");
    
    printf("请输入用户名 (3-15字符): ");
    fflush(stdout);
    if (fgets(username, sizeof(username), stdin) == NULL) {
        printf("输入错误！\n");
        return;
    }
    username[strcspn(username, "\n")] = '\0';
    
    printf("请输入密码 (6-20字符, 包含字母和数字): ");
    fflush(stdout);
    if (fgets(password, sizeof(password), stdin) == NULL) {
        printf("输入错误！\n");
        return;
    }
    password[strcspn(password, "\n")] = '\0';
    
    printf("请确认密码: ");
    fflush(stdout);
    if (fgets(confirm_password, sizeof(confirm_password), stdin) == NULL) {
        printf("输入错误！\n");
        return;
    }
    confirm_password[strcspn(confirm_password, "\n")] = '\0';
    
    int result = register_user(username, password, confirm_password);
    if (result > 0) {
        printf("\n注册成功！您的用户ID是: %d\n", result);
    } else {
        printf("\n注册失败，请检查输入信息。\n");
    }
}

// 用户登录菜单
void login_user_menu() {
    char username[16];
    char password[21];
    
    printf("\n========== 用户登录 ==========\n");
    
    printf("请输入用户名: ");
    fflush(stdout);
    if (fgets(username, sizeof(username), stdin) == NULL) {
        printf("输入错误！\n");
        return;
    }
    username[strcspn(username, "\n")] = '\0';
    
    printf("请输入密码: ");
    fflush(stdout);
    if (fgets(password, sizeof(password), stdin) == NULL) {
        printf("输入错误！\n");
        return;
    }
    password[strcspn(password, "\n")] = '\0';
    
    int result = login_user(username, password);
    if (result > 0) {
        clear_screen();
        current_user_id = result;
        printf("\n登录成功！欢迎回来，%s\n", username);
        display_user_menu();
    } else {
        printf("\n登录失败，用户名或密码错误。\n");
    }
}

// 显示用户菜单
void display_user_menu() {
    int choice;
    
    while (current_user_id != -1) {
        //clear_screen();
        
        printf("\n========== 用户菜单 ==========\n");
        int index = find_user_by_id(current_user_id);
        if (index != -1) {
            printf("当前用户: %s (ID: %d)\n", users[index].username, current_user_id);
        }
        printf("1. 查询余额\n");
        printf("2. 转账\n");
        printf("3. 查看账户信息\n");
        printf("4. 退出登录\n");
        printf("==============================\n");
        
        choice = get_int_input("请选择操作");
        
        switch (choice) {
            case 1:
                query_balance_menu();
                wait_for_enter();
                break;
            case 2:
                transfer_money_menu();

                wait_for_enter();
                break;
            case 3:
                {
                    int index = find_user_by_id(current_user_id);
                    if (index != -1) {
                        print_user(&users[index]);
                        wait_for_enter();
                    }
                }
                break;
            case 4:
                logout_user();
                return;
            default:
                printf("无效选择，请重新输入！\n");
                wait_for_enter();
        }
    }
}

// 查询余额菜单
void query_balance_menu() {
    double balance = query_balance(current_user_id);
    if (balance >= 0) {
        printf("\n您的当前余额为: %.2f\n", balance);
    } else {
        printf("查询余额失败。\n");
    }
}

// 转账菜单
void transfer_money_menu() {
    int to_user_id;
    double amount;
    
    printf("\n========== 转账操作 ==========\n");
    
    to_user_id = get_int_input("请输入收款人用户ID");
    if (to_user_id == -1) {
        printf("无效的用户ID！\n");
        return;
    }
    
    printf("请输入转账金额: ");
    fflush(stdout);
    if (scanf("%lf", &amount) != 1) {
        printf("无效输入！\n");
        // 清除输入缓冲区
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {}
        return;
    }
    // 清除输入缓冲区
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
    
    int result = transfer_money(current_user_id, to_user_id, amount);
    if (result == 1) {
        printf("\n转账成功！\n");
        printf("您的当前余额为: %.2f\n", query_balance(current_user_id));
    } else {
        printf("\n转账失败，错误代码: %d\n", result);
    }
}

// 退出登录
void logout_user() {
    printf("\n用户 %d 已退出登录。\n", current_user_id);
    current_user_id = -1;
}