#include "User_Auth.h"
#include <iostream>
#include <fstream>

using namespace std;

// Hàm băm đơn giản (DJB2)
unsigned long simpleHash(const string& str) {
    unsigned long hash = 5381;
    for (char c : str)
        hash = ((hash << 5) + hash) + c;
    return hash;
}

// Tạo node mới
UserNode* createUser(string uname, string pass) {
    UserNode* node = new UserNode;
    node->username = uname;
    node->hashedPassword = simpleHash(pass);
    node->left = node->right = nullptr;
    return node;
}

// Chèn tài khoản người dùng vào BST 
UserNode* insertUserNode(UserNode* root, const string& username, string passWord)
{

    if (root == nullptr) {
        return createUser(username, passWord);
    }
    if (root->username < username) {
        root->left = insertUserNode(root->left, username, passWord);
    }
    else if (username > root->username) {
        root->right = insertUserNode(root->right, username, passWord);
    }

    return root;
}
  // Tìm người dùng + kiểm tra mật khẩu dùng trong BST
    UserNode* searchUser(UserNode* root, string uname, string pass) {
    if (!root) return nullptr;
    if (uname < root->username)
         return searchUser(root->left, uname, pass);
    else if (uname > root->username)
         return searchUser(root->right, uname, pass);
    else {
          if (pass == "") return root; // kiểm tra tồn tại username
          if (root->hashedPassword == simpleHash(pass))
             return root;
             else
             return nullptr;
         }
 }

// Ghi dữ liệu user xuống file (Duyệt cây theo thứ tự NLR)
    void saveToFile(UserNode* root, ofstream& out) {
        if (!root) return;
        saveToFile(root->left, out);
        out << root->username << " " << root->hashedPassword << endl;
        saveToFile(root->right, out);
    }


// Hàm đăng nhập
/*bool loginUser(UserNode* root, const string& username, const string& passWord) {

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
}*/
// Đọc dữ liệu từ file
UserNode* loadFromFile(const string& filename) {
    ifstream in(filename);
    string uname;
    unsigned long hash;
    UserNode* root = nullptr;

    while (in >> uname >> hash) {
        UserNode* node = new UserNode;
        node->username = uname;
        node->hashedPassword = hash;
        node->left = node->right = nullptr;
        root = insertUserNode(root, uname, ""); // Dùng insert để giữ đúng vị trí
       UserNode* found = searchUser(root, uname, ""); 
       if (found) found->hashedPassword = hash; // Gán lại hash
    }
    return root;
}
// Đọc dữ liệu user từ file
/*UserNode* loadUsersFromFile(const string& filename) {
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
}*/
