#include <cstdlib>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>

using namespace std;

int main(int argc, char** argv){
	
	string branch;
	char takenOrNot;
	int branchAddress;
	int numTaken;
	int numNotTaken;
	ofstream clear;
	clear.open(argv[2], ofstream::out | ofstream::trunc);
	clear.close();

	ifstream input(argv[1], ios::in);

	ofstream output;
	output.open(argv[2], ios_base::app);
	
	while(true){
		input >> branch;
		if(input.eof())
			break;
		takenOrNot = branch[0];
		branch.erase(remove(branch.begin(), branch.end(), 'x'), branch.end());
		branchAddress = strtol(branch.c_str(), NULL, 16); 
	}
	
	input.close();
	output.close();
	return 0;
}
