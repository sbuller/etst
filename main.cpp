#include <iostream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

int tab_width = 4;

int tsn(int xpos, int tsi) {	//lets try some
	int next_ts = ((xpos / tab_width) + 1) * tab_width;	//comments
	int spaces = (next_ts - xpos);	//inline
	return spaces;
}

struct LineType {
	int indent_level;
	vector<string> cell;
};

int next_stop(int pos) {
	return (pos / tab_width + 1) * tab_width;
}

string join(LineType line) {
	stringstream ss;
	int cur_width;

	int indent_width = tab_width * line.indent_level;
	ss <<string(indent_width, ' ');
	cur_width = indent_width;

	for (auto s : line.cell) {
		int w = cur_width + s.size();
	}

	return ss.str();
}

int main(int argc, char *argv[]) {
	if (argc == 2) tab_width = atoi(argv[1]);

	for (string line; getline(cin, line); ) {
		string::size_type pos;
		bool in_indent = true;
		int tsi = 0;
		int xpos = 0;

		stringstream ss;

		for (auto ch = line.begin(); ch != line.end(); ch++) {
			if (*ch == '\t') {
				int len = tsn(xpos, tsi);
				ss <<string(len, ' ');
				if (!in_indent) tsi++;
				xpos += len;
			} else {
				in_indent = false;
				xpos++;
				ss <<*ch;
			}
		}

		cout <<ss.str() <<endl;
	}
	return 0;
}
