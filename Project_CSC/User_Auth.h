#pragma once

#include <iostream>
#include <string>
#include <functional>

using namespace std;

// Cấu trúc cho xác thực người dùng (BST Node)
struct UserNode {
    string username;
    size_t hashedPassword;
    UserNode* left;
    UserNode* right;

    UserNode(const string& uname, size_t hPass)
        : username(uname), hashedPassword(hPass), left(nullptr), right(nullptr) {
    }
};

// Hàm mã hóa mật khẩu
size_t hashedPassWord(const string& password);

// Xác thực lại mật khẩu 2 lần
bool confirmPassWord(const string& passWord1, const string& passWord2);

// Chèn tài khoản người dùng vào BST 
UserNode* insertUserNode(UserNode* root, const string& username, const string& passWord);

// Hàm tìm kiếm người dùng trong BST
UserNode* findUser(UserNode* root, const string& username);

// Hàm đăng nhập
bool loginUser(UserNode* root, const string& username, const string& passWord);
