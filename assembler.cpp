#include <iostream>
#include <vector>
#include <fstream>
#include <map>
#include <unordered_map>
#include <string>
using namespace std;
// Map of opcodes.
unordered_map<string, int> opcodes = {
	{"START", 11}, // done
	{"MOVE", 1}, // done
	{"ADD", 2}, // done
	{"SUB", 3}, // done
	{"MUL", 4}, // done
	{"DIV", 5}, // done
	{"XOR", 6}, // done
	{"NOT", 7}, // done
	{"AND", 8}, // done
	{"OR", 9}, // done
	{"SET", 10}, 
	{"PUSH", 13}, // done
	{"PUT", 14},
	{"POP", 15}, // done
	{"LOOP", 16}, // done
	{"BR", 17},
	{"SHOW", 12},
	{"HALT", 18}, // done
	{"LOAD", 19}
};
// A function to remove an element from a vector.
vector<string> removeElement(vector<string> in, int pos) {
	vector<string> res = {};
	for (int i = 0; i < in.size(); i++) {
		if (i == pos) {
			continue;
		}
		res.push_back(in[i]);
	}
	return res;
}

// A functions that converts strings to vector strings.
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
// The function opcoder to make opcode instructions

vector<int> opcoder(vector<string> in) {
	vector<int> opcoded_res = {};
	int s = opcodes[in[0]];
	switch (s) {
		case 11: {
			opcoded_res.push_back(opcodes[in[0]]);
			return opcoded_res;
		}
		case 1: {
			opcoded_res.push_back(opcodes[in[0]]);
			removeElement(in, 0);
			string regs = "";
			for (char c : in[1]) {
				if (c == 'R') {
					continue;
				}
				regs += c;
			}
			opcoded_res.push_back(stoi(regs));
			opcoded_res.push_back(stoi(in[2]));
			return opcoded_res;
		}
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
		case 9: {
			opcoded_res.push_back(opcodes[in[0]]);
			removeElement(in, 0);
			string regs = "";
			for (string i : in) {
				for (char c : i) {
					if (c == 'R') {
						continue;
					}
					regs += c;
				}
				opcoded_res.push_back(stoi(regs));
				regs = "";
			}
			return opcoded_res;
		}
		case 15:
		case 18:
		case 16: {
			opcoded_res.push_back(opcodes[in[0]]);
			return opcoded_res;
		}
		case 13: {
			opcoded_res.push_back(opcodes[in[0]]);
			removeElement(in, 0);
			string regs = "";
			for (string i : in) {
				for (char c : i) {
					if (c == 'R') {
						continue;
					}
					regs += c;
				}
				opcoded_res.push_back(stoi(regs));
				regs = "";
			}
			return opcoded_res;
		}
		case 10: {
			opcoded_res.push_back(in[0]);
			removeElement(in, 0);
			removeElement(in, 0);
			string regs = "";
			for (string i : in) {
				for (char c : i) {
					if (c == 'R') {
						continue;
					}
					regs += c;
				}
				opcoded_res.push_back(stoi(regs));
				regs = "";
			}
			return opcoded_res;
		}
	};
	
}


int main() {
	// Generation of registers.
	unordered_map<string, int> registers;
	int numregisters = 32;

	for (int i = 0; i < numregisters; ++i) {
		string regname = "R" + to_string(i);
		registers[regname] = 0;
		cout << "R" << i << "=" << registers[regname] << "\n";
	}
}
// Converion to binary and storing it in binary file.
void binary(const vector<vector<int>>& machineCode, const string& filename) {
	ofstream out(filename , ios::binary);
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
	cout<< "Binary Written to: " << filename << endl;
}
// Using binary() to convert to binary
int main() {
    ifstream infile("main.svm");
    ofstream txtOut("program.txt");
    vector<vector<int>> machineCode; 
    string line;
    while (getline(infile, line)) {
        vector<string> tokens = split(line);
        if (tokens.empty()) continue;

        vector<int> encoded = opcoder(tokens);
        machineCode.push_back(encoded); 

        for (int val : encoded) {
            txtOut << val << " ";
        }
        txtOut << "\n";
    }

    binary(machineCode, "program.bin");
}

