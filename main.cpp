#include "AnaliticFunctionReader.h"
#include <iostream>

int main() {

	AnaliticFunctionReader czfun("fun1.txt");
	czfun.convertSchema();

	std::cout << czfun.function_value_in_point(5.5) << std::endl;

	return 0;
}


