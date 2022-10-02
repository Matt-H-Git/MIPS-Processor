//if.h
/*
Read next instruction from memory (address pointed to by PC)
Store in instruction register (IFID)
*/

#ifndef _IF_H
#define _IF_H

#include <vector>
#include <string>
#include <iostream>

using namespace std;

//---------------------------Class Header for Fetch

class fetch{
	public:
	void readInput(vector<string>&InstructionMemory, unsigned pCount); //To be extended to read assembly later;
	void sendNextInstruction(vector<vector<string>>& IFID, vector<string>& InstructionMemory, unsigned pCount);
	void setDebug(){debug = true;}
	void IncreaseInstructionCount(){InstructionCount++;}
	unsigned GetInstructionCount(){return InstructionCount;}
	void setJump(bool set, unsigned pCount);
	private:
	bool debug = false;
	bool jump = false;
	unsigned InstructionCount = 0; //To keep count of how many instructions are left to be read in
	unsigned InstructionsFetched = 0;
};

//---------------------------Class Function Declarations for Fetch
void fetch::setJump(bool set, unsigned pCount){
	//pCount denotes the number instruction being read in
	InstructionsFetched = pCount;
	jump = set;
}

void fetch::readInput(vector<string>&InstructionMemory, unsigned pCount){
	ifstream instructionFile;
	string IN;
	instructionFile.open("assemblyinstructions.txt"); //Opening file
	if(debug){cout<<endl<< "Opened File assemblyinstructions.txt" << endl;}
	//Read file and store it in memory
	int row = 0;
	while(getline(instructionFile, IN)){
		InstructionMemory.push_back(IN); //Read all the instructions into InstructionMemory, feed into IFID one instruction at a time
		//This will sort itself out when control signals are implemented and only one instruction is read in for each value of pCount
		if(debug){cout << IN << endl;}
		IncreaseInstructionCount();
	}
	instructionFile.close();
	if(debug){cout <<"Instruction Count:	"<<InstructionCount<<endl;}
}

void fetch::sendNextInstruction(vector<vector<string>>& IFID, vector<string>& InstructionMemory, unsigned pCount){
	if(InstructionsFetched == InstructionCount){
		if(debug){cout << "All instructions fetched" << endl;}
	}
	else if(jump){ //Preparing to jump / branch
		if(debug){cout << "Not reading instructions, preparing to jump" << endl;}
	}
	else{
		if(debug){cout <<endl<<endl<< "//----------Fetching instruction: "<<endl<<InstructionMemory[InstructionsFetched] << endl;}
		IFID.push_back(vector<string>());
		IFID[pCount+1].push_back(InstructionMemory[InstructionsFetched]);
		IFID[pCount+1].push_back("1"); //Control signal for decode
		InstructionsFetched++;
	}
}

#endif