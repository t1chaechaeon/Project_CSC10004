#include "LeaderBoard.h"

// Hàm chèn người chơi vào BST
BSTNode* insertNode(BSTNode* root, const string& username, int score) {
    if (root == nullptr) return new BSTNode(username, score);

    if (score > root->score) {
        root->right = insertNode(root->right, username, score); // Chèn vào nhánh phải (điểm cao hơn)
    }
    else {
        root->left = insertNode(root->left, username, score); // Chèn vào nhánh trái (điểm thấp hơn)
    }

    return root;
}

// Hàm duyệt cây theo thứ tự giảm dần để hiển thị bảng xếp hạng (từ cao xuống thấp)
void inOrderTraversal(BSTNode* root) {
    if (root == nullptr) return;

    inOrderTraversal(root->right); // In điểm cao trước
    cout << root->username << " - " << root->score << endl;
    inOrderTraversal(root->left);
}

// Lưu bảng xếp hạng vào file nhị phân (Duyệt cây theo thứ tự giảm dần)
void saveToFile(BSTNode* root, ofstream& outFile) {
    if (root == nullptr) return;

    saveToFile(root->right, outFile); // Lưu điểm cao trước
    outFile.write((char*)&root->score, sizeof(root->score));

    size_t len = root->username.size();
    outFile.write((char*)&len, sizeof(len));
    outFile.write(root->username.c_str(), len);

    saveToFile(root->left, outFile);
}

// Tải bảng xếp hạng từ file nhị phân
BSTNode* loadFromFile(ifstream& inFile) {
    BSTNode* root = nullptr;
    while (!inFile.eof()) {
        int score;
        size_t len;
        string username;

        inFile.read((char*)&score, sizeof(score));
        if (inFile.eof()) break; // Tránh đọc dữ liệu rác

        inFile.read((char*)&len, sizeof(len));
        username.resize(len);
        inFile.read(&username[0], len);

        root = insertNode(root, username, score);
    }
    return root;
}

// Hàm giải phóng bộ nhớ BST
void deleteBST(BSTNode* root) {
    if (root == nullptr) return;

    deleteBST(root->left);
    deleteBST(root->right);
    delete root;
}
