#include "Resume.h"

#define SAVE_FILE "savegame.dat"

// Lưu trạng thái game vào file nhị phân
void saveGame(int** matrix, int n, unsigned int score) {
    std::ofstream outFile(SAVE_FILE, std::ios::binary);
    if (!outFile) {
        std::cerr << "Khong the mo file de luu game!" << std::endl;
        return;
    }

    outFile.write(reinterpret_cast<char*>(&n), sizeof(n)); // Lưu kích thước bảng
    outFile.write(reinterpret_cast<char*>(&score), sizeof(score)); // Lưu điểm số

    // Lưu ma trận game
    for (int i = 0; i < n; ++i) {
        outFile.write(reinterpret_cast<char*>(matrix[i]), n * sizeof(int));
    }

    outFile.close();
}

// Tải trạng thái game từ file nhị phân
bool loadGame(int**& matrix, int n, unsigned int& score) {
    std::ifstream inFile(SAVE_FILE, std::ios::binary);
    if (!inFile) {
        std::cerr << "Khong the tai game!" << std::endl;
        return false;
    }

    int savedN;
    inFile.read(reinterpret_cast<char*>(&savedN), sizeof(savedN));
    if (savedN != n) {
        std::cerr << "Kich thuong bang khong phu hop!" << std::endl;
        return false;
    }

    inFile.read(reinterpret_cast<char*>(&score), sizeof(score));

    // Khởi tạo lại ma trận nếu cần
    if (matrix == nullptr) {
        matrix = new int* [n];
        for (int i = 0; i < n; ++i) {
            matrix[i] = new int[n] {};
        }
    }

    // Đọc dữ liệu vào ma trận
    for (int i = 0; i < n; ++i) {
        inFile.read(reinterpret_cast<char*>(matrix[i]), n * sizeof(int));
    }

    inFile.close();
    return true;
}
