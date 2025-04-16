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

};
// Chèn người chơi vào BST
BSTNode* insertNode(BSTNode* root, const string& username, unsigned int score);

// Hàm lưu BST vào file nhị phân theo thứ tự giảm dần
void saveToFile(BSTNode* root, ofstream& out);

// Tải bảng xếp hạng từ tập tin nhị phân
BSTNode* loadFromFile(ifstream& inFile);

// Duyệt cây theo thứ tự giảm dần để hiển thị bảng xếp hạng
void PrintLeaderboard(BSTNode* root);

// Giải phóng bộ nhớ của BST
void deleteBST(BSTNode* root);