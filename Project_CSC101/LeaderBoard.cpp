#include "LeaderBoard.h"
#include <iomanip>

BSTNode* insertNode(BSTNode* root, const string& username, unsigned int score) {
    if (root == nullptr) {
        BSTNode* newNode = new BSTNode;
        newNode->username = username;
        newNode->score = score;
        newNode->left = newNode->right = nullptr;
        return newNode;
    }

    if (username == root->username) {
        // Nếu tên người chơi trùng nhau, cập nhật điểm nếu điểm mới cao hơn
        if (score > root->score) {
            root->score = score;
        }
    }
    else if (score > root->score) {
        root->right = insertNode(root->right, username, score); // Chèn sang phải nếu điểm cao hơn
    }
    else if (score < root->score) {
        root->left = insertNode(root->left, username, score);   // Chèn sang trái nếu điểm thấp hơn
    }
    else {
        // Nếu điểm bằng nhau thì so sánh theo tên để phân biệt
        if (username < root->username) {
            root->left = insertNode(root->left, username, score);
        }
        else {
            root->right = insertNode(root->right, username, score);
        }
    }

    return root;
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

// Hàm duyệt cây theo thứ tự giảm dần để hiển thị bảng xếp hạng (từ cao xuống thấp)
void PrintLeaderboard(BSTNode* root) {
    if (root == nullptr) return;

    static int rank = 1; // Giữ thứ hạng giữa các lần gọi đệ quy

    // In tiêu đề bảng 1 lần duy nhất
    static bool headerPrinted = false;
    if (!headerPrinted) {
        cout << left << setw(5) << "No."
             << left << setw(20) << "Player"
             << right << setw(10) << "Score" << endl;
        cout << string(35, '-') << endl;
        headerPrinted = true;
    }

    // Duyệt cây theo thứ tự giảm dần
    PrintLeaderboard(root->right);

    // In thông tin người chơi
    cout << left << setw(5) << rank++
         << left << setw(20) << root->username
         << right << setw(10) << root->score << endl;

    PrintLeaderboard(root->left);

    // Reset lại khi kết thúc duyệt cây
    if (rank == 2) {
        rank = 1;
        headerPrinted = false;
    }
}

// Hàm giải phóng bộ nhớ BST
void deleteBST(BSTNode* root) {
    if (root == nullptr) return;

    deleteBST(root->left);
    deleteBST(root->right);
    delete root;
}