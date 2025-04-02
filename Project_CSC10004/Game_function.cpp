#include "Game_function.h"
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <cstring>

// Khai báo biến toàn cục để lưu trạng thái Undo/Redo
int historyIndex = 0, historyCount = 0;
int*** history = new int** [HISTORY_SIZE];
unsigned int historyScore[HISTORY_SIZE];

// Tạo ma trận game
void createMatrix(int**& matrix, int n) {
    matrix = new int* [n];
    for (int i = 0; i < n; i++) {
        matrix[i] = new int[n]();
    }
    spawnNewTile(matrix, n);
    spawnNewTile(matrix, n);
}

// Giải phóng bộ nhớ
void freeMatrix(int**& matrix, int n) {
    for (int i = 0; i < n; i++) {
        delete[] matrix[i];
    }
    delete[] matrix;
    matrix = nullptr;
}

// Hàm di chuyển chung
bool move(int** matrix, int n, unsigned int& score, int dx, int dy) {
    bool moved = false;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int x = i, y = j;
            while (x + dx >= 0 && x + dx < n && y + dy >= 0 && y + dy < n && matrix[x + dx][y + dy] == 0) {
                matrix[x + dx][y + dy] = matrix[x][y];
                matrix[x][y] = 0;
                x += dx;
                y += dy;
                moved = true;
            }
            if (x + dx >= 0 && x + dx < n && y + dy >= 0 && y + dy < n && matrix[x + dx][y + dy] == matrix[x][y]) {
                matrix[x + dx][y + dy] *= 2;
                score += matrix[x + dx][y + dy];
                matrix[x][y] = 0;
                moved = true;
            }
        }
    }
    return moved;
}

bool move_to_Left(int** matrix, int n, unsigned int& score) {
    return move(matrix, n, score, 0, -1);
}

bool move_to_Right(int** matrix, int n, unsigned int& score) {
    return move(matrix, n, score, 0, 1);
}

bool move_to_Up(int** matrix, int n, unsigned int& score) {
    return move(matrix, n, score, -1, 0);
}

bool move_to_Down(int** matrix, int n, unsigned int& score) {
    return move(matrix, n, score, 1, 0);
}

// Kiểm tra game over
bool checkGameOver(int** matrix, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (matrix[i][j] == 0) return false;
            if (i < n - 1 && matrix[i][j] == matrix[i + 1][j]) return false;
            if (j < n - 1 && matrix[i][j] == matrix[i][j + 1]) return false;
        }
    }
    return true;
}

// Kiểm tra thắng
bool checkWin(int** matrix, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (matrix[i][j] == 2048) return true;
        }
    }
    return false;
}

// Sinh ô mới
void spawnNewTile(int** matrix, int n) {
    srand(time(0));
    int x, y;
    do {
        x = rand() % n;
        y = rand() % n;
    } while (matrix[x][y] != 0);
    matrix[x][y] = (rand() % 10 == 0) ? 4 : 2;
}

// In bảng game
void printMatrix(int** matrix, int n, unsigned int score) {
    std::cout << "Score: " << score << "\n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            std::cout << std::setw(5) << matrix[i][j] << " ";
        }
        std::cout << "\n";
    }
}

// Copy ma trận
void copyMatrix(int** src, int**& dest, int n) {
    if (dest == nullptr) {
        dest = new int* [n];
        for (int i = 0; i < n; i++) {
            dest[i] = new int[n];
        }
    }
    for (int i = 0; i < n; i++) {
        memcpy(dest[i], src[i], n * sizeof(int));
    }
}

// Lưu trạng thái Undo/Redo
void saveState(int** matrix, int n, unsigned int score) {
    copyMatrix(matrix, history[historyIndex], n);
    historyScore[historyIndex] = score;
    historyIndex = (historyIndex + 1) % HISTORY_SIZE;
    if (historyCount < HISTORY_SIZE) historyCount++;
}

// Undo
void undo(int**& matrix, int n, unsigned int& score) {
    if (historyCount > 0) {
        historyIndex = (historyIndex - 1 + HISTORY_SIZE) % HISTORY_SIZE;
        copyMatrix(history[historyIndex], matrix, n);
        score = historyScore[historyIndex];
        historyCount--;
    }
}

// Redo
void redo(int**& matrix, int n, unsigned int& score) {
    if (historyCount < HISTORY_SIZE) {
        copyMatrix(history[historyIndex], matrix, n);
        score = historyScore[historyIndex];
        historyIndex = (historyIndex + 1) % HISTORY_SIZE;
        historyCount++;
    }
}
