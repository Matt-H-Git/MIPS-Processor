//mem.h
/*
Access memory and read/write as applicable
Store result from EXMEM into memory txt file and pass necessary data to MEMWB
*/
#include <iostream>
#include <fstream> //To read / write to files

//---------------------------Class Header

class mem{
	public:
	unsigned read(unsigned address, unsigned pCount, vector<vector<unsigned>>& MEMWB, vector<vector<unsigned>>& EXMEM);
	void write(unsigned value);
	unsigned load(unsigned address, unsigned pCount, vector<vector<unsigned>>& MEMWB, vector<vector<unsigned>>& EXMEM);
	void store(unsigned value, unsigned address);
	void setDebug(){debug = true;}
	private:
	bool debug = false;
};

//---------------------------Class Body
//Address relates to each line in the file, with a different line being a different register
unsigned mem::read(unsigned address, unsigned pCount, vector<vector<unsigned>>& MEMWB, vector<vector<unsigned>>& EXMEM){ 
	if(debug){cout <<"mem::read" << endl;}
	if(debug){cout <<"Reading from address " << address << endl;}
	ifstream File;
	string IN;
	unsigned value;
	int i =0;
	File.open("DataMemory.txt"); //Opening file
	while(getline(File,IN)){
		if (i == address){
			value = stoi(IN);
			File.close();
			MEMWB[pCount+1].push_back(value); //[1]
			if(debug){cout <<"Read in: "<<value<<endl;}
			return value;
		}
		i++;
	}
}

void mem::write(unsigned value){
	//Write value to a new line in DataMemory
	if (debug){cout <<"mem::write Writing to Data memory: " << value << endl;}
	string IN;
	fstream file;
	file.open("DataMemory.txt"); //Opening file
	vector<unsigned> tempMemory;
	while(getline(file,IN)){
		tempMemory.push_back(stoi(IN)); 
	}
	file.close();
	tempMemory.push_back(value);
	file.open("DataMemory.txt", fstream::out | fstream::trunc); 
	for (int i = 0; i < tempMemory.size() ; i++){
		file << tempMemory[i] << endl;
	}
	file.close();
}

unsigned mem::load(unsigned address, unsigned pCount, vector<vector<unsigned>>& MEMWB, vector<vector<unsigned>>& EXMEM){ 
	if(debug){cout <<"mem::load" << endl;}
	if(debug){cout <<"Loading from address " << address << endl;}
	if(debug){cout << "pCount: " << pCount << endl;}
	ifstream File;
	string IN;
	File.open("General_Registers.txt"); //Opening file
	unsigned value;
	int i =0;
	while(getline(File,IN)){
		if (i == address){
			value = stoi(IN);
			File.close();
			MEMWB[pCount+1].push_back(value); //[1]
			if(debug){cout <<"Read in: "<<value << " into " << pCount+1<<endl;}
			return value;
		}
		i++;
	}
}

void mem::store(unsigned value, unsigned address){ //Same as mem::write but for General_Registers.txt and specifically for SW command
	if (debug){cout <<"mem::store Writing to General Registers" << endl;}
	if (debug) {cout <<"Writing value " << value << " at address " << address << endl;}
	fstream File;
	string IN;
	File.open("General_Registers.txt"); 
	vector<unsigned> tempMemory;
	while(getline(File,IN)){
		tempMemory.push_back(stoi(IN)); 
	}
	File.close();
	tempMemory[address] = value;
	File.open("General_Registers.txt", fstream::out | fstream::trunc); 
	for (int i = 0; i < tempMemory.size() ; i++){
		File << tempMemory[i] << endl;
	}
	File.close();
}