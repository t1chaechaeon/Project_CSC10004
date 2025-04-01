#pragma once
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

// Định nghĩa cấu trúc nút cho BST
struct BSTNode {
    string username;
    int score;
    BSTNode* left;
    BSTNode* right;

    BSTNode(const string& uname, int scr)
        : username(uname), score(scr), left(nullptr), right(nullptr) {
    }
};

// Chèn người chơi vào BST
BSTNode* insertNode(BSTNode* root, const string& username, int score);

// Duyệt cây theo thứ tự giảm dần để hiển thị bảng xếp hạng
void inOrderTraversal(BSTNode* root);

// Lưu bảng xếp hạng vào tập tin nhị phân
void saveToFile(BSTNode* root, ofstream& outFile);

// Tải bảng xếp hạng từ tập tin nhị phân
BSTNode* loadFromFile(ifstream& inFile);

// Giải phóng bộ nhớ của BST
void deleteBST(BSTNode* root);
