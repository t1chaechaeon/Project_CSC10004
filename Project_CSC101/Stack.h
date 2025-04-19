#pragma once

struct GameState {
    int** matrix;
    unsigned int score;
};

struct StackNode {
    GameState state;
    StackNode* next;
};

struct Stack {
    StackNode* top;
};

void initStack(Stack& s);
bool isEmpty(Stack& s);
void push(Stack& s, int** matrix, int n, unsigned int score);
bool pop(Stack& s, GameState& state, int n);
void clearStack(Stack& s, int n);
int** cloneMatrix(int** matrix, int n);
