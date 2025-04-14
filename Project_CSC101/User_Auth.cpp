#include "User_Auth.h"
#include <iostream>
#include <fstream>

using namespace std;

// Hàm mã hóa mật khẩu
size_t hashPassWord(const string& password) {
    size_t hash = 0;
    for (char c : password) {
        hash = (hash * 31) + c; // Nhân với số nguyên tố 31 để tăng tính phân tán
   }
    return hash;
}

// Xác thực lại mật khẩu 2 lần
bool confirmPassWord(const string& passWord1, const string& passWord2) {
    return passWord1 == passWord2;
}

// Ghi dữ liệu user xuống file (Duyệt cây theo thứ tự NLR)
void saveUsersToFile(UserNode* root, ofstream& outFile) {
    if (root == nullptr) return;

    size_t len = root->username.size();
    outFile.write(reinterpret_cast<char*>(&len), sizeof(len)); // Ghi độ dài tên
    outFile.write(root->username.c_str(), len);                // Ghi tên

    outFile.write(reinterpret_cast<char*>(&root->hashedPassword), sizeof(root->hashedPassword));

    saveUsersToFile(root->left, outFile);
    saveUsersToFile(root->right, outFile);
}

// Chèn tài khoản người dùng vào BST 
UserNode* insertUserNode(UserNode* root, const string& username, const string& passWord)
{
    size_t hashedPassword = hashPassWord(passWord);
    if (root == nullptr) {
        return new UserNode(username, hashedPassword);
    }
    if (root->username < username) {
        root->left = insertUserNode(root->left, username, passWord);
    }
    else if (username > root->username) {
        root->right = insertUserNode(root->right, username, passWord);
    }

    return root;
}
// Hàm tìm kiếm người dùng trong BST
UserNode* findUser(UserNode* root, const string& username) {
    if (root == nullptr) {
        return root;
    }
    if (root->username == username) {
        return root;
    }
    else if (root->username > username) {
        root->left = findUser(root->left, username);
    }
    else {
        root->right = findUser(root->right, username);
    }
    return root;
}
// Hàm đăng nhập
bool loginUser(UserNode* root, const string& username, const string& passWord) {
    if (root == nullptr) {
        return false; // Node rỗng, người dùng không tồn tại
    }
    // So sánh username với root
    if (username == root->username) {
        // Kiểm tra mật khẩu đã băm (hashed password)
        return root->hashedPassword == hashPassWord(passWord);
    }
    else if (username < root->username) {
        return loginUser(root->left, username, passWord);
    }
    else {
        return loginUser(root->right, username, passWord);
    }
}
// Đọc dữ liệu user từ file
UserNode* loadUsersFromFile(const string& filename) {
    ifstream inFile(filename, ios::binary);
    if (!inFile) return nullptr;

    UserNode* root = nullptr;

    while (!inFile.eof()) {
        size_t len;
        if (!inFile.read(reinterpret_cast<char*>(&len), sizeof(len))) break;

        string username(len, ' ');
        inFile.read(&username[0], len);

        size_t hashedPassword;
        inFile.read(reinterpret_cast<char*>(&hashedPassword), sizeof(hashedPassword));

        // Dùng password giả để lưu (vì đã băm rồi)
        root = insertUserNode(root, username, to_string(hashedPassword));
        root->hashedPassword = hashedPassword; // Gán đúng hash (vì hàm insert băm lại)
    }

    inFile.close();
    return root;
}
