#include <iostream>
#include <algorithm>
#include <cctype>
#include <string>
#include <cstring>
#include <vector>
#include <stdio.h>
#include <fstream>	
#include <stdlib.h>
#include "Assembler.h"

using namespace std;

int main() {
	
	Assembler* ass = new Assembler();
	
	string fileName; //read file name (assembler code file name)
	
	cout << "Please input the name of the file you want to convert from " << endl;
	cout << "assembily code to machine code: ";
	cout << "" << endl;
	
	cin >> fileName;
	
	ass->assemble(fileName);
	
	cout << "Your code has been converted" << endl;
	
}


void Assembler::assemble(string fileName){
	bool first = true;
	
	string line;
	
	for(int i = 0; i < 2; i++){
		//make sure temp output storage is clear
		tempOutput.clear();

		ifstream source(fileName.c_str());

		if (source.is_open()) {

			while (getline(source, line)){

				decodeLine(first,line);

			}

			source.close();
				
			lineNum = 0;	

		}
		if(first){ //after first loop append first set of 0, then values to output file
			ofstream outputFile("machineCode.txt");
			
			
			outputFile << "00000000000000000000000000000000\n";
			
			for(vector<string>::const_iterator i = tempOutput.begin(); i != tempOutput.end(); ++i) {
				outputFile << *i << '\n';
			}
		}else{ //after second loop, append instructions to output file, then second set of 0
			
			for(vector<string>::const_iterator i = tempOutput.begin(); i != tempOutput.end(); ++i) {
				outputFile << *i << '\n';
			}
			
			outputFile << "00000000000000000000000000000000\n";
			
			outputFile.close();
		}
		first = false;
	}
}

void Assembler::decodeLine(bool first,string line){

	line.erase(remove_if(line.begin(), line.end(), isspace), line.end()); //remove all white space

	if(first){
		
		if(line[0] != ';'){ //if the entire line is not a comment, increment line count
			lineNum++;
		}
		
		for(int i = 0; i < line.length(); i++){
			if(line[i] != ';'){
				if(colonCount < 3){
					if(line[i] == ':'){
						colonCount++;
					}
				}else{
					if(line[i] == ':'){
						string varName = line.substr(0, i-1);
						addLabel(varName);
						val = "";
						for(int j = i+4; line[j]!=';'; j++){
							val += line[j];
						}
						
						tempOutput.push_back(decToBinVar(val)); //add string val to temp vector of strings
						
					}
				}
			
			}else break;
		}
		
	}else{
		
		if(line[0] != ';'){
			lineNum++;
		}
		
		for(int k = 0; k < line.length(); k++){
			if(line[k] != ';'){
				if(lineNum == 1){
					if(line[k] != ':'){
						//do nothing
					}else{
						string opCode = line.substr(k+1,k+3);
						string varName = "";
						for(int l = k+4; line[l]!=';'; l++){
							varName += line[l];
						}
						//add string to temp vector of strings
						string address = getAddress(varName);
						string opCodeBin = findOpCode(opCode);
						addOps(opCodeBin,address);
					}
				}
				if(line[k] == 'E'){
					//add the stop line of code
					addOps("111", "00000");
					//end the loop
					break;
				}else{
					string opCode = line.substr(0,2);
					string varName = "";
						for(int m = 3; line[m]!=';'; m++){
							varName += line[m];
						}
					//add string to temp vector of strings
					string address = getAddress(varName);
					string opCodeBin = findOpCode(opCode);
					addOps(opCodeBin,address);
				}
			}else break;
		}
	}
}

void Assembler::addLabel(string varName){
	symbolTable[numOfLabels][0] = varName;
	symbolTable[numOfLabels][1] = decToBinAdd(to_string(lineNum));
}

void Assembler::addOps(string opCode,string address){
	string output = "";
	
	output += address;
	output += "00000000";
	output += opCode;
	output += "0000000000000000";
	
	tempOutput.push_back(output);
}

string Assembler::getAddress(string name){
	for (int i = 0; i < symbolTable.size(); i++){
		for (int j = 0; j < symbolTable[i].size(); j++)
		{
			if(symbolTable[i][j].compare(name) == 0){
				return symbolTable[i][1]; //return the address of the found label
			}
		}
	}
}

string Assembler::decToBinAdd(string toConvert){
	int rule[] = {1,2,4,8,16};
	string binNum = "00000";
	int dec = stoi(toConvert);
	
	for(int i = 5; i > 0; i--){
		if(dec>=rule[i]){
			dec = dec - rule[i];
			binNum[i] = '1';
		}
	}
	
	return binNum;
}

string Assembler::decToBinVar(string toConvert){
	int rule[32];
	//populate array
	int ruleNum = 1;
	for(int j = 0; j < 32; j++){
		rule[j] = ruleNum;
		ruleNum = ruleNum*2;
	}
	
	string binNum = "00000000000000000000000000000000";
	int dec = stoi(toConvert);
	
	
	for(int i = 32; i > 0; i--){
		if(dec>=rule[i]){
			dec = dec - rule[i];
			binNum[i] = '1';
		}
	}
	
	return binNum;
}

string Assembler::findOpCode(string opCode){
	
	//instruction set
	if (opCode == "JMP"){
		return "000";
	}
	if (opCode == "JRP"){
		return "100";
	}
	if (opCode == "LDN"){
		return "010";
	}
	if (opCode == "STO"){
		return "110";
	}
	if (opCode == "SUB"){
		return "001";
	}
	if (opCode == "CMP"){
		return "011";
	}
	if (opCode == "STP"){
		return "111";
	}
}
