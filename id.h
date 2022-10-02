//id.h
/*
Take instruction from IFID register, decode and store result in IDEX
*/
#include <vector>
#include <algorithm> //For lowerCase function

#ifndef _ID_H
#define _ID_H

using namespace std;
//--------------------------- Class Header

class id{
	public:
	void decode(vector<vector<string>>& IFID, vector<vector<unsigned>>&IDEX, unsigned pCount);
	void searchCodes(vector<string>& opName, unsigned& retrn); //
	void searchRegisters(vector<string>& opName, unsigned& retrn); //
	void Itype(vector<vector<unsigned>>&IDEX, string instruction);
	void Rtype(vector<vector<unsigned>>&IDEX, string instruction);
	void LoadAndStore(vector<vector<unsigned>>&IDEX, string instruction);
	void Jump(vector<vector<unsigned>>&IDEX, string instruction);
	void Branch(vector<vector<unsigned>>&IDEX, string instruction);
	void Out(vector<vector<unsigned>>&IDEX, string instruction);
	void setDebug(){debug = true;}
	void updatepCount(unsigned progCount){pCount = progCount;}
	void updateStall(bool update){stall = update;}
	bool checkStall(){return stall;}
	void checkDataHazard(vector<vector<unsigned>>& IDEX);
	unsigned checkOP(unsigned op);
	vector<string> labelList;
	void setJump(bool set){jump = set;}
	unsigned checkStallReq(){return stallRequest;}
	void resetStallReq(){stallRequest = 0;}
	private:
	bool jump = false;
	bool debug = false;
	void lowerCase(string *mixed){transform(mixed->begin(), mixed->end(), mixed->begin(), ::tolower);}
	string vecToString(vector<string>& vec); //
	string opcodeList[23] = {"ERROR", "ADD", "ADDI", "AND", "ANDI", "BEQ", "BNE", "J", "LW", "NOR", "OR", "ORI", "SLT", "SW", "SUB", "NORI", "SUBI", "BLT", "BGT", "BLE", "BGE", "OUT", "END"};//, "LOOP1", "LOOP2", "BRANCHA", "BRANCHB","EXIT"};
	string registerList[32] = {"$zero", "$at", "$v0", "$v1", "$a0", "$a1", "$a2", "$a3", "$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7", "$s0", "$s1", "$s2", "$s3", "$s4", "$s5", "$s6", "$s7", "$t8", "$t9", "$k0", "$k1", "$gp", "$sp", "$fp", "$ra"};
	int position; ////position denotes which character in the instruction is being looked at
	unsigned RegisterNo;
	unsigned CodeNo; //Both these vars to be passed into searchRegisters and searchCodes to return the values from the functions.
	unsigned pCount;
	//unsigned stall = 0; //States how long to stall for
	bool stall = false; //States if the section should stall
	unsigned stallRequest = 0; //States how long ID is requesting the processor to stall for
};

//--------------------------- Class Body

//Implementations of instructions are derived from the MIPS Green Sheet
void id::decode(vector<vector<string>>& IFID, vector<vector<unsigned>>&IDEX, unsigned progCount){
	updatepCount(progCount); //Update the classes internal program counter value

	if(jump){
		if(debug){cout << "Decode disabled, jumping soon" << endl;}
	}
	else{
	if(debug){
		//Checking the instruction to be decoded
		cout <<endl<<endl<<endl<<"//----------Decoding" <<endl << endl<< "PC: "<<pCount<<"	Instruction: "<<IFID[pCount][0]<<endl;
	}
	string instruction = IFID[pCount][0]; //Take current instruction
	vector<string> command; //Will store each separated decimal code (opcode, register locations)
	unsigned op = 0; //Denotes the opcode in decimal
	position = 0;
	string typeConversion; //Used for type conversion since a pointer cannot be pushed onto a vector
	//Search for the opcode
	bool leave = false;
	if(debug){cout<< "Reading through instruction for op code" << endl;}
	while (!leave){
		//if(debug){cout << "instruction["<<position<<"]:		"<<instruction[position]<<endl;}
		typeConversion = instruction[position];
		
		if (typeConversion == " "){ //Keeping these separate in case I need to label one later for loops or something
			if(debug){
				//Checking the opcode being extracted from the instruction
				cout << "Checking opcode:	";
				for(int i=0;i<command.size();i++){
					cout << command[i];
				}
				cout << endl;
			}
			searchCodes(command, op);
			command.clear();
			leave = true;
		}
		if (typeConversion == ":"){ //Found label, clear command but carry on
			if(debug) {cout << "Found a label" << endl;}
			command.clear();
			position+= 2; //Move out of the space after the colon
		}
		else{
			command.push_back(typeConversion);
			position++;
		}
	}
	typeConversion = instruction[position];
	if (typeConversion == " "){position++;} //To take into account the blank space between command and first register if there is one
	string delimeter; // The delimeter will be updated as the instruction is decoded
	command.clear(); //Clear command before new characters are added in switch case
	IDEX.push_back(vector<unsigned>()); 
	IDEX[pCount+1].push_back(op); //[0] //Required for ex
	//Wrote each case statement separately since in testing there was a weird case of once one case statement finished, each consecutive one after just started for no reason
	if(debug){cout<< "switch(op): " <<op << endl;}
	switch(op){
		case 0:
			cout << "Error, no opcode detected.";
			break;
		case 1: //ADD
			Rtype(IDEX, instruction);
			break;
		case 2: //ADDI
			Itype(IDEX, instruction);
			break;
		case 3: //AND
			Rtype(IDEX, instruction);
			break;
		case 4: //ANDI
			Itype(IDEX, instruction);
			break;
		case 5: //BEQ
			Branch(IDEX, instruction);
			break;
		case 6: //BNE
			Branch(IDEX, instruction);
			break;
		case 7: //J
			Jump(IDEX, instruction);
			break;
		case 8: //LW // lw $t0, $t1 //Load value of t1 into t0
			LoadAndStore(IDEX, instruction);
			IDEX[pCount+1].push_back(1); //WB control signal
			break;
		case 9: //NOR
			Rtype(IDEX, instruction);
			break;
		case 10: //OR
			Rtype(IDEX, instruction);
			break;
		case 11: //ORI
			Itype(IDEX, instruction);
			break;
		case 12: //SLT
			Rtype(IDEX, instruction); // slt $1, $2, $3  //$1 = 1 if ($2 < $3) else $1 = 0
			break;
		case 13: //SW //sw $t0, $t1 //Store value of to into t1
			LoadAndStore(IDEX, instruction);
			IDEX[pCount+1].push_back(0); //WB control signal
			break;
		case 14: //SUB
			Rtype(IDEX, instruction);
			break;
		case 15: //NORI
			Itype(IDEX, instruction);
			break;
		case 16: //SUBI
			Itype(IDEX, instruction);
			break;
		case 17: //BLT
			Branch(IDEX, instruction);
			break;
		case 18: //BGT
			Branch(IDEX, instruction);
			break;
		case 19: //BLE
			Branch(IDEX, instruction);
			break;
		case 20: //BGE
			Branch(IDEX, instruction);
			break;
		case 21: //OUT //Output value at register
			Out(IDEX, instruction);
			break;
		case 22: // END
			if(debug){cout <<"//------------------------Ending program -------------------------//" << endl;}
			stallRequest = 2;
			IDEX[pCount+1].push_back(0); //[1] blank
			IDEX[pCount+1].push_back(0); //[2] blank
			IDEX[pCount+1].push_back(0); //[3] blank
			IDEX[pCount+1].push_back(1); //[4] EX control signal
			IDEX[pCount+1].push_back(0); //[5] blank
			IDEX[pCount+1].push_back(0); //[6] blank
			break;
	}
	checkDataHazard(IDEX);	
	}	
}

void id::checkDataHazard(vector<vector<unsigned>>& IDEX){
	if(debug){cout << "Checking data hazard, stall request: " << stallRequest << endl;}
	/*
	Check current instruction opcode against next instruction opcode to check for how many registers to check against
	Also need to check 2 instructions back, but only stall for 1 cycle
	Rtype instructions have 3 registers
	Itype have 2 registers (includes LW and SW)
	Also OUT only has 1 register
	Rtype opcodes:
	1,3,9,10,12,14
	Itype opcodes:
	2,4,5,6,8,11,13,15,16,17,18,19,20
	OUT opcode:
	21
	create function to find whether Itype, Rtype or OUT for next and current instruction
	Use the bools to complete register checks
	*/
	//Rtype = 3, Itype = 2, OUT = 1
	unsigned previous,current, next; //To store what type of instruction the current and next instructions are
	current = checkOP(IDEX[pCount][0]);
	next = checkOP(IDEX[pCount+1][0]);
	
	//Read in register numbers into 2 vectors
	vector<unsigned> previousRegs, currentRegs, nextRegs;
	for(int i=0;i<current;i++){
		currentRegs.push_back(IDEX[pCount][i+1]);
	}
	for(int i=0;i<next;i++){
		nextRegs.push_back(IDEX[pCount+1][i+1]);
	}
	//Run a nested loop to cross check if any values match
	for (int i=0;i<currentRegs.size();i++){
		for(int y=0;y<nextRegs.size();y++){
				if (currentRegs[i] == nextRegs[y]){
					if (stallRequest == 0){
						stallRequest = 2;
						if(debug){cout << "Stalling for 2 cycles" << endl;}
						} //Comparing current to next instruction
				}
		}
	}
	if(pCount != 0){
		if(stallRequest == 0){
			previous = checkOP(IDEX[pCount-1][0]);
			for(int i=0;i<previous;i++){
				previousRegs.push_back(IDEX[pCount-1][i+1]);
			}
			for (int i=0;i<previousRegs.size();i++){
				for(int y=0;y<nextRegs.size();y++){
						if (previousRegs[i] == nextRegs[y]){
							if (stallRequest == 0){
								stallRequest = 1;
								if(debug){cout << "Stalling for 1 cycle" << endl;}
								} //Comparing previous instruction to next instruction
						}
				}
			}
		}
	}
}
unsigned id::checkOP(unsigned op){
	unsigned result = 0;
	if (  (op == 1)||  (op == 3)||  (op == 9)||  (op == 10)||  (op == 12)||  (op == 14)){
		result = 3; //Rtype
	}
	else if(  (op == 2)||  (op == 4)||  (op == 5)||  (op == 6)||  (op == 8)||  (op == 11)||  (op == 13)||  (op == 15)||  (op == 16)||  (op == 17)||  (op == 18)||  (op == 19)||  (op == 20)){
		result = 2; //Itype
	}
	else if(  op == 21){
		result = 1; //OUT
	}
	return result;
}

void id::Out(vector<vector<unsigned>>&IDEX, string instruction){
	if(debug){cout<< "OUT" << endl;}
	string typeConversion;
	vector<string> command;
	typeConversion = instruction[position];
	unsigned regCount = 0;
	while (regCount != 1){ //Only reading 1 register into index [1]
		if(typeConversion == "$"){
			command.push_back(typeConversion);
			typeConversion = instruction[position+1];
			command.push_back(typeConversion);
			typeConversion = instruction[position+2];
			command.push_back(typeConversion);
			searchRegisters(command, RegisterNo);
			//if(debug){cout << "Searched registers, found regNo:		"<<RegisterNo << endl;}
			IDEX[pCount+1].push_back(RegisterNo);
			if(debug){cout << "Pushed back regNo:	" << RegisterNo << endl;}
			command.clear();
			regCount++;
			if(debug){cout<<"Read in "<<regCount<<" registers" << endl;}
		}
		position++;
		typeConversion = instruction[position];
	}
	//Control signals
	IDEX[pCount+1].push_back(0); //[2] //Blank
	IDEX[pCount+1].push_back(0); //[3] //Blank
	IDEX[pCount+1].push_back(1); //[4] //Control signal for EX
	IDEX[pCount+1].push_back(1); //[5] //MEM
	IDEX[pCount+1].push_back(0); //[6] //WB
}

void id::Jump(vector<vector<unsigned>>&IDEX, string instruction){
	//Same as id::Branch but without reading in the first 2 registers
	if(debug){cout <<"id::Jump"<<endl;}
	string typeConversion;
	vector<string> command;
	typeConversion = instruction[position];
	while (position != instruction.length()){
		if (typeConversion == " "){ //Ensure a space is never written in
			position++;
			typeConversion = instruction[position];
		}
		else{
			command.push_back(typeConversion);
			position++;
			typeConversion = instruction[position]; //Read in the rest of the line, which is the rest of the label
		}
		
	}
	string label;
	label = vecToString(command); //Label can be called anything
	lowerCase(&label);
	labelList.push_back(label);
	if(debug){cout <<"Read in label: " << label << endl;}
	IDEX[pCount+1].push_back((labelList.size())-1); //IDEX[][1] //.size() will give the last index, which is the index of the stored label
	command.clear();
	//Control signals missing
	IDEX[pCount+1].push_back(0); //[2]//Blank
	IDEX[pCount+1].push_back(0); //[3]//Blank
	IDEX[pCount+1].push_back(1); //[4]//Control signal for EX
	IDEX[pCount+1].push_back(0); //[5]//Control signal for MEM
	IDEX[pCount+1].push_back(0); //[6]//Control signal for WB
}

void id::Branch(vector<vector<unsigned>>&IDEX, string instruction){
	//Format:
	/*
		opcode (up until space) (already determined and stored)
		first register
		second register
		Label
		(first register *Comparator* second register) branch to label
	*/
	if(debug){cout <<"id::Branch"<<endl;}
	string typeConversion;
	vector<string> command;
	typeConversion = instruction[position];
	unsigned searchAtIndex;
	unsigned regCount = 0;
	
	while (regCount != 3){
		if (regCount == 2){
			//Read in label to [3]
			if (typeConversion == " "){ //Ensure a space is never written in
				command.clear();
			}
			else{
				while (position != instruction.length()){
					command.push_back(typeConversion);
					position++;
					typeConversion = instruction[position]; //Read in the rest of the line, which is the rest of the label
				}
				string label;
				label = vecToString(command); //Label can be called anything
				lowerCase(&label);
				labelList.push_back(label);
				if(debug){cout <<"Read in label: " << label << "	in index: " << labelList.size()-1<<endl;}
				IDEX[pCount+1].push_back((labelList.size())-1); //IDEX[][3] //.size() will give the last index, which is the index of the stored label
				command.clear();
				regCount++;
			}
		}
		else if(typeConversion == "$"){ //Registers read in to [1], [2]
			command.push_back(typeConversion);
			typeConversion = instruction[position+1];
			command.push_back(typeConversion);
			typeConversion = instruction[position+2];
			command.push_back(typeConversion);
			position+=3;
			searchRegisters(command, RegisterNo);
			//if(debug){cout << "Searched registers, found regNo:		"<<RegisterNo << endl;}
			IDEX[pCount+1].push_back(RegisterNo);
			if(debug){cout << "Pushed back regNo:	" << RegisterNo << endl;}
			command.clear();
			regCount++;
			if(debug){cout<<"Read in "<<regCount<<" registers" << endl;}
		}
		position++;
		typeConversion = instruction[position];
	}
	//Control signals missing
	IDEX[pCount+1].push_back(1); //[4]//Control signal for EX
	IDEX[pCount+1].push_back(0); //[5]//Control signal for MEM
	IDEX[pCount+1].push_back(0); //[6]//Control signal for WB
}

void id::Itype(vector<vector<unsigned>>&IDEX, string instruction){ //ADDI, ORI
	if(debug){cout <<"id::I-type"<<endl;}
	string typeConversion;
	vector<string> command;
	int registerCount = 0; 
	string delimeter = ",";
	typeConversion = instruction[position];
	while (position != instruction.length()){
		if (registerCount == 2){ //Loop here once the two registers have been read in
			unsigned constant;
			sscanf(&instruction[position], "%d", &constant);
			cout << "constant " <<constant << endl;			
			IDEX[pCount+1].push_back(constant); //Pushing back the constant
			if(debug){cout << "Reached constant, added to IDEX. Instruction finished reading.	position: " << position << "	instruction length:	" << instruction.length() << endl;}
			break;
		}
		if(typeConversion != delimeter){ //Find out the two register names
			command.push_back(typeConversion);
		}
		if (typeConversion == delimeter){ //Loop here once a whole register name has been read in
			//if(debug){cout << "== delimeter" << endl;}
			searchRegisters(command, RegisterNo);
			//if(debug){cout << "Searched registers, found regNo:		"<<RegisterNo << endl;}
			IDEX[pCount+1].push_back(RegisterNo);
			if(debug){cout << "Pushed back regNo:	" << RegisterNo << endl;}
			command.clear();
			registerCount++;
		}
		position++;
		typeConversion = instruction[position];
		if(typeConversion == " "){
			position++;
			typeConversion = instruction[position];
		}
	}
	//Adding control signals to register
	if(debug){cout << "Adding Control signals for id::I-type (1 0 1)" << endl;}
	IDEX[pCount+1].push_back(1); //EX
	IDEX[pCount+1].push_back(0); //MEM
	IDEX[pCount+1].push_back(1); //WB
}

void id::Rtype(vector<vector<unsigned>>&IDEX, string instruction){ //ADD , AND , NOR , OR , SUB
	if(debug){cout <<"id::R-type"<<endl;} //Same as I-type but without register count since there are always 3 registers for R-type instructions
	string typeConversion;
	vector<string> command;
	typeConversion = instruction[position];
	unsigned searchAtIndex;
	unsigned regCount = 0;
	//Read in 2 characters after a $ sign then search for register, and count for 3 registers
	while (regCount != 3){
		if(typeConversion == "$"){
			command.push_back(typeConversion);
			typeConversion = instruction[position+1];
			command.push_back(typeConversion);
			typeConversion = instruction[position+2];
			command.push_back(typeConversion);
			position+=3;
			searchRegisters(command, RegisterNo);
			//if(debug){cout << "Searched registers, found regNo:		"<<RegisterNo << endl;}
			IDEX[pCount+1].push_back(RegisterNo);
			if(debug){cout << "Pushed back regNo:	" << RegisterNo << endl;}
			command.clear();
			regCount++;
			if(debug){cout<<"Read in "<<regCount<<" registers" << endl;}
		}
		position++;
		typeConversion = instruction[position];
	}
	//Adding control signals to register, same as I type
	if(debug){cout << "Adding Control signals for id::R-type (1 0 1)" << endl;}
	IDEX[pCount+1].push_back(1); //EX
	IDEX[pCount+1].push_back(0); //MEM
	IDEX[pCount+1].push_back(1); //WB
}

void id::LoadAndStore(vector<vector<unsigned>>&IDEX, string instruction){
	if(debug){cout<< "LW, SW" << endl;}
	string typeConversion;
	vector<string> command;
	string delimeter = ",";
	
	typeConversion = instruction[position];
	unsigned regCount = 0;
	while (regCount != 2){ //Same as R-type but only reading in 2 registers
		cout << typeConversion;
		if(typeConversion == "$"){
			command.push_back(typeConversion);
			typeConversion = instruction[position+1];
			command.push_back(typeConversion);
			typeConversion = instruction[position+2];
			command.push_back(typeConversion);
			position+=3;
			searchRegisters(command, RegisterNo);
			//if(debug){cout << "Searched registers, found regNo:		"<<RegisterNo << endl;}
			IDEX[pCount+1].push_back(RegisterNo);
			if(debug){cout << "Pushed back regNo:	" << RegisterNo << endl;}
			command.clear();
			regCount++;
			if(debug){cout<<"Read in "<<regCount<<" registers" << endl;}
		}
		position++;
		typeConversion = instruction[position];
	}
	IDEX[pCount+1].push_back(0);//Pushing blank number so control signal indexes match that of R and I types
	//Adding control signals to register
	IDEX[pCount+1].push_back(1); //EX
	IDEX[pCount+1].push_back(1); //MEM
	//WB is different for LW and SW so is written in the switch case statement
}


string id::vecToString(vector<string>& vec){ //Convert the vector to a string
	string retrn = ""; //String to return
	for (int i=0;i<vec.size();i++){ //The first index of the vector was a space and couldn't stop it from being written to the string
		retrn.append(vec[i]);
	}
	return retrn;
}

void id::searchCodes(vector<string>& opName, unsigned& retrn){
	string opString = vecToString(opName);
	lowerCase(&opString);
	for(retrn=0;retrn<32;retrn++){
		lowerCase(&opcodeList[retrn]);
	}
	retrn = 0;
	while (opString != opcodeList[retrn]){
		retrn++;
		//if(debug){cout << "retrn:  " << retrn << "	opString:  "<<opString << "		" << "regList:  " << opcodeList[retrn] << endl; }
	}
}

void id::searchRegisters(vector<string>& opName, unsigned& retrn){
	string opString = vecToString(opName);
	lowerCase(&opString);
	retrn = 0;
	lowerCase(&registerList[0]);
	while (opString != registerList[retrn]){
		lowerCase(&registerList[retrn]);
		retrn++;
		//if(debug){cout << "retrn:  " << retrn << "	opString:  "<<opString << "		" << "regList:  " << registerList[retrn] << endl; }
	}
}

#endif
