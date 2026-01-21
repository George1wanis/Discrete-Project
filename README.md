# Logic Proposition Evaluator in C++

**Author:** George Wanis Ayed Wanis  
**Student ID:** 2300280  
**Task:** 1  
**Team:** 31  

---

## Overview

This C++ program evaluates logical propositions using the **Shunting Yard Algorithm** and **Postfix evaluation**. It allows the user to input multiple premises and a conclusion, constructs a truth table, and determines whether the conclusion is **valid**, **satisfiable**, or **falsifiable**.

The program supports logical operators:
- `~` : NOT  
- `&` : AND  
- `|` : OR  
- `>` : IMPLICATION  
- `=` : BICONDITIONAL  

---

## Resources

The project was inspired and supported by the following tutorials:
- [Shunting Yard Algorithm](https://youtu.be/Wz85Hiwi5MY) – Comp Sci in 5  
- [Postfix Stack Evaluator](https://youtu.be/bebqXO8H4eA) – Comp Sci in 5  
- [Maps in C++ (std::map and std::unordered_map)](https://youtu.be/KiB0vRi2wlc)  
- [DIY Programming Language #1: The Shunting Yard Algorithm](https://youtu.be/unh6aK8WMwM)  

---

## Example Problem

Example scenario assigned to the team:

```
The children are playing Football or Tennis.
If they are watching TV or playing Tennis, then they are not playing Football.
Therefore, they are not playing Football or they are not watching TV.
```

- Premises in logical form:
  1. F | T  
  2. V | T > ~F  
- Conclusion: ~F | ~V  

The truth table for this example is generated automatically by the program.

---

## Functions Overview

The program contains the following main functions:

1. **`precedence(char op)`**  
   Returns the precedence of logical operators.

2. **`toPostfix(const string &infix)`**  
   Converts an infix logical expression to postfix notation using the Shunting Yard Algorithm.

3. **`variablesIndexInVector(const vector<char>& vars, char c)`**  
   Returns the index of a variable in a vector of variables.

4. **`evaluateStatementInPostFix(const string &postfix, const vector<char>& vars, const vector<int>& values)`**  
   Evaluates a postfix logical expression for a given row of truth values.

5. **`extractVariables(const vector<string>& premises, const string& conclusion)`**  
   Extracts all unique variables from the premises and conclusion.

6. **`main()`**  
   - Prompts the user to input the number of premises, the premises themselves, and the conclusion.  
   - Generates all combinations of truth values.  
   - Converts premises and conclusion to postfix notation.  
   - Evaluates each row of the truth table.  
   - Determines if the conclusion is **satisfiable** or **valid**.

---

## Usage

1. Compile the program:

```bash
g++ -o logic_evaluator main.cpp
```

2. Run the executable:

```bash
./logic_evaluator
```

3. Follow the prompts:
- Enter the number of premises.
- Enter each premise using logical operators (`~`, `&`, `|`, `>`, `=`).
- Enter the conclusion.

The program will output:
- The truth table with premises, conclusion, and implication results.  
- Whether the conclusion is satisfiable or valid.

---

## Notes

- Malformed expressions will trigger an error, and evaluation will stop.  
- Variables are automatically detected from the input expressions.  
- Supports complex logical statements with multiple operators and parentheses.

---

## Example Output

```
Variables the user did input: F T V
F T V | P1 P2 | C | Imp
0 0 0 | 0 0 | 1 | 1
...
Satisfiable? YES
Valid?       FALSIFIABLE
```

---

## License

This project is for educational purposes as part of a university assignment.
