#ifndef _POP_INSTRUCTIONS_H_
#define _POP_INSTRUCTIONS_H_
#include "Instructions.h"

using namespace std;

namespace myvm {

class PopInstruction : public Instruction {
public:
    PopInstruction(bool isPop2 = false) { mPop2 = isPop2; }
    virtual ~PopInstruction() {};
    virtual void run(Frame* frame);
private:
    bool mPop2 = false;
};

}

#endif