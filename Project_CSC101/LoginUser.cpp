#include <iostream>
#include <string>
#include <cstdlib> // Thêm thư viện để sử dụng exit()
#include "LoginUser.h"

using namespace std;


void newGame() {
	cout << "Starting a new game..." << endl;
	cout << "Press Enter to continue.";
	cin.get();
	cin.ignore();
}

void loginProcess(UserNode* root) {


	string password;
	int attempt = 3;
	string username;
	cout << "Enter username: ";
	cin >> username;
	UserNode* user = searchUser(root, username, "");
	if (!user) {
		cout << "Account not found\n";
		return;
	}

	while (attempt--) {
		
		cout << "Enter password: ";
		cin >> password;

		if (simpleHash(password) == user->hashedPassword) {
			cout << "Login succsessful!" << endl;
			newGame();
			return;
		}
		else {
			if (attempt > 0) {
				cout << "Login failed: Incorrect password.You have " << attempt << " attemps left.\n";
			}
		}
	}
	cout << "Too many failed attempts. Exiting progrma." << endl;
	exit(0); // Dừng chương trình ngay lập tức
}