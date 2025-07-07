#include <iostream>
#include <vector>
#include <fstream>
#include <map>
#include <unordered_map>
#include <string>
using namespace std;
// Map of opcodes.
unordered_map<string, int> opcodes = {
	{"START", 0},
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
	{"PUSH", 11},
	{"PUT", 12},
	{"POP", 13},
	{"CALL", 14},
	{"RET", 15},
	{"LOOP:", 16},
	{"COND", 17},
	{"LOOPEND;", 18},
	{"SHOW", 19},
	{"HALT", 20}
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
		case 0: {
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
		case 16:
		case 18:
		case 20:
			opcoded_res.push_back(opcodes[in[0]]);
			return opcoded_res;
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
	/*
	vector<int> stack = {};
	ifstream Read("main.svm");
	string text;
	while (getline(Read, text)) {
		
	} */
}
