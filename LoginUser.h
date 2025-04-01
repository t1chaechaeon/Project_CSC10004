#pragma once
#include "User_Auth.h"
#include <iostream>
#include <fstream>
#include <string>

// Hàm đăng nhập trả về UserNode*
UserNode* login_User(UserNode* root, const string& username, const string& password);

// Hàm đăng nhập có giới hạn số lần thử
void loginProcess(UserNode* root);

// Hàm bắt đầu trò chơi
void newGame();
