#include <iostream>
#include <string>
using namespace std;

#define MAX 100   // max size of the stacks

// stack for characters 
class CharStack {
    char arr[MAX];
    int top;
public:
    CharStack() { top = -1; }

    bool isEmpty() { return top == -1; }
    bool isFull() { return top == MAX - 1; }

    void push(char c) {
        if (isFull()) {
            cout << "Stack overflow!" << endl;
            return;
        }
        top++;
        arr[top] = c;
    }

    char pop() {
        if (isEmpty()) {
            cout << "Stack underflow!" << endl;
            return '\0';
        }
        char c = arr[top];
        top--;
        return c;
    }

    char peek() {
        if (isEmpty()) return '\0';
        return arr[top];
    }
};

// stack for doubles
class DoubleStack {
    double arr[MAX];
    int top;
public:
    DoubleStack() { top = -1; }

    bool isEmpty() { return top == -1; }

    void push(double x) {
        if (top == MAX - 1) {
            cout << "Stack overflow!" << endl;
            return;
        }
        top++;
        arr[top] = x;
    }

    double pop() {
        if (isEmpty()) {
            cout << "Stack underflow!" << endl;
            return 0;
        }
        double x = arr[top];
        top--;
        return x;
    }
};

// stack for strings
class StringStack {
    string arr[MAX];
    int top;
public:
    StringStack() { top = -1; }

    bool isEmpty() { return top == -1; }

    void push(string s) {
        if (top == MAX - 1) {
            cout << "Stack overflow!" << endl;
            return;
        }
        top++;
        arr[top] = s;
    }

    string pop() {
        if (isEmpty()) {
            cout << "Stack underflow!" << endl;
            return "";
        }
        string s = arr[top];
        top--;
        return s;
    }
};

// helper functions 

// checks if the character is an operator
bool isOperator(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/' || c == '^');
}

// checks if the character is an operand (letter or digit)
bool isOperand(char c) {
    return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'));
}

// higher number = higher priority
int precedence(char op) {
    if (op == '^') return 3;
    if (op == '*' || op == '/') return 2;
    if (op == '+' || op == '-') return 1;
    return 0;   // for brackets
}

// reverses a string 
string reverseString(string s) {
    string rev = "";
    for (int i = s.length() - 1; i >= 0; i--) {
        rev = rev + s[i];
    }
    return rev;
}

// power function 
double myPower(double base, double expo) {
    int e = (int)expo;
    double result = 1;
    for (int i = 0; i < e; i++) {
        result = result * base;
    }
    return result;
}

// does the calculation for one operator
double calculate(double a, double b, char op) {
    if (op == '+') return a + b;
    if (op == '-') return a - b;
    if (op == '*') return a * b;
    if (op == '/') {
        if (b == 0) {
            cout << "Error: division by zero!" << endl;
            return 0;
        }
        return a / b;
    }
    if (op == '^') return myPower(a, b);
    return 0;
}

string convertInfix(string expr, bool isPrefix) {
    CharStack st;
    string result = "";

    for (int i = 0; i < (int)expr.length(); i++) {
        char c = expr[i];

        if (c == ' ') {
            continue;   // skip spaces
        }
        else if (isOperand(c)) {
            result = result + c;   // operands go directly to the answer
        }
        else if (c == '(') {
            st.push(c);
        }
        else if (c == ')') {
            // pop till we find the opening bracket
            while (!st.isEmpty() && st.peek() != '(') {
                result = result + st.pop();
            }
            st.pop();   // removing the '(' also
        }
        else if (isOperator(c)) {
            // pop operators with higher (or equal) priority first
            while (!st.isEmpty() && st.peek() != '(') {
                int topPrec = precedence(st.peek());
                int curPrec = precedence(c);

                bool shouldPop;
                if (c == '^') {
                    // ^ goes right to left so the rule is different
                    if (isPrefix) shouldPop = (topPrec >= curPrec);
                    else shouldPop = (topPrec > curPrec);
                }
                else {
                    if (isPrefix) shouldPop = (topPrec > curPrec);
                    else shouldPop = (topPrec >= curPrec);
                }

                if (shouldPop) {
                    result = result + st.pop();
                }
                else {
                    break;
                }
            }
            st.push(c);
        }
    }

    // put whatever is left in the stack
    while (!st.isEmpty()) {
        result = result + st.pop();
    }

    return result;
}

// 1. infix to postfix
string infix_to_postfix(string expr) {
    return convertInfix(expr, false);
}

// 2. infix to prefix
// idea: reverse the expression, swap the brackets, convert like postfix, then reverse again
string infix_to_prefix(string expr) {
    string rev = reverseString(expr);

    // swap ( and )
    for (int i = 0; i < (int)rev.length(); i++) {
        if (rev[i] == '(') rev[i] = ')';
        else if (rev[i] == ')') rev[i] = '(';
    }

    string temp = convertInfix(rev, true);
    return reverseString(temp);
}

// 3. postfix to infix
string postfix_to_infix(string expr) {
    StringStack st;

    for (int i = 0; i < (int)expr.length(); i++) {
        char c = expr[i];

        if (c == ' ') continue;

        if (isOperand(c)) {
            string s = "";
            s = s + c;
            st.push(s);
        }
        else if (isOperator(c)) {
            // second operand comes out first
            string b = st.pop();
            string a = st.pop();
            string combined = "(" + a + c + b + ")";
            st.push(combined);
        }
    }

    return st.pop();
}

// 4. evaluate postfix
double evaluate_postfix(string expr) {
    DoubleStack st;

    for (int i = 0; i < (int)expr.length(); i++) {
        char c = expr[i];

        if (c == ' ') continue;

        if (c >= '0' && c <= '9') {
            st.push(c - '0');   // char to number
        }
        else if (isOperator(c)) {
            double b = st.pop();
            double a = st.pop();
            st.push(calculate(a, b, c));
        }
    }

    return st.pop();
}

// 5. evaluate prefix 
double evaluate_prefix(string expr) {
    DoubleStack st;

    for (int i = expr.length() - 1; i >= 0; i--) {
        char c = expr[i];

        if (c == ' ') continue;

        if (c >= '0' && c <= '9') {
            st.push(c - '0');
        }
        else if (isOperator(c)) {
            // here first pop is the FIRST operand
            double a = st.pop();
            double b = st.pop();
            st.push(calculate(a, b, c));
        }
    }

    return st.pop();
}

// ---------- main function with a menu ----------
int main() {
    int choice;
    string expr;

    do {
        cout << "\n===== Expression Converter =====" << endl;
        cout << "1. Infix to Postfix" << endl;
        cout << "2. Infix to Prefix" << endl;
        cout << "3. Postfix to Infix" << endl;
        cout << "4. Evaluate Postfix" << endl;
        cout << "5. Evaluate Prefix" << endl;
        cout << "0. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();   // to remove the enter key left in the buffer

        if (choice >= 1 && choice <= 5) {
            cout << "Enter expression (no need for spaces): ";
            getline(cin, expr);
        }

        if (choice == 1) {
            cout << "Postfix: " << infix_to_postfix(expr) << endl;
        }
        else if (choice == 2) {
            cout << "Prefix: " << infix_to_prefix(expr) << endl;
        }
        else if (choice == 3) {
            cout << "Infix: " << postfix_to_infix(expr) << endl;
        }
        else if (choice == 4) {
            cout << "Result: " << evaluate_postfix(expr) << endl;
        }
        else if (choice == 5) {
            cout << "Result: " << evaluate_prefix(expr) << endl;
        }
        else if (choice != 0) {
            cout << "Wrong choice, try again." << endl;
        }

    } while (choice != 0);

    cout << "Bye!" << endl;
    return 0;
}