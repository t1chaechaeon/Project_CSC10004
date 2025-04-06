#include "Game_function.h"
#include "Stack.h"
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <cstring>

Stack undoStack;
Stack redoStack;


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
    srand(time(0)); // Gọi lại srand mỗi lần tạo ô mới (không khuyến khích)

    int x, y;
    do {
        x = rand() % n;
        y = rand() % n;
    } while (matrix[x][y] != 0); // Lặp đến khi tìm được ô trống

    // Gán giá trị mới vào ô trống
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

// Sao chép ma trận
void copyMatrix(int** src, int**& dest, int n) {
    if (src == nullptr || dest == nullptr || n <= 0) return;

    // Giải phóng bộ nhớ nếu dest đã tồn tại
    for (int i = 0; i < n; ++i) {
        delete[] dest[i];
    }
    delete[] dest;

    // Cấp phát bộ nhớ mới cho dest
    dest = new int* [n];
    for (int i = 0; i < n; ++i) {
        dest[i] = new int[n];
    }

    // Sao chép dữ liệu từ src sang dest
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
    push(redoStack, matrix, n, score); // Lưu trạng thái hiện tại vào redo

    // Sao chép prev.matrix vào matrix
    copyMatrix(prev.matrix, matrix, n);
    score = prev.score;

    // Giải phóng prev.matrix sau khi sao chép
    for (int i = 0; i < n; ++i) delete[] prev.matrix[i];
    delete[] prev.matrix;
}



// Hàm Redo
void redo(int**& matrix, int n, unsigned int& score) {
    GameState next;
    if (!pop(redoStack, next, n)) {
        std::cout << "No more redo!\n";
        return;
    }
    push(undoStack, matrix, n, score); // Lưu trạng thái hiện tại vào undo

    // Sao chép next.matrix vào matrix
    copyMatrix(next.matrix, matrix, n);
    score = next.score;

    // Giải phóng next.matrix sau khi sao chép
    for (int i = 0; i < n; ++i) delete[] next.matrix[i];
    delete[] next.matrix;
}
