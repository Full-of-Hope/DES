#include<iostream>
#include"des.h"
using namespace std;

void generate_keys(const vector<int>& key);
void drawMain();
void drawEncrypt();
void drawDecrypt();
void handleMouse(int x, int y);
//界面枚举状态
enum State { MAIN, ENCRYPT, DECRYPT };//主界面、加密、解密
State c_state = MAIN;
//输入状态枚举
enum ActiveInput { TEXT_INPUT, KEY_INPUT };
ActiveInput activeInput = TEXT_INPUT;
//全局变量
vector<vector<int>> key_arr;//密钥组
string inputtext;//输入文本
string resulttext;//输出文本
string encryptKey;//加密密钥
string decryptKey;//解密密钥
string encryptionKey;//比较对象
//按钮
const int BTN_WIDTH = 100;
const int BTN_HEIGHT = 40;
RECT mainEncryptBtn = { 220, 150, 220 + BTN_WIDTH, 150 + BTN_HEIGHT };
RECT mainDecryptBtn = { 220, 250, 220 + BTN_WIDTH, 250 + BTN_HEIGHT };
//加密
RECT e_Input = { 50, 50, 590, 80 };
RECT e_KeyInput = { 50, 130, 590, 160 };
RECT e_Result = { 50, 210, 590, 240 };
//解密
RECT d_Input = { 50, 50, 590, 80 };
RECT d_KeyInput = { 50, 130, 590, 160 };
RECT d_Result = { 50, 210, 590, 240 };
//操作
RECT backBtn = { 50, 300, 150, 340 };
RECT actionBtn = { 450, 300, 550, 340 };

int main() {
    initgraph(640, 480);
    ExMessage message;//鼠标信息

    while (true) {
        if (peekmessage(&message)) {//事件
            if (message.message == WM_LBUTTONDOWN) {//鼠标
                handleMouse(message.x, message.y);
            }
            else if (message.message == WM_CHAR) {//键盘
                if (message.ch == 8) {
                    if (c_state == ENCRYPT) {
                        if (activeInput == TEXT_INPUT && !inputtext.empty())
                            inputtext.pop_back();
                        else if (activeInput == KEY_INPUT && !encryptKey.empty())
                            encryptKey.pop_back();
                    }
                    else if (c_state == DECRYPT) {
                        if (activeInput == TEXT_INPUT && !inputtext.empty())
                            inputtext.pop_back();
                        else if (activeInput == KEY_INPUT && !decryptKey.empty())
                            decryptKey.pop_back();
                    }
                }
                else if (message.ch >= 32 && message.ch <= 126) {
                    if (c_state == ENCRYPT) {
                        if (activeInput == TEXT_INPUT)
                            inputtext += char(message.ch);
                        else if (activeInput == KEY_INPUT && encryptKey.size() < 8)
                            encryptKey += char(message.ch);
                    }
                    else if (c_state == DECRYPT) {
                        if (activeInput == TEXT_INPUT) {
                            char c = toupper(message.ch);
                            if ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F'))
                                inputtext += c;
                        }
                        else if (activeInput == KEY_INPUT && decryptKey.size() < 8)
                            decryptKey += char(message.ch);
                    }
                }
            }
        }
        //批量绘图
        BeginBatchDraw();
        cleardevice();
        switch (c_state) {//状态
        case MAIN:
            drawMain();
            break;
        case ENCRYPT:
            drawEncrypt();
            break;
        case DECRYPT:
            drawDecrypt();
            break;
        }
        EndBatchDraw();
        Sleep(10);
    }
    closegraph();
    return 0;
}

void generate_keys(const vector<int>& key) {//密钥
    key_arr.resize(16);
    for (int i = 0; i < 16; ++i)
        key_arr[i] = getkey(i + 1, key);
}
//主界面
void drawMain() {
    cleardevice();
    setbkmode(TRANSPARENT);
    settextcolor(WHITE);
    //加密
    rectangle(mainEncryptBtn.left, mainEncryptBtn.top, mainEncryptBtn.right, mainEncryptBtn.bottom);
    outtextxy(mainEncryptBtn.left + 20, mainEncryptBtn.top + 10, _T("加密"));
    //解密
    rectangle(mainDecryptBtn.left, mainDecryptBtn.top, mainDecryptBtn.right, mainDecryptBtn.bottom);
    outtextxy(mainDecryptBtn.left + 20, mainDecryptBtn.top + 10, _T("解密"));
}
//加密界面
void drawEncrypt() {
    cleardevice();
    settextcolor(WHITE);
    //明文
    outtextxy(e_Input.left, e_Input.top - 20, _T("明文（任意长度，自动补零）:"));
    rectangle(e_Input.left, e_Input.top, e_Input.right, e_Input.bottom);
    outtextxy(e_Input.left + 10, e_Input.top + 10, inputtext.c_str());
    //密钥
    outtextxy(e_KeyInput.left, e_KeyInput.top - 20, _T("密钥（8字符）:"));
    rectangle(e_KeyInput.left, e_KeyInput.top, e_KeyInput.right, e_KeyInput.bottom);
    outtextxy(e_KeyInput.left + 10, e_KeyInput.top + 10, encryptKey.c_str());
    //密文
    outtextxy(e_Result.left, e_Result.top - 20, _T("加密结果:"));
    rectangle(e_Result.left, e_Result.top, e_Result.right, e_Result.bottom);
    outtextxy(e_Result.left + 10, e_Result.top + 10, resulttext.c_str());
    //操作
    rectangle(backBtn.left, backBtn.top, backBtn.right, backBtn.bottom);
    outtextxy(backBtn.left + 20, backBtn.top + 10, _T("返回"));
    rectangle(actionBtn.left, actionBtn.top, actionBtn.right, actionBtn.bottom);
    outtextxy(actionBtn.left + 20, actionBtn.top + 10, _T("加密"));
}
//解密界面
void drawDecrypt() {
    cleardevice();
    settextcolor(WHITE);
    //密文
    outtextxy(d_Input.left, d_Input.top - 20, _T("密文（16进制，16倍数长度）:"));
    rectangle(d_Input.left, d_Input.top, d_Input.right, d_Input.bottom);
    outtextxy(d_Input.left + 10, d_Input.top + 10, inputtext.c_str());
    //密钥
    outtextxy(d_KeyInput.left, d_KeyInput.top - 20, _T("密钥（8字符）:"));
    rectangle(d_KeyInput.left, d_KeyInput.top, d_KeyInput.right, d_KeyInput.bottom);
    outtextxy(d_KeyInput.left + 10, d_KeyInput.top + 10, decryptKey.c_str());
    //明文
    outtextxy(d_Result.left, d_Result.top - 20, _T("解密结果:"));
    rectangle(d_Result.left, d_Result.top, d_Result.right, d_Result.bottom);
    outtextxy(d_Result.left + 10, d_Result.top + 10, resulttext.c_str());
    //按钮
    rectangle(backBtn.left, backBtn.top, backBtn.right, backBtn.bottom);
    outtextxy(backBtn.left + 20, backBtn.top + 10, _T("返回"));
    rectangle(actionBtn.left, actionBtn.top, actionBtn.right, actionBtn.bottom);
    outtextxy(actionBtn.left + 20, actionBtn.top + 10, _T("解密"));
}
//处理鼠标事件
void handleMouse(int x, int y) {
    switch (c_state) {
    case MAIN:
        if (x > mainEncryptBtn.left && x < mainEncryptBtn.right &&
            y > mainEncryptBtn.top && y < mainEncryptBtn.bottom) {
            c_state = ENCRYPT;
            inputtext.clear();
            encryptKey.clear();
            resulttext.clear();
            activeInput = TEXT_INPUT;
        }
        else if (x > mainDecryptBtn.left && x < mainDecryptBtn.right &&
            y > mainDecryptBtn.top && y < mainDecryptBtn.bottom) {
            c_state = DECRYPT;
            inputtext.clear();
            decryptKey.clear();
            resulttext.clear();
            activeInput = TEXT_INPUT;
        }
        break;

    case ENCRYPT:
        if (x > backBtn.left && x < backBtn.right &&
            y > backBtn.top && y < backBtn.bottom) {
            c_state = MAIN;
            inputtext.clear();
            encryptKey.clear();
            resulttext.clear();
        }
        else if (x > actionBtn.left && x < actionBtn.right &&
            y > actionBtn.top && y < actionBtn.bottom) {
            if (encryptKey.size() != 8) {
                resulttext = "密钥必须为8字符!";
                break;
            }
            //产生密钥
            vector<int> key_bin = string_binary(encryptKey);
            if (key_bin.size() != 64) {
                resulttext = "无效密钥!";
                break;
            }
            generate_keys(key_bin);
            encryptionKey = encryptKey;

            // 补零处理
            string padded_input = inputtext;
            size_t padded_len = (inputtext.length() + 7) / 8 * 8;
            padded_input.resize(padded_len, '\0');

            // 分块加密
            string cipher_hex;
            for (int i = 0; i < padded_len; i += 8) {
                string block = padded_input.substr(i, 8);
                vector<int> plain_bits = string_binary(block);
                vector<int> cipher_bits = progress(plain_bits, key_arr, 1);
                cipher_hex += binary_Hex(cipher_bits);
            }
            resulttext = cipher_hex;
        }
        else if (x >= e_Input.left && x <= e_Input.right &&
            y >= e_Input.top && y <= e_Input.bottom) {
            activeInput = TEXT_INPUT;
        }
        else if (x >= e_KeyInput.left && x <= e_KeyInput.right &&
            y >= e_KeyInput.top && y <= e_KeyInput.bottom) {
            activeInput = KEY_INPUT;
        }
        break;

    case DECRYPT:
        if (x > backBtn.left && x < backBtn.right &&
            y > backBtn.top && y < backBtn.bottom) {
            c_state = MAIN;
            inputtext.clear();
            decryptKey.clear();
            resulttext.clear();
        }
        else if (x > actionBtn.left && x < actionBtn.right &&
            y > actionBtn.top && y < actionBtn.bottom) {
            if (inputtext.empty() || inputtext.size() % 16 != 0) {
                resulttext = "密文长度必须是16的倍数!";
                break;
            }
            bool valid_hex = all_of(inputtext.begin(), inputtext.end(), [](char c) {
                return isxdigit(c) != 0;
            });
            if (!valid_hex) {
                resulttext = "密文包含无效字符!";
                break;
            }
            if (decryptKey.size() != 8) {
                resulttext = "密钥必须为8字符!";
                break;
            }
            if (decryptKey != encryptionKey) {
                resulttext = "密钥不匹配!";
                break;
            }

            vector<int> key_bin = string_binary(decryptKey);
            if (key_bin.size() != 64) {
                resulttext = "无效密钥!";
                break;
            }
            generate_keys(key_bin);

            // 分块解密
            string decrypted_text;
            for (int i = 0; i < inputtext.size(); i += 16) {
                string block_hex = inputtext.substr(i, 16);
                vector<int> cipher_bits = hex_binary(block_hex);
                vector<int> plain_bits = progress(cipher_bits, key_arr, 0);
                decrypted_text += binary_string(plain_bits);
            }
            resulttext = decrypted_text;
        }
        else if (x >= d_Input.left && x <= d_Input.right &&
            y >= d_Input.top && y <= d_Input.bottom) {
            activeInput = TEXT_INPUT;
        }
        else if (x >= d_KeyInput.left && x <= d_KeyInput.right &&
            y >= d_KeyInput.top && y <= d_KeyInput.bottom) {
            activeInput = KEY_INPUT;
        }
        break;
    }
}