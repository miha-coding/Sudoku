#include "Sudoku.h"


Sudoku::Sudoku(const std::vector<dtype>& sud) {

	N = sud.size();						// e.g. 81
	maxNumber = (dtype)sqrt(N);			// e.g. 9
	blockSize = (dtype)sqrt(maxNumber);	// e.g. 3 (3x3 block for 9 nums)

	if (blockSize * blockSize != maxNumber) {
		cerr << "Invalid sizes for Sudoku" << endl;
	}

	sudoku = std::vector<dtype>(sud);

	cout << "Unsolved Sudoku: " << endl;
	print();

}

Sudoku::~Sudoku() { }


dtype Sudoku::toIndex(dtype row, dtype col) const {
	return row * maxNumber + col;
}

const std::pair<dtype, dtype> Sudoku::fromIndex(dtype ind) const {
	return std::pair<dtype, dtype>(ind / maxNumber, ind % maxNumber);
}

void Sudoku::print() const {
	for (int i = 0; i < N; ++i) {
		if (!(i % blockSize)) cout << "\t";	// 3 (1 to 9 sudoku)
		if (!(i % (dtype)pow(blockSize, 2))) cout << "\n"; // 9
		if (!(i % (dtype)pow(blockSize, 3))) cout << "\n"; // 27
		cout << sudoku[i] << " ";
	}
	cout << endl;
}

bool Sudoku::solve_it() {
	std::list<dtype> mut_ind;  // List with indices of numbers that can be changed.
	for (dtype i = 0; i < N; ++i) {
		if (sudoku[i] == 0) mut_ind.push_back(i);
	}

	std::list<dtype>::const_iterator cit = mut_ind.begin();
	dtype first_ind = *cit;
	dtype cur_val, add, cur_ind;
	std::pair<dtype, dtype> ind;
	while (cit != mut_ind.end()) {
		cur_ind = *cit;  // current index
		ind = fromIndex(cur_ind);  // current row/col
		cur_val = sudoku[cur_ind];  // value at position row,col
		add = cur_val + 1;  // possible new value
		while (add <= maxNumber) {
			if (insertible(ind.first, ind.second, add)) {
				// Number is allowed -> set it
				sudoku[cur_ind] = add;
				++cit;
				break;
			}
			++add;
		}
		if (sudoku[first_ind] > maxNumber) {
			// Sudoku not solvable
			cout << "\nSudoku not solvable." << endl;
			return false;
		}
		if (add > maxNumber) {
			// All numbers forbidden -> change number before;
			sudoku[cur_ind] = 0;
			--cit;
		}

	}
	cout << "\nSudoku solved. The solution is: " << endl;
	print();
	return true; // Sudoku solved

}


bool Sudoku::solve_rec() {
	std::list<dtype> open_pos;
	for (dtype i = 0; i < N; ++i) {
		if (sudoku[i] == 0) {
			open_pos.push_back(i);
		}
	}

	std::list<dtype>::const_iterator cit = open_pos.begin();
	bool suc = solve_rec(open_pos, cit);
	if (suc) {
		cout << "\nSudoku solved. The solution is: " << endl;
		print();
	}
	else {
		cout << "\nSudoku not solvable." << endl;
	}
	return suc;
}

bool Sudoku::solve_rec(std::list<dtype>& open_pos, std::list<dtype>::const_iterator& cit) {
	if (cit == open_pos.end()) {
		// No open position left -> solution found
		return true;
	}

	dtype curind = *cit;
	std::pair<dtype, dtype> pos = fromIndex(curind);
	dtype curval = sudoku[curind];
	if (curval + 1 > maxNumber) {
		// No valid number in valid range found
		return false;
	}
	++cit;
	for (dtype i = curval + 1; i <= maxNumber; ++i) {
		if (insertible(pos.first, pos.second, i)) {
			sudoku[curind] = i;
			if (solve_rec(open_pos, cit)) {
				// solution found
				return true;
			}
		}
	}
	// No solution found
	--cit;
	sudoku[curind] = 0;
	return false;
}

bool Sudoku::insertible(dtype row, dtype col, dtype num) {
	//check row/col
	for (dtype i = 0; i < maxNumber; ++i) {
		if (i != col and sudoku[toIndex(row, i)] == num) return false; // row
		if (i != row and sudoku[toIndex(i, col)] == num) return false; // col
	}
	//check block
	// For a 1 to 9 Sudoku, blocksize is 3 (3x3 block)
	dtype rmin = (row / blockSize) * blockSize;
	dtype rmax = (row / blockSize + 1) * blockSize - 1;
	dtype cmin = (col / blockSize) * blockSize;
	dtype cmax = (col / blockSize + 1) * blockSize - 1;

	for (dtype r = rmin; r <= rmax; ++r) {
		for (dtype c = cmin; c <= cmax; ++c) {
			if ((r != row or c != col) and sudoku[toIndex(r, c)] == num)
				return false;
		}
	}

	return true;
}

void readFileReg(std::string name, dtype maxNumber, std::vector<dtype>& out) {
	std::fstream f(name);
	if (!f.is_open()) {
		cerr << "Fehler beim Öffnen der Datei." << endl;
		return;
	}

	std::string str;
	f.seekg(0, std::ios::end);  // Set position to end of file
	str.reserve(f.tellg());  // Get 'length' of file
	f.seekg(0, std::ios::beg);  // Set postition to start of file

	// Write comlete text from file to string
	str.assign((std::istreambuf_iterator<char>(f)),
		std::istreambuf_iterator<char>());


	dtype num;
	dtype cnt = 0;
	std::stringstream ss;

	std::regex reg("([\\d]{1,2})");
	std::sregex_iterator it(str.begin(), str.end(), reg);
	std::sregex_iterator it_end; // Shows to the end

	dtype N;
	if (maxNumber != 0) {
		N = maxNumber * maxNumber;
	}
	else {
		N = distance(it, it_end);
		maxNumber = (dtype)sqrt(N);
		//cout << "N = " << N << " -> maxNumber = " << maxNumber << endl;
	}

	for (std::sregex_iterator i = it; i != it_end; ++i) {
		ss << (*i).str();
		ss >> num;
		ss.clear();
		if (num<0 or num>maxNumber) {
			cerr << "Number " << num << " out of range!";
			f.close();
			return;
		}
		out.push_back(num);
		++cnt;
		if (cnt > N) break;  // Sudoku is full
	}
	out.shrink_to_fit();
	f.close();
}
