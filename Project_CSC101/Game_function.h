#pragma once

#include <iostream>

// Giới hạn số lần Undo/Redo lưu trữ
#define HISTORY_SIZE 10

// Hàm tạo bảng game
void createMatrix(int**& matrix, int n);

// Hàm giải phóng bộ nhớ của bảng game
void freeMatrix(int**& matrix, int n);

// Các hàm di chuyển
bool move_to_Left(int** matrix, int n, unsigned int& score);
bool move_to_Right(int** matrix, int n, unsigned int& score);
bool move_to_Up(int** matrix, int n, unsigned int& score);
bool move_to_Down(int** matrix, int n, unsigned int& score);

// Kiểm tra trạng thái game
bool checkGameOver(int** matrix, int n);
bool checkWin(int** matrix, int n);

// Sinh ô mới sau mỗi lượt di chuyển
void spawnNewTile(int** matrix, int n);

// In bảng game ra màn hình
void printMatrix(int** matrix, int n, unsigned int score);

// Lưu trữ trạng thái game để Undo/Redo bằng mảng vòng
// Sao chép ma trận
void copyMatrix(int** src, int**& dest, int n);
void saveState(int** matrix, int n, unsigned int score);
void undo(int**& matrix, int n, unsigned int& score);
void redo(int**& matrix, int n, unsigned int& score);