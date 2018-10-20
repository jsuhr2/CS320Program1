#include <cstdlib>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>

using namespace std;

int predictor1bit(int table[], int i, string tON){
	int add = 0;
	if(!(tON.compare("T"))){
		if(table[i] >= 4)
			add++;
		else
			table[i] += 4;
	}else{
		if(table[i] < 4)
			add++;
		else
			table[i] -= 4;
	}
	return add;
}

int predictor2bit(int table[], int i, string tON){
	int add = 0;
	int remain = table[i] % 4;
	if(!(tON.compare("T"))){
		if(remain == 3)
			add++;
		else if(remain == 2){
			table[i]++;
			add++;
		}
		else
			table[i]++;
	}else{
		if(remain == 0)
			add++;
		else if(remain == 1){
			table[i]--;
			add++;
		}
		else
			table[i]--;
	}
	return add;
}

int tournamentPredictor(int table[], int bimodalPrediction, int gsharePrediction, string tON, long long bA){
	int add = 0;
	if(!(tON.compare("T"))){
		if(table[bA % 2048] == 0){
			if(gsharePrediction >= 2){
				add++;
			} else if(bimodalPrediction >= 2)
				table[bA % 2048] += 1;
		}else if(table[bA % 2048] == 1){
			if(gsharePrediction >= 2){
				add++;
				if(bimodalPrediction <= 1)
					table[bA % 2048] -= 1;
			} else if(bimodalPrediction >= 2)
				table[bA % 2048] += 1;
		}else if(table[bA % 2048] == 2){
			if(bimodalPrediction >= 2){
				add++;
				if(gsharePrediction <= 1)
					table[bA % 2048] += 1;
			} else if(gsharePrediction >= 2)
				table[bA % 2048] -= 1;
		}else if (table[bA % 2048] == 3){
			if(bimodalPrediction >= 2){
				add++;
			} else if(gsharePrediction >= 2)
				table[bA % 2048] -= 1;
		}
	}else{
		if(table[bA % 2048] == 0){
			if(gsharePrediction <= 1){
				add++;
			} else if(bimodalPrediction <= 1)
				table[bA % 2048] += 1;
		}else if(table[bA % 2048] == 1){
			if(gsharePrediction <= 1){
				add++;
				if(bimodalPrediction >= 2)
					table[bA % 2048] -= 1;
			} else if(bimodalPrediction <= 1)
				table[bA % 2048] += 1;
		}else if(table[bA % 2048] == 2){
			if(bimodalPrediction <= 1){
				add++;
				if(gsharePrediction >= 2)
					table[bA % 2048] += 1;
			} else if(gsharePrediction <= 1)
				table[bA % 2048] -= 1;
		}else if(table[bA % 2048] == 3){
			if(bimodalPrediction <= 1){
				add++;
			} else if(gsharePrediction <= 1)
				table[bA % 2048] -= 1;
		}
	}
	return add;
}

int main(int argc, char** argv){
//initialize variables	
	string branch;
	string takenOrNot;
	long long branchAddress = 0;
	int numTaken = 0;
	int numNotTaken = 0;
	int numBranches = 0;
	int bit1_16, bit1_32, bit1_128, bit1_256, bit1_512, bit1_1024, bit1_2048;
	bit1_16 = bit1_32 = bit1_128 = bit1_256 = bit1_512 = bit1_1024 = bit1_2048 =  0;
	int bit2_16, bit2_32, bit2_128, bit2_256, bit2_512, bit2_1024, bit2_2048;
	bit2_16 = bit2_32 = bit2_128 = bit2_256 = bit2_512 = bit2_1024 = bit2_2048 =  0;
	int g3, g4, g5, g6, g7, g8, g9, g10, g11;
	g3 = g4 = g5 = g6 = g7 = g8 = g9 = g10 = g11 = 0; 
	int globalHistReg = 0;
	int tournamentCount = 0;
	int maxLength = 0;

//varying table sizes
	int table16 [16];
	int table32 [32];
	int table128 [128];
	int table256 [256];
	int table512 [512];
	int table1024 [1024];
	int table2048 [2048];

//gshare predictor
	int gtable [9][2048];

//tournament predictor
	int entries [2048];

//clear output file
	ofstream clear;
	clear.open(argv[2], ofstream::out | ofstream::trunc);
	clear.close();

//open files
	ifstream input(argv[1], ios::in);

	ofstream output;
	output.open(argv[2], ios_base::app);

//initialize all tables to 7, bit 3 will be used for the 1 bit predictor and bits 1 and 2 will be used for 2 bit predictor (starts with strongly taken and taken -> 7)
	for(int i = 0; i < 2048; i++){
		if(i < 16)
			table16[i] = 7;
		if(i < 32)
			table32[i] = 7;
		if(i < 128)
			table128[i] = 7;
		if(i < 256)
			table256[i] = 7;
		if(i < 512)
			table512[i] = 7;
		if(i < 1024)
			table1024[i] = 7;
		table2048[i] = 7;	
	}
//initialize all gshare tables to 3 - always taken
	for(int i = 0; i < 2048; i++){
		for(int j = 0; j < 9; j++){
			gtable[j][i] = 3;
		}
	}

//initialize entries array with 0
	for(int i = 0; i < 2048; i++){
		entries[i] = 0;
	}

	while(true){
		//extract info
		input  >> branch;
		if(input.eof())
			break;
		input >> takenOrNot;
		branch.erase(remove(branch.begin(), branch.end(), 'x'), branch.end());
		branchAddress = stol(branch.c_str(), NULL, 16);
		
		//compute tournament
		tournamentCount += tournamentPredictor(entries, (table2048[branchAddress % 2048] % 4), gtable[8][(branchAddress % 2048) ^ (globalHistReg % 2048)], takenOrNot, branchAddress);
		
		//compute gshare
		maxLength = 1 << 3;
		g3 += predictor2bit(gtable[0], (branchAddress % 2048) ^ (globalHistReg % maxLength), takenOrNot);
		maxLength = 1 << 4;
		g4 += predictor2bit(gtable[1], (branchAddress % 2048) ^ (globalHistReg % maxLength), takenOrNot); 
		maxLength = 1 << 5;
		g5 += predictor2bit(gtable[2], (branchAddress % 2048) ^ (globalHistReg % maxLength), takenOrNot); 
		maxLength = 1 << 6;
		g6 += predictor2bit(gtable[3], (branchAddress % 2048) ^ (globalHistReg % maxLength), takenOrNot); 
		maxLength = 1 << 7;
		g7 += predictor2bit(gtable[4], (branchAddress % 2048) ^ (globalHistReg % maxLength), takenOrNot); 
		maxLength = 1 << 8;
		g8 += predictor2bit(gtable[5], (branchAddress % 2048) ^ (globalHistReg % maxLength), takenOrNot); 
		maxLength = 1 << 9;
		g9 += predictor2bit(gtable[6], (branchAddress % 2048) ^ (globalHistReg % maxLength), takenOrNot); 
		maxLength = 1 << 10;
		g10 += predictor2bit(gtable[7], (branchAddress % 2048) ^ (globalHistReg % maxLength), takenOrNot); 
		maxLength = 1 << 11;
		g11 += predictor2bit(gtable[8], (branchAddress % 2048) ^ (globalHistReg % maxLength), takenOrNot); 

		globalHistReg = globalHistReg << 1;

		//compute parts 1 and 2 along with total number of commands, update history register
		if(!(takenOrNot.compare("T"))){
			globalHistReg++;
			numTaken++;
		}
		if(!(takenOrNot.compare("NT"))){
			numNotTaken++;
		}
		numBranches++;

		//compute 1 bit bimodal predictors
		bit1_16 += predictor1bit(table16, branchAddress % 16, takenOrNot);
		bit1_32 += predictor1bit(table32, branchAddress % 32, takenOrNot);
		bit1_128 += predictor1bit(table128, branchAddress % 128, takenOrNot);
		bit1_256 += predictor1bit(table256, branchAddress % 256, takenOrNot);
		bit1_512 += predictor1bit(table512, branchAddress % 512, takenOrNot);
		bit1_1024 += predictor1bit(table1024, branchAddress % 1024, takenOrNot);
		bit1_2048 += predictor1bit(table2048, branchAddress % 2048, takenOrNot);
		//compute 2 bit bimodal predictors
		bit2_16 += predictor2bit(table16, branchAddress % 16, takenOrNot);
		bit2_32 += predictor2bit(table32, branchAddress % 32, takenOrNot);
		bit2_128 += predictor2bit(table128, branchAddress % 128, takenOrNot);
		bit2_256 += predictor2bit(table256, branchAddress % 256, takenOrNot);
		bit2_512 += predictor2bit(table512, branchAddress % 512, takenOrNot);
		bit2_1024 += predictor2bit(table1024, branchAddress % 1024, takenOrNot);
		bit2_2048 += predictor2bit(table2048, branchAddress % 2048, takenOrNot);
		
		//keep history register to 11 bits max
		globalHistReg = globalHistReg & 2047;
	}
	//print parts 1 and 2
	output << numTaken << ", " << numBranches << "; " << endl;
	output << numNotTaken << ", " << numBranches << "; " << endl;
	
	//print part 3
	output << bit1_16 << ", " << numBranches << "; ";
	output << bit1_32 << ", " << numBranches << "; ";
	output << bit1_128 << ", " << numBranches << "; ";
	output << bit1_256 << ", " << numBranches << "; ";
	output << bit1_512 << ", " << numBranches << "; ";
	output << bit1_1024 << ", " << numBranches << "; ";
	output << bit1_2048 << ", " << numBranches << "; " << endl;
	
	//print part 4
	output << bit2_16 << ", " << numBranches << "; ";
	output << bit2_32 << ", " << numBranches << "; ";
	output << bit2_128 << ", " << numBranches << "; ";
	output << bit2_256 << ", " << numBranches << "; ";
	output << bit2_512 << ", " << numBranches << "; ";
	output << bit2_1024 << ", " << numBranches << "; ";
	output << bit2_2048 << ", " << numBranches << "; " << endl;

	//print part 5
	output << g3 << ", " << numBranches << "; ";
	output << g4 << ", " << numBranches << "; ";
	output << g5 << ", " << numBranches << "; ";
	output << g6 << ", " << numBranches << "; ";
	output << g7 << ", " << numBranches << "; ";
	output << g8 << ", " << numBranches << "; ";
	output << g9 << ", " << numBranches << "; ";
	output << g10 << ", " << numBranches << "; ";
	output << g11 << ", " << numBranches << "; " << endl;
	
	//print part 6
	output << tournamentCount << ", " << numBranches << "; " << endl;
	
	input.close();
	output.close();
	return 0;
}
