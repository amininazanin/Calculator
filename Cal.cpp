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

int main()
{
    return 0;
}