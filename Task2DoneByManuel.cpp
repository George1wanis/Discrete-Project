#include <iostream>
#include <vector>
#include <string>
using namespace std;

// Logic gate functions
bool AND(bool a, bool b) { return a && b; }
bool OR(bool a, bool b) { return a || b; }
bool XOR(bool a, bool b) { return a ^ b; }
bool NAND(bool a, bool b) { return !(a && b); }
bool NOR(bool a, bool b) { return !(a || b); }
bool XNOR(bool a, bool b) { return !(a ^ b); }
bool NOT(bool a) { return !a; }

// Gate structure
struct Gate {
    string type;
    vector<int> inputs; // indices of inputs (0..N-1 for primary inputs, or later outputs)
};

// Evaluate a gate
bool evalGate(const Gate& g, const vector<bool>& values) {
    if (g.type == "AND") return AND(values[g.inputs[0]], values[g.inputs[1]]);
    if (g.type == "OR") return OR(values[g.inputs[0]], values[g.inputs[1]]);
    if (g.type == "XOR") return XOR(values[g.inputs[0]], values[g.inputs[1]]);
    if (g.type == "NAND") return NAND(values[g.inputs[0]], values[g.inputs[1]]);
    if (g.type == "NOR") return NOR(values[g.inputs[0]], values[g.inputs[1]]);
    if (g.type == "XNOR") return XNOR(values[g.inputs[0]], values[g.inputs[1]]);
    if (g.type == "NOT") return NOT(values[g.inputs[0]]);
    return false;
}

// Function to input and evaluate a circuit
vector<bool> evaluateCircuit(int numInputs) {
    int numLevels;
    cout << "Enter number of levels: ";
    cin >> numLevels;

    vector<vector<Gate>> levels(numLevels);

    int globalGateIndex = numInputs; // outputs start after inputs
    for (int l = 0; l < numLevels; l++) {
        int numGates;
        cout << "Enter number of gates in level " << l+1 << ": ";
        cin >> numGates;

        for (int g = 0; g < numGates; g++) {
            Gate gate;
            cout << "Gate " << g+1 << " type (AND, OR, XOR, NAND, NOR, XNOR, NOT): ";
            cin >> gate.type;

            int numInputsGate = (gate.type == "NOT") ? 1 : 2;
            gate.inputs.resize(numInputsGate);

            for (int j = 0; j < numInputsGate; j++) {
                cout << "Enter input " << j+1
                     << " (0.." << globalGateIndex-1 << " for available signals): ";
                cin >> gate.inputs[j];
            }

            levels[l].push_back(gate);
            globalGateIndex++; // next output index
        }
    }

    // Evaluate truth table
    int totalCombinations = 1 << numInputs;
    vector<bool> outputs(totalCombinations);

    for (int mask = 0; mask < totalCombinations; mask++) {
        vector<bool> values(numInputs);
        for (int i = 0; i < numInputs; i++) {
            values[i] = (mask >> i) & 1;
        }

        for (auto& level : levels) {
            for (auto& g : level) {
                bool out = evalGate(g, values);
                values.push_back(out);
            }
        }

        outputs[mask] = values.back(); // final output
    }

    return outputs;
}

int main() {
    int numInputs;
    cout << "Enter number of primary inputs: ";
    cin >> numInputs;

    cout << "\n--- Circuit 1 ---\n";
    vector<bool> outputs1 = evaluateCircuit(numInputs);

    cout << "\n--- Circuit 2 ---\n";
    vector<bool> outputs2 = evaluateCircuit(numInputs);

    // Compare outputs
    bool equivalent = true;
    for (int i = 0; i < outputs1.size(); i++) {
        if (outputs1[i] != outputs2[i]) {
            equivalent = false;
            break;
        }
    }

    if (equivalent) {
        cout << "\nThe two circuits are equivalent.\n";
    } else {
        cout << "\nThe two circuits are NOT equivalent.\n";
    }

    return 0;
}
