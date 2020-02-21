#include <iostream>
#include <fstream>
#include <vector>
#include <set>

using namespace std;
using uint = unsigned int;

uint B;	// # of books
uint L; // # of libraries
uint D;	// # of days

struct library {
	uint N; // # of books
	uint T; // time for registration
	uint M; // # scan/day
	set<uint> books;
};

vector<uint> books;	// individual books scores
vector<library> libraries;

struct scan {
	uint Y; // library id
	uint K; // number of books
	vector<uint> books;
};

uint A; // # of used libraries
vector<scan> scans;

uint
score()
{
	uint T = 0;
	uint score = 0;
	set<uint> done;

	for (auto& s: scans) {
		auto& lib = libraries[s.Y];
		T += lib.T;
		auto t = T;

		uint scanned_today = 0;

		for (auto& b: s.books) {
			if (lib.books.find(b) == end(lib.books)) {
				cerr << "Error: book " << b << 
				    " not in library " << s.Y << "\n";
				return 0;
			}
			if (scanned_today == lib.M) {
				t++;
				scanned_today = 0;
			}
			scanned_today++;
			if (t == D)
				break;
			if (done.find(b) == end(done)) {
				cout << "library: " << s.Y <<
				    " scanned " << b << "(score " <<
				    books[b] << ") on day " << t << "\n";
				score += books[b];
				done.insert(b);
			}
		}
	}
	return score;
}

int
main(int argc, char *argv[])
{
	ifstream in(argv[1]);
	in >> B >> L >> D;

	books.resize(B);
	for (auto& b: books)
		in >> b;

	libraries.resize(L);
	for(auto&l: libraries) {
		in >> l.N >> l.T >> l.M;
		for (uint i = 0; i != l.N; i++) {
			uint k;
			in >> k;
			l.books.insert(k);
		}
	}

	ifstream out(argv[2]);
	out >> A;
	scans.resize(A);

	for(auto& l: scans) {
		out >> l.Y;
		out >> l.K;
		for (uint i = 0; i != l.K; i++) {
			uint k;
			out >> k;
			l.books.push_back(k);
		}
	}
	cout << score() << "\n";
}
