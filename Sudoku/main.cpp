#include "Sudoku.h"


int main(int argc, char* argv[]) {
	std::vector<dtype> sud;
	
	if (argc == 2) {
		readFileReg(argv[1], 0, sud);
	}
	else if (argc == 3) {
		std::stringstream ss(argv[2]);
		dtype maxnum;
		ss >> maxnum;
		cout << "maxnum: " << maxnum << endl;
		readFileReg(argv[1], maxnum, sud);
	}
	else if (argc == 1) {
		readFileReg("Sudoku.txt", 0, sud);
	}
	else {
		std::cerr << "Error: Only 2 or 3 parameter allowed!" << std::endl;
		return 1;
	}



	Sudoku s(sud);
	auto t0 = std::chrono::high_resolution_clock::now();
	bool b = s.solve_rec();
	auto t1 = std::chrono::high_resolution_clock::now();
	if (!b) cout << "Found no valid solution (rec)!" << endl;
	else
		cout << "recursive: "
		<< std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count()
		<< " microseconds" << endl;

	std::cout << "\n\n\n";
	Sudoku scp(sud);
	t0 = std::chrono::high_resolution_clock::now();
	b = scp.solve_it();
	t1 = std::chrono::high_resolution_clock::now();
	if (!b) cout << "Found no valid solution (it)!" << endl;
	else
		cout << "iterative: "
		<< std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count()
		<< " microseconds" << endl;

	return 0;
}
