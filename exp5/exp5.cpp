#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <sstream>
using namespace std;

map<string, vector<vector<string>>> grammar;
map<string, set<string>> firstSet, followSet;
map<pair<string, string>, vector<string>> parsingTable;
string startSymbol;

void computeFirst(string nonTerm);
void computeFollow(string nonTerm);
void constructParsingTable();

// Function to compute FIRST sets
void computeFirst(string nonTerm) {
    if (!firstSet[nonTerm].empty()) return;
    for (auto &prod : grammar[nonTerm]) {
        for (int i = 0; i < prod.size(); i++) {
            string symbol = prod[i];
            if (!grammar.count(symbol)) { // Terminal
                firstSet[nonTerm].insert(symbol);
                break;
            } else { // Non-terminal
                computeFirst(symbol);
                firstSet[nonTerm].insert(firstSet[symbol].begin(), firstSet[symbol].end());
                if (firstSet[symbol].find("ε") == firstSet[symbol].end()) break;
            }
        }
    }
}

// Function to compute FOLLOW sets
void computeFollow(string nonTerm) {
    if (!followSet[nonTerm].empty()) return;
    if (nonTerm == startSymbol) followSet[nonTerm].insert("$");
    for (auto &rule : grammar) {
        for (auto &prod : rule.second) {
            for (size_t i = 0; i < prod.size(); i++) {
                if (prod[i] == nonTerm) {
                    for (size_t j = i + 1; j < prod.size(); j++) {
                        string nextSymbol = prod[j];
                        if (!grammar.count(nextSymbol)) {
                            followSet[nonTerm].insert(nextSymbol);
                            break;
                        }
                        followSet[nonTerm].insert(firstSet[nextSymbol].begin(), firstSet[nextSymbol].end());
                        if (firstSet[nextSymbol].find("ε") == firstSet[nextSymbol].end()) break;
                    }
                    if (i + 1 == prod.size() || firstSet[prod[i + 1]].count("ε")) {
                        computeFollow(rule.first);
                        followSet[nonTerm].insert(followSet[rule.first].begin(), followSet[rule.first].end());
                    }
                }
            }
        }
    }
}

// Construct the Predictive Parsing Table
void constructParsingTable() {
    for (auto &rule : grammar) {
        string nonTerm = rule.first;
        for (auto &prod : rule.second) {
            set<string> firstAlpha;
            if (!grammar.count(prod[0])) firstAlpha.insert(prod[0]);
            else firstAlpha.insert(firstSet[prod[0]].begin(), firstSet[prod[0]].end());
            for (string terminal : firstAlpha) {
                if (terminal != "ε") parsingTable[{nonTerm, terminal}] = prod;
            }
            if (firstAlpha.count("ε")) {
                for (string terminal : followSet[nonTerm]) {
                    parsingTable[{nonTerm, terminal}] = prod;
                }
            }
        }
    }
}

int main() {
    int numRules;
    cout << "Enter number of production rules: ";
    cin >> numRules;
    cin.ignore();
    cout << "Enter the grammar (e.g., E -> T E'):\n";
    for (int i = 0; i < numRules; i++) {
        string line;
        getline(cin, line);
        stringstream ss(line);
        string lhs, arrow, rhs;
        ss >> lhs >> arrow; // Read non-terminal and '->'
        vector<string> production;
        while (ss >> rhs) {
            if (rhs == "|") {
                grammar[lhs].push_back(production);
                production.clear();
            } else {
                production.push_back(rhs);
            }
        }
        grammar[lhs].push_back(production);
        if (i == 0) startSymbol = lhs; // First rule defines start symbol
    }
    
    // Compute FIRST and FOLLOW sets
    for (auto &rule : grammar) computeFirst(rule.first);
    for (auto &rule : grammar) computeFollow(rule.first);
    
    // Construct parsing table
    constructParsingTable();
    
    // Output FIRST sets
    cout << "\nFIRST sets:\n";
    for (auto &entry : firstSet) {
        cout << "FIRST(" << entry.first << ") = { ";
        for (auto &symbol : entry.second) cout << symbol << " ";
        cout << "}\n";
    }
    
    // Output FOLLOW sets
    cout << "\nFOLLOW sets:\n";
    for (auto &entry : followSet) {
        cout << "FOLLOW(" << entry.first << ") = { ";
        for (auto &symbol : entry.second) cout << symbol << " ";
        cout << "}\n";
    }
    
    // Output Parsing Table
    cout << "\nPredictive Parsing Table:\n";
    for (auto &entry : parsingTable) {
        cout << "M[" << entry.first.first << ", " << entry.first.second << "] = ";
        for (auto &symbol : entry.second) cout << symbol << " ";
        cout << "\n";
    }
    return 0;
}
