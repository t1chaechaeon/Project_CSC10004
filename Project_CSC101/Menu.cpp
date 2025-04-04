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

    bool running = true;

    while (running) {
    #if defined(_WIN32)
        system("cls"); // Xóa màn hình trên Windows

    #endif
        printMatrix(gameBoard, BOARD_SIZE, score);
        cout << "Use arrow keys to move, U - Undo, R - Redo, S - save & Exit\n";

        char move = _getch();

        if (move == -32) { // Kiểm tra nếu là phím mũi tên
            move = _getch(); // Nhận giá trị phím mũi tên tiếp theo

            switch (move) {
            case 72: running = move_to_Up(gameBoard, BOARD_SIZE, score); break;
            case 80: running = move_to_Down(gameBoard, BOARD_SIZE, score); break;
            case 75: running = move_to_Left(gameBoard, BOARD_SIZE, score); break;
            case 77: running = move_to_Right(gameBoard, BOARD_SIZE, score); break;
            default: cout << "Invalid move! Try again.\n";
                cout << "Press Enter to continue.";
                cin.get();
               
                playGame();
            }
        }
        else {
            switch (toupper(move)) {

            case 'U': undo(gameBoard, BOARD_SIZE, score); break;
            case 'R': redo(gameBoard, BOARD_SIZE, score); break;
            case 'S': {
                saveGame(gameBoard, BOARD_SIZE, score);
                cout << "Game saved! Returning to menu...\n";
                return;
            }
            default:
                cout << "Invalid move! Try again.\n";
                cout << "\nPree Enter to continue...";
                cin.get();
                
                playGame();
            }
        }

        if (checkWin(gameBoard, BOARD_SIZE)) {
            cout << "Congratulations! You won the game!\n";
            cout << "\nPree Enter to return to menu...";
            cin.get();
          
            mainMenu();
        }
        else if (checkGameOver(gameBoard, BOARD_SIZE)) {
            cout << "Game Over!\n";
            cout << "\nPree Enter to return to menu...";
            cin.get();
            
            mainMenu();
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

    int choice = 1; // Lựa chọn đầu tiên (mặc định)
    const int numOptions = 5;

    while (true) {

        //Xoa man hinh console
    #if defined(_WIN32)
        system("cls"); // Xóa màn hình trên Windows

    #endif
        // Menu chỉ hiển thị lại sau khi một hành động kết thúc
        cout << "Use arrow keys to navigate, Enter to select:\n\n";

        if (choice == 1) cout << ">> 1. Register\n"; else cout << "   1. Register\n";
        if (choice == 2) cout << ">> 2. Login & New Game\n"; else cout << "   2. Login & New Game\n";
        if (choice == 3) cout << ">> 3. Resume Game\n"; else cout << "   3. Resume Game\n";
        if (choice == 4) cout << ">> 4. Leaderboard\n"; else cout << "   4. Leaderboard\n";
        if (choice == 5) cout << ">> 5. Exit\n"; else cout << "   5. Exit\n";

        char key = _getch(); // Bắt phím

        if (key == 72) {
            if (choice > 1) {
                --choice;
            }
            else {
                choice = numOptions;
            }
        }
        else if (key == 80) {
            if (choice < numOptions) {
                ++choice;
            }
            else {
                choice = 1;
            }
        }
        else  if (key == 13) { // Enter
            system("cls");
            cout << "You selected option: " << choice << endl;


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
            cout << "\nPress Enter to continue...";
            _getch();
        }
    }
}