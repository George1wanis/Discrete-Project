// Resources:   https://youtu.be/Wz85Hiwi5MY - Comp Sci in 5: Shunting Yard Algorithm
//              https://youtu.be/bebqXO8H4eA - Comp Sci in 5: Post Fix Stack Evaluator
//              https://youtu.be/KiB0vRi2wlc - Maps in C++ (std::map and std::unordered_map)

#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <map>
#include <set>
#include <limits>
//This header provides information about the properties of fundamental data types,
//like their minimum and maximum values.
//It’s useful for writing code that needs to handle edge cases and ensure that values stay within safe ranges.

using namespace std;

// Operator precedence
int prec(char op) {
    if (op == '~') return 4;
    if (op == '&') return 3;
    if (op == '|') return 2;
    if (op == '>') return 1;
    return 0;
}

// The Shunting Yard algorithm is a method developed by Edsger Dijkstra for parsing mathematical expressions. It’s used to convert infix expressions (the standard notation we write, like (3 + 4) * 5) into postfix notation (also called Reverse Polish Notation, like 3 4 + 5 *).

// Infix notation: This is the common format, like 3 + 4 * 5.
// Postfix notation: The operators come after their operands, like 3 4 5 * +
// When you put them together, "postfix" literally means "placed after." In the context of expressions,
// it means that the operators come after their operands, as opposed to infix notation, where the operator is placed between operands.

// Postfix notation, also known as Reverse Polish Notation (RPN),
// was introduced and popularized by the Polish mathematician Jan Łukasiewicz in the 1920s.
// He developed it as a way to eliminate the need for parentheses and to simplify the parsing of expressions.

// So, while Jan Łukasiewicz was indeed a significant figure in logic and mathematics,
// he’s more closely associated with prefix notation.
// The concept of postfix notation and its popularization in computing is more tied to the development of early computing and the work of others like Charles Hamblin and the creators of early calculators.



string toPostfix(const string &infix) { // Convert infix expression into postfix (Shunting Yard)
    // the infix is passed by refrence, The const keyword is used to ensure that the function doesn’t modify the parameter it receives.
    // When a parameter is passed as a const reference, the compiler can optimize the code more effectively, knowing that it doesn’t need to worry about unintended side effects.
    // The size() method of a std::string actually returns a value of type std::string::size_type, which is an unsigned integral type. It’s usually defined as unsigned int
    // The isalpha() function is a standard C++ library function that checks whether a given character is an alphabetic letter (either uppercase or lowercase).
    stack<char> st;
    string output = "";
    for (int i = 0; i < (int)infix.size(); i++) {
        char c = infix[i];

        if (isalpha((unsigned char)c)) {
            output += c;
        }
        else if (c == '(') {
            st.push(c);
        }
        else if (c == ')') {
            while (!st.empty() && st.top() != '(') {
                output += st.top();
                st.pop();
            }
            if (!st.empty()) st.pop(); // remove '('
        }
        else { // operator
            while (!st.empty() && prec(st.top()) >= prec(c)) {
                output += st.top();
                st.pop();
            }
            st.push(c);
        }
    }
    while (!st.empty()) {
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
            int x = st.top(); st.pop();
            st.push(!x);
        }
        else {
            if (st.size() < 2) return 0; // malformed
            int b = st.top(); st.pop();
            int a = st.top(); st.pop();

            if (c == '&') st.push(a && b);
            else if (c == '|') st.push(a || b);
            else if (c == '>') st.push((!a) || b);
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
    cout << "How many variables are we dealing with? ";
    int number_of_variables;
    cin >> number_of_variables;
    //cin.ignore(n, c) means: skip up to n characters OR stop earlier if you find character c, This calls the static function max() inside the template.
    //If you're solely using cin >> and not mixing it with getline() or other input methods, then you typically won't have to worry about leftover newline characters causing issues.
    //The main reason to use cin.ignore() is when you mix different types of input methods, like when you use cin >> and then getline() afterward.
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    // Ask user for premises + conclusion
    int premiseCount;
    cout << "How many premises? ";
    cin >> premiseCount;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    vector<string> premises(premiseCount);
    string conclusion;
    cout << "Enter premises (use letters for variables, e.g. F|T or (F&~G)>H ):\n";
    for (int i = 0; i < premiseCount; i++) {
        getline(cin, premises[i]);
        if (premises[i].empty()) { i--; continue; } // ensure non-empty
    }   // when trying to put a white space as a premise it acted in a wird way and said that the argument was unsatisfiably although it was! ⚠️
        // what pieces of the code jumps over "/n"s?
    cout << "Enter conclusion:\n";
    getline(cin, conclusion);
    while (conclusion.empty()) getline(cin, conclusion);

    // Detect variables actually used
    vector<char> vars = extractVariables(premises, conclusion);
    if ((int)vars.size() != number_of_variables) {
        cout << "[Notice] Detected " << vars.size() << " distinct variable(s) in the formulas: ";
        for (char v : vars) cout << v << ' ';
        cout << "\nAdjusting number_of_variables to " << vars.size() << ".\n";
        number_of_variables = (int)vars.size();
    }

    // Generate truth table with columns corresponding to vars[0], vars[1], ...
    int rows = 1 << number_of_variables;
    vector<vector<int>> table(rows, vector<int>(number_of_variables));
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < number_of_variables; j++) {
            table[i][j] = (i >> (number_of_variables - j - 1)) & 1;
        }
    }
//-----------------------------------------------------------------------------------------------------------------------
    // Convert all to postfix
    vector<string> premPost(premiseCount);
    for (int i = 0; i < premiseCount; i++)
        premPost[i] = toPostfix(premises[i]);

    string conclPost = toPostfix(conclusion);

    // Print Truth Table Header
    cout << "\n\n=== FULL TRUTH TABLE ===\n";
    for (char v : vars) cout << v << " | ";
    for (int i = 0; i < premiseCount; i++) cout << "P" << i+1 << " ";
    cout << "| Conclusion | Implication\n";
    // Separator line
    cout << string(number_of_variables * 2 + premiseCount * 3 + 12, '-') << "\n";

    // Evaluate rows & print table
    bool sat = false;
    bool valid = true;
    for (int r = 0; r < rows; r++) {

        map<char,int> values;

        // Map each detected variable to the corresponding table column
        for (int c = 0; c < number_of_variables; c++)
            values[ vars[c] ] = table[r][c];

        // Evaluate premises for this row
        vector<int> premVal(premiseCount);
        bool allPremTrue = true;

        for (int i = 0; i < premiseCount; i++) {
            premVal[i] = evalPostfix(premPost[i], values);
            if (premVal[i] == 0) allPremTrue = false;
        }

        int conclVal = evalPostfix(conclPost, values);

        // Implication value (premises > conclusion)
        int implication = (!allPremTrue) || conclVal;

        // Print variables
        for (char v : vars) cout << values[v] << " ";
        cout << "| ";
        // Print premises
        for (int i = 0; i < premiseCount; i++) cout << premVal[i] << " ";
        cout << "| " << conclVal << " | " << implication << endl;

        if (allPremTrue) sat = true; // Check satisfiability
        if (allPremTrue && conclVal == 0) valid = false; // Check validity
    }
    cout << "\nSatisfiable?  " << (sat ? "YES" : "NO");
    cout << "\nValid?        " << (valid ? "YES" : "FALSIFIABLE") << endl;
    return 0;
}

