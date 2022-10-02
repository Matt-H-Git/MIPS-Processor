#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>

#include "pc.h"
#include "if.h"
#include "ex.h"
#include "id.h"
#include "wb.h"
//mem.h is included in ex.h so cannot be included here


#define DEBUG true //set this to false to disable debugging mode

/*Improvements
Make all classes inherit a 'control' class
A few repeated functions in each class which could be removed by including this
Improve decoding so that it is more global and not specific to each type of instruction
*/

void clearMemory();
void clearRegisters(vector<vector<unsigned>>&IDEX,vector<vector<unsigned>>&EXMEM,vector<vector<unsigned>>&MEMWB,vector<vector<string>>&IFID, unsigned pCount);
void checkHazards(id& Decode, ex& EX, bool& dataHazard, unsigned& stall);
void printStringVector(vector<vector<string>>&vec);
void printUnsignedVector(vector<vector<unsigned>>&vec);
void updateRegisters(vector<vector<unsigned>>&IDEX,vector<vector<unsigned>>&EXMEM,vector<vector<unsigned>>&MEMWB,vector<vector<string>>&IFID, unsigned pCount);
void updateUnsigned(vector<vector<unsigned>>&vec, unsigned rowMax);
void updateString(vector<vector<string>>&vec, unsigned rowMax);

int main(){
	int i =0; //Counts number of cycles //Information purpose only
	unsigned stall = 0; //Used to determine how many loops to stall for
	
	//Initialising classes
	fetch Fetch;
	id Decode;
	pc PC;
	alu ALU;
	ex EX;
	mem MEM;
	wb WB;
	
	if (DEBUG){ //Enabling debug for each class instantiation
		Fetch.setDebug();
		Decode.setDebug();
		ALU.setDebug();
		EX.setDebug();
		MEM.setDebug();
		WB.setDebug();
	}
	
	//Initialising registers and memory as vectors
	vector<vector<unsigned>> IDEX, EXMEM, MEMWB, DataMemory;
	vector<string> InstructionMemory;
	vector<vector<string>> IFID;
	
	if(DEBUG){cout << "Initialising registers" << endl;}
	updateRegisters(IDEX, EXMEM, MEMWB, IFID, 0); //Initialise all registers for first loop
	clearMemory(); //This clears the General_Registers and DataMemory text files to all 0 values
	
	bool dataHazard = false; //Detecting if the program is stalling or not
	//Read in all instructions and store into Instruction Memory
	if(DEBUG){cout<<"readFile start" << endl;}
	Fetch.readInput(InstructionMemory, PC.accessPC());
	if(DEBUG){
		cout <<endl <<"Reading InstructionMemory" << endl;
		for(int i=0;i<InstructionMemory.size();i++){
			cout << InstructionMemory[i] << endl;
		}
	}
	if(DEBUG){cout<<"readFile complete" <<endl<< endl;}
	//Start pipelining from here
	
	while (!EX.checkEnd()){
		
		//Fetch instruction from memory
		if(DEBUG){cout<<endl<<endl<<endl<<"				Program Count:		"<<PC.accessPC()<<"	Hazard status:	" << dataHazard<<endl;}
		if(DEBUG){cout <<"						Number of cycles:		" <<i<<endl<<endl<<endl;}
		if(DEBUG){ //Check values of key registers
			fstream File;
			string IN;
			File.open("General_Registers.txt"); 
			vector<unsigned> tempMemory; 
			while(getline(File,IN)){
				tempMemory.push_back(stoi(IN)); 
			}	
			File.close();
			cout <<"$t0: " << tempMemory[8] <<" $t1: "<<tempMemory[9]<<" $t2: "<<tempMemory[10]<<" $t3: "<<tempMemory[11]<<" $t4: " << tempMemory[12] << endl;
		}
		
		if(DEBUG){ //States of IDEX, EXMEM and MEMWB
			//if(DEBUG){cout <<endl<< "IFID:" << endl;}
			//if(DEBUG){printStringVector(IFID);}
			//if(DEBUG){cout <<endl<< "IDEX:" << endl;}
			//if(DEBUG){printUnsignedVector(IDEX);}
			//if(DEBUG){cout <<endl<< "EXMEM:" << endl;}
			//if(DEBUG){printUnsignedVector(EXMEM);}
			//if(DEBUG){cout <<endl<< "MEMWB:" << endl;}
			//if(DEBUG){printUnsignedVector(MEMWB);}
		}
		
		if (dataHazard == false){ //If no data hazard (stall) fetch next instruction
			Fetch.sendNextInstruction(IFID, InstructionMemory, PC.accessPC());
		}
		
		
		//Decode the instruction
		if(Decode.checkStall()){
			if(DEBUG){cout << "Stalling ID" << endl;}
			IFID.push_back(vector<string>());
			IFID[PC.accessPC()+1].push_back(IFID[PC.accessPC()][0]);
			IFID[PC.accessPC()+1].push_back(IFID[PC.accessPC()][1]);
		}
		else if(stoi(IFID[PC.accessPC()][1])){
			Decode.decode(IFID, IDEX, PC.accessPC());
		}
		
		//execute the instruction
		EX.updatepCount(PC.accessPC());//Always update EX pCount since it is used for jumps / branches
		if(EX.checkStall()){
			if(DEBUG){cout << "Stalling EX" << endl;}
			IDEX.push_back(vector<unsigned>()); 
			EXMEM.push_back(vector<unsigned>());
			for(unsigned i=0;i<IDEX[PC.accessPC()].size();i++){
				IDEX[PC.accessPC()+1].push_back(IDEX[PC.accessPC()][i]); //Copy current execute instruction to next row
			}
			for(unsigned i=0;i<EXMEM[PC.accessPC()].size();i++){
				EXMEM[PC.accessPC()+1].push_back(EXMEM[PC.accessPC()][i]); //Copy current memory instruction to next row otherwise next section won't run (out of index)
			}
		}
		else if(IDEX[PC.accessPC()][4]){
			EX.labelList = Decode.labelList; //update labels
			EX.execute(PC.accessPC(),IDEX,EXMEM,ALU,MEM, InstructionMemory);
		}
		
		//Store the result in memory
		if(EXMEM[PC.accessPC()][0]){
			if(DEBUG){cout <<endl<<endl<<endl<<"//----------Accessing Memory" << endl;}
			MEMWB.push_back(vector<unsigned>());
			if(EXMEM[PC.accessPC()][4] == 8){ //LW
				//Load value at [3] into register, write into address [2] in WB
				MEMWB[(PC.accessPC()+1)].push_back(1);//[0]Push back control signal
				MEM.load(EXMEM[PC.accessPC()][3], PC.accessPC(), MEMWB, EXMEM); //Access reg2
				MEMWB[(PC.accessPC()+1)].push_back(EXMEM[PC.accessPC()][2]); //[2]Push back address of value to write
				MEMWB[(PC.accessPC()+1)].push_back(EXMEM[PC.accessPC()][5]); //[3] opcode
			}
			if(EXMEM[PC.accessPC()][4] == 13){ //SW
				MEMWB[PC.accessPC()+1].push_back(0); //[0] //WB control signal
				//Store value from register [3] into register [2]
				unsigned value = MEM.load(EXMEM[PC.accessPC()][2], PC.accessPC(), MEMWB, EXMEM); //address
				cout <<"Value: "<<value << endl;
				MEM.store(value, EXMEM[PC.accessPC()][3]);//value, address
			}
			if (EXMEM[PC.accessPC()][4] == 21){ //opcode == 20 //OUT
				MEMWB[(PC.accessPC()+1)].push_back(0); //[0] //Control signal
				MEM.write(EXMEM[PC.accessPC()][2]);
			}
		}
		else if(!EXMEM[PC.accessPC()][0]){ //If no memory access required, wait until next cycle for WB
			MEMWB.push_back(vector<unsigned>());
			MEMWB[(PC.accessPC()+1)].push_back(EXMEM[PC.accessPC()][1]); //[0]//WB control signal
			MEMWB[(PC.accessPC()+1)].push_back(EXMEM[PC.accessPC()][2]); //[1]//value
			MEMWB[(PC.accessPC()+1)].push_back(EXMEM[PC.accessPC()][3]); //[2]//address
		}
		
		if(MEMWB[PC.accessPC()][0]){ //Write back
			if(DEBUG){cout<<endl<<endl<<endl<<"//----------Write back start" << endl;}
			WB.Write_Back(MEMWB[PC.accessPC()][1],MEMWB[PC.accessPC()][2]); //value, address
		}
		
		if(DEBUG){cout <<endl<<endl<<"//----------End of cycle checks" <<endl;}
		if (EX.returnpCount() != PC.accessPC()){ //Checking for jumps / branches to setup
			PC.setJumpTo(EX.returnpCount());
			PC.findWhenToJump();
			Fetch.setJump(true, 0); //Disable IF
			Decode.setJump(true); //Disable ID
			EX.setJump(true); //Disable EX
			if(DEBUG){cout << "Branching on "<<PC.whenToJump() <<" cycle" << endl;}
			PC.count();
			if(DEBUG){cout << "Increased PC" << endl;}
			checkHazards(Decode,EX,  dataHazard, stall);
			updateRegisters(IDEX, EXMEM, MEMWB, IFID, PC.accessPC());
			if(DEBUG){cout << "Updated Registers" << endl;}
		}
		else if(PC.accessPC() == PC.whenToJump()){ //Jumping / Branching
			PC.setPC(PC.jumpTo());
			PC.setWhenToJump(-1);
			//Clear registers and index them to PC
			Fetch.setJump(false, PC.accessPC()); //Enable IF again
			Decode.setJump(false); //Enable ID
			EX.setJump(false); //Enable EX
			clearRegisters(IDEX, EXMEM, MEMWB, IFID, PC.accessPC());
			if(DEBUG){cout << "Branched to line "<<PC.accessPC() << endl;}
		}
		else{
			PC.count();
			if(DEBUG){cout << "Increased PC" << endl;}
			checkHazards(Decode,EX,  dataHazard, stall);
			updateRegisters(IDEX, EXMEM, MEMWB, IFID, PC.accessPC());
			if(DEBUG){cout << "Updated Registers" << endl;}
		}
		
		i++;
	}
	if (EX.checkEnd()){
		cout << "//----------END OF PROGRAM---------//" << endl;
		cout <<endl<<endl<< "Number of cycles: " <<i << endl;
		cout <<endl<<endl<< "Number of instructions executed: " <<EX.getExecutions() << endl;
		
		
	}
	return 0;
}
void clearMemory(){ //Clearing text files for initialisation
	//Just clear data memory
	fstream file;
	file.open("DataMemory.txt",  fstream::out | fstream::trunc);
	file.close();
	//Set all of General_Registers lines to 0
	file.open("General_Registers.txt",  fstream::out | fstream::trunc);
	for (int i=0; i<32;i++){
		file << 0 << endl; //Sets the 32 general registers to 0
	}
	file.close();
}

void clearRegisters(vector<vector<unsigned>>&IDEX,vector<vector<unsigned>>&EXMEM,vector<vector<unsigned>>&MEMWB,vector<vector<string>>&IFID, unsigned pCount){
	//Clear and update indexes to pCount value
	IFID.clear();
	IDEX.clear();
	EXMEM.clear();
	MEMWB.clear();
	updateString(IFID, pCount);
	updateUnsigned(IDEX, pCount);
	updateUnsigned(EXMEM, pCount);
	updateUnsigned(MEMWB, pCount);
}
void checkHazards(id& Decode,ex& EX, bool& dataHazard, unsigned& stall){
	if(stall > 0){
		cout << "Stall > 0" << endl;
		Decode.updateStall(true);
		EX.updateStall(true);
		stall--;
	}
	if (Decode.checkStallReq() != 0){ //Initial stall set-up
		cout << "Initial stall setup" << endl;
		stall = Decode.checkStallReq();
		Decode.resetStallReq();
		dataHazard = true; //Stop IF reading instructions
		//Set stall flags on ID and EX
		Decode.updateStall(true);
		EX.updateStall(true);
	}
	if(stall == 0){
		//Enable sections again
		cout <<"Disabling stall" << endl;
		dataHazard = false;
		Decode.updateStall(false);
		EX.updateStall(false);
		
	}
	
}

void printStringVector(vector<vector<string>>&vec){
	for(int i=0;i<vec.size();i++){
		cout <<endl <<"Row:	"<< i << endl;
		for(int y=0;y<vec[i].size();y++){
			cout << vec[i][y] << "    ";
		}
	}
	cout << endl;
}

void printUnsignedVector(vector<vector<unsigned>>&vec){
	for(int i=0;i<vec.size();i++){
		cout <<endl <<"Row:	"<< i << endl;
		for(int y=0;y<vec[i].size();y++){
			cout << vec[i][y] << "    ";
		}
	}
	cout << endl;
}

void updateRegisters(vector<vector<unsigned>>&IDEX,vector<vector<unsigned>>&EXMEM,vector<vector<unsigned>>&MEMWB,vector<vector<string>>&IFID, unsigned pCount){ 
	//This function will add rows of 0s into registers which aren't being used in the next cycle
	//Registers need to have data in them otherwise the program will break when checking control signals
	updateString(IFID, pCount);
	updateUnsigned(IDEX, pCount);
	updateUnsigned(EXMEM, pCount);
	updateUnsigned(MEMWB, pCount);
}

void updateUnsigned(vector<vector<unsigned>>&vec, unsigned rowMax){ //rowMax is just pCount
	unsigned rowCount = 0;
	
	for(int y=0;y<vec.size();y++){ //Get current number of rows in the register
		rowCount++;
	}
	if(!rowMax){
		vec.push_back(vector<unsigned>());
			for(int i=0;i<7;i++){
				vec[0].push_back(0);
			}
	}
	else if (rowMax >0){
		for(int y=rowCount;y<(rowMax+1);y++){ //increase no. rows to match rowMax
			vec.push_back(vector<unsigned>());
			for(int i=0;i<7;i++){
				vec[y].push_back(0);
			}
		}
	}
}

void updateString(vector<vector<string>>&vec, unsigned rowMax){ //Same as updateUnsigned but for a 2d string vector
	unsigned rowCount = 0;
	
	for(int y=0;y<vec.size();y++){ //Get current number of rows in the register
		rowCount++;
	}
	if(!rowMax){
		vec.push_back(vector<string>());
			for(int i=0;i<2;i++){
				vec[0].push_back("0");
			}
	}
	if(rowMax>0){ //Only edit IFID if not the first instruction
		for(int y=rowCount;y<(rowMax+1);y++){ //increase no. rows to match rowMax
			vec.push_back(vector<string>());
			for(int i=0;i<2;i++){
				vec[y].push_back("0");
			}
		}
	}
}