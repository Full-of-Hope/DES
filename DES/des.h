#include<iostream>
#include<vector>
#include<bitset>
#include<graphics.h>
#include<string>
#include<algorithm>

extern const int PC_1[56];//��Կ�û���64->56
extern const int PC_2[48];//��Կѹ���û���56��>48
extern const int move[16];//��Կÿ��ѭ������λ��
extern const int ip[64];//��ʼ�û���
extern const int e_table[48];//��չ�û���E�������ݴ�32λ��չΪ48λ
extern const int ip_1[64];//���ʼ�û���
extern const int p[32];//P���û�
extern const int sbox[8][64];//S����

std::vector<int>getkey(int n, std::vector<int>);//��ȡ��n����Կ
std::vector<int> decimalToBinary(int num);//���s�д��������
std::vector <int>o_ead(std::vector<int>content, const std::vector<int>key);//һ�ּӽ���
std::vector<int>o_exchange(std::vector<int>content);//��ʼ�û�
std::vector<int>exchange(std::vector<int> content);//��16�ֺ����ҽ���
std::vector<int>r_exchange(std::vector<int> content);//���ʼ�û�
std::vector<int>progress(std::vector<int> content, std::vector<std::vector<int>> key, int choice);//���ܻ����
std::vector<int> string_binary(const std::string& s);//�ַ���->������
std::string binary_string(const std::vector<int>& bin);//������->�ַ���
std::vector<int> hex_binary(const std::string& hex);//ʮ�����ơ�>������
std::string binary_Hex(const std::vector<int>& bin);//�����ơ�>ʮ������