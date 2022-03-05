#include "AnaliticFunctionReader.h"

int AnaliticFunctionReader::MAX_FUNS = 20;

AnaliticFunctionReader::AnaliticFunctionReader() {
	_empty = true;
	BasicFunctions::Initialize();
}

AnaliticFunctionReader::AnaliticFunctionReader( std::string name ) {
	
	BasicFunctions::Initialize();
	std::fstream file(name, std::ios::in);
		std::getline(file, _line);
	file.close();
	convertSchema();
}

void AnaliticFunctionReader::convertSchema() {
	deleteSpaces();
	_schema.clear();

	for( int i = 0; i < _line.size(); ++i ) {
		if( _line[i] == '(' ) {
			priorytet += MAX_FUNS;
		}
		else if( _line[i] == ')' ) {
			priorytet -= MAX_FUNS;
		}
		else if( _line[i] == '+' ) {
			_schema.push_back( BF::SUM + priorytet );
		}
		else if( _line[i] == '-' ) {
			_schema.push_back( BF::SUBSTRACT + priorytet );
		}
		else if( _line[i] == '*' ) {
			_schema.push_back( BF::MULTIPLY + priorytet );
		}
		else if( _line[i] == '/' ) {
			_schema.push_back( BF::DIVIDE + priorytet );
		}
		else if( _line[i] == '^' ) {
			_schema.push_back( BF::POWER + priorytet );
		}
		else if( _line[i] == 'l' ) {
			if( _line[i+1] == 'n' ) {
				++i;
				_schema.push_back(-2);
				_schema.push_back( BF::LN + priorytet );
			}
			else if( _line[i+1] == 'g' ) {
				++i;
				_schema.push_back(-2);
				_schema.push_back( BF::LG + priorytet );
			}
			else if( _line[i+1] == 'o' ) {
				i += 2;
				_schema.push_back(-2);
				_schema.push_back( BF::LOG + priorytet );
			}
		}
		else if( _line[i] == 'x' ) {
			_schema.push_back(-1);
		}
		else {
			_schema.push_back( std::stof( _line.substr(i, _line.size() - i) ) );
		}
	}
}

std::vector<double> AnaliticFunctionReader::getSchema() {
	return _schema;
}

void AnaliticFunctionReader::deleteSpaces() {

	std::string new_line = "";
	for( auto i : _line ){
		if( i != ' ')
			new_line += i;
	} 
	_line = new_line;
}

void AnaliticFunctionReader::printSchema() {
		std::cout << "|";
		for( auto i : _schema )
			std::cout << i << "|";
		std::cout << std::endl;
}

std::string AnaliticFunctionReader::getLine() {
	return _line;
}

int AnaliticFunctionReader::find_max_index( std::vector<double>& line ) {
	int max = line.size() - 2;
	for(int i = line.size() - 2; i > 0; i-=2 ) {
		if( line[i] > line[max] ) {
			max = i;
		}
	}
	return max;
}


double AnaliticFunctionReader::function_value_in_point(double x) {

	std::vector<double> copy = _schema;
	int index;
	double val;
	double x1;
	double x2;

	while( copy.size() >= 3 ) {
		index = find_max_index(copy);
		x1 = (copy[index-1] == -1 ? x : copy[index-1]);
		x2 = (copy[index+1] == -1 ? x : copy[index+1]);
		val = BasicFunctions::fun_vec[ (int)copy[index] % MAX_FUNS ](x1, x2);

		copy[index+1] = val;
		copy.erase( copy.begin() + index-1, copy.begin() + index+1);
	}
	return copy[0];
}
