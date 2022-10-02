//pc.h

#ifndef _PC_H
#define _PC_H
using namespace std;

class pc{
	public:
	pc() {setPC(0);}
	void count(){ProgramCount++;}
	void setPC(unsigned input){ProgramCount = input;}
	unsigned accessPC(){return ProgramCount;}
	void findWhenToJump(){PCwhenToJump = ProgramCount + 1;} //Gives enough time for any current running operation to finish (any current op only needs to at most use both MEM and WB)
	void setWhenToJump(int IN){PCwhenToJump = IN;}
	void setJumpTo(unsigned jump){PCtoJumpTo = jump;}
	int whenToJump(){return PCwhenToJump;}
	unsigned jumpTo(){return PCtoJumpTo;}
	private:
	unsigned ProgramCount;
	unsigned PCtoJumpTo; //Tells what line to jump to on jtype or branch
	int PCwhenToJump = -1; //Tells what PC value to jump / branch at
};
#endif