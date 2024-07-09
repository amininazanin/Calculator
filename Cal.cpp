#include <iostream>
#include <string>
#include <sstream>
#include <cmath>
#include <stdexcept>
#include <cctype>
#include <map>
#include <functional>
#include <vector>
#include <algorithm>

using namespace std;

template <typename T>
class Stack
{
private:
    T *arr;
    int top;
    int capacity;

public:
    Stack(int size = 100)
    {
        arr = new T[size];
        capacity = size;
        top = -1;
    }

    ~Stack()
    {
        delete[] arr;
    }

    void push(T x)
    {
        if (isFull())
        {
            throw runtime_error("Stack Overflow");
        }
        arr[++top] = x;
    }

    T pop()
    {
        if (isEmpty())
        {
            throw runtime_error("Stack Underflow");
        }
        return arr[top--];
    }

    T peek()
    {
        if (isEmpty())
        {
            throw runtime_error("Stack is Empty");
        }
        return arr[top];
    }

    bool isEmpty()
    {
        return top == -1;
    }

    bool isFull()
    {
        return top == capacity - 1;
    }

    int size()
    {
        return top + 1;
    }
};

bool isOperator(const string& token) {
    static const string operators = "+-*/^";
    return token.length() == 1 && operators.find(token) != string::npos;
}

bool isFunction(const string& token) {
    static const vector<string> functions = {"sin", "cos", "tan", "asin", "acos", "atan", "log", "ln", "sqrt", "abs"};
    return find(functions.begin(), functions.end(), token) != functions.end();
}

int getPrecedence(const string& op) {
    if (op == "^") return 4;
    if (op == "*" || op == "/") return 3;
    if (op == "+" || op == "-") return 2;
    return 0;
}

int main()
{
    return 0;
}