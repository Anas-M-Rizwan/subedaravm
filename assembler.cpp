#include <iostream>
#include <vector>
#include <fstream>
#include <map>
#include <unordered_map>
#include <string>
#include <cctype>
#include <algorithm>
using namespace std;

// Map of opcodes.
unordered_map<string, int> opcodes = {
    {"START", 11},
    {"MOVE", 1},
    {"ADD", 2},
    {"SUB", 3},
    {"MUL", 4},
    {"DIV", 5},
    {"XOR", 6},
    {"NOT", 7},
    {"AND", 8},
    {"OR", 9},
    {"SET", 10},
    {"PUSH", 13},
    {"PUT", 14},
    {"POP", 15},
    {"LOOP", 16},
    {"BR", 17},
    {"SHOW", 12},
    {"HALT", 18},
    {"LOAD", 19}
};


// A function to remove an element from a vector.
vector<string> removeElement(vector<string> in, int pos) {
    vector<string> res;
    for (int i = 0; i < in.size(); i++) {
        if (i != pos) {
            res.push_back(in[i]);
        }
    }
    return res;
}

// A function that converts a string to a list of tokens split by space.
vector<string> split(const string& str) {
    vector<string> tokens;
    string token;
    for (char ch : str) {
        if (ch == ' ') {
            if (!token.empty()) {
                tokens.push_back(token);
                token.clear();
            }
        } else {
            token += ch;
        }
    }
    if (!token.empty()) {
        tokens.push_back(token);
    }
    return tokens;
}

// A helper to safely check if a string is a number.
bool isNumber(const string& s) {
    return !s.empty() && all_of(s.begin(), s.end(), ::isdigit);
}

// Convert instruction vector to opcode form.
vector<int> opcoder(vector<string> in) {
    vector<int> opcoded_res;

    if (in.empty()) return {};
    if (opcodes.find(in[0]) == opcodes.end()) {
        cerr << "❌ Unknown instruction: " << in[0] << endl;
        return {};
    }

    int s = opcodes[in[0]];

    switch (s) {
        case 11: // START
        case 15: // POP
        case 16: // LOOP
        case 18: // HALT
            opcoded_res.push_back(s);
            return opcoded_res;

        case 1: { // MOVE R1 5
            opcoded_res.push_back(s);
            in = removeElement(in, 0);

            string reg = in[0].substr(1); // remove 'R'
            if (!isNumber(reg) || !isNumber(in[1])) {
                cerr << "Invalid MOVE args\n";
                return {};
            }
            opcoded_res.push_back(stoi(reg));
            opcoded_res.push_back(stoi(in[1]));
            return opcoded_res;
        }

        case 2: case 3: case 4: case 5: case 6: case 7: case 8: case 9: { // e.g. ADD R1 R2 R3
            opcoded_res.push_back(s);
            in = removeElement(in, 0);
            for (const string& token : in) {
                if (token[0] != 'R') {
                    cerr << "Expected register, got: " << token << endl;
                    return {};
                }
                string num = token.substr(1);
                if (!isNumber(num)) {
                    cerr << "Invalid register number: " << token << endl;
                    return {};
                }
                opcoded_res.push_back(stoi(num));
            }
            return opcoded_res;
        }

        case 13: { // PUSH R1 R2 ...
            opcoded_res.push_back(s);
            in = removeElement(in, 0);
            for (const string& token : in) {
                string num = token.substr(1);
                if (!isNumber(num)) {
                    cerr << "Invalid PUSH arg: " << token << endl;
                    return {};
                }
                opcoded_res.push_back(stoi(num));
            }
            return opcoded_res;
        }

        case 10: { // SET A1 R1
            opcoded_res.push_back(s);
            in = removeElement(in, 0);
            string addr = in[0].substr(1); // A1
            string reg = in[1].substr(1);  // R1
            if (!isNumber(addr) || !isNumber(reg)) {
                cerr << "Invalid SET args\n";
                return {};
            }
            opcoded_res.push_back(stoi(addr));
            opcoded_res.push_back(stoi(reg));
            return opcoded_res;
        }

        case 14: { // PUT A1 R2
            opcoded_res.push_back(s);
            in = removeElement(in, 0);
            string addr = in[0].substr(1);
            string reg = in[1].substr(1);
            if (!isNumber(addr) || !isNumber(reg)) {
                cerr << "Invalid PUT args\n";
                return {};
            }
            opcoded_res.push_back(stoi(addr));
            opcoded_res.push_back(stoi(reg));
            return opcoded_res;
        }

        case 17: { // BR R1 5
            opcoded_res.push_back(s);
            string reg = in[1].substr(1);
            if (!isNumber(reg) || !isNumber(in[2])) {
                cerr << "Invalid BR args\n";
                return {};
            }
            opcoded_res.push_back(stoi(reg));
            opcoded_res.push_back(stoi(in[2]));
            return opcoded_res;
        }

        case 12: { // SHOW R3
            opcoded_res.push_back(s);
            string reg = in[1].substr(1);
            if (!isNumber(reg)) {
                cerr << "Invalid SHOW arg\n";
                return {};
            }
            opcoded_res.push_back(stoi(reg));
            return opcoded_res;
        }

        case 19: { // LOAD A1 R2
            opcoded_res.push_back(s);
            string addr = in[1].substr(1);
            string reg = in[2].substr(1);
            if (!isNumber(addr) || !isNumber(reg)) {
                cerr << "Invalid LOAD args\n";
                return {};
            }
            opcoded_res.push_back(stoi(addr));
            opcoded_res.push_back(stoi(reg));
            return opcoded_res;
        }
    }

    return opcoded_res;
}

// Write machine code to binary file
void binary(const vector<vector<int>>& machineCode, const string& filename) {
    ofstream out(filename, ios::binary);
    if (!out) {
        cerr << "Error opening file: " << filename << endl;
        return;
    }

    for (const auto& instruction : machineCode) {
        for (int value : instruction) {
            unsigned char byte = static_cast<unsigned char>(value);
            out.write(reinterpret_cast<const char*>(&byte), sizeof(byte));
        }
    }

    out.close();
    cout << "✅ Binary written to: " << filename << endl;
}

// Main: read .svm, encode, and write output
int main() {
    ifstream infile("main.svm");
    ofstream txtOut("program.txt");  // human-readable output
    vector<vector<int>> machineCode;
    string line;

    while (getline(infile, line)) {
        if (line.empty()) continue;
        vector<string> tokens = split(line);
        if (tokens.empty()) continue;

        vector<int> encoded = opcoder(tokens);
        if (encoded.empty()) continue;

        machineCode.push_back(encoded);

        for (int val : encoded) {
            txtOut << val << " ";
        }
        txtOut << "\n";
    }

    binary(machineCode, "program.bin");
    cout << "✅ Assembler finished successfully.\n";
}
