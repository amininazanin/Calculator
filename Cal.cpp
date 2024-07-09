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

vector<string> tokenize(const string& input) {
    vector<string> tokens;
    string token;
    bool expectOperand = true;

    for (size_t i = 0; i < input.length(); ++i) {
        char c = input[i];
        if (isspace(c)) continue;

        if (isdigit(c) || (c == '-' && expectOperand && i+1 < input.length() && isdigit(input[i+1]))) {
            token = c;
            while (++i < input.length() && (isdigit(input[i]) || input[i] == '.')) {
                token += input[i];
            }
            --i;
            tokens.push_back(token);
            expectOperand = false;
        } else if (isalpha(c)) {
            token = c;
            while (++i < input.length() && isalpha(input[i])) {
                token += input[i];
            }
            --i;
            tokens.push_back(token);
            expectOperand = false;
        } else if (c == '-' && expectOperand) {
            tokens.push_back("0");  // Insert a zero for unary minus
            tokens.push_back(string(1, c));
            expectOperand = false;
        } else {
            tokens.push_back(string(1, c));
            expectOperand = (c == '(' || isOperator(string(1, c)));
        }
    }
    return tokens;
}

string infixToPostfix(const vector<string>& tokens) {
    Stack<string> s;
    stringstream postfix;

    for (const auto& token : tokens) {
        if (isdigit(token[0]) || (token[0] == '-' && token.length() > 1 && isdigit(token[1]))) {
            postfix << token << " ";
        } else if (token == "(") {
            s.push(token);
        } else if (token == ")") {
            while (!s.isEmpty() && s.peek() != "(") {
                postfix << s.pop() << " ";
            }
            if (s.isEmpty()) throw runtime_error("Mismatched parentheses");
            s.pop();
        } else if (isOperator(token) || isFunction(token)) {
            while (!s.isEmpty() && s.peek() != "(" && 
                   (isFunction(s.peek()) || getPrecedence(s.peek()) > getPrecedence(token) ||
                    (getPrecedence(s.peek()) == getPrecedence(token) && token != "^"))) {
                postfix << s.pop() << " ";
            }
            s.push(token);
        } else {
            throw runtime_error("Invalid token: " + token);
        }
    }

    while (!s.isEmpty()) {
        if (s.peek() == "(") throw runtime_error("Mismatched parentheses");
        postfix << s.pop() << " ";
    }

    return postfix.str();
}


int main()
{
    return 0;
}