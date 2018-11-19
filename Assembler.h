#include <iostream>
#include <string>
#include <vector>
#ifndef ASSEMBLER_H
#define ASSEMBLER_H

using namespace std;

class Assembler{
	private:
	string fileName;
	int lineNum;
	int colonCount;
	int numOfLabels;
	string val;

	vector<string> tempOutput;
	vector<vector<string>> symbolTable;
	
	public:
	//constructor/destructor
	Assembler();
	~Assembler();
	
	//getters and setters
	string getFileName();
	string getOutputName();
	string getAddress(string name);
	
	void setFileName(string fileName);
	void addLabel(string varName);
	void setOutputVSize(int tableSize);
	
	//main methods
	void assemble(string fileName);
	void decodeLine(bool first,string line);
	string findOpCode(string opCode);
	void addOps(string opCode,string address);
	
	string decToBinVar(string toConvert);
	string decToBinAdd(string toConvert);
};

string Assembler::getFileName(){
	return this->fileName;
}
#endif
