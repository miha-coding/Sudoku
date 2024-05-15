#ifndef SUDOKU_H
#define SUDOKU_H

#include<iostream>
#include<cstring>
#include<cmath>
#include<map>
#include<set>
#include<vector>
#include<queue>
#include<iterator>
#include<list>
#include<fstream>
#include<sstream>
#include<chrono>
#include<memory>
#include<regex>

using std::cout, std::cin, std::cerr, std::endl;
using dtype = size_t;

void readFileReg(std::string name, dtype maxNumber, std::vector<dtype>& out);

class Sudoku {
private:
	dtype N;
	std::vector<dtype> sudoku;
	dtype maxNumber;
	dtype blockSize;

	dtype toIndex(dtype row, dtype col) const;
	const std::pair<dtype, dtype> fromIndex(dtype ind) const;
	bool insertible(dtype row, dtype col, dtype num);
	bool solve_rec(std::list<dtype>& open_pos, std::list<dtype>::const_iterator& cit);
public:
	Sudoku(const std::vector<dtype>& sud);
	~Sudoku();
	void print() const;
	bool solve_it();  // iterative solver
	bool solve_rec(); // recursive solver


};

#endif
