#include <iostream>
#include <vector>
#include <fstream>
#include <map>
#include <unordered_map>

using namespace std;

vector<string> split(string t) {
	vector<string> res = {};
	string i = "";
	
	for (char c : t) {
		if (c == ' ') {
			if (!i.empty()) {
				res.push_back(i);
				i = "";
			}
		} else {
			i+=c;
		}
	}
	
	if (!i.empty()) {
		res.push_back(i);
	}
	
	return res;
}

int main() {
	unordered_map<string, int> registers = {{"R0", 0}, {"R1", 0}, {"R2", 0}, {"R3", 0}};
	vector<int> stack = {};
	ifstream Read("main.svm");
	string text;
	while (getline(Read, text)) {
		vector<string> file = split(text);
		if (file[0] == "MOVE") {
			registers[file[1]] = stoi(file[2]);
		}
		cout << "\n";
	}
}
