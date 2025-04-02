#pragma once
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

// Lưu trạng thái game vào file nhị phân
void saveGame(int** matrix, int n, unsigned int score);

// Tải trạng thái game từ file nhị phân
bool loadGame(int**& matrix, int n, unsigned int& score);