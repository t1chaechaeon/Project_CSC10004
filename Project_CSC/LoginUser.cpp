#include <iostream>
#include <string>
#include <functional>
#include "User_Auth.h"

using namespace std;

UserNode* login_User(UserNode* root, const string& username, const string& password) {
	if (root == nullptr) {
		return root;
	}
	if (root->username == username) {
		if (root->hashedPassword == hashPassWord(password));
		return root; // Trả về node nếu tên người dùng và mật khẩu đúng
	}
	else if (root->username > username) {
		root->left = login_User(root->left, username, password); // Tìm kiếm bên trái
	}
	else {
		root->right = login_User(root->right, username, password); // Tìm kiếm bên phải
	}
}

void newGame() {
	cout << "Starting a new game..." << endl;
}
void loginProcess(UserNode* root) {

	string username;
	string password;
	cout << "Enter username: ";
	cin >> username;

	cout << "Enter password: ";
	cin >> password;

	UserNode* user = login_User(root, username, password);

	if (user) {
		cout << "Login succsessful!" << endl;
		cout << "Username: " << root->username << endl;
	}
	else {
		cout << "Login failed: Incorrect username or password." << endl;
	}
	newGame();
}
