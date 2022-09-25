/*
 * Copyright (c) 2022 Huang Dezhi <cshdzxjtu@163.com>
 * All Rights Reserved
 *
 */

#include "Instructions.h"
#include "LoadInstructions.h"
#include "NewInstruction.h"
#include "DupInstruction.h"
#include "FieldInstructions.h"
#include "InvokeInstructions.h"
#include "ReturnInstruction.h"
#include "ConstantInstructions.h"
#include "StoreInstructions.h"
#include "PopInstructions.h"
#include "AddInstructions.h"
#include <iostream>

using namespace std;

namespace myvm {

struct InstructionDesc {
    uint8_t opcode;
    string description;
};

static const char* instructionDescs[] = {
    "nop", 
    "aconst_null",
    "iconst_m1",
    "iconst_0",
    "iconst_1",
    "iconst_2",
    "iconst_3",
    "iconst_4",
    "iconst_5",
    "lconst_0",
    "lconst_1",
    "fconst_0",
    "fconst_1",
    "fconst_2",
    "dconst_0",
    "dconst_1",
    "bipush",
    "sipush",
    "ldc", 
    "ldc_w",
    "ldc2_w",
    "iload",
    "lload",
    "fload",
    "dload",
    "aload",
    "iload_0",
    "iload_1",
    "iload_2",
    "iload_3",
    "lload_0",
    "lload_1",
    "lload_2",
    "lload_3",
    "fload_0",
    "fload_1",
    "fload_2",
    "fload_3",
    "dload_0",
    "dload_1",
    "dload_2",
    "dload_3",
    "aload_0",
    "aload_1",
    "aload_2",
    "aload_3",
    "iaload",
    "laload",
    "faload",
    "daload",
    "aaload",
    "baload",
    "caload",
    "saload",
    "istore",
    "lstore",
    "fstore",
    "dstore",
    "astore",
    "istore_0",
    "istore_1",
    "istore_2",
    "istore_3",
    "lstore_0",
    "lstore_1",
    "lstore_2",
    "lstore_3",
    "fstore_0",
    "fstore_1",
    "fstore_2",
    "fstore_3",
    "dstore_0",
    "dstore_1",
    "dstore_2",
    "dstore_3",
    "astore_0",
    "astore_1",
    "astore_2",
    "astore_3",
    "iastore",
    "lastore",
    "fastore",
    "dastore",
    "aastore",
    "bastore",
    "castore",
    "sastore",
    "pop", 
    "pop2",
    "dup", 
    "dup_x1",
    "dup_x2",
    "dup2",
    "dup2_x1",
    "dup2_x2",
    "swap",
    "iadd",
    "ladd",
    "fadd",
    "dadd",
    "isub",
    "lsub",
    "fsub",
    "dsub",
    "imul",
    "lmul",
    "fmul",
    "dmul",
    "idiv",
    "ldiv",
    "fdiv",
    "ddiv",
    "irem",
    "lrem",
    "frem",
    "drem",
    "ineg",
    "lneg",
    "fneg",
    "dneg",
    "ishl",
    "lshl",
    "ishr",
    "lshr",
    "iushr",
    "lushr",
    "iand",
    "land",
    "ior", 
    "lor", 
    "ixor",
    "lxor",
    "iinc",
    "i2l", 
    "i2f", 
    "i2d", 
    "l2i", 
    "l2f", 
    "l2d", 
    "f2i", 
    "f2l", 
    "f2d", 
    "d2i", 
    "d2l",
    "d2f",
    "i2b",
    "i2c",
    "i2s",
    "lcmp",
    "fcmpl",
    "fcmpl",
    "dcmpl",
    "dcmpg",
    "ifeq",
    "ifne",
    "iflt",
    "ifge",
    "ifgt",
    "ifle",
    "if_icmpeq",
    "if_icmpne",
    "if_icmplt",
    "if_icmpge",
    "if_icmpgt",
    "if_icmple",
    "if_acmpeq",
    "if_acmpne",
    "goto",
    "jsr",
    "ret",
    "tableswitch",
    "lookupswitch",
    "ireturn",
    "lreturn",
    "freturn",
    "dreture",
    "areturn",
    "return",
    "getstatic",
    "putstatic",
    "getfield",
    "putfield",
    "invokevirtual",
    "invokespecial",
    "invokestatic",
    "invokeinteface",
    "invokedynamic",
    "new", 
    "newarray",
    "anewarray",
    "arraylength",
    "athrow",
    "checkcast",
    "instanceof",
    "monitorenter",
    "monitorexit",
    "wide",
    "multianewarray",
    "ifnull",
    "ifnonnull",
    "goto_w",
    "jsr_w",
};

const char* Instruction::getOpCodeDesc(uint8_t opCode) {
    if (opCode > JSR_W) {
        return "Invalid code";
    }
    return instructionDescs[opCode];
}

Instruction* Instruction::interpreteCode(uint8_t *code) {
    uint8_t opCode = *code;
    
    switch (opCode) {
        //TODO: 
        case ALOAD:
            return new AloadInstruction(code);
        case ALOAD_0:
        case ALOAD_1:
        case ALOAD_2:
        case ALOAD_3:
            return new AloadInstruction(code, opCode - ALOAD_0);
            break;
        case NEW:
            return new NewInstruction(code);
        case DUP:
            return new DupInstruction();
        case PUTFIELD:
            return new PutFieldInstruction(code);
        case GETFIELD:
            return new GetFieldInstruction(code);
        case INVOKESPECIAL:
            return new InvokeSpecialInstruction(code);
        case INVOKEVIRTUAL:
            return new InvokeVirtualInstruction(code);
        case ICONST_M1:
        case ICONST_0:
        case ICONST_1:
        case ICONST_2:
        case ICONST_3:
        case ICONST_4:
        case ICONST_5:
            return new IConstantInstruction(code, opCode - ICONST_0);
        case ASTORE:
            return new AStoreInstruction(code);
        case ASTORE_0:
        case ASTORE_1:
        case ASTORE_2:
        case ASTORE_3:
            return new AStoreInstruction(code, opCode - ASTORE_0);
        case POP:
            return new PopInstruction();
        case POP2:
            return new PopInstruction(true);
        case RETURN:
            return new ReturnInstruction(code);
        case IADD:
            return new IAddInstruction(code);
        default:
            cout << "Unknown op code : " << (int32_t)opCode << endl;
    }
    return nullptr;
}

}