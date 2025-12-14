// Resources:
// https://youtu.be/Wz85Hiwi5MY - Comp Sci in 5: Shunting Yard Algorithm
// https://youtu.be/bebqXO8H4eA - Comp Sci in 5: Post Fix Stack Evaluator
// https://youtu.be/KiB0vRi2wlc - Maps in C++ (std::map and std::unordered_map)
// https://youtu.be/unh6aK8WMwM - DIY Programming Language #1: The Shunting Yard Algorithm
#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <map>
#include <set>
using namespace std;
int precedence(char op) { // Operator precedence أسبقية
    if (op == '~') return 5;
    if (op == '&') return 4;
    if (op == '|') return 3;
    if (op == '>') return 2;
    if (op == '=') return 1;
    return 0;
}
string toPostfix(const string &infix) {
    stack<char> st; // this is our holding stack
    string output = ""; // this is the main string
    for (int i = 0; i < (int)infix.size(); i++) { //loops on each charachter in the given infix proposition
        char c = infix[i];
        if (isalpha((unsigned char)c)) {
            output += c; // character added to the main string
        }
        else if (c == '(') { // added to the holding stack
            st.push(c);
        }
        else if (c == ')') {
            while (!st.empty() && st.top() != '(') {
                output += st.top(); // stack flushing
                st.pop();
            }
            if (!st.empty()) st.pop(); // remove '(', But Why check !st.empty() anyway? Because this is defensive programming. What happens if the user writes: A & B)
        }
        else { // operator
            while (!st.empty() && precedence(st.top()) >= precedence(c)) {
                output += st.top(); // stack flushing
                st.pop();
            }
            st.push(c);
        }
    }
    while (!st.empty()) { // stack flushing after finishing the main input string
        output += st.top();
        st.pop();
    }
    return output;
}
int evalPostfix(const string &postfix, map<char,int>& values) { // Evaluate postfix expression for a specific truth-table row
    stack<int> st;
    for (char c : postfix) {
        if (isalpha((unsigned char)c)) {
            st.push(values[c]);
        }
        else if (c == '~') {
            if (st.empty()) return 0; // malformed, but safe-guard
            int x = st.top();
            st.pop();
            st.push(!x);
        }
        else {
            if (st.size() < 2) return 0; // malformed
            int b = st.top(); st.pop();
            int a = st.top(); st.pop();

            if (c == '&') st.push(a && b);
            else if (c == '|') st.push(a || b);
            else if (c == '>') st.push((!a) || b);
            else if (c == '=') st.push(((!a) || b)&&((!b) || a));
            else st.push(0); // unknown operator
        }
    }
    if (st.empty()) return 0;
    return st.top();
}
vector<char> extractVariables(const vector<string> &premises, const string &conclusion) { // Extract variable letters from strings (in order of first occurrence)
    vector<char> vars;
    set<char> seen;
    // [&] means the lambda can use vars and seen directly by reference.
    auto consider = [&](char c){
        if (isalpha((unsigned char)c)) {
            if (!seen.count(c)) { //seen is a C++ std::set containing characters. A set is a collection of unique elements — it automatically avoids duplicates.You can check if an element is in the set in several ways, one of which is .count().
                seen.insert(c);
                vars.push_back(c); //vars.push_back(c) adds the character c to the end of the vector vars.
            }
        }
    };
    for (auto &p : premises)
        for (char c : p) consider(c); // Each time consider(c) appears, the lambda runs with the current character c. So if a premise has 5 characters, the lambda is called 5 times for that premise.
    for (char c : conclusion) consider(c);
    return vars; // [X, Y, Z]
}

int main() {
    cout << "Hello, I'm George Wanis. Welcome to my humble program!\n";
    int premiseCount;
    cout << "How many premises? ";
    cin >> premiseCount; // Ask user for premises + conclusion
    vector<string> premises(premiseCount);
    string conclusion;
    cout << "Enter premises (use letters for variables, e.g. F|T or (F&~G)>H ):" << endl;
    for (int i = 0; i < premiseCount; i++) {
        cin >> premises[i];
        if (premises[i].empty()) { i--; continue; } // ensure non-empty
    }
    cout << "Enter conclusion:" << endl;
    cin >> conclusion;
    while (conclusion.empty()) cin >> conclusion;
    // Detect variables actually used
    vector<char> vars = extractVariables(premises, conclusion);
    int number_of_variables;
    number_of_variables = (int)vars.size();
    cout << "[Notice] Detected " << number_of_variables << " distinct variable(s) in the formulas: ";
    for (char v : vars) cout << v << ' ';
    int rows = 1 << number_of_variables; // rows = 2^n number_of_variables
    vector<vector<int>> table(rows, vector<int>(number_of_variables)); // Generate truth table with columns corresponding to vars[0], vars[1], ...
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < number_of_variables; j++) {
            table[i][j] = (i >> (number_of_variables - j - 1)) & 1; // bit extraction / bit masking
        }
    }
//-----------------------------------------------------------------------------------------------------------------------
    // Convert all to postfix
    vector<string> premPost(premiseCount);
    for (int i = 0; i < premiseCount; i++) premPost[i] = toPostfix(premises[i]); // if the user inputed the string "P>T" first, the program should assign premPost[0] to the string "PT>".
    string conclPost = toPostfix(conclusion);
    // Print Truth Table Header
    cout << endl << endl << "=== FULL TRUTH TABLE ===" << endl;
    for (char v : vars) cout << v << " | ";
    for (int i = 0; i < premiseCount; i++) cout << "P" << i+1 << " ";
    cout << "| Conclusion | Implication" << endl;
    // Separator line
    cout << string(number_of_variables * 2 + premiseCount * 3 + 12, '-') << "\n";
    // Evaluate rows & print table
    bool satisfiable = false;
    bool valid = true;
    for (int r = 0; r < rows; r++) { //loop on each row
        map<char,int> values;
        for (int c = 0; c < number_of_variables; c++) // Map each detected variable to the corresponding table column
            values[ vars[c] ] = table[r][c];
        // Evaluate premises for this row
        vector<int> premVal(premiseCount);
        bool allPremTrue = true;
        for (int i = 0; i < premiseCount; i++) { //loops on the premises
            premVal[i] = evalPostfix(premPost[i], values);
            if (premVal[i] == 0) allPremTrue = false;
        }

        int conclusionValue = evalPostfix(conclPost, values);
        int implication = (!allPremTrue) || conclusionValue; // Implication value (premises > conclusion) “If all premises are true, then the conclusion must be true.” تتابع
        for (char v : vars) cout << values[v] << " "; // Print variables
        cout << "| ";
        for (int i = 0; i < premiseCount; i++) cout << premVal[i] << " "; // Print premises
        cout << "| " << conclusionValue << " | " << implication << endl;

        if (allPremTrue && conclusionValue == 1) satisfiable = true; // Check satisfiability for each row, Why you do check the conclusion here? This is very important: Satisfiability cares to show if the argument was satisfiable at least once.
        if (allPremTrue && conclusionValue == 0) valid = false; // Check validity for each row. if a row is found to be not valid then the whole argument isn't valid.
    }
    cout << "\nSatisfiable?  " << (satisfiable ? "YES" : "NO");
    cout << "\nValid?        " << (valid ? "YES" : "FALSIFIABLE") << endl;
    return 0;
}
