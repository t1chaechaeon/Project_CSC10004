
#include "LoginUser.h"

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
// Hàm bắt đầu trò chơi
void newGame() {
	cout << "Starting a new game..." << endl;
}
// Hàm đăng nhập có giới hạn số lần thử
void loginProcess(UserNode* root) {

	string username;
	string password;
	cout << "Enter username: ";
	cin >> username;


	int attempt = 0;
	const int maxAttempts = 3;

	while (attempt < maxAttempts) {
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
			cout << "Login failed: Incorrect username or password." << endl;
			++attempt;
		}
	}
	cout << "Too may faled attempt. Please reset your password or try again later." << endl;
}
