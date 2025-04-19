#include "Stack.h"
#include <cstring>

// Khởi tạo stack
void initStack(Stack& s) {
    s.top = nullptr;
}

// Kiểm tra stack rỗng
bool isEmpty(Stack& s) {
    return s.top == nullptr;
}

// Hàm sao chép ma trận (deep copy)
int** cloneMatrix(int** matrix, int n) {
    int** newMatrix = new int* [n];
    for (int i = 0; i < n; ++i) {
        newMatrix[i] = new int[n];
        std::memcpy(newMatrix[i], matrix[i], n * sizeof(int));
    }
    return newMatrix;
}

// Push trạng thái vào stack
void push(Stack& s, int** matrix, int n, unsigned int score) {
    StackNode* newNode = new StackNode;
    newNode->state.matrix = cloneMatrix(matrix, n);
    newNode->state.score = score;
    newNode->next = s.top;
    s.top = newNode;
}

// Pop trạng thái khỏi stack
bool pop(Stack& s, GameState& state, int n) {
    if (isEmpty(s)) return false;

    StackNode* temp = s.top;
    s.top = s.top->next;

    state.matrix = temp->state.matrix;
    state.score = temp->state.score;

    delete temp;
    return true;
}

// Giải phóng toàn bộ stack
void clearStack(Stack& s, int n) {
    GameState temp;
    while (pop(s, temp, n)) {
        if (temp.matrix != nullptr) {
            for (int i = 0; i < n; ++i)
                delete[] temp.matrix[i];
            delete[] temp.matrix;
            temp.matrix = nullptr;
        }
    }
}
