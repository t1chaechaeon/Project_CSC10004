#include <iostream>
#include <string>
#include <functional>
#include <cstdlib> // Thêm thư viện để sử dụng exit()
#include "User_Auth.h"

using namespace std;

UserNode* login_User(UserNode* root, const string& username, const string& password) {
	if (root == nullptr) {
		return root;
	}
	if (root->username == username) {
		if (root->hashedPassword == hashPassWord(password)) {
			return root; // Trả về node nếu tên người dùng và mật khẩu đúng
		}
		else {
			return nullptr; //Trả về nullptr nếu mật khẩu sai
		}
	}
	else if (root->username > username) {
		return login_User(root->left, username, password);// Tìm kiếm bên trái
	}
	else {
		return login_User(root->right, username, password); // Tìm kiếm bên phải
	}
}

void newGame() {
	cout << "Starting a new game..." << endl;
	cout << "Press Enter to continue.";
	cin.get();
	cin.ignore();
}

void loginProcess(UserNode* root) {

	string username;
	string password;
	int attempt = 0;
	const int MAX_ATTEMP = 3;

	while (attempt < MAX_ATTEMP) {
		cout << "Enter username: ";
		cin >> username;

		cout << "Enter password: ";
		cin >> password;

		UserNode* user = login_User(root, username, password);

		if (user) {
			cout << "Login succsessful!" << endl;
			cout << "Username: " << root->username << endl;
			newGame();
			return;
		}
		else {
			attempt++;
			cout << "Login failed: Incorrect username or password." << endl;

			if (attempt == MAX_ATTEMP) {
				cout << "Too many failed attempts. Exiting progrma." << endl;
				exit(0); // Dừng chương trình ngay lập tức
			}
		}
	}
}