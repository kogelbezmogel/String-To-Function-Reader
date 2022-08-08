#include "AnaliticFunctionReader.h"
#include "tests.h"
#include <iostream>

int main() {

	//correctness_test();
	//time_test_fastest();
	time_test();
	/*
	AnaliticFunctionReader f_reader;
	f_reader.setLine("(x-2) * (x+5) - x^x / x");
	f_reader.setLine("x^2");
	f_reader.convertSchema();
	f_reader.printSchema();
	std::cout << "f() = " << f_reader.function_value_in_point(3) << "\n";

	f_reader.setLine("(x-2) * (x+5) - x^x / x");
	f_reader.convertSchema();	
	f_reader.printSchema();
	std::cout << "f() = " << f_reader.function_value_in_point(3) << "\n";
	*/

	std::cout << "Koniec\n";

	return 0;
}


