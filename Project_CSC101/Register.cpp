#include "Register.h"
#include <fstream>

// Xác thực lại mật khẩu 2 lần
bool confirmPassWord(const string& passWord1, const string& passWord2) {
    return passWord1 == passWord2;
}
// Đăng ký + nhập dữ liệu
UserNode* resgisterUserProcess(UserNode* root) {
    string username;
    string password1;
    string password2;
    cout << "Enter username: ";
    cin >> username;
    if (searchUser(root, username, "") != nullptr) {
        cout << "Username already exists! Try again.\n";
        return root;
    }

    cout << "Enter password: ";
    cin >> password1;
    
    cout << "Enter password again: ";
    cin >> password2;

    if (!confirmPassWord(password1, password2)) {
        cout << "The password you enterd is incorrect. Please try again.";
        return root;
    }

    root = insertUserNode(root, username, password1);
    cout << "Registration successful.\n";
    return root;
}