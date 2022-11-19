#include "AnaliticFunctionReader.h"
#include <algorithm>
#include <functional>
#include <thread>
#include <mutex>

int AnaliticFunctionReader::MAX_FUNS = 20;


int number_length_in_str(std::string line) {
  int len = 0;
  bool num_passed = false;
  bool dot_passed = false;
  bool exp_notation = false;

  for (const char &c : line) {
    if (std::isdigit(c)) { num_passed = true; ++len; }
	else if (c == '.' && !dot_passed) { dot_passed = true; ++len; }
	else if (c == 'e' && num_passed && !exp_notation) { exp_notation = true; ++len; }
	else if (c == '-' && exp_notation) { ++len; }
	else { break; }
  }
  return len;
}


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
	_indexes_for_instruction.clear();
	_indexes_for_x_values.clear();
	_instruction_schemas.clear();

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
			int length = number_length_in_str( _line.substr(i, _line.size() ) );
			std::cout << "Len: " << length << "\n";
			double num = std::stof( _line.substr(i, _line.size()) );
			std::cout << "Substr: " << _line.substr(i, _line.size()) << "\n";
			//double num = 100;
			_schema.push_back( num );
			i = i + length - 1; // moving pointer reagard to length of numer
		}
	}

	// sorting the indexes
	int schema_len = _schema.size();
	int max_index;
	std::vector<double> copy_schema = _schema ; 
	while ( schema_len > 1 ) {

		max_index = find_max_index(copy_schema);
		
		_indexes_for_instruction.push_back(max_index);
		_instruction_schemas.push_back(copy_schema);

		copy_schema[max_index + 1] = 0;
		copy_schema.erase( copy_schema.begin() + max_index - 1, copy_schema.begin() + max_index + 1 );

		schema_len -= 2;
	}
	// place for result of all schemas
	if( _schema.size() > 1 )
		_instruction_schemas.push_back( std::vector<double>({0}) );
	else if( _schema.size() == 1 ) // it is const function or linear
		_instruction_schemas.push_back( { _schema[0] } );

	for(int i = 0; i < _schema.size(); i+=2)
		if(_schema[i] == -1 )
			_indexes_for_x_values.push_back(i);
}


void AnaliticFunctionReader::setLine(std::string line) {
	_line = line;
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
	std::cout << "---------MAIN SCHEMA---------\n";
	for( auto i : _schema )
		std::cout << "[" << i << "]";
	std::cout << "\n---------    ---------\n";
}


void AnaliticFunctionReader::printAllSchemas() {

	printSchema();

	std::cout << "\n\n----------ADDITIONAL SCHEMAS----------\n";
	for( const auto& schema : _instruction_schemas) {
		for( const auto& el : schema)
			std::cout << "[" << el << "]";
		std::cout << "\n";
	}
	std::cout << "-----------    -----------\n\n";

	std::cout << "\n-----------INDEXES-----------\n";
	for( auto ind : _indexes_for_instruction )
		std::cout << "[" << ind << "]";
	std::cout << "\n-----------    -----------\n\n";

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


double AnaliticFunctionReader::operator() ( const double& x ) {
	int index;
	double val;
	double x1;
	double x2;

	for( int index : _indexes_for_x_values ) 
		_instruction_schemas[0][index] = x;

	for( int i = 0; i < _indexes_for_instruction.size(); i++ ) {
		index = _indexes_for_instruction[i];
		
		x1 = _instruction_schemas[i][index-1];
		x2 = _instruction_schemas[i][index+1];
		val = BasicFunctions::fun_vec[ (int)_instruction_schemas[i][index] % MAX_FUNS ](x1, x2);

		// wstawienie wczesniej wyliczonych wartosci do nastepnego schematu przed indeksem 	
		for(int k = 0; k < index - 1; k++)
			_instruction_schemas[i+1][k] = _instruction_schemas[i][k];
	
		// wstawienie wyliczonej wartosci do kolejnego schematu
		_instruction_schemas[i+1][index - 1] = val;

		// wstawianie wczesniej wyliczonych wartosci do nastepnego schematu po indeksie
		for(int k = index+2; k < _instruction_schemas[i].size(); k++)
			_instruction_schemas[i + 1][k - 2] = _instruction_schemas[i][k];	
	}
	return _instruction_schemas[ _instruction_schemas.size() - 1 ][0];
}


// operator() for pararell excecution
double AnaliticFunctionReader::operator() (const double& x, std::vector<std::vector<double>>& instruction_schemas, std::vector<int>& indexes_for_instruction, std::vector<int>& indexes_for_x_values) {
	int index;
	double val;
	double x1;
	double x2;

	for( int index : indexes_for_x_values ) 
		instruction_schemas[0][index] = x;

	for( int i = 0; i < indexes_for_instruction.size(); i++ ) {
		index = indexes_for_instruction[i];
		
		x1 = instruction_schemas[i][index-1];
		x2 = instruction_schemas[i][index+1];
		val = BasicFunctions::fun_vec[ (int)instruction_schemas[i][index] % MAX_FUNS ](x1, x2);

		// wstawienie wczesniej wyliczonych wartosci do nastepnego schematu przed indeksem 	
		for(int k = 0; k < index - 1; k++)
			instruction_schemas[i+1][k] = instruction_schemas[i][k];
	
		// wstawienie wyliczonej wartosci do kolejnego schematu
		instruction_schemas[i+1][index - 1] = val;

		// wstawianie wczesniej wyliczonych wartosci do nastepnego schematu po indeksie
		for(int k = index+2; k < instruction_schemas[i].size(); k++)
			instruction_schemas[i + 1][k - 2] = instruction_schemas[i][k];	
	}
	return instruction_schemas[ instruction_schemas.size() - 1 ][0];
}


void AnaliticFunctionReader::function_args_to_results (std::vector<double>::iterator start, const std::vector<double>::iterator end) {
	//creating a copy of schemas for a thread
	std::vector<std::vector<double>> instruction_schemas = _instruction_schemas;
	std::vector<int> indexes_for_instruction = _indexes_for_instruction;
	std::vector<int> indexes_for_x_values = _indexes_for_x_values;

	while ( start < end ) {
		(*start) = this->operator() (*start, instruction_schemas, indexes_for_instruction, indexes_for_x_values);
		++start;
	}
}


std::vector<double> AnaliticFunctionReader::operator() (std::vector<double>& args) {

	std::vector<Slice> data_slices;
	std::vector<double> results(args);
	std::vector<double> results_one_thread(args);
	std::vector<std::thread> threads;
	const uint16_t num_threads = std::max( (int) std::thread::hardware_concurrency() - 1, 1 );
	int batch_size = std::ceil( (double) args.size() / num_threads );

	auto start = std::chrono::high_resolution_clock::now();
	function_args_to_results(results_one_thread.begin(), results_one_thread.end() );
	auto end = std::chrono::high_resolution_clock::now();
	std::cout << "single thread exec time: " << std::chrono::duration<double>(end - start).count() << "\n";
	
	
	std::cout << "I have available " << num_threads << " threads\n";
	std::cout << "Batch size: " << batch_size << "\n";

	start = std::chrono::high_resolution_clock::now();

	// creating slices of args for threads
	int a;
	int b;
	for( int i = 0; i < num_threads; i++ ) {
		a = i*batch_size;
		b = std::min( (i+1)*batch_size, (int) args.size() );
		data_slices.push_back( Slice(results.begin() + a, results.begin() + b) );
	}

	for( Slice& slice : data_slices )
		threads.emplace_back( AnaliticFunctionReader::function_args_to_results, this, slice.first, slice.second );

	for( auto& t : threads )
		t.join();

	end = std::chrono::high_resolution_clock::now();
	std::cout << "pararell exec time: " << std::chrono::duration<double>(end - start).count() << "\n";

	// printing wrong answers becouse of thread overwritting
	for(int j = 0; j < results_one_thread.size(); j++) 
		if( results_one_thread[j] != results[j] )
			std::cout << "! Error " << results_one_thread[j] << " != " << results[j] << std::endl;

	return results;
}
