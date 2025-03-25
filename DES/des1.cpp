#include"des.h"
//密钥置换表：64->56
const int PC_1[56] = {
	57 , 49 , 41 , 33 , 25 , 17 , 9 ,
	1 ,58 , 50 , 42 , 34 , 26 , 18 ,
	10 , 2 ,59 , 51 , 43 , 35 , 27 ,
	19 , 11 , 3 ,60 , 52 , 44 , 36 ,
	63 , 55 , 47 , 39 ,31 , 23 , 15 ,
	7 , 62 , 54 , 46 , 38 ,30 , 22 ,
	14 , 6 , 61 , 53 , 45 , 37 ,29 ,
	21 , 13 , 5 , 28 , 20 , 12 , 4
};
//密钥压缩置换表：56—>48
const int PC_2[48] = {
	14 , 17 , 11 , 24 , 1 , 5 ,
	3 , 28 ,15 , 6 , 21 , 10 ,
	23 , 19 , 12 , 4 ,26 , 8 ,
	16 , 7 , 27 , 20 , 13 , 2 ,
	41 , 52 , 31 , 37 , 47 , 55 ,
	30 , 40 ,51 , 45 , 33 , 48 ,
	44 , 49 , 39 , 56 ,34 , 53 ,
	46 , 42 , 50 , 36 , 29 , 32
};
//密钥每轮循环左移位数
const int move[16] = {
	1 , 1 , 2 , 2 , 2 , 2 , 2 , 2 , 1 , 2 , 2 , 2 , 2 , 2 , 2 , 1
};
//初始置换表
const int ip[64] = {
	58 , 50 , 42 , 34 , 26 , 18 , 10 , 2 , 60 , 52 , 44 , 36 , 28 , 20 , 12 , 4 ,
	62 , 54 , 46 , 38 , 30 , 22 , 14 , 6 , 64 , 56 , 48 , 40 , 32 , 24 , 16 , 8 ,
	57 , 49 , 41 , 33 , 25 , 17 , 9 , 1 , 59 , 51 , 43 , 35 , 27 , 19 , 11 , 3 ,
	61 , 53 , 45 , 37 , 29 , 21 , 13 , 5 , 63 , 55 , 47 , 39 , 31 , 23 , 15 , 7
};
//扩展置换表E，将数据从32位扩展为48位
const int e_table[48] = {
	32 , 1 , 2 , 3 , 4 , 5 , 4 , 5 , 6 , 7 , 8 , 9 , 8 , 9 , 10 , 11 ,
	12 , 13 , 12 , 13 , 14 , 15 , 16 , 17 , 16 , 17 , 18 , 19 , 20 , 21 , 20 , 21 ,
	22 , 23 , 24 , 25 , 24 , 25 , 26 , 27 , 28 , 29 , 28 , 29 , 30 , 31 , 32 , 1
};
//逆初始置换表
const int ip_1[64] = {
	40 , 8 , 48 , 16 , 56 , 24 , 64 , 32 , 39 , 7 , 47 , 15 , 55 , 23 , 63 , 31 ,
	38 , 6 , 46 , 14 , 54 , 22 , 62 , 30 , 37 , 5 , 45 , 13 , 53 , 21 , 61 , 29 ,
	36 , 4 , 44 , 12 , 52 , 20 , 60 , 28 , 35 , 3 , 43 , 11 , 51 , 19 , 59 , 27 ,
	34 , 2 , 42 , 10 , 50 , 18 , 58 , 26 , 33 , 1 , 41 , 9 , 49 , 17 , 57 , 25
};
//P盒置换
const int p[32] = {
	16 , 7 , 20 , 21 , 29 , 12 , 28 , 17 , 1 , 15 , 23 , 26 , 5 , 18 , 31 , 10 ,
	2 , 8 , 24 , 14 , 32 , 27 , 3 , 9 , 19 , 13 , 30 , 6 , 22 , 11 , 4 , 25
};
//S盒子
const int sbox[8][64] = {
		{//S1盒子
	14 , 4 , 13 , 1 , 2 , 15 , 11 , 8 , 3 , 10 , 6 , 12 , 5 , 9 , 0 , 7 ,
		0 , 15 , 7 , 4 , 14 , 2 , 13 , 1 , 10 , 6 , 12 , 11 , 9 , 5 , 3 , 8 ,
		4 , 1 , 14 , 8 , 13 , 6 , 2 , 11 , 15 , 12 , 9 , 7 , 3 , 10 , 5 , 0 ,
		15 , 12 , 8 , 2 , 4 , 9 , 1 , 7 , 5 , 11 , 3 , 14 , 10 , 0 , 6 , 13
} ,
	{//S2盒子
		15 , 1 , 8 , 14 , 6 , 11 , 3 , 4 , 9 , 7 , 2 , 13 , 12 , 0 , 5 , 10 ,
		3 , 13 , 4 , 7 , 15 , 2 , 8 , 14 , 12 , 0 , 1 , 10 , 6 , 9 , 11 , 5 ,
		0 , 14 , 7 , 11 , 10 , 4 , 13 , 1 , 5 , 8 , 12 , 6 , 9 , 3 , 2 , 15 ,
		13 , 8 , 10 , 1 , 3 , 15 , 4 , 2 , 11 , 6 , 7 , 12 , 0 , 5 , 14 , 9
} ,
	{//S3盒子
		10 , 0 , 9 , 14 , 6 , 3 , 15 , 5 , 1 , 13 , 12 , 7 , 11 , 4 , 2 , 8 ,
		13 , 7 , 0 , 9 , 3 , 4 , 6 , 10 , 2 , 8 , 5 , 14 , 12 , 11 , 15 , 1 ,
		13 , 6 , 4 , 9 , 8 , 15 , 3 , 0 , 11 , 1 , 2 , 12 , 5 , 10 , 14 , 7 ,
		1 , 10 , 13 , 0 , 6 , 9 , 8 , 7 , 4 , 15 , 14 , 3 , 11 , 5 , 2 , 12
} ,
	{//S4盒子
		7 , 13 , 14 , 3 , 0 , 6 , 9 , 10 , 1 , 2 , 8 , 5 , 11 , 12 , 4 , 15 ,
		13 , 8 , 11 , 5 , 6 , 15 , 0 , 3 , 4 , 7 , 2 , 12 , 1 , 10 , 14 , 9 ,
		10 , 6 , 9 , 0 , 12 , 11 , 7 , 13 , 15 , 1 , 3 , 14 , 5 , 2 , 8 , 4 ,
		3 , 15 , 0 , 6 , 10 , 1 , 13 , 8 , 9 , 4 , 5 , 11 , 12 , 7 , 2 , 14
} ,
		{//S5盒子
	2 , 12 , 4 , 1 , 7 , 10 , 11 , 6 , 8 , 5 , 3 , 15 , 13 , 0 , 14 , 9 ,
		14 , 11 , 2 , 12 , 4 , 7 , 13 , 1 , 5 , 0 , 15 , 10 , 3 , 9 , 8 , 6 ,
		4 , 2 , 1 , 11 , 10 , 13 , 7 , 8 , 15 , 9 , 12 , 5 , 6 , 3 , 0 , 14 ,
		11 , 8 , 12 , 7 , 1 , 14 , 2 , 13 , 6 , 15 , 0 , 9 , 10 , 4 , 5 , 3
} ,
		{//S6盒子
	12 , 1 , 10 , 15 , 9 , 2 , 6 , 8 , 0 , 13 , 3 , 4 , 14 , 7 , 5 , 11 ,
		10 , 15 , 4 , 2 , 7 , 12 , 9 , 5 , 6 , 1 , 13 , 14 , 0 , 11 , 3 , 8 ,
		9 , 14 , 15 , 5 , 2 , 8 , 12 , 3 , 7 , 0 , 4 , 10 , 1 , 13 , 11 , 6 ,
		4 , 3 , 2 , 12 , 9 , 5 , 15 , 10 , 11 , 14 , 1 , 7 , 6 , 0 , 8 , 13
} ,
	{//S7盒子
		4 , 11 , 2 , 14 , 15 , 0 , 8 , 13 , 3 , 12 , 9 , 7 , 5 , 10 , 6 , 1 ,
		13 , 0 , 11 , 7 , 4 , 9 , 1 , 10 , 14 , 3 , 5 , 12 , 2 , 15 , 8 , 6 ,
		1 , 4 , 11 , 13 , 12 , 3 , 7 , 14 , 10 , 15 , 6 , 8 , 0 , 5 , 9 , 2 ,
		6 , 11 , 13 , 8 , 1 , 4 , 10 , 7 , 9 , 5 , 0 , 15 , 14 , 2 , 3 , 12
} ,
	{//S8盒子
		13 , 2 , 8 , 4 , 6 , 15 , 11 , 1 , 10 , 9 , 3 , 14 , 5 , 0 , 12 , 7 ,
		1 , 15 , 13 , 8 , 10 , 3 , 7 , 4 , 12 , 5 , 6 , 11 , 0 , 14 , 9 , 2 ,
		7 , 11 , 4 , 1 , 9 , 12 , 14 , 2 , 0 , 6 , 10 , 13 , 15 , 3 , 5 , 8 ,
		2 , 1 , 14 , 7 , 4 , 10 , 8 , 13 , 15 , 12 , 9 , 0 , 3 , 5 , 6 , 11
}
};
//获取第n轮密钥
std::vector<int> getkey(int n, std::vector<int> key) {
	std::vector<int> nkey(48);
	std::vector<int> temp(56);
	// 初始PC-1置换
	for (int i = 0; i < 56; i++) {
		temp[i] = key[PC_1[i] - 1];
	}
	// 计算总移位次数（前n-1轮的移位总和）
	int total_shift = 0;
	for (int i = 0; i < n; i++) { // n从1开始，对应索引0到n-1
		total_shift += move[i];
	}
	// 循环左移
	for (int j = 0; j < total_shift; j++) {
		int temp1 = temp[0];
		int temp2 = temp[28];
		for (int a = 0; a < 27; a++) {
			temp[a] = temp[a + 1];
			temp[a + 28] = temp[a + 29];
		}
		temp[27] = temp1;
		temp[55] = temp2;
	}
	// PC-2置换生成48位子密钥
	for (int i = 0; i < 48; i++) {
		nkey[i] = temp[PC_2[i] - 1];
	}
	return nkey;
}
std::vector<int> decimalToBinary(int num) {//S盒处理，进制转换
	std::bitset<4> bits(num);
	std::vector<int> binaryArray;
	for (int i = 0; i < bits.size(); ++i) {
		binaryArray.push_back(bits[i]);
	}
	return binaryArray;//从低往高：12->0011
}
std::vector<int>o_exchange(std::vector<int> content) {//初始置换
	std::vector<int>temp(64);
	for (int i = 0; i < 64; i++) {
		temp[i] = content[ip[i] - 1];
	}
	return temp;
}
//每轮加解密
std::vector <int>o_ead(std::vector<int>content, const std::vector<int>key) {//48位的密钥
	std::vector<int>left(32);
	std::vector<int>right(32);
	std::vector<int>result(64);
	for (int i = 0; i < 32; i++) {
		left[i] = content[i];//32位
		right[i] = content[i + 32];//32位
	}
	std::vector<int>right1(48);
	for (int j = 0; j < 48; j++) {
		right1[j] = right[e_table[j] - 1];
	}//right1--48位
	for (int j = 0; j < 48; j++) {//right与密钥进行异或
		right1[j] ^= key[j];
	}
	std::vector<int> temp(32);
	for (int i = 0; i < 8; i++) {//通过S盒
		int row = right1[0 + 6 * i] * 2 + right1[6 * i + 5];//行号
		int col = right1[6 * i + 1] * 8 + right1[6 * i + 2] * 4 + right1[6 * i + 3] * 2 + right1[6 * i + 4];//列号
		int sdata = sbox[i][row * 16 + col];
		std::vector<int> temp1 = decimalToBinary(sdata);
		for (int j = 0; j < 4; j++) {
			temp[j + 4 * i] = temp1[3 - j];
		}
	}
	for (int i = 0; i < 32; i++) {//P置换
		right[i] = temp[p[i] - 1];
	}
	for (int j = 0; j < 32; j++) {//right与left进行异或
		right[j] ^= left[j];
	}
	for (int i = 0; i < 32; i++) {
		result[i] = content[i + 32];
		result[i + 32] = right[i];
	}
	return result;
}
std::vector<int>exchange(std::vector<int> content) {//第16轮后左右交换
	for (int j = 0; j < 32; j++) {
		int temp = content[j];
		content[j] = content[j + 32];
		content[j + 32] = temp;
	}
	return content;
}
std::vector<int>r_exchange(std::vector<int> content) {//逆初始置换
	std::vector<int>temp(64);
	for (int i = 0; i < 64; i++) {
		temp[i] = content[ip_1[i] - 1];
	}
	return temp;
}
std::vector<int>progress(std::vector<int> content, std::vector<std::vector<int>> key, int choice) {//加密或解密
	std::vector<int>temp1 = o_exchange(content);
	switch (choice) {
	case 1:
	{
		for (int i = 0; i < 16; i++) {
			temp1 = o_ead(temp1, key[i]);
		}
		return r_exchange(exchange(temp1));
		break;
	}
	case 0:
		for (int i = 15; i >= 0; i--) {
			temp1 = o_ead(temp1, key[i]);
		}
		return r_exchange(exchange(temp1));
		break;
	default:
		break;
	}
}
std::string binary_string(const std::vector<int>& bin) {//二进制->字符串
	std::string s;
	for (int i = 0; i < bin.size() && i < 64; i += 8) {
		std::bitset<8> bits;
		for (int j = 0; j < 8 && i + j < bin.size(); ++j) {
			bits[7 - j] = bin[i + j];
		}
		s += char(bits.to_ulong());
	}
	return s;
}
std::vector<int> string_binary(const std::string& s) {//字符串->二进制
	std::vector<int> bin;
	for (char c : s) {
		std::bitset<8> bits(c);
		for (int i = 7; i >= 0; --i) {
			bin.push_back(bits[i]);
		}
	}
	return bin;
}
std::vector<int> hex_binary(const std::string& hex) {//十六进制->二进制
	std::vector<int> bin;
	for (char c : hex) {
		int val = isdigit(c) ? c - '0' : toupper(c) - 'A' + 10;
		for (int i = 3; i >= 0; --i)
			bin.push_back((val >> i) & 1);
	}
	while (bin.size() < 64) bin.push_back(0);
	return bin;
}
std::string binary_Hex(const std::vector<int>& bin) {//二进制->十六进制
	std::string hex;
	for (int i = 0; i < 64; i += 4) {
		int val = 0;
		for (int j = 0; j < 4; ++j)
			val = (val << 1) | bin[i + j];
		hex += "0123456789ABCDEF"[val];
	}
	return hex;
}