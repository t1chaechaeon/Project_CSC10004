#include "Menu.h"
#include "Stack.h"
#include <iostream>
#include <fstream>
#include <cctype>


using namespace std;
std::string currentUsername;

extern Stack undoStack;
extern Stack redoStack;

#define BOARD_SIZE 4 // Kích thước bảng 4x4
#define SAVE_FILE "savegame.dat";

// Biến lưu trạng thái game
int** gameBoard = nullptr;
unsigned int score = 0;
UserNode* userTree = nullptr;        // Cây BST lưu user
BSTNode* leaderboardTree = nullptr;  // Cây BST lưu điểm số
UserNode* loggedInUser = nullptr;

// Đặt lại trạng thái trò chơi về ban đầu
void resetGameState() {
    score = 0;
    if (gameBoard != nullptr) {
        freeMatrix(gameBoard, BOARD_SIZE);
        gameBoard = nullptr;
    }
    clearStack(undoStack, BOARD_SIZE);
    clearStack(redoStack, BOARD_SIZE);
}
// Hàm thoát game
void exitGame() {
    cout << "Exiting game...\n";
}

// Chơi game 2048
void playGame(bool isResumed = false){
   
    if (!isResumed) {
        void resetGameState(); //// Reset toàn bộ trước khi bắt đầu game mới
        createMatrix(gameBoard, BOARD_SIZE); // Chỉ tạo khi không resume
        spawnNewTile(gameBoard, BOARD_SIZE);
        spawnNewTile(gameBoard, BOARD_SIZE);
    }

    bool running = true;

    while (running) {
#if defined(_WIN32)
        system("cls");
#endif
        printMatrix(gameBoard, BOARD_SIZE, score);
        cout << "Use arrow keys to move, U - Undo, R - Redo, S - Save & Exit\n";

        char move = _getch();
        bool moved = false;

        if (move == -32) {  // Kiểm tra di chuyển với phím mũi tên
            move = _getch();

            switch (move) {
            case 72: moved = move_to_Up(gameBoard, BOARD_SIZE, score); break;
            case 80: moved = move_to_Down(gameBoard, BOARD_SIZE, score); break;
            case 75: moved = move_to_Left(gameBoard, BOARD_SIZE, score); break;
            case 77: moved = move_to_Right(gameBoard, BOARD_SIZE, score); break;
            default:
                cout << "Invalid move! Try again.\n";
                cout << "Press Enter to continue.";
                cin.get();
                continue;
            }

            if (moved) {
                push(undoStack, gameBoard, BOARD_SIZE, score);  // Lưu trạng thái vào undoStack
                clearStack(redoStack, BOARD_SIZE);               // Xoá redoStack sau mỗi di chuyển
                spawnNewTile(gameBoard, BOARD_SIZE);             // Tạo ô mới
            }
        }
        else {
            switch (toupper(move)) {
            case 'U':
                undo(gameBoard, BOARD_SIZE, score); // Thực hiện Undo
                break;
            case 'R':
                redo(gameBoard, BOARD_SIZE, score); // Thực hiện Redo
                break;
            case 'S':
                saveGame(gameBoard, BOARD_SIZE, score);

                cout << "Game saved! Returning to menu...\n";
                return;

            default:
                cout << "Invalid key! Try again.\n";
                cout << "Press Enter to continue...";
                cin.get();
            }
        }

        // Kiểm tra thắng/thua
        if (checkWin(gameBoard, BOARD_SIZE)) {

            cout << "Congratulations! You won the game!\n";

            // Lưu vào leaderboard
            leaderboardTree = insertNode(leaderboardTree, userTree->username, score);
            ofstream outFile("leaderboard.dat", ios::binary);
            saveToFile(leaderboardTree, outFile);
            outFile.close();

            // Xoá save file sau khi kết thúc
            deleteSavedGame();
            freeMatrix(gameBoard, BOARD_SIZE);

            cout << "\nPress Enter to return to menu...";
            cin.get();
            mainMenu();
            return;
        }

            if (checkGameOver(gameBoard, BOARD_SIZE)) {
   
                cout << "Game Over!\n";
                // Lưu vào leaderboard
                leaderboardTree = insertNode(leaderboardTree,userTree->username ,score);
                ofstream outFile("leaderboard.dat", ios::binary);
                saveToFile(leaderboardTree, outFile);
                outFile.close();

                // Xoá save file sau khi kết thúc
                deleteSavedGame();
                freeMatrix(gameBoard, BOARD_SIZE);
                cout << "\nPress Enter to return to menu...";
                cin.get();
                mainMenu();
                return;
            }
    }
}

// Hiển thị menu chính
void mainMenu() {
    int choice = 1;
    const int numOptions = 5;

    while (true) {
#if defined(_WIN32)
        system("cls");
#endif
        cout << "Use arrow keys to navigate, Enter to select:\n\n";

        if (choice == 1) cout << ">> 1. Register\n"; else cout << "   1. Register\n";
        if (choice == 2) cout << ">> 2. Login & New Game\n"; else cout << "   2. Login & New Game\n";
        if (choice == 3) cout << ">> 3. Resume Game\n"; else cout << "   3. Resume Game\n";
        if (choice == 4) cout << ">> 4. Leaderboard\n"; else cout << "   4. Leaderboard\n";
        if (choice == 5) cout << ">> 5. Exit\n"; else cout << "   5. Exit\n";

        char key = _getch();

        if (key == 72) { // Arrow up
            if (choice > 1) --choice;
            else choice = numOptions;
        }
        else if (key == 80) { // Arrow down
            if (choice < numOptions) ++choice;
            else choice = 1;
        }
        else if (key == 13) { // Enter
            system("cls");
            cout << "You selected option: " << choice << endl;

            switch (choice) {
            case 1:
                userTree = resgisterUserProcess(userTree);
                break;
            case 2:

                // Reset toàn bộ trạng thái trước khi đăng nhập tài khoản mới
                resetGameState();

                // Đăng nhập
                loginProcess(userTree);
                playGame(false);

                break;
            case 3:
                if (loadGame(gameBoard, BOARD_SIZE, score)) {
                    cout << "Game loaded successfully!\n";
                    playGame(true); // Đánh dấu là đang resume
                }
                else {
                    cout << "No saved game found!\n";
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
                break;
            }
            case 5:
                exitGame();
                return;
            default:
                cout << "Invalid choice! Try again.\n";
            }
            cout << "\nPress Enter to continue...";
            (void)_getch();
        }
    }
}
