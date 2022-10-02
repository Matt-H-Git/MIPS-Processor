//wb.h
/*
Write result of function back to register if applicable
*/
#include <fstream>

class wb{
	public:
	void setDebug(){debug = true;}
	void Write_Back(unsigned value, unsigned address);
	private:
	bool debug = false;
};
//Think this is incorrect maybe?
void wb::Write_Back(unsigned value, unsigned address){ //same as mem::write, take in whole file, edit correct value and rewrite file
	if(debug){cout << "Begin Write_Back" << endl;}
	fstream File;
	string IN;
	File.open("General_Registers.txt"); 
	vector<unsigned> tempMemory;
	while(getline(File,IN)){
		tempMemory.push_back(stoi(IN)); 
	}
	File.close();
	tempMemory[address] = value;
	if(debug){cout <<"Replaced value " << value <<" at address " << address <<  endl;}
	File.open("General_Registers.txt", fstream::out | fstream::trunc); 
	for (int i = 0; i < tempMemory.size() ; i++){
		File << tempMemory[i] << endl;
	}
	File.close();
}