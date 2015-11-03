#include <iostream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;	//add

struct Line;
typedef string::size_type Width;	//some

int tab_width = 4;

struct Line {		//comments
	Line (string line) {	//to the file
		auto pos = line.begin();
		indent = 0;

		for (; pos < line.end() && *pos == '\t'; ++pos) indent++;

		auto last = pos;
		for (; pos < line.end(); ++pos) {
			if (*pos == '\t') {
				cell_widths.push_back(pos - last);
				cells.push_back(string(last, pos));
				last = pos;
			}
		}
		cells.push_back(string(last, pos));
	}
	int stop_count() { return indent + cell_widths.size(); }
	void print(vector<Width> stops) {
		Width leading_indent_size = tab_width * indent;
		auto cell = cells.begin();
		auto last_cell = cells.end();
		auto width_i = cell_widths.begin();
		auto last_width_i = cell_widths.end();
		Width w = leading_indent_size;


		if (leading_indent_size) cout <<string(leading_indent_size, ' ');
		if (!cells.empty()) {
			cout <<"~"; cout.flush();
			cout <<*cell;
			w += *width_i;
			cell++;
			width_i++;
		}
		cout <<"`"; cout.flush();

		if (!stops.empty()) {
			auto stop = stops.begin() + indent;
			while (cell < cells.end() && stop < stops.end()) {
				Width d = *stop - w;
				cout <<string(d, ' ');
				w = *stop;
				cout <<*cell;
				w += *(width_i++);
				cell++;
				stop++;
			}
		}
		cout <<"$" <<endl;
	}

	int indent;
	vector<Width> cell_widths;
	vector<string> cells;
};

struct Group {
	Group() : stop_count(-1) { }

	void reset() {
		lines.clear();
		cell_widths.clear();
		stop_count = -1;
	}

	void add_line(Line l) {
		if (l.stop_count() == stop_count) {
			lines.push_back(l);
			update_group_cell_widths(l);
		} else {
			lines.clear();
			set_group_cell_widths(l);
		}
	}

	void set_group_cell_widths(Line l) {
		cell_widths.clear();
		for (int i=0; i<l.indent; ++i) {
			cell_widths.push_back(0);
		}
		for (auto cw : l.cell_widths) {
			cell_widths.push_back(cw);
		}
	}

	void update_group_cell_widths(Line l) {
		int pos = l.indent;
		for (auto cw : l.cell_widths) {
			if (cw > cell_widths[pos]) cell_widths[pos] = cw;
			pos++;
		}
	};

	vector<Width> tabstops() {
		int xpos = 0;
		bool indent = true;
		vector<Width> ts;
		for (auto cell = cell_widths.begin(); cell < cell_widths.end(); cell++) {
			if (*cell == 0 && indent) {
				xpos += tab_width;
			} else if (*cell == 0) {
				xpos++;
			} else {
				xpos += *cell;
				indent = false;
			}
			ts.push_back(xpos);
		}
		return ts;
	}

	void print() {
		vector<Width> stops = tabstops();
		for (auto line : lines) {
			line.print(stops);
		}
	}

	vector<Width> cell_widths;
	vector<Line> lines;
	int stop_count;
};

int main(int argc, char *argv[]) {
	if (argc == 2) tab_width = atoi(argv[1]);

	Group g;

	for (string line; getline(cin, line); ) {
		Line l(line);

		if (g.stop_count == l.stop_count()) {
			cout <<"1: "; cout.flush();
			g.add_line(l);
		} else if (l.cells.size() <= 1) {
			cout <<"2: "; cout.flush();
			g.print();
			g.reset();
			l.print(vector<Width>());
		} else {
			cout <<"3: "; cout.flush();
			g.print();
			g.reset();
			g.add_line(l);
		}
	}
	g.print();
	return 0;
}
