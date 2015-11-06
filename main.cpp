/*
 * The MIT License (MIT)
 * 
 * Copyright (C) 2015  Stefan Buller
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
*/


#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct Line;
typedef string::size_type Width;

int tab_width = 4;

struct Line {
	Line (string line) {
		auto pos = line.begin();
		auto next_start = pos;
		for (; pos != line.end(); ++pos) {
			if (*pos == '\t') {
				string s = string(next_start, pos);
				Width w = s.size();
				cell_widths.push_back(w);
				cells.push_back(s);
				next_start = pos + 1;
			}
		}
		string s = string(next_start, pos);
		cell_widths.push_back(s.size());
		cells.push_back(s);
	}
	int stop_count() { return cell_widths.size(); }
	void print(vector<Width> stops) {
		auto cell = cells.begin();
		auto last_cell = cells.end();
		auto width_i = cell_widths.begin();
		auto last_width_i = cell_widths.end();
		Width w = 0;


		if (!cells.empty()) {
			cout <<*cell;
			w += *width_i;
			cell++;
			width_i++;
		}

		if (!stops.empty()) {
			auto stop = stops.begin();
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
		cout <<endl;
	}

	vector<Width> cell_widths;
	vector<string> cells;
};

struct Group {
	Group() : stop_count(-1) { }

	void end() {
		if (stop_count == -1) return;
		print();
		lines.clear();
		cell_widths.clear();
		stop_count = -1;
	}

	void add_line(Line l) {
		if (stop_count == -1) {
			stop_count = l.stop_count();
			lines.push_back(l);
			set_group_cell_widths(l);
		} else if (l.stop_count() == stop_count) {
			lines.push_back(l);
			update_group_cell_widths(l);
		} else {
			lines.clear();
			set_group_cell_widths(l);
		}
	}

	void set_group_cell_widths(Line l) {
		cell_widths.clear();
		for (auto cw : l.cell_widths) {
			cell_widths.push_back(cw);
		}
	}

	void update_group_cell_widths(Line l) {
		int pos = 0;
		for (auto cw : l.cell_widths) {
			if (cw > cell_widths[pos]) cell_widths[pos] = cw;
			pos++;
		}
	};

	vector<Width> tabstops() {
		int xpos = 0;
		vector<Width> ts;
		for (auto cell = cell_widths.begin(); cell < cell_widths.end(); cell++) {
			if (*cell == 0) {
				xpos++;
			} else {
				xpos += *cell + 1;
			}
			int next_stop_num = (xpos / tab_width) + 1;
			int next_stop_pos = tab_width * next_stop_num;
			xpos = next_stop_pos;
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
			g.add_line(l);
		} else if (l.cells.size() <= 1) {
			// There's no tabs on this line. End the current group,
			// but don't start another. The are no tabstops that would be
			// dependent on following lines, and it may be output immediately.
			g.end();
			l.print(vector<Width>());
		} else {
			g.end();
			g.add_line(l);
		}
	}
	g.end();
	return 0;
}
