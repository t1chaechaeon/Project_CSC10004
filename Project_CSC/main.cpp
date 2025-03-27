#include <iostream>
#include <fstream>
#include "User_Auth.h"
#include "LeaderBoard.h"
#include "Game_function.h"
#include "Resume.h"
#include "Register.h"

#define BOARD_SIZE 4 // Kích thước bảng 4x4

using namespace std;

// Hàm menu chính
void mainMenu();
// Hàm chơi game
void playGame();

// Biến lưu trạng thái game
int** gameBoard = nullptr;
unsigned int score = 0;
UserNode* userTree = nullptr; // Cây BST lưu user
BSTNode* leaderboardTree = nullptr; // Cây BST lưu điểm số

int main() {
    mainMenu();
    return 0;
}

// Hiển thị menu chính
void mainMenu() {
    int choice;
    string username, password;

    while (true) {
        cout << "1. Register\n";
        cout << "2. Login & New Game\n";
        cout << "3. Resume Game\n";
        cout << "4. Leaderboard\n";
        cout << "5. Exit\n";

        cout << "Choose an option: ";

        while (!(cin >> choice)) {
            cout << "Invalid input! Please enter a number: ";
            cin.clear();
            cin.ignore(10000, '\n');
        }

        switch (choice) {
        case 1:
            userTree = registerUser(userTree);
            break;
        case 2: {
            cout << "Enter username: ";
            cin >> username;
            cout << "Enter password: ";
            cin >> password;

            // Kiểm tra đăng nhập
            if (loginUser(userTree, username, password)) {
                cout << "Login successful! Starting new game...\n";
                playGame();
            }
            else {
                cout << "Invalid username or password!\n";
            }
            break;
        }
        case 3: {
            if (loadGame(gameBoard, BOARD_SIZE, score)) {
                cout << "Game loaded successfully!\n";
                playGame();
            }
            else {
                cout << "No saved game found!\n";
            }
            break;
        }
        case 4: {
            ifstream inFile("leaderboard.dat", ios::binary);
            if (inFile) {
                leaderboardTree = loadFromFile(inFile);
                inFile.close();
                cout << "Leaderboard:\n";
                inOrderTraversal(leaderboardTree);
            }
            else {
                cout << "No leaderboard data found.\n";
            }
            break;
        }
        case 5:
            cout << "Exiting game...\n";
            return;
        default:
            cout << "Invalid choice! Try again.\n";
        }
    }
}

// Chơi game 2048
void playGame() {
    createMatrix(gameBoard, BOARD_SIZE);
    spawnNewTile(gameBoard, BOARD_SIZE);
    spawnNewTile(gameBoard, BOARD_SIZE);

    char move;
    bool running = true;

    while (running) {
        printMatrix(gameBoard, BOARD_SIZE, score);
        cout << "Enter move (WASD - Move, U - Undo, R - Redo, S - Save & Exit): ";
        cin >> move;

        switch (toupper(move)) {
        case 'W': running = move_to_Up(gameBoard, BOARD_SIZE, score); break;
        case 'A': running = move_to_Left(gameBoard, BOARD_SIZE, score); break;
        case 'S': running = move_to_Down(gameBoard, BOARD_SIZE, score); break;
        case 'D': running = move_to_Right(gameBoard, BOARD_SIZE, score); break;
        case 'U': undo(gameBoard, BOARD_SIZE, score); break;
        case 'R': redo(gameBoard, BOARD_SIZE, score); break;
        case 'Z': {
            saveGame(gameBoard, BOARD_SIZE, score);
            cout << "Game saved! Returning to menu...\n";
            return;
        }
        default:
            cout << "Invalid move! Try again.\n";
        }

        if (checkWin(gameBoard, BOARD_SIZE)) {
            cout << "Congratulations! You won the game!\n";
            break;
        }
        else if (checkGameOver(gameBoard, BOARD_SIZE)) {
            cout << "Game Over!\n";
            break;
        }
        else {
            spawnNewTile(gameBoard, BOARD_SIZE);
        }
    }

    // Lưu điểm vào bảng xếp hạng
    leaderboardTree = insertNode(leaderboardTree, "Player", score);
    ofstream outFile("leaderboard.dat", ios::binary);
    saveToFile(leaderboardTree, outFile);
    outFile.close();

    freeMatrix(gameBoard, BOARD_SIZE);
}