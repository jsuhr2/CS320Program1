#include <cstdlib>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char** argv){
	
	
	
	ofstream clear;
	clear.open(argv[2], ofstream::out | ofstream::trunc);
	clear.close();

	ifstream input(argv[1], ios::in);

	ofstream output;
	output.open(argv[2], ios_base::app);

	
	
	input.close();
	output.close();
	return 0;
}
