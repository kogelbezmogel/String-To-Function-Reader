#include <iostream>
#include <fstream>
#include <chrono>
#include <functional>
#include "AnaliticFunctionReader.h"

void time_test_fastest() {

	std::function< double(double) > fastest_fun = [](double x) { return (x-2) * (x+5) - std::pow(x, x) / x - std::pow(x,2) - 5 * x + 9; };
	std::vector<double> measures;

	for(int k = 0; k < 100; k++) {
		int amount = 10000000;
		double x_delta = (1000. - 500.) / amount;
		auto start = std::chrono::high_resolution_clock::now();
		for(int i = 0; i < amount; i++)
			fastest_fun( i * x_delta );
		auto stop = std::chrono::high_resolution_clock::now();

		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
		measures.push_back( duration.count() );
		std::cout << k << ") Exec time: " << duration.count() << " milliseconds\n";
	}

	double sum = 0;
	for( double d : measures )
		sum += d;

	std::cout << "average time: " << sum / measures.size() << "\n";

}

void time_test() {
	AnaliticFunctionReader f_reader;
	f_reader.setLine("(x-2) * (x+5) - x^x / x");
	f_reader.convertSchema();

	std::vector<double> measures;

	for(int k = 0; k < 100; k++) {
		int amount = 10000000;
		double x_delta = (1000. - 500.) / amount;
		auto start = std::chrono::high_resolution_clock::now();
		for(int i = 0; i < amount; i++)
			f_reader.function_value_in_point( i * x_delta ); 
		auto stop = std::chrono::high_resolution_clock::now();

		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
		measures.push_back( duration.count() );
		std::cout << k << ") Exec time: " << duration.count() << " milliseconds\n";
	}

	double sum = 0;
	for( double d : measures )
		sum += d;

	std::cout << "average time: " << sum / measures.size() << "\n";
}

void correctness_test() {
	AnaliticFunctionReader f_reader;
	std::function< double(double) > fun;
	double d_error = 1e-5;
	double step = 1.0;
	
	fun = []( double x ) { return std::pow(x, 2); };
	f_reader.setLine("x^2");
	f_reader.convertSchema();
	std::cout << "\n\n\n" << f_reader.getLine() << "\n";
	for(double d = 0; d < 10;  d += step) 
		if( fun(d) - d_error <= f_reader.function_value_in_point(d) && f_reader.function_value_in_point(d) <= fun(d) + d_error )
			;
		else 
			std::cout << fun(d) << " != " << f_reader.function_value_in_point(d) << "\n";

	fun = []( double x ) { return std::pow(x, 3); };
	f_reader.setLine("x^3");
	f_reader.convertSchema();
	std::cout << "\n\n\n" << f_reader.getLine() << "\n";
	for(double d = 0; d < 10;  d += step) 
		if( fun(d) - d_error <= f_reader.function_value_in_point(d) && f_reader.function_value_in_point(d) <= fun(d) + d_error );
		else 
			std::cout << fun(d) << " != " << f_reader.function_value_in_point(d) << "\n";


	fun = []( double x ) { return (x-2) * (x+5) - std::pow(x, x) / x; };
	f_reader.setLine("(x-2) * (x+5) - x^x / x");
	f_reader.convertSchema();
	std::cout << "\n\n\n" << f_reader.getLine() << "\n";
	for(double d = 0; d < 10;  d += step) 
		if( fun(d) - d_error <= f_reader.function_value_in_point(d) && f_reader.function_value_in_point(d) <= fun(d) + d_error );
		else 
			std::cout << "fun(" << d << ") = " << fun(d) << " != " << f_reader.function_value_in_point(d) << "\n";


	fun = []( double x ) { return std::pow(x, 3) - std::pow(x, 5) + 10 * x;  };
	f_reader.setLine("x^3 - x^5 + 10 * x");
	f_reader.convertSchema();
	std::cout << "\n\n\n" << f_reader.getLine() << "\n";
	f_reader.printAllSchemas();
	for(double d = 0; d < 10; d += step) 
		if( fun(d) - d_error <= f_reader.function_value_in_point(d) && f_reader.function_value_in_point(d) <= fun(d) + d_error );
		else 
			std::cout << "fun(" << d << ") = " << fun(d) << " != " << f_reader.function_value_in_point(d) << "\n";

}

void test_functions_from_file( std::string file_name ) {
	std::fstream file( file_name.c_str() );
	std::string fun;
	std::vector< std::string > fun_list;
		while( std::getline(file, fun) )
			fun_list.push_back(fun);
	file.close();

	std::cout << "List of functions: \n";
	for( std::string line : fun_list )
		std::cout << "--" << line << "\n";
	std::cout << "\n\n";

	AnaliticFunctionReader f_reader;
	for( std::string line : fun_list ) {
		f_reader.setLine(line);
		f_reader.convertSchema();
		std::cout << "\n\n###################################################################\n";
		
		std::cout << "Testing function: " << line << "\n";
		std::cout << "Schema inside: ";
		f_reader.printSchema();
		std::cout << "\n";

		std::cout << "Line inside:   " << f_reader.getLine() << "\n";
		
		for( double d = 0; d < 11; d += 1 )
			std::cout << "f(" << d << ") = " << f_reader.function_value_in_point(d) << "\n";
		
		std::cout << "\n###################################################################\n";
	}
}