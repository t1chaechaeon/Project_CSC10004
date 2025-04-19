#include "Game_function.h"
#include "Stack.h"
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <windows.h> // Để dùng màu trong console

Stack undoStack;
Stack redoStack;

// Đặt màu chữ
void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

// Trả về màu dựa trên giá trị ô
int getColorForValue(int value) {
    switch (value) {
    case 2: return 8;  // Màu xám sáng
    case 4: return 14; // Màu vàng sáng
    case 8: return 10; // Màu xanh lá cây sáng
    case 16: return 13; // Màu tím sáng
    case 32: return 12; // Màu đỏ sáng
    case 64: return 9;  // Màu xanh dương sáng
    case 128: return 11; // Màu xanh biển sáng
    case 256: return 15; // Màu trắng sáng
    case 512: return 5;  // Màu hồng sáng
    case 1024: return 6; // Màu cam sáng
    case 2048: return 4; // Màu vàng đậm
    default: return 7;   // Màu mặc định
    }
}

// Tạo ma trận game
void createMatrix(int**& matrix, int n) {
    matrix = new int* [n];
    for (int i = 0; i < n; i++) {
        matrix[i] = new int[n]();
    }
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
            if (x + dx >= 0 && x + dx < n && y + dy >= 0 && y + dy < n && matrix[x + dx][y + dy] == matrix[x][y] && matrix[x][y] != 0) {
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
    system("cls");
    std::cout << "\n\t\t      2048 GAME\n";
    std::cout << "\t\t    -----------------\n";
    std::cout << "\t\t       Score: " << score << "\n\n";

    // Dòng phân cách giữa các ô
    std::string line = "+-------+-------+-------+-------+\n";

    for (int i = 0; i < n; ++i) {
        std::cout << "\t\t    " << line;  // In dòng phân cách
        std::cout << "\t\t    ";  // Khoảng cách trước khi in các ô

        for (int j = 0; j < n; ++j) {
            std::cout << "|";  // In dấu phân cách giữa các ô

            // Nếu ô trống, in dấu cách, nếu không in giá trị ô
            if (matrix[i][j] == 0) {
                std::cout << std::setw(6) << " ";  // Căn chỉnh trống
            }
            else {
                setColor(getColorForValue(matrix[i][j]));  // Đặt màu cho ô
                std::cout << std::setw(6) << matrix[i][j];  // Căn chỉnh giá trị ô
                setColor(7);  // Đặt màu mặc định
            }
        }

        std::cout << "|\n";  // Kết thúc dòng ô
    }

    std::cout << "\t\t    " << line;  // In dòng phân cách cuối cùng
    std::cout << "\n\n\tUse arrow keys to move. U - Undo, R - Redo, S - Save & Exit\n";
}


// Sao chép ma trận
void copyMatrix(int** src, int**& dest, int n) {
    if (src == nullptr || dest == nullptr || n <= 0) return;
    for (int i = 0; i < n; ++i) delete[] dest[i];
    delete[] dest;

    dest = new int* [n];
    for (int i = 0; i < n; ++i) {
        dest[i] = new int[n];
    }

    for (int i = 0; i < n; i++) {
        memcpy(dest[i], src[i], n * sizeof(int));
    }
}

// Hàm Undo
void undo(int**& matrix, int n, unsigned int& score) {
    GameState prev;
    if (!pop(undoStack, prev, n)) {
        std::cout << "No more undo!\n";
        return;
    }
    push(redoStack, matrix, n, score);

    for (int i = 0; i < n; ++i) delete[] matrix[i];
    delete[] matrix;

    matrix = prev.matrix;
    score = prev.score;
    prev.matrix = nullptr;
}

// Hàm Redo
void redo(int**& matrix, int n, unsigned int& score) {
    GameState next;
    if (!pop(redoStack, next, n)) {
        std::cout << "No more redo!\n";
        return;
    }
    push(undoStack, matrix, n, score);

    for (int i = 0; i < n; ++i) delete[] matrix[i];
    delete[] matrix;

    matrix = next.matrix;
    score = next.score;
    next.matrix = nullptr;
}
