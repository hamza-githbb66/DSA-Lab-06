#include <iostream>
#include <string>
using namespace std;

#define MAX 100

// stack of chars for brackets 
class CharStack {
    char arr[MAX];
    int top;
public:
    CharStack() { top = -1; }

    bool isEmpty() { return top == -1; }

    void push(char c) {
        if (top == MAX - 1) {
            cout << "Stack is full!" << endl;
            return;
        }
        top++;
        arr[top] = c;
    }

    char pop() {
        if (isEmpty()) return '\0';
        char c = arr[top];
        top--;
        return c;
    }

    char peek() {
        if (isEmpty()) return '\0';
        return arr[top];
    }
};

// stack of strings for tag names
class StringStack {
    string arr[MAX];
    int top;
public:
    StringStack() { top = -1; }

    bool isEmpty() { return top == -1; }

    void push(string s) {
        if (top == MAX - 1) {
            cout << "Stack is full!" << endl;
            return;
        }
        top++;
        arr[top] = s;
    }

    string pop() {
        if (isEmpty()) return "";
        string s = arr[top];
        top--;
        return s;
    }

    string peek() {
        if (isEmpty()) return "";
        return arr[top];
    }
};

// ======================= PART A =======================
// checks if ( ) { } [ ] are balanced and nested properly
bool isValidBrackets(string s) {
    CharStack st;

    for (int i = 0; i < (int)s.length(); i++) {
        char c = s[i];

        // opening bracket -> push it
        if (c == '(' || c == '{' || c == '[') {
            st.push(c);
        }
        // closing bracket -> check with the top of stack
        else if (c == ')' || c == '}' || c == ']') {
            if (st.isEmpty()) {
                return false;   // closing came but nothing was opened
            }

            char top = st.pop();

            if (c == ')' && top != '(') return false;
            if (c == '}' && top != '{') return false;
            if (c == ']' && top != '[') return false;
        }
        // other characters are ignored
    }

    // if stack is not empty then some bracket was never closed
    return st.isEmpty();
}

// ======================= PART B =======================
struct MarkupResult {
    bool valid;
    string errors[50];
    int errorCount;
};

// adds an error message in the result
void addError(MarkupResult &r, string msg) {
    if (r.errorCount < 50) {
        r.errors[r.errorCount] = msg;
        r.errorCount++;
    }
    r.valid = false;
}

MarkupResult validate_markup(string s) {
    MarkupResult result;
    result.valid = true;
    result.errorCount = 0;

    StringStack st;
    int i = 0;
    int n = s.length();

    while (i < n) {
        if (s[i] == '<') {
            // find where this tag ends
            int end = i + 1;
            while (end < n && s[end] != '>') {
                end++;
            }

            if (end == n) {
                addError(result, "Tag starting at position " + to_string(i) + " is missing '>'");
                break;   // nothing more to read
            }

            // text inside < >
            string tag = s.substr(i + 1, end - i - 1);

            if (tag.length() == 0) {
                addError(result, "Empty tag <> at position " + to_string(i));
            }
            else if (tag[0] == '/') {
                // closing tag like </div>
                string name = tag.substr(1);

                if (st.isEmpty()) {
                    addError(result, "Closing tag </" + name + "> has no opening tag");
                }
                else if (st.peek() == name) {
                    st.pop();   // matched, all good
                }
                else {
                    addError(result, "Expected </" + st.peek() + "> but found </" + name + ">");
                    // pop the wrong one so we dont get the same error again and again
                    st.pop();
                }
            }
            else if (tag[tag.length() - 1] == '/') {
                // self closing tag like <br/>, nothing to push
            }
            else {
                // opening tag like <div>
                // we only want the name so stop at the first space
                string name = "";
                for (int k = 0; k < (int)tag.length(); k++) {
                    if (tag[k] == ' ') break;
                    name = name + tag[k];
                }
                st.push(name);
            }

            i = end + 1;   // move after '>'
        }
        else {
            i++;   // normal text, skip
        }
    }

    // anything still in the stack was never closed
    while (!st.isEmpty()) {
        addError(result, "Tag <" + st.pop() + "> was never closed");
    }

    return result;
}

// ---------- main  function----------
int main() {
    int choice;
    string input;

    do {
        cout << "\n===== Validators =====" << endl;
        cout << "1. Bracket Validator" << endl;
        cout << "2. HTML/XML Tag Validator" << endl;
        cout << "3. Run my test cases" << endl;
        cout << "0. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();

        if (choice == 1) {
            cout << "Enter string: ";
            getline(cin, input);

            if (isValidBrackets(input)) cout << "Valid - brackets are balanced" << endl;
            else cout << "Invalid - brackets are NOT balanced" << endl;
        }
        else if (choice == 2) {
            cout << "Enter markup: ";
            getline(cin, input);

            MarkupResult r = validate_markup(input);

            if (r.valid) {
                cout << "Valid markup" << endl;
            }
            else {
                cout << "Invalid markup. Errors:" << endl;
                for (int i = 0; i < r.errorCount; i++) {
                    cout << "  " << (i + 1) << ". " << r.errors[i] << endl;
                }
            }
        }
        else if (choice == 3) {
            // some test cases 
            string b[6] = { "{[()]}", "([)]", "((a+b)", "a+b)", "", "{[(a+b)*c]-d}/e" };
            cout << "\n--- Bracket tests ---" << endl;
            for (int i = 0; i < 6; i++) {
                cout << "\"" << b[i] << "\" -> ";
                if (isValidBrackets(b[i])) cout << "Valid" << endl;
                else cout << "Invalid" << endl;
            }

            string m[6] = {
                "<html><body><p>Hello</p></body></html>",
                "<div><p>Hi</div></p>",
                "<div><br/><p>text</p></div>",
                "<ul><li>one</li>",
                "</div>",
                "<a href=\"x.com\">link</a>"
            };
            cout << "\n--- Markup tests ---" << endl;
            for (int i = 0; i < 6; i++) {
                cout << m[i] << endl;
                MarkupResult r = validate_markup(m[i]);
                if (r.valid) {
                    cout << "   -> Valid" << endl;
                }
                else {
                    cout << "   -> Invalid" << endl;
                    for (int j = 0; j < r.errorCount; j++) {
                        cout << "      " << r.errors[j] << endl;
                    }
                }
            }
        }
        else if (choice != 0) {
            cout << "Wrong choice." << endl;
        }

    } while (choice != 0);

    cout << "Bye!" << endl;
    return 0;
}