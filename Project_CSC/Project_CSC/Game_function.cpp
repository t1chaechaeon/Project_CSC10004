#include "Game_function.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

#define MAX_HISTORY 10 // Số bước có thể undo/redo tối đa

using namespace std;

const int EMPTY = 0; // Giá trị ô trống

// Lịch sử trạng thái trò chơi
struct GameState {
    int** matrix;
    unsigned int score;
};

GameState history[MAX_HISTORY]; // Mảng vòng lưu trạng thái
int historyIndex = -1; // Vị trí hiện tại trong lịch sử
int historySize = 0;   // Số trạng thái đang lưu

// Tạo bảng game
void createMatrix(int**& matrix, int n) {
    matrix = new int* [n];
    for (int i = 0; i < n; i++) {
        matrix[i] = new int[n]();
    }
}

// Giải phóng bộ nhớ
void freeMatrix(int**& matrix, int n) {
    if (matrix) {
        for (int i = 0; i < n; i++) {
            delete[] matrix[i];
        }
        delete[] matrix;
        matrix = nullptr;
    }
}

// Sao chép ma trận
void copyMatrix(int** src, int**& dest, int n) {
    if (!dest) {
        dest = new int* [n];
        for (int i = 0; i < n; i++) {
            dest[i] = new int[n]();
        }
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            dest[i][j] = src[i][j];
        }
    }
}

// Lưu trạng thái vào lịch sử (Undo/Redo)
void saveState(int** matrix, int n, unsigned int score) {
    historyIndex = (historyIndex + 1) % MAX_HISTORY;
    if (historySize < MAX_HISTORY) {
        historySize++;
    }

    copyMatrix(matrix, history[historyIndex].matrix, n);
    history[historyIndex].score = score;
}

// Hoàn tác (Undo)
void undo(int**& matrix, int n, unsigned int& score) {
    if (historySize > 1) { // Phải có ít nhất 2 trạng thái để quay lại
        historyIndex = (historyIndex - 1 + MAX_HISTORY) % MAX_HISTORY;
        copyMatrix(history[historyIndex].matrix, matrix, n);
        score = history[historyIndex].score;
        historySize--; // Giảm số bước có thể redo
    }
}

// Làm lại (Redo)
void redo(int**& matrix, int n, unsigned int& score) {
    if (historySize < MAX_HISTORY) {
        int nextIndex = (historyIndex + 1) % MAX_HISTORY;
        if (history[nextIndex].matrix) {
            historyIndex = nextIndex;
            copyMatrix(history[historyIndex].matrix, matrix, n);
            score = history[historyIndex].score;
            historySize++;
        }
    }
}

// Kiểm tra trạng thái trò chơi
bool checkGameOver(int** matrix, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (matrix[i][j] == EMPTY) return false;
            if (j < n - 1 && matrix[i][j] == matrix[i][j + 1]) return false;
            if (i < n - 1 && matrix[i][j] == matrix[i + 1][j]) return false;
        }
    }
    return true;
}

bool checkWin(int** matrix, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (matrix[i][j] == 2048) return true;
        }
    }
    return false;
}

// In bảng game
void printMatrix(int** matrix, int n, unsigned int score) {
    system("cls");
    cout << "Score: " << score << endl;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << matrix[i][j] << "\t";
        }
        cout << endl;
    }
}

// Sinh số ngẫu nhiên (2 hoặc 4)
void spawnNewTile(int** matrix, int n) {
    srand(time(0));
    int emptyCells = 0;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (matrix[i][j] == EMPTY) emptyCells++;
        }
    }

    if (emptyCells == 0) return; // Không còn chỗ trống

    int target = rand() % emptyCells;
    int count = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (matrix[i][j] == EMPTY) {
                if (count == target) {
                    matrix[i][j] = (rand() % 2 + 1) * 2;
                    return;
                }
                count++;
            }
        }
    }
}

// Di chuyển trái
bool move_to_Left(int** matrix, int n, unsigned int& score) {
    bool moved = false;
    for (int i = 0; i < n; i++) {
        int target = 0;
        for (int j = 0; j < n; j++) {
            if (matrix[i][j] != EMPTY) {
                if (target > 0 && matrix[i][target - 1] == matrix[i][j]) {
                    matrix[i][target - 1] *= 2;
                    score += matrix[i][target - 1];
                    matrix[i][j] = EMPTY;
                    moved = true;
                }
                else {
                    swap(matrix[i][target], matrix[i][j]);
                    if (target != j) moved = true;
                    target++;
                }
            }
        }
    }
    if (moved) spawnNewTile(matrix, n);
    return moved;
}

bool move_to_Right(int** matrix, int n, unsigned int& score) {
	bool moved = false;
	for (int i = 0; i < n; i++) {
		int target = n - 1;
		for (int j = n - 1; j >= 0; j--) {
			if (matrix[i][j] != EMPTY) {
				if (target < n - 1 && matrix[i][target + 1] == matrix[i][j]) {
					matrix[i][target + 1] *= 2;
					score += matrix[i][target + 1];
					matrix[i][j] = EMPTY;
					moved = true;
				}
				else {
					swap(matrix[i][target], matrix[i][j]);
					if (target != j) moved = true;
					target--;
				}
			}
		}
	}
	if (moved) spawnNewTile(matrix, n);
	return moved;
}

bool move_to_Up(int** matrix, int n, unsigned int& score) {
	bool moved = false;
	for (int j = 0; j < n; j++) {
		int target = 0;
		for (int i = 0; i < n; i++) {
			if (matrix[i][j] != EMPTY) {
				if (target > 0 && matrix[target - 1][j] == matrix[i][j]) {
					matrix[target - 1][j] *= 2;
					score += matrix[target - 1][j];
					matrix[i][j] = EMPTY;
					moved = true;
				}
				else {
					swap(matrix[target][j], matrix[i][j]);
					if (target != i) moved = true;
					target++;
				}
			}
		}
	}
	if (moved) spawnNewTile(matrix, n);
	return moved;
}

bool move_to_Down(int** matrix, int n, unsigned int& score) {
	bool moved = false;
	for (int j = 0; j < n; j++) {
		int target = n - 1;
		for (int i = n - 1; i >= 0; i--) {
			if (matrix[i][j] != EMPTY) {
				if (target < n - 1 && matrix[target + 1][j] == matrix[i][j]) {
					matrix[target + 1][j] *= 2;
					score += matrix[target + 1][j];
					matrix[i][j] = EMPTY;
					moved = true;
				}
				else {
					swap(matrix[target][j], matrix[i][j]);
					if (target != i) moved = true;
					target--;
				}
			}
		}
	}
	if (moved) spawnNewTile(matrix, n);
	return moved;
}

