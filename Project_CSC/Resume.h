#pragma once
#include <iostream>
#include <fstream>

void saveGame(int** matrix, int n, unsigned int score);


bool loadGame(int**& matrix, int n, unsigned int& score);
