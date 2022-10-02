//test
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
using namespace std;
//Final showcase program
/*
int main(){
	int output = 0;
	for(int input=1;input<201;input++){
		output += 2 * input;
		output--;
		cout << output << endl;
	}
}
*/
/*
class test{
	public:
	void setStall(bool set){stall = set;}
	bool returnStall(){return stall;}
	private:
	bool stall = false;
	
};
int main(){
	test T;
	cout << T.returnStall() << endl;
	T.setStall(true);
	cout << T.returnStall() << endl;
	
}
*/
//Testing more efficient showcase program
/*
int main(){
	int max = 201;
	for (int input=1;input<max;input++){
		int square = 0;
		for(int i=0;i<input;i++){
			square += input;
		}
		cout << square << endl;
	}
}
*/
//Testing writing lines to a file
/*
int main(){
	unsigned value = 1;
	string IN;
	fstream file;
	file.open("DataMemory.txt"); //Opening file
	file << value;
	file.close();
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
*/
//Finding a word in a string
//Put 2 indexes in, find the one which starts at 0
/*
int main(){
	string label = "loop";
	vector<string> a = {"Not here", "maybe here", "loop here", "definitely not a loop here"};
	string search;
	unsigned match;
	unsigned index;
	for (int i=0;i<a.size();i++){
		search = a[i];
	match = search.find(label);
	cout << "Match: " << match << endl;
	if (match != string::npos)
	{
		// Found!
		cout << "found at : " << match << endl;
		if(match == 0){
			index = i;
		}
	}
	}
	cout << "Match found at index: " << index << endl;
}
*/
//Figuring out the showcase program
/*
int main(){
	int val = 3;
	int mask = 1;
	int factorsum = val;
	int sum = 0;
	while (val >= mask)
    {	
		cout <<"val: "<< val << "	mask: " << mask << endl;
		cout <<( val & mask )<< endl;
        if ((val & mask) == mask)
        {
			cout <<"(val & mask) == mask" << endl;
            sum += factorsum;
			cout << "sum = " << sum<<endl;
        }
        factorsum += factorsum;
		cout << "factorsum = " << factorsum << endl;
        mask += mask;
		cout << "mask = " << mask << endl;
    }
}

*/
//Showcase program in C++
/*
int main(){
	//int input
	for(int input=1;input<201;input++){
		int mask = 1;
		int factorsum = input;
		int output = 0;
		while (input >= mask){
			if ((input & mask) == mask){
				output += factorsum;
			}
		factorsum += factorsum;
		mask += mask;
		}
		if (output){cout << output << endl;}
	}
}
*/
//Testing copying 2d vectors
/*
int main(){ //Copy 2nd line into a new 3rd line
	vector<vector<unsigned>> test = {
		{1, 1, 1},
			{2, 2 ,3}
	};
	test.push_back(vector<unsigned>());
	//Would use pCount to denote the latest row of data to be copied onto the new line
	unsigned pCount = 1;
	for (int i=0;i<test[pCount].size();i++){
		cout <<test[pCount][i];
		test[pCount+1].push_back(test[pCount][i]);
	}
	cout << endl;
	for(int y=0;y<test.size();y++){
		for (int i=0;i<test[y].size();i++){
		cout <<test[y][i];
	}
	cout << endl;
	}
}
*/
//Testing finding number of rows in a vector and increasing it to match a specific number
/*
int main(){
	vector<vector<unsigned>> test;
	unsigned rowMax = 4;
	for(int y=0;y<2;y++){ //Only 2 rows, increase it to 4
		test.push_back(vector<unsigned>());
		for(int i=0;i<4;i++){
			test[y].push_back(0);
		}
	}
	unsigned rowCount = 0;
	for(int y=0;y<test.size();y++){
		rowCount++;
	}
	unsigned rowDifference = rowMax - rowCount;
	if (rowDifference == 0){
		cout << "No change needed" << endl;
	}
	else{
		cout <<"Row difference is "<<rowDifference << endl;
		for(int y=rowCount;y<rowMax;y++){ //Only 2 rows, increase it to 4
			test.push_back(vector<unsigned>());
		for(int i=0;i<4;i++){
			test[y].push_back(0);
		}
	}
	}
	cout << "Final vector" << endl;
	for(int y=0;y<4;y++){ //Only 2 rows, increase it to 4
		
		for(int i=0;i<4;i++){
			cout << test[y][i];
		}
		cout << endl;
	}
}
*/
//Testing pointers
/*
void test(unsigned& y){
	unsigned i;
	i = 5;
	y = i;
}
int main(){
	unsigned y;
	y = 0;
	test(y);
	cout << y;
	
}
*/
/*
int main(){
	string testStr;
	vector<string> testVec = {"hi", "this", "is"};
	
	testStr = testVec[0];
	cout << testStr << endl;
	
	cout << testStr.length() << endl;
	
	cout << testStr[0] << endl;
	
	
}
*/
//Testing writing to specific lines in a file
/*
int main(){
	fstream File;
	unsigned value = 001000110;
	unsigned address = 2;
	string IN;
	File.open("DataMemory.txt"); //Opening file
	vector<unsigned> tempMemory;
	while(getline(File,IN)){
		cout << IN << "		" << stoi(IN) << endl;
		tempMemory.push_back(stoi(IN)); //IN is a string, so convert to int
	}
	File.close();
	cout << tempMemory[address] << endl;
	//Write in the new value
	tempMemory[address] = value;
	cout << tempMemory[address] << endl;
	//Rewrite the whole file
	File.open("DataMemory.txt", fstream::out | fstream::trunc);
	for (int i = 0; i < tempMemory.size() ; i++){
		cout << tempMemory[i] << endl;
		File << tempMemory[i] << endl;
	}
	File.close();
}
*/
//Testing accessing the register file
/*
int main(){
	unsigned address = 3;
	unsigned value; //Value to be returned
	ifstream file;
	string IN;
	file.open("General_Registers.txt");
	int i =0;
	while(getline(file,IN)){
		if (i == address){
			value = stoi(IN);
			cout<< value << endl;
		}
		i++;
	}
	return 0;
}
*/
//Testing assembly conversion (mainly splitting up the string)
/*
void printVec(vector<string>& vec){
	cout << "printVec" << endl;
	for (int i =0; i < vec.size(); i++){
		cout << vec.at(i);
	}
	cout <<endl;//<<"testa "<< endl;
}
int main(){
	string instruction = "add $t1, $t2, $s6";
	string test[4] = {"add", "$s6", "$t1", "$t2"};
	int position = 0;
	int s;
	vector<string> store;
	vector<string> printTest = {"test1", "test2"};
	string delimeter = ",";
	string typeConversion; //Used for type conversion since a pointer cannot be pushed onto a vector
	//Try using while loops since can't break out of the for loop very well
	typeConversion = instruction[position];
	while (typeConversion != " "){
		store.push_back(typeConversion);
		cout << instruction[position] << " " << store[position] << " "<< "		Position: "<< position<<endl;
		position++;
		typeConversion = instruction[position]; //Make sure this is after position++
	}
	printVec(store);
	store.clear();
	cout << "Between while loops, current position: " <<instruction[position]<< endl;
	position++;
	typeConversion = instruction[position];
	while (position != instruction.length()){
		while (typeConversion != delimeter){
			store.push_back(typeConversion);
			cout << instruction[position] << " " << " "<< "		Position: "<< position<<endl;
			position++;
			typeConversion = instruction[position];
		}
		printVec(store);
		store.clear();
		cout << position << " " << instruction.length() << endl;
		if (typeConversion == delimeter){
			position++;
		}
	}
	cout << "Final loop" << endl;
	return 0;
}
*/
//Testing string things
/*
int main(){
	string test = "hello world";
	for (int i=0;i<test.length();i++){
		cout << test[i];
	}
	return 0;
}
*/
//Fetching number of columns in a vector (for dynamic vectors with different column numbers)
/*
int main(){
	vector<vector<unsigned>> test = {
		{1, 2},
			{3, 4}
	};
	cout << test[1][0]; //[row][column]
	return 0;
}
*/
/* //Testing number of elements in array
int main(){
	string registerList[32] = {"$zero", "$at", "$v0", "$v1", "$a0", "$a1", "$a2", "$a3", "$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7", "$s0", "$s1", "$s2", "$s3", "$s4", "$s5", "$s6", "$s7", "$t8", "$t9", "$k0", "$k1", "$gp", "$sp", "$fp", "$ra"};
	for (unsigned i=0;i<(sizeof(registerList)/sizeof(string));i++){
		cout << i;
	}
	cout << (sizeof(registerList)/sizeof(string));
	return 0;
}
*/
//Changing vector<string> into string
/*
int main(){
	vector<string> codes = {"$", "a", "0"};
	string a;
	//a = str(codes.begin(), codes.end());
	for (int i=0;i<codes.size();i++){
	a.append(codes[i]);
	}
	cout << a << endl;
	return 0;
}
*/
//Testing if mismatched cases will equal
/*
#include <algorithm> //for lowerCase
void lowerCase(string *mixed){
	transform(mixed->begin(), mixed->end(), mixed->begin(), ::tolower);
	//Can cope with numbers and symbols
}
int main(){
	string opName = "$A0";
	cout << opName << endl;
	string registerList[32] = {"$zero", "$at", "$v0", "$v1", "$a0", "$a1", "$a2", "$a3", "$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7", "$s0", "$s1", "$s2", "$s3", "$s4", "$s5", "$s6", "$s7", "$t8", "$t9", "$k0", "$k1", "$gp", "$sp", "$fp", "$ra"};
	int i = 0;
	lowerCase(&registerList[i]);
	lowerCase(&opName);
	cout << opName << endl;
	cout << registerList[i] << endl;
	for (int i=0; i<31;i++){
		lowerCase(&registerList[i]);
		if (opName == registerList[i]){
			cout << (i+1) << endl;
		}
	}
	return 0;
}
*/
/*
//Testing pushback onto a vector pointer
void memoryIN(vector<vector<unsigned>>& memory, unsigned input){
	memory[0].push_back(input); //error came from not including the [0] to denote which row was being pushed onto
}
int main(){
	vector<vector<unsigned>> memory;
	memory.push_back(vector<unsigned>());
	unsigned input = 5;
	memoryIN(memory, input);
	cout<< 	memory[0][0];
	return 0;
}
*/
/*
//Testing 2d vectors
int main(){
	vector<vector<int>> test
	{
		{1, 1, 1},
		{2, 2, 2},
		{3, 3, 3}
	
	};
	for (int i = 0; i < test.size(); i++)
    {
        for (int j = 0; j < test[i].size(); j++)
        {
            cout << test[i][j] << " "; //j is the column, i is the row
        }   
        cout << endl;
    }
	vector<vector<int>> second;
	second.push_back(vector<int>());
	int row = 0;
	for(int i=0;i<4;i++){
		second[row].push_back(i);
	}
	row = 1;
	second.push_back(vector<int>());
	for(int i=0;i<4;i++){
		second[row].push_back(i);
	}
	for (int i = 0; i < second.size(); i++)
    {
        for (int j = 0; j < second[i].size(); j++)
        {
            cout << second[i][j] << " "; //j is the column, i is the row
        }   
        cout << endl;
    }
	return 0;
}
*/
/*//Testing array sizes for memory storage (maybe consider vectors?)
int main(){
	unsigned memory[8] = {1610719907, 1, 2, 3, 4, 5, 6, 7};
	//To access parts of memory
	cout << memory[1] << endl;
	for (int i=0; i < (sizeof(memory)/sizeof(unsigned)) ; i++){
		cout << memory[i] << endl;
	}
}
*/
/*//Test block for reading file data
int main(){ // Reading the instruction into a format so it can be split up
	//Can a string hex number be split up?
	//read it line by line into an unsigned variable
	//detect eol character? Bad idea, using getline is the best idea
	char data[100];
	string IN;
	ifstream instructionFile;
	instructionFile.open("instructions.txt");
	//instructionFile >> data;
	//cout <<data << endl; //Reads in 1 line
	while(getline(instructionFile, IN)){
		cout << IN << endl;
		unsigned num = stoi(IN, 0, 16);
		cout << num << endl;
	}
	instructionFile.close();
	return 0;
}
*/

/* //Test block for masking hex numbers with different length
int main(){
	unsigned sixBitSplit = 0x3f;
	unsigned fiveBitSplit = 0x1f;
	unsigned instruction = 0xA0A1A2A3;
	unsigned funct = instruction & sixBitSplit;
	unsigned shamt = (instruction >> 6) & fiveBitSplit;
	unsigned rd = (instruction >> 11) & fiveBitSplit;
	printf("funct: %d\n", funct);
	printf("shamt: %d\n", shamt);
	printf("rd: %d\n", rd);
	
	return 0;
	
}
*/

//Source for masking hex numbers
//https://stackoverflow.com/questions/12798611/splitting-a-hex-number
/*		//Splitting up a hex number (works)
const unsigned HexSplit = 0xff;
int main(){
	//8-bit digit hex number which needs to be split up
	unsigned instruction = 0xA3A2A1A0;
	//Portions work from right to left
	unsigned first = instruction & HexSplit;
	unsigned second = (instruction >> 8) & HexSplit;
	//cout << n << '\n';
	//printf("%d", n);
	printf("First %d\n", first);
	printf("Second %d\n", second);
  return 0;
}
*/