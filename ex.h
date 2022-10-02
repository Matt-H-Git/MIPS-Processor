//ex.h
/*
Use ALU to compute result or address
Take instruction from IDEX, pass to ALU and store result in EXMEM
*/
#include "mem.h"
#include <vector>
#include <algorithm>

#ifndef _EX_H
#define _EX_H

using namespace std;
//--------------------------- ALU Header
class alu{
	public:
	unsigned Rtype(unsigned pCount, vector<vector<unsigned>>& EXMEM,unsigned op, unsigned rs,unsigned rt);
	unsigned Itype(unsigned pCount, vector<vector<unsigned>>& EXMEM,unsigned op, unsigned rs,unsigned immediate);
	unsigned Branch(unsigned op, unsigned left, unsigned right);
	void setDebug(){debug = true;}
	private:
	bool debug = false;
	
};
//--------------------------- EX Header

class ex{
	public:
	void execute(unsigned pCount, vector<vector<unsigned>>& IDEX,vector<vector<unsigned>>& EXMEM, alu& ALU, mem& MEM, vector<string>& InstructionMemory); 
	unsigned fetchValue(unsigned address);
	void Rtype(vector<vector<unsigned>>& IDEX, vector<vector<unsigned>>& EXMEM, alu& ALU); 
	void Itype(vector<vector<unsigned>>& IDEX, vector<vector<unsigned>>& EXMEM, alu& ALU); 
	void Jtype(vector<vector<unsigned>>& IDEX, vector<vector<unsigned>>& EXMEM, vector<string>& InstructionMemory); 
	void Branch(vector<vector<unsigned>>& IDEX, vector<vector<unsigned>>& EXMEM, vector<string>& InstructionMemory, alu& ALU); 
	void LWSW(vector<vector<unsigned>>& IDEX, vector<vector<unsigned>>& EXMEM);
	void Out(vector<vector<unsigned>>& IDEX, vector<vector<unsigned>>& EXMEM);
	void lowerCase(string *mixed){transform(mixed->begin(), mixed->end(), mixed->begin(), ::tolower);}
	void setDebug(){debug = true;}
	void updatepCount(unsigned progCount){pCount = progCount;}
	void updateStall(bool update){stall = update;}
	bool checkStall(){return stall;}
	unsigned returnpCount(){return pCount;}
	vector<string> labelList;
	void setJump(bool set){jump = set;}
	bool checkEnd(){return endProgram;}
	bool setEnd(bool set){endProgram = set;}
	unsigned getExecutions(){return noExecutions;}
	private:
	unsigned noExecutions = 0;
	bool jump = false;
	bool stall = false;
	bool debug = false;
	unsigned pCount = 0;
	bool endProgram = false;
};

//--------------------------- EX Body

void ex::execute(unsigned progCount, vector<vector<unsigned>>& IDEX,vector<vector<unsigned>>& EXMEM, alu& ALU, mem& MEM, vector<string>& InstructionMemory){
	EXMEM.push_back(vector<unsigned>()); //Preparing for new row of data
	if(jump){
		if(debug){cout << "Execute disabled, jumping soon" << endl;}
	}
	else{
		if(debug){cout <<endl<<endl<<endl<<"//----------Execute start" << endl <<"switch case:	"<<IDEX[pCount][0] << endl;}
		noExecutions++;
		//The values stored in IDEX are the register addresses, the actual values are fetched inside the function call for the ALU later
		EXMEM[pCount+1].push_back(IDEX[pCount][5]); //Add control signals first so no index mismatch
		EXMEM[pCount+1].push_back(IDEX[pCount][6]);
		switch(IDEX[pCount][0]){
			case 1://ADD
				Rtype(IDEX, EXMEM, ALU);
			break;
			case 2://ADDI
				Itype(IDEX, EXMEM, ALU);
			break;
			case 3://AND
				Rtype(IDEX, EXMEM, ALU);
			break;
			case 4://ANDI
				Itype(IDEX, EXMEM, ALU);
			break;
			case 5: //BEQ
				Branch(IDEX,EXMEM, InstructionMemory, ALU);
			break;
			case 6: // BNE
				Branch(IDEX,EXMEM, InstructionMemory, ALU);
			break;
			case 7://J
				Jtype(IDEX, EXMEM, InstructionMemory);
			break;
			case 8://LW
				LWSW(IDEX, EXMEM);
			break;
			case 9://NOR
				Rtype(IDEX, EXMEM, ALU);
			break;
			case 10://OR
				Rtype(IDEX, EXMEM, ALU);
			break;
			case 11://ORI
				Itype(IDEX, EXMEM, ALU);
			break;
			case 12://SLT
				Rtype(IDEX, EXMEM, ALU);
			break;
			case 13://SW
				LWSW(IDEX, EXMEM);
			break;	
			case 14://SUB
				Rtype(IDEX, EXMEM, ALU);
			break;
			case 15://NORI
				Itype(IDEX,EXMEM,ALU);
			break;
			case 16://SUBI
				Itype(IDEX,EXMEM,ALU);
			break;
			case 17: //BLT
				Branch(IDEX,EXMEM, InstructionMemory, ALU);
			break;
			case 18: //BGT
				Branch(IDEX,EXMEM, InstructionMemory, ALU);
			break;
			case 19: //BLE
				Branch(IDEX,EXMEM, InstructionMemory, ALU);
			break;
			case 20: //BGE
				Branch(IDEX,EXMEM, InstructionMemory, ALU);
			break;
			case 21: //OUT
				Out(IDEX,EXMEM);
			break;
			case 22: //END
				if(!jump){
					endProgram = true;
					cout << "endProgram: " << endProgram << endl;
				}
			break;
		}
	}
}

void ex::Out(vector<vector<unsigned>>& IDEX, vector<vector<unsigned>>& EXMEM){
	if(debug){cout<< "ex::Out" << endl;}
	EXMEM[pCount+1].push_back(fetchValue(IDEX[pCount][1])); //[2]value at address
	EXMEM[pCount+1].push_back(0); //[3]blank
	EXMEM[pCount+1].push_back(IDEX[pCount][0]); //[4] //opcode
}

void ex::Rtype(vector<vector<unsigned>>& IDEX, vector<vector<unsigned>>& EXMEM, alu& ALU){
	if(debug){cout<< "ex::Rtype" << endl;}
	unsigned rd = ALU.Rtype(pCount, EXMEM, IDEX[pCount][0], fetchValue(IDEX[pCount][1]),fetchValue(IDEX[pCount][2])); // [0] = opcode, [1] = rs, [2] = rt, [3] = rd
	EXMEM[pCount+1].push_back(IDEX[pCount][3]); //Pushing back address of rd
	EXMEM[pCount+1].push_back(0); //Push back blank opcode, used for LWSW but need the index here
}

void ex::Itype(vector<vector<unsigned>>& IDEX, vector<vector<unsigned>>& EXMEM, alu& ALU){
	if(debug){cout<< "ex::Itype" << endl;}
	unsigned rt = ALU.Itype(pCount, EXMEM, IDEX[pCount][0], fetchValue(IDEX[pCount][1]), IDEX[pCount][3]); //[0] = opcode, [1] = rs, [2] = rt, [3] = immediate
	EXMEM[pCount+1].push_back(IDEX[pCount][2]); //Pushing back address of rt
	EXMEM[pCount+1].push_back(0); //Push back blank opcode, used for LWSW but need the index here
}

void ex::LWSW(vector<vector<unsigned>>& IDEX, vector<vector<unsigned>>& EXMEM){
	EXMEM[pCount+1].push_back(IDEX[pCount][1]); //[1] //Push back registers
	EXMEM[pCount+1].push_back(IDEX[pCount][2]); //[2]
	EXMEM[pCount+1].push_back(IDEX[pCount][0]); //[3] //Push back opcode last, needed for later
} //[Control][control][reg1][reg2][opcode]


void ex::Jtype(vector<vector<unsigned>>& IDEX, vector<vector<unsigned>>& EXMEM, vector<string>& InstructionMemory){
	if(debug){cout<< "ex::Jtype" << endl;}
	//Search for address line of label
	vector<unsigned> tempMemory;
	string label = labelList[IDEX[pCount][1]];
	unsigned match;
	unsigned setPC; //This will be set when the correct label line is found
	for(int i=0;i<InstructionMemory.size();i++){
		if(debug) {cout <<"Searching instruction memory" << endl;}
		//Search for the desired word in each line
		match = InstructionMemory[i].find(label);
		if (match == 0){ //match denotes the index of where the first character of the label is found
		//If it is 0, then it is the branch label we are looking for
			updatepCount(i);
			if(debug){cout <<"Jumped to line " << InstructionMemory[i] << endl <<"Line number: " << i << endl;}
		}
	}
}

void ex::Branch(vector<vector<unsigned>>& IDEX, vector<vector<unsigned>>& EXMEM, vector<string>& InstructionMemory, alu& ALU){
	if(debug){cout<< "ex::Branch" << endl;}
	//Make ALU function to decide bool
	//if(debug){"Fetching value " << fetchValue(IDEX[pCount][1]) << " from address " <<IDEX[pCount][1] << " and comparing to right value " << fetchValue(IDEX[pCount][2]) << " from address " << IDEX[pCount][2] << endl;}
	cout << fetchValue(IDEX[pCount][1]) << endl;
	cout << IDEX[pCount][1] << endl;
	cout << fetchValue(IDEX[pCount][2]) << endl;
	cout << IDEX[pCount][2] << endl;
	if(ALU.Branch(IDEX[pCount][0], fetchValue(IDEX[pCount][1]), fetchValue(IDEX[pCount][2]))){ //If OUT returns as 1 //op, left, right
		vector<unsigned> tempMemory;
		string label = labelList[IDEX[pCount][3]];
		if(debug){cout << "Label: "<<label << endl;}
		string search;
		unsigned match;
		unsigned setPC; //This will be set when the correct label line is found
		unsigned index;
		for(int i=0;i<InstructionMemory.size();i++){
			search = InstructionMemory[i];
			lowerCase(&search);
			match = search.find(label);
			//if(debug){cout <<"Instruction: " << search << "	Match: " << match<< endl;}
			//Search for the desired word in each line
			if(match != string::npos){ //Ensuring match isn't out of index (npos is end of string)
				
				//if(debug){cout << "found at : " << match << endl;}
				if(match == 0){
					//if(debug){cout <<"set index" << endl;}
					index = i;
				}
			}
		}
		//if(debug){cout << "Match found at index: " << index << endl;}
		updatepCount(index);
		if(debug){cout <<"branched to line " << InstructionMemory[index] << endl;}
	}
	else{
		if(debug){cout << "Branch condition not fulfilled, no branching" << endl;}
	}	
}

unsigned ex::fetchValue(unsigned address){
	//Open file, read in a line (the address number corresponds to the line), return it
	unsigned value; //Value to be returned
	ifstream file;
	string IN;
	file.open("General_Registers.txt");
	int i =0;
	while(getline(file,IN)){
		if (i == address){
			value = stoi(IN);
			return value;
		}
		i++;
	}
}



//--------------------------- ALU Body

unsigned alu::Rtype(unsigned pCount, vector<vector<unsigned>>& EXMEM,unsigned op, unsigned rs,unsigned rt){
	if(debug){cout << "alu::Rtype"<<endl;}
	unsigned OUT, IN1, IN2; //IN1 and IN2 are inputs to the ALU
	IN1 = rs;
	IN2 = rt;
	if(debug) {cout <<"IN1: " << IN1 << "	IN2: " << IN2 << endl;}
	switch(op){ //The numbers corresponding to each function is listed in readfile.h index of opcodeList[]
		case 1:
			OUT = IN1 + IN2; //ADD
			break;
		case 3:
			OUT = IN1 & IN2; //AND
			break;
		case 9:
			OUT = ~(IN1 | IN2); //NOR
			break;
		case 10:
			OUT = IN1 | IN2; //OR
			break;
		case 12: 
			if (IN1 < IN2){
				OUT = 1;//SLT
			}
			else
				OUT = 0;
			break;
		case 14:
			OUT = IN1 - IN2; //SUB
			break;
	}
	EXMEM[pCount+1].push_back(OUT); //Store the result in memory
	if(debug){cout << "R-type instruction complete and stored in EXMEM, result: " << OUT << endl;}
	return OUT; //To be stored in rd
}


unsigned alu::Itype(unsigned pCount, vector<vector<unsigned>>& EXMEM,unsigned op, unsigned rs,unsigned immediate){
	if(debug){cout << "alu::Itype"<<endl;}
	unsigned OUT, IN1, IN2; //IN1 and IN2 are inputs to the ALU
	IN1 = rs;
	IN2 = immediate;
	switch(op){
		case 2:
			OUT = IN1 + IN2; // ADDI
			break;
		case 4:
			OUT = IN1 & IN2; //ANDI
			break;
			
		case 11:
			OUT = IN1 | IN2; //ORI
			break;
		
		case 15:
			OUT = ~(IN1 | IN2); //NORI
			break;
			
		case 16:
			OUT = IN1 - IN2; //SUBI
			break;
			
	}
	EXMEM[pCount+1].push_back(OUT); //Store the result in memory
	if(debug){cout << "I-type instruction complete and stored in EXMEM, result: " << OUT << endl;}
	return OUT; //To be stored in rt	
}

unsigned alu::Branch(unsigned op, unsigned left, unsigned right){
	unsigned OUT = 0;
	if(debug){cout <<"Comparing left " << left << " to right " << right << endl;}
	switch(op){
		case 5: //BEQ
		if (left == right){
			OUT = 1;
		}
		break;
		case 6: //BNE
		if (left != right){
			OUT = 1;
		}
		break;
		case 17: //BLT
		if (left < right){
			OUT = 1;
		}
		break;
		case 18: //BGT
		if (left > right){
			OUT = 1;
		}
		break;
		case 19: //BLE
		if (left <= right){
			OUT = 1;
		}
		break;
		case 20: //BGE
		if (left >= right){
			OUT = 1;
		}
		break;
	}
	return OUT;
}

/*
For R-type:
instruction format:
| opcode | rs | rt | rd | shamt | funct
arithmetic format:
rd = rs *op* rt

For I-type:
instruction format:
| opcode | rs | rt | immediate
arithmetic format:
rt = rs *op* immediate

From id:
[0] = opcode
[1] = first register
[2] = second register
[3] = last register / immediate
**These are not actual values, those need to be fetched still using fetchValue
*/

#endif