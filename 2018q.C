// oracle for the 2018 qualifiers
#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <cstdlib>
#include <limits>
#include <cstdint>
#include <string>
#include <fstream>

using namespace std;
using uint = uint32_t;
const uint UMAX = numeric_limits<uint>::max();

// input data + processing

uint R, C, F, N, B, T;
// row, column, fleet, rides, bonus, total time

struct ride {
	uint a, b; // start pos
	uint x, y; // end pos
	uint s; // earliest start
	uint f; // latest finish

	uint length() const;
};

vector<ride> rides;

uint absdiff(uint a, uint b)
{
	return a < b ? b-a : a-b;
}

uint distance(uint a, uint b, uint x, uint y)
{
	return absdiff(a, x) + absdiff(b, y);
}

uint ride::length() const
{
	return distance(a, b, x, y);
}


// result
struct vehicle {
	uint x, y; // current position
	uint t; // current time
	vector<uint>idx;
	uint score(set<uint>&);
};

vector<vehicle> vehicles;

template<class In>
In& operator>>(In& in, ride& r)
{
	in >> r.a >> r.b >> r.x >> r.y >> r.s >> r.f;
	return in;
}

template<class Out>
Out& operator<<(Out& out, const vehicle& v)
{
	out << v.idx.size() << " ";
	copy(begin(v.idx), end(v.idx), ostream_iterator<uint>(out, " "));
	return out;
}

void read_input_file(istream& in)
{
	in >> R >> C >> F >> N >> B >> T;
	rides.resize(N);
	for (auto& r: rides)
		in >> r;
}

uint
read_uint(string& s, int ln)
{
	int v;
	string::size_type sz;
	if (s[0] == ' ') {
		cerr << "Too many spaces in line " << ln << "\n";
		exit(1);
	}
	try {
		v = stoi(s, &sz);
		if (s.length() == sz) {
			s.erase();
		} else {
			if (s[sz] != ' ') {
				cerr << "Incorrect input line#" << ln << " " 
				    << s << "\n";
				exit(1);
			}
			s.erase(0, sz+1);
		} 
    	} catch(...) {
		cerr << "Bad integer value at line#" << ln << " " << s << "\n";
		exit(1);
	}
	return v;
}

void read_output_file(istream& out)
{
	vehicles.resize(F);
	int ln = 0;
	for (auto& v: vehicles) {
		string l;
		if (!getline(out, l))
			cerr << "Incomplete output file\n";
		ln++;
		uint sz = read_uint(l, ln);
		v.idx.resize(sz);
		for (auto& i: v.idx)
			i = read_uint(l, ln);
	}
}

uint vehicle::score(set<uint>& check)
{
	uint score = 0;
	x = y = t = 0;
	for (auto i: idx) {
		auto [it, known] = check.insert(i);
		if (!known) {
			cerr << "Doubly used ride " << i << "\n";
			::exit(1);
		}
		const auto& r = rides[i];
		auto d = distance(x, y, r.a, r.b);
		t += d;
		score += r.length();
		if (t <= r.s) {
			t = r.s;
			score += B;
		}
		t += r.length();
		x = r.x;
		y = r.y;
		if (t > r.f) {
			cerr << "End of ride #" << i << " too late " 
			    << t<< "\n"; 
			::exit(1);
		}
	}
	if (t > T) {
		cerr << "Simulation time exceeded " << t << "\n";
		::exit(1);
	}
	return score;
}

uint full_score(set<uint>& check)
{
	uint score = 0;

	for (auto& v: vehicles) {
		score += v.score(check);
	}

	return score;
}

uint rides_left(set<uint>& check)
{
	uint left = 0;
	for (uint i = 0; i != rides.size(); i++)
		left += 1 - check.count(i);
	return left;
}

int
main(int argc, char *argv[])
{
	
	if (argc != 3) {
		cerr << "Usage: " << argv[0] << " in out" << "\n";
		cerr << "shows error in input formats + score\n";
		::exit(1);
	}
	string s = argv[1];

	ifstream in(s);
	cout << "Reading: " << s << "\n";
	read_input_file(in);
//	write_input();

	s = argv[2];

	ifstream out(s);
	cout << "Reading: " << s << "\n";
	read_output_file(out);

	set<uint> check;
	cout << "Score: " << full_score(check) << "\n";
	cout << "Remaining rides:  " << rides_left(check) << "\n";
}
