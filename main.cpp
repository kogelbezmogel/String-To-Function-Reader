#include "AnaliticFunctionReader.h"
#include "tests.h"
#include <iostream>

int main() {

	//correctness_test();
	//time_test_fastest();
	//test_functions_from_file( "test_fun.txt" );

	
	AnaliticFunctionReader f_reader;
	std::function< double(double) > fun = []( double x ) { return x; };
	double arg = 1;

	f_reader.setLine("x");
	f_reader.convertSchema();
	std::cout << "\n\n\n" << f_reader.getLine() << "\n";
	f_reader.printAllSchemas();
	std::cout << "fun(" << arg << ") = " << fun(arg) << " != " << f_reader.function_value_in_point(arg) << "\n";
	
	std::cout << "Koniec\n";
	return 0;
}


