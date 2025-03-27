#include "User_Auth.h"
#include <iostream>
#include <fstream>

using namespace std;

// Ghi dữ liệu user xuống file (Duyệt cây theo thứ tự NLR)
void saveUsersToFile(UserNode* root, ofstream& outFile) {
    if (root == nullptr) return;

    outFile.write(reinterpret_cast<char*>(&root->username), sizeof(root->username));
    outFile.write(reinterpret_cast<char*>(&root->hashedPassword), sizeof(root->hashedPassword));

    saveUsersToFile(root->left, outFile);
    saveUsersToFile(root->right, outFile);
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
