#include<iostream>
#include<vector>
#include<bitset>
#include<graphics.h>
#include<string>
#include<algorithm>

extern const int PC_1[56];//密钥置换表：64->56
extern const int PC_2[48];//密钥压缩置换表：56—>48
extern const int move[16];//密钥每轮循环左移位数
extern const int ip[64];//初始置换表
extern const int e_table[48];//扩展置换表E，将数据从32位扩展为48位
extern const int ip_1[64];//逆初始置换表
extern const int p[32];//P盒置换
extern const int sbox[8][64];//S盒子

std::vector<int>getkey(int n, std::vector<int>);//获取第n轮密钥
std::vector<int> decimalToBinary(int num);//针对s盒处理进制数
std::vector <int>o_ead(std::vector<int>content, const std::vector<int>key);//一轮加解密
std::vector<int>o_exchange(std::vector<int>content);//初始置换
std::vector<int>exchange(std::vector<int> content);//第16轮后左右交换
std::vector<int>r_exchange(std::vector<int> content);//逆初始置换
std::vector<int>progress(std::vector<int> content, std::vector<std::vector<int>> key, int choice);//加密或解密
std::vector<int> string_binary(const std::string& s);//字符串->二进制
std::string binary_string(const std::vector<int>& bin);//二进制->字符串
std::vector<int> hex_binary(const std::string& hex);//十六进制—>二进制
std::string binary_Hex(const std::vector<int>& bin);//二进制—>十六进制