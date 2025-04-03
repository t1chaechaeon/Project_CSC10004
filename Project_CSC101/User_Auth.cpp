#include "User_Auth.h"
#include <iostream>
#include <fstream>

using namespace std;

// Hàm mã hóa mật khẩu
size_t hashPassWord(const string& password) {
    hash<string> hasher;
    size_t hashed = hasher(password);
    return hashed;
}

// Xác thực lại mật khẩu 2 lần
bool confirmPassWord(const string& passWord1, const string& passWord2) {
    return passWord1 == passWord2;
}

// Ghi dữ liệu user xuống file (Duyệt cây theo thứ tự NLR)
void saveUsersToFile(UserNode* root, ofstream& outFile) {
    if (root == nullptr) return;

    outFile.write(reinterpret_cast<char*>(&root->username), sizeof(root->username));
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
    string username;
    size_t hashedPassword;

    while (inFile.read(reinterpret_cast<char*>(&username), sizeof(username))) {
        inFile.read(reinterpret_cast<char*>(&hashedPassword), sizeof(hashedPassword));
        root = insertUserNode(root, username, to_string(hashedPassword)); // Chèn vào BST
    }

    inFile.close();
    return root;
}