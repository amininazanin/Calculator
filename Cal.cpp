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

double evaluatePostfix(const string& postfix) {
    Stack<double> s;
    stringstream ss(postfix);
    string token;
    map<string, function<double(double)>> unaryFunctions = {
        {"sin", [](double x) { return sin(x); }},
        {"cos", [](double x) { return cos(x); }},
        {"tan", [](double x) { return tan(x); }},
        {"asin", [](double x) { return asin(x); }},
        {"acos", [](double x) { return acos(x); }},
        {"atan", [](double x) { return atan(x); }},
        {"log", [](double x) { return log10(x); }},
        {"ln", [](double x) { return log(x); }},
        {"sqrt", [](double x) { return sqrt(x); }},
        {"abs", [](double x) { return abs(x); }}
    };

    while (ss >> token) {
        if (isdigit(token[0]) || (token[0] == '-' && token.length() > 1 && isdigit(token[1]))) {
            s.push(stod(token));
        } else if (isOperator(token)) {
            if (s.size() < 2) throw runtime_error("Invalid expression: insufficient operands for operator " + token);
            double b = s.pop();
            double a = s.pop();
            double result;
            switch (token[0]) {
                case '+': result = a + b; break;
                case '-': result = a - b; break;
                case '*': result = a * b; break;
                case '/':
                    if (b == 0) throw runtime_error("Division by zero");
                    result = a / b;
                    break;
                case '^': result = pow(a, b); break;
            }
            s.push(result);
            cout << a << " " << token << " " << b << " = " << result << endl;
        } else if (isFunction(token)) {
            if (s.isEmpty()) throw runtime_error("Invalid expression: insufficient operands for function " + token);
            double a = s.pop();
            double result = unaryFunctions[token](a);
            s.push(result);
            cout << token << "(" << a << ") = " << result << endl;
        }
    }

    if (s.size() != 1) throw runtime_error("Invalid expression: too many operands");
    return s.peek();
}



int main()
{string expression;
    cout << "Enter an arithmetic expression: ";
    getline(cin, expression);

    try {
        vector<string> tokens = tokenize(expression);
        string postfix = infixToPostfix(tokens);
        cout << "Postfix expression: " << postfix << endl;
        cout << "Step-by-step evaluation:" << endl;
        double result = evaluatePostfix(postfix);
        cout << "Final result: " << result << endl;
    } catch (const exception& e) {
        cout << "Error: " << e.what() << endl;
    }

    return 0;
}