#ifndef BALANCE_H
#define BALANCE_H

// 查询余额函数声明
double query_balance(int user_id);

// 测试相关函数声明
void test_basis_path();
void set_user_balance(int user_id, double balance);
void set_user_status(int user_id, int status);
void test_equivalence_classes();

#endif