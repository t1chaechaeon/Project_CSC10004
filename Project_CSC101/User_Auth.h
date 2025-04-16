#pragma once

#include <iostream>
#include "fstream"
#include <string>

using namespace std;

// Hàm băm đơn giản (DJB2)
unsigned long simpleHash(const string& str);
// Cấu trúc cho xác thực người dùng (BST Node)

struct UserNode {
    string username = "";
    unsigned long hashedPassword = 0;
    UserNode* left = nullptr;
    UserNode* right = nullptr;
};

// Tạo node mới
UserNode* createUser(string uname, string pass);

// Chèn tài khoản người dùng vào BST 
UserNode* insertUserNode(UserNode* root, const string& username, string passWord);

// Tìm người dùng + kiểm tra mật khẩu dùng trong BST
UserNode* searchUser(UserNode* root, string uname, string pass);

// Ghi dữ liệu user xuống file (Duyệt cây theo thứ tự NLR)
void saveToFile(UserNode* root, ofstream& out);

// Đọc dữ liệu từ file
UserNode* loadFromFile(const string& filename);





// Hàm đăng nhập
/*bool loginUser(UserNode* root, const string& username, const string& passWord);
// Ghi dữ liệu user xuống file(Duyệt cây theo thứ tự NLR)
void saveUsersToFile(UserNode* root, ofstream& outFile);
// Đọc dữ liệu user từ file
UserNode* loadUsersFromFile(const string& filename);*/