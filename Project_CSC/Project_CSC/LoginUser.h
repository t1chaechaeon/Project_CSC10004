#pragma once
#include "User_Auth.h"
#include <iostream>
#include "LoginUser.h"

void newGame();
// Hàm đăng nhập trả về UserNode*
UserNode* login_User(UserNode* root, const string& username, const string& password);
//hàm đăng nhập trong hàm main 
void loginProcess(UserNode* root);
