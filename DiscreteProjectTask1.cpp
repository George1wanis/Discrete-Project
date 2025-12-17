// George Wanis Ayed Wanis 2300280 Task 1 Team 31

// Resources:
// https://youtu.be/Wz85Hiwi5MY - Comp Sci in 5: Shunting Yard Algorithm
// https://youtu.be/bebqXO8H4eA - Comp Sci in 5: Post Fix Stack Evaluator
// https://youtu.be/KiB0vRi2wlc - Maps in C++ (std::map and std::unordered_map)
// https://youtu.be/unh6aK8WMwM - DIY Programming Language #1: The Shunting Yard Algorithm


// The assigned example to our team:

// The children are playing Football or Tennis.
// If they are watching TV or playing Tennis, then they are not playing Football.
// Therefore, they are not playing Football or they are not watching TV.

// F|T
// V|T>~F
// ~F|~V

// The truth table for this shall be provided in the paper work handed on LMS
// The Program contains 5 functions and a main

// Example to show The shunting yard algorithm to dr. Betty:
// (P&Q|(M&B))
//  O:PQ&MB&|
//  S:

#include <iostream>
#include <string>
#include <vector>

using namespace std;
int errorsCatched = 0;

int precedence(char op) { // Operator precedence
    if (op == '~') return 5;
    if (op == '&') return 4;
    if (op == '|') return 3;
    if (op == '>') return 2;
    if (op == '=') return 1;
    return 0;
}


string toPostfix(const string &infix) { // 3+4 //34+
    vector<char> st;   // this is our holding pile
    string output; // this is the main string

    for (char c : infix) { //loops on each charachter in the given infix proposition
        if (isspace((unsigned char)c)) continue;

        else if (isalpha((unsigned char)c)) {
            output += c; // character added to the main string
        }
        else if (c == '(') {
            st.push_back(c); // added to the holding pile
        }
        else if (c == ')') {
            while (!st.empty() && st.back() != '(') { // pile flushing
                output += st.back();
                st.pop_back();
            }
            if (!st.empty()) st.pop_back(); // remove '('
        }
        else { // operator
            while (!st.empty() && precedence(st.back()) >= precedence(c)) {
                output += st.back(); // pile flushing
                st.pop_back();
            }
            st.push_back(c);
        }
    }

    while (!st.empty()) { // pile flushing after finishing the main input string
        output += st.back();
        st.pop_back();
    }
    return output;
}

int variablesIndexInVector(const vector<char>& vars, char c) { // takes a vector and a charcter and returns the character's index in that vector
    for (int i = 0; i < (int)vars.size(); i++)
        if (vars[i] == c) return i;
    return -1;
}

int evaluateStatementInPostFix(const string &postfix, const vector<char>& vars, const vector<int>& values) { // Evaluate the value of a certain premise or conclusion in postfix notation based on a specific truth-table row
// takes the the premise or the conclusion string in postfix notation, the vector of variables, and the values of the variables in a certain row we wish to evaluate the premise or the conclusion in.
//A&B>T
//AB&T>
// {A, B, T}
// {0, 0, 0}
    vector<int> st; // pile
    for (char c : postfix) {
        if (isalpha((unsigned char)c)) {
            st.push_back(values[variablesIndexInVector(vars, c)]); // we push our value of our variable to the pile
        }
        else if (c == '~') {
            if (st.empty()) { // this caches the invalid input "~" and "A&~". we go through this if the expression is malformed, and this means to Stop evaluating the expression it is invalid and treat it as false.
                errorsCatched = 1;
                return 0;
            }
            int x = st.back(); st.pop_back();
            st.push_back(!x);
        }
        else {
            if (st.size() < 2){ // It catches invalid expressions where a binary operator does not have enough operands. example for this are the statements "A&" and "&AB".
                errorsCatched = 1;
                return 0;
            }
            int b = st.back(); st.pop_back();
            int a = st.back(); st.pop_back();

            if (c == '&') st.push_back(a && b);
            else if (c == '|') st.push_back(a || b);
            else if (c == '>') st.push_back((!a) || b);
            else if (c == '=') st.push_back(((!a) || b) && ((!b) || a));
        }
    }
    if (st.size() != 1) { // if the pile contains more or less than a single element
        errorsCatched = 1; // mark an error
        return 0;
    }
    return st.back(); // if the function ran smoothly and no errors was catched
}

vector<char> extractVariables(const vector<string>& premises, const string& conclusion) {
    vector<char> vars;

    for (const auto& p : premises) { // loop on each premise
        for (char c : p) { // loop on each character in the premise
            if (!isalpha((unsigned char)c)) continue; // skip non-letter characters

            // Check if the variable is already in the list
            bool exists = false;
            for (char v : vars) {
                if (v == c) {
                    exists = true;
                    break;
                }
            }
            // Add variable if not already in the list
            if (!exists) {
                vars.push_back(c);
            }
        }
    }

    // Process conclusion
    for (char c : conclusion) { // loops on every character in the conclusion
        if (!isalpha((unsigned char)c)) continue;

        bool exists = false;
        for (char v : vars) {
            if (v == c) {
                exists = true;
                break;
            }
        }

        if (!exists) {
            vars.push_back(c);
        }
    }

    return vars;
}

int main() {
    cout << "Hello, I'm George Wanis. Welcome to my humble program!\n";
    int premiseCount;
    string line;

    while (true) {
        cout << "How many premises? ";
        getline(cin, line);
        try {
            premiseCount = stoi(line);
            if (premiseCount <= 0) {
                cout << "Please enter a positive number.\n";
                continue;
            }
            break; // valid input
        }
        catch (const invalid_argument&) {
            cout << "Invalid input. Please enter a valid number.\n";
        }
    }

    vector<string> premises(premiseCount);
    string conclusion;

    cout << "Enter premises:\n"; for (int i = 0; i < premiseCount; i++) getline(cin, premises[i]);

    cout << "Enter conclusion:\n"; getline(cin, conclusion);

    vector<char> vars = extractVariables(premises, conclusion); // Now, I have a vector of my variables
    int n = vars.size();
    int rows = 1 << n;
    cout << endl << "Variables the user did input: "; for (char v : vars) cout << v << " "; cout << endl << endl; // Feedback to user

    vector<vector<int>> table(rows, vector<int>(n)); // number of elements in table = rows, the elements themselves are vectors of integers.

    for (int i = 0; i < rows; i++){ // these 2 nested loops are responsible for the binary pattern in the truth table
        for (int j = 0; j < n; j++){
            table[i][j] = (i >> (n - j - 1)) & 1;
        }
    }

    vector<string> premisesButPostFix(premiseCount);
    for (int i = 0; i < premiseCount; i++)
        premisesButPostFix[i] = toPostfix(premises[i]);

    string conclusionButPostFix = toPostfix(conclusion);

    for (char v : vars) cout << v << " "; cout << "| "; for (int i = 0; i < premiseCount; i++) cout << "P" << i + 1 << " "; cout << "| C | Imp\n"; // Table Header

    bool satisfiable = false;
    bool valid = true;
    for (int r = 0; r < rows; r++) {
        vector<int> values = table[r];
        vector<int> PremisesValues(premiseCount);
        bool AllPremisesAreTrue = true;

        for (int i = 0; i < premiseCount; i++) {
            PremisesValues[i] = evaluateStatementInPostFix(premisesButPostFix[i], vars, values);
            if(errorsCatched) break; // stop immediately if error
            if (!PremisesValues[i]) AllPremisesAreTrue = false;
        }

        int conclusionValue = evaluateStatementInPostFix(conclusionButPostFix, vars, values);
        if(errorsCatched){
            cout << endl << "An error has been detected in your input." << endl;
            break;
        }
        int implication = (!AllPremisesAreTrue) || conclusionValue; // The premises implies the conclusion

        for (int v : values) cout << v << " ";
        cout << "| ";
        for (int v : PremisesValues) cout << v << " ";
        cout << "| " << conclusionValue << " | " << implication << endl;

        if (AllPremisesAreTrue && conclusionValue) satisfiable = true;
        if (AllPremisesAreTrue && !conclusionValue) valid = false;
    }
    if(!errorsCatched){
        cout << "\nSatisfiable? " << (satisfiable ? "YES" : "NO");
        cout << "\nValid?       " << (valid ? "YES" : "FALSIFIABLE") << endl;
    }
    return 0;
}
