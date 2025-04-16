#pragma once
#include <iostream>
#include <string>
#include <functional>
#include "User_Auth.h"
#include "Register.h"
#include "LoginUser.h"
#include "Game_function.h"
#include "LeaderBoard.h"
#include "Resume.h"
#include "Stack.h"
#include <conio.h> // Thư viện cho getch()

using namespace std;

// Hiển thị menu chính
void mainMenu();
// Hàm thoát game
void exitGame();