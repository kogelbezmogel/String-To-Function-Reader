#include "AnaliticFunctionReader.h"
//#include "tests.h"
#include <iostream>
#include <numeric>

int main() {

	//correctness_test();
	//time_test_fastest();
	//test_functions_from_file( "test_fun.txt" );

	
	AnaliticFunctionReader f_reader;
	std::function< double(double) > fun = []( double x ) { return x; };
	double arg = 1;

	f_reader.setLine("5 * x ^ 2");
	f_reader.convertSchema();
	//std::cout << "\n\n\n" << f_reader.getLine() << "\n";
	f_reader.printAllSchemas();
	//std::cout << "fun(" << arg << ") = " << fun(arg) << " != " << f_reader.function_value_in_point(arg) << "\n";
	
	//std::vector<double> args(100000000);
	//std::iota(args.begin(), args.end(), 1);
	//std::vector<double> results;
	//results = f_reader(args);

	/*for(auto res : results )
		std::cout << res << " ";*/

	std::cout << "f(10) = " << f_reader(10) << "\n";

	std::cout << "\nKoniec\n";
	return 0;
}


