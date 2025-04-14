#include "Stack.h"
#include <cstring>

// Khởi tạo Stack
void initStack(Stack& s) {
    s.top = nullptr;
}

// Kiểm tra xem Stack có rỗng không
bool isEmpty(Stack& s) {
    return s.top == nullptr;
}

// Hàm cloneMatrix giúp sao chép ma trận
int** cloneMatrix(int** matrix, int n) {
    int** newMatrix = new int* [n];
    for (int i = 0; i < n; ++i) {
        newMatrix[i] = new int[n];
        std::memcpy(newMatrix[i], matrix[i], n * sizeof(int));
    }
    return newMatrix;
}


// Hàm push để lưu trạng thái vào stack
void push(Stack& s, int** matrix, int n, unsigned int score) {
    StackNode* newNode = new StackNode;
    newNode->state.matrix = cloneMatrix(matrix, n);
    newNode->state.score = score;
    newNode->next = s.top;
    s.top = newNode;
}

// Hàm pop để lấy trạng thái từ stack
bool pop(Stack& s, GameState& state, int n) {
    if (isEmpty(s)) return false;

    StackNode* temp = s.top;
    s.top = s.top->next;
    state.matrix = temp->state.matrix;
    state.score = temp->state.score;
    delete temp;
    return true;
}

// Hàm giải phóng bộ nhớ của stack
void clearStack(Stack& s, int n) {
    GameState temp;
    while (pop(s, temp, n)) {
        for (int i = 0; i < n; ++i)
            delete[] temp.matrix[i];
        delete[] temp.matrix;
    }
}
