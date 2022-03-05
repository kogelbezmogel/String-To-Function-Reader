#ifndef __BASIC_FUNCTIONS__
#define __BASIC_FUNCTIONS__


#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <functional>

typedef std::vector< std::function<double(double, double)> > FunVector;

namespace BF {
enum functions { SUM = 0, SUBSTRACT, MULTIPLY, DIVIDE, POWER, LN, LOG, LG };
}

struct BasicFunctions {
	// + - * / ^ ln log lg

	static void Initialize() {
		fun_vec.push_back( [](double x, double y) { return x + y; } );
		fun_vec.push_back( [](double x, double y) { return x - y; } );
		fun_vec.push_back( [](double x, double y) { return x * y; } );
		fun_vec.push_back( [](double x, double y) { return x / y; } );
		fun_vec.push_back( [](double x, double y) { return std::pow(x, y); } );
		fun_vec.push_back( [](double x, double y) { return std::log(y); } );
	}

	static FunVector fun_vec;
};


#endif //__BASIC_FUNCTIONS__