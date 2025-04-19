#include "Resume.h"
#include <iostream>
#include <fstream>
#include <iomanip> // Để căn chỉnh text

#define SAVE_FILE "savegame.dat"

using namespace std;

void printBoxedMessage(const string& message) {
    int width = message.length() + 4;
    cout << "\n" << string(width, '=') << "\n";
    cout << "| " << message << " |\n";
    cout << string(width, '=') << "\n\n";
}

void deleteSavedGame() {
    string currentUsername;
    string saveFile = currentUsername + SAVE_FILE;
    if (remove(saveFile.c_str()) == 0)
        printBoxedMessage("Save file deleted.");
    else
        printBoxedMessage("No save file found to delete.");
}

// Lưu trạng thái game vào file nhị phân
void saveGame(int** matrix, int n, unsigned int score) {
    ofstream outFile(SAVE_FILE, ios::binary);
    if (!outFile) {
        printBoxedMessage("❌ Cannot open file!");
        return;
    }

    outFile.write(reinterpret_cast<char*>(&n), sizeof(n)); // Lưu kích thước bảng
    outFile.write(reinterpret_cast<char*>(&score), sizeof(score)); // Lưu điểm số

    for (int i = 0; i < n; ++i) {
        outFile.write(reinterpret_cast<char*>(matrix[i]), n * sizeof(int));
    }

    outFile.close();
    printBoxedMessage("✅ Game saved successfully!");
}

// Tải trạng thái game từ file nhị phân
bool loadGame(int**& matrix, int n, unsigned int& score) {
    ifstream inFile(SAVE_FILE, ios::binary);
    if (!inFile) {
        printBoxedMessage("❌ Cannot download file!!!!.");
        return false;
    }

    int savedN;
    inFile.read(reinterpret_cast<char*>(&savedN), sizeof(savedN));
    if (savedN != n) {
        printBoxedMessage("❌ Kich thuoc bang khong phu hop!");
        return false;
    }

    inFile.read(reinterpret_cast<char*>(&score), sizeof(score));

    if (matrix != nullptr) {
        for (int i = 0; i < n; ++i) delete[] matrix[i];
        delete[] matrix;
    }

    matrix = new int* [n];
    for (int i = 0; i < n; ++i)
        matrix[i] = new int[n];

    for (int i = 0; i < n; ++i) {
        inFile.read(reinterpret_cast<char*>(matrix[i]), n * sizeof(int));
    }

    inFile.close();
    printBoxedMessage("✅ Game loaded successfully!");
    return true;
}
