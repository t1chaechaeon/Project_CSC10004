#include "Menu.h"
#include "Stack.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <thread>
#include <chrono>
#include <cctype>
#include <conio.h> // _getch()

using namespace std;

#define BOARD_SIZE 4
#define SAVE_FILE "savegame.dat"

std::string currentUsername;

extern Stack undoStack;
extern Stack redoStack;

int** gameBoard = nullptr;
unsigned int score = 0;
UserNode* userTree = nullptr;
BSTNode* leaderboardTree = nullptr;
UserNode* loggedInUser = nullptr;

// ===== Utility =====
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void loadingEffect(string text, int delay = 50) {
    for (char c : text) {
        cout << c << flush;
        this_thread::sleep_for(chrono::milliseconds(delay));
    }
}

void printBorderLine() {
    cout << "+------+------+------+------+\n";
}

// ===== Giao diện Bảng Game =====
void printMatrix(int** board, int size, int score) {
    cout << "========[SCORE: " << score << "]========\n\n";
    printBorderLine();
    for (int i = 0; i < size; ++i) {
        cout << "|";
        for (int j = 0; j < size; ++j) {
            if (board[i][j] == 0)
                cout << setw(5) << "." << " |";
            else
                cout << setw(5) << board[i][j] << " |";
        }
        cout << "\n";
        printBorderLine();
    }
    cout << "\n[Arrow Keys] Move   [U] Undo   [R] Redo   [S] Save & Exit\n";
}

// ===== Reset Game =====
void resetGameState() {
    score = 0;
    if (gameBoard != nullptr) {
        freeMatrix(gameBoard, BOARD_SIZE);
        gameBoard = nullptr;
    }
    clearStack(undoStack, BOARD_SIZE);
    clearStack(redoStack, BOARD_SIZE);
}

// ===== Thoát =====
void exitGame() {
    loadingEffect("Exiting game...\n");
}

// ===== Chơi Game =====
void playGame(bool isResumed = false) {
    if (!isResumed) {
        resetGameState();
        createMatrix(gameBoard, BOARD_SIZE);
        spawnNewTile(gameBoard, BOARD_SIZE);
        spawnNewTile(gameBoard, BOARD_SIZE);
    }

    bool running = true;

    while (running) {
        clearScreen();
        printMatrix(gameBoard, BOARD_SIZE, score);

        char move = _getch();
        bool moved = false;

        if (move == -32) {
            move = _getch();
            switch (move) {
            case 72: moved = move_to_Up(gameBoard, BOARD_SIZE, score); break;
            case 80: moved = move_to_Down(gameBoard, BOARD_SIZE, score); break;
            case 75: moved = move_to_Left(gameBoard, BOARD_SIZE, score); break;
            case 77: moved = move_to_Right(gameBoard, BOARD_SIZE, score); break;
            default: continue;
            }

            if (moved) {
                push(undoStack, gameBoard, BOARD_SIZE, score);
                clearStack(redoStack, BOARD_SIZE);
                spawnNewTile(gameBoard, BOARD_SIZE);
            }
        }
        else {
            switch (toupper(move)) {
            case 'U': undo(gameBoard, BOARD_SIZE, score); break;
            case 'R': redo(gameBoard, BOARD_SIZE, score); break;
            case 'S':
                saveGame(gameBoard, BOARD_SIZE, score);
                cout << "Game saved! Returning to menu...\n";
                return;
            default:
                continue;
            }
        }

        // Win
        if (checkWin(gameBoard, BOARD_SIZE)) {
            cout << "Congratulations! You won!\n";
            leaderboardTree = insertNode(leaderboardTree, userTree->username, score);
            ofstream outFile("leaderboard.dat", ios::binary);
            saveToFile(leaderboardTree, outFile);
            outFile.close();
            deleteSavedGame();
            freeMatrix(gameBoard, BOARD_SIZE);
            cout << "\nPress Enter to return to menu...";
            cin.get(); cin.get();
            return;
        }

        // Lose
        if (checkGameOver(gameBoard, BOARD_SIZE)) {
            cout << "Game Over!\n";
            leaderboardTree = insertNode(leaderboardTree, userTree->username, score);
            ofstream outFile("leaderboard.dat", ios::binary);
            saveToFile(leaderboardTree, outFile);
            outFile.close();
            deleteSavedGame();
            freeMatrix(gameBoard, BOARD_SIZE);
            cout << "\nPress Enter to return to menu...";
            cin.get(); cin.get();
            return;
        }
    }
}

// ===== Vẽ Menu =====
void drawMenuOption(int index, int currentChoice, const string& text) {
    if (index == currentChoice)
        cout << ">> [" << text << "]\n";
    else
        cout << "   [" << text << "]\n";
}

void drawMainMenu(int currentChoice) {
    cout << "=====================================\n";
    cout << "            \033[1;36m2048 GAME\033[0m         \n";
    cout << "=====================================\n\n";

    drawMenuOption(1, currentChoice, "Register");
    drawMenuOption(2, currentChoice, "Login & New Game");
    drawMenuOption(3, currentChoice, "Resume Game");
    drawMenuOption(4, currentChoice, "Leaderboard");
    drawMenuOption(5, currentChoice, "Exit");
}

// ===== Menu Chính =====
void mainMenu() {
    int choice = 1;
    const int numOptions = 5;

    while (true) {
        clearScreen();
        drawMainMenu(choice);

        char key = _getch();
        if (key == 72) choice = (choice > 1) ? choice - 1 : numOptions;
        else if (key == 80) choice = (choice < numOptions) ? choice + 1 : 1;
        else if (key == 13) {
            clearScreen();
            switch (choice) {
            case 1:
                userTree = resgisterUserProcess(userTree);
                break;
            case 2:
                resetGameState();
                loginProcess(userTree);
                playGame(false);
                break;
            case 3:
                if (loadGame(gameBoard, BOARD_SIZE, score)) {
                    cout << "Game loaded successfully!\n";
                    this_thread::sleep_for(chrono::milliseconds(500));
                    playGame(true);
                }
                else {
                    cout << "No saved game found!\n";
                    this_thread::sleep_for(chrono::milliseconds(1000));
                }
                break;
            case 4: {
                ifstream inFile("leaderboard.dat", ios::binary);
                if (inFile) {
                    leaderboardTree = loadFromFile(inFile);
                    inFile.close();
                    cout << "Leaderboard:\n";
                    PrintLeaderboard(leaderboardTree);
                }
                else {
                    cout << "No leaderboard data found.\n";
                }
                cout << "\nPress Enter to return...";
                cin.get(); cin.get();
                break;
            }
            case 5:
                exitGame();
                return;
            }
        }
    }
}
