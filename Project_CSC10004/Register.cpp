#include "Register.h"
#include <fstream>

UserNode* registerUser(UserNode* root) {
    string username, password, confirmPassword;

    cout << "Enter new username: ";
    cin >> username;

    if (findUser(root, username) != nullptr) {
        cout << "Username already exists! Try again.\n";
        return root;
    }

    cout << "Enter password: ";
    cin >> password;
    cout << "Confirm password: ";
    cin >> confirmPassword;

    if (!confirmPassWord(password, confirmPassword)) {
        cout << "Passwords do not match! Try again.\n";
        return root;
    }

    // Chèn user vào BST
    root = insertUserNode(root, username, password);

    // Ghi xuống file ngay sau khi đăng ký
    ofstream outFile("users.dat", ios::binary | ios::app);
    if (outFile) {
        outFile.write(reinterpret_cast<char*>(&username), sizeof(username));
        size_t hashedPwd = hashPassWord(password);
        outFile.write(reinterpret_cast<char*>(&hashedPwd), sizeof(hashedPwd));
        outFile.close();
    }

    cout << "Registration successful! Account saved.\n";
    return root;
}