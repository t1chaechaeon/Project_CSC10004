#include "Menu.h"
#include <iostream>
#include <fstream>
#include <cctype>


using namespace std;


#define BOARD_SIZE 4 // Kích thước bảng 4x4

// Biến lưu trạng thái game
int** gameBoard = nullptr;
unsigned int score = 0;
UserNode* userTree = nullptr; // Cây BST lưu user
BSTNode* leaderboardTree = nullptr; // Cây BST lưu điểm số

// Hàm thoát game
void exitGame() {
    cout << "Exiting game...\n";
    // Dọn dẹp bộ nhớ hoặc thực hiện các thao tác kết thúc nếu cần
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
        move = _getch(); // Nhận ký tự trực tiếp, không cần Enter

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
// Hiển thị menu chính
void mainMenu() {
    int choice;

    

    while (true) {

        //Xoa man hinh console
    #if defined(_WIN32)
            system("cls"); // Xóa màn hình trên Windows

    #endif
        // Menu chỉ hiển thị lại sau khi một hành động kết thúc
        cout << "1. Register\n";
        cout << "2. Login & New Game\n";
        cout << "3. Resume Game\n";
        cout << "4. Leaderboard\n";
        cout << "5. Exit\n";

        cout << "Choose an option: ";
        // Xử lý đầu vào sai
        while (!(cin >> choice)) {
            cout << "Invalid input! Please enter a number: ";
            cin.clear();
            cin.ignore(10000, '\n');
        }

        switch (choice) {
        case 1:
            userTree = registerUser(userTree); // Đăng ký người dùng mới
            break;
        case 2: {
            loginProcess(userTree); // Đăng nhập
            playGame();
            break;
        }
        case 3: {
            if (loadGame(gameBoard, BOARD_SIZE, score)) {
                cout << "Game loaded successfully!\n";
                playGame(); // Tiếp tục chơi game đã lưu
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
            exitGame(); // Thoát chương trình
            return;
            
        default:
            cout << "Invalid choice! Try again.\n";
        }
        // Pause to allow the user to see the output before clearing the screen again
        cout << "\nPress Enter to continue...";
        cin.ignore();
        cin.get();
    }
}