/*
 * Copyright (c) 2022 Huang Dezhi <cshdzxjtu@163.com>
 * All Rights Reserved
 *
 */

#ifndef _INSTRCTIONS_H_
#define _INSTRCTIONS_H_

#include <stdint.h>
#include <string>
#include <iostream>
#include "classloader/Frame.h"
#include "classloader/ThreadLocalStorage.h"
#include "common/utils.h"
using namespace std;

namespace myvm {

class OperandStack;

enum OpCode {
    NOP = 0x0, // Do nothing
    ACONST_NULL = 0x01, // push null
    ICONST_M1 = 0x02, // Push int constant
    ICONST_0 = 0x03, 
    ICONST_1 = 0x04, 
    ICONST_2 = 0x05, 
    ICONST_3 = 0x06, 
    ICONST_4 = 0x07, 
    ICONST_5 = 0x08, 
    LCONST_0 = 0x09,  // Push long constant
    LCONST_1 = 0xa, 
    FCONST_0 = 0xb, // Push float
    FCONST_1 = 0xc,
    FCONST_2 = 0xd,
    DCONST_0 = 0xe, // push double
    DCONST_1 = 0xf,

    BIPUSH = 0x10, // push byte
    SIPUSH = 0x11, // Push short
    LDC = 0x12, // Push item from run-time constant pool
    LDC_W = 0x13, // Push item from run-time constant pool (wide index)
    LDC2_W = 0x14, // Push item from run-time constant pool (wide index)
    ILOAD = 0x15, // Load int from local variable
    LLOAD = 0x16, // Load long from local variable
    FLOAD = 0x17, // Load float from local variable
    DLOAD = 0x18, // load double frome local variable
    ALOAD = 0x19, // load reference from local variable
    ILOAD_0 = 0x1a, //load int from local variable
    ILOAD_1 = 0x1b,
    ILOAD_2 = 0x1c,
    ILOAD_3 = 0x1d,
    LLOAD_0 = 0x1e, //load long from local variable
    LLOAD_1 = 0x1f,

    LLOAD_2 = 0x20,
    LLOAD_3 = 0x21,
    FLOAD_0 = 0x22, // Load float from local variable
    FLOAD_1 = 0x23,
    FLOAD_2 = 0x24,
    FLOAD_3 = 0x25,
    DLOAD_0 = 0x26, //load double from local variable
    DLOAD_1 = 0x27,
    DLOAD_2 = 0x28,
    DLOAD_3 = 0x29,
    ALOAD_0 = 0x2a, // load reference from local variable
    ALOAD_1 = 0x2b,
    ALOAD_2 = 0x2c,
    ALOAD_3 = 0x2d,
    IALOAD = 0x2e, // Load int from array 
    LALOAD = 0x2f, // Load long from array

    FALOAD = 0x30, // Load float from array 
    DALOAD = 0x31, // load double from array
    AALOAD = 0x32, // load reference from array
    BALOAD = 0x33, // load byte or boolean from array
    CALOAD = 0x34, // load char from array
    SALOAD = 0x35, // Load short from array
    ISTORE = 0x36, // Store int into local variable
    LSTORE = 0x37, // Store long into local variable
    FSTORE = 0x38, // Store float into local variable
    DSTORE = 0x39, // store double into local variable
    ASTORE = 0x3a, // store reference into local variable
    ISTORE_0 = 0x3b, // Store int into local variable
    ISTORE_1 = 0x3c,
    ISTORE_2 = 0x3d,
    ISTORE_3 = 0x3e,
    LSTORE_0 = 0x3f, // Store long into local variable

    LSTORE_1 = 0x40,
    LSTORE_2 = 0x41,
    LSTORE_3 = 0x42,
    FSTORE_0 = 0x43, // Store float into local variable
    FSTORE_1 = 0x44,
    FSTORE_2 = 0x45,
    FSTORE_3 = 0x46,
    DSTORE_0 = 0x47, // store double into local variable
    DSTORE_1 = 0x48,
    DSTORE_2 = 0x49,
    DSTORE_3 = 0x4a,
    ASTORE_0 = 0x4b, // store reference int local variable
    ASTORE_1 = 0x4c, 
    ASTORE_2 = 0x4d, 
    ASTORE_3 = 0x4e, 
    IASTORE = 0x4f, // Store into int array

    LASTORE = 0x50, // Store into long array
    FASTORE = 0x51, // store into float array
    DASTORE = 0x52, // store into double array
    AASTORE = 0x53, // store into reference array
    BASTORE = 0x54, // store into byte or boolean array
    CASTORE = 0x55, // store into char array
    SASTORE = 0x56, // Store into short array
    POP = 0x57, // Pop the top operand stack value
    POP2 = 0x58, // Pop the top one or two operand stack values
    DUP = 0x59, //  duplicate the top operand stack value
    DUP_X1 = 0x5a, // Duplicate the top operand stack value and insert two values down
    DUP_X2 = 0x5b, // Duplicate the top operand stack value and insert two or three values down
    DUP2 = 0x5c, // Duplicate the top one or two operand stack values
    DUP2_X1 = 0x5d, // Duplicate the top one or two operand stack values and insert two or three values down
    DUP2_X2 = 0x5e, // Duplicate the top one or two operand stack values and insert two, three, or four values down
    SWAP = 0x5f, // Swap the top two operand stack values

    IADD = 0x60, // Add int
    LADD = 0x61, // Add long
    FADD = 0x62, // Add float
    DADD = 0x63, // add double
    ISUB = 0x64, // Subtract int
    LSUB = 0x65, // Subtract long
    FSUB = 0x66, // Subtract float
    DSUB = 0x67, // substract double
    IMUL = 0x68, // Multiply int
    LMUL = 0x69, // Multiply long
    FMUL = 0x6a, // Multiply float
    DMUL = 0x6b, // multiply double
    IDIV = 0x6c, // Divide int
    LDIV = 0x6d, // Divide long
    FDIV = 0x6e, // Divide float
    DDIV = 0x6f, // divide double

    IREM = 0x70, // Remainder int
    LREM = 0x71, // Remainder long
    FREM = 0x72, // Remainder float
    DREM = 0x73, // remainder double
    INEG = 0x74, // Negate int
    LNEG = 0x75, // egate long
    FNEG = 0x76, // Negate float
    DNEG = 0x77, // negate double
    ISHL = 0x78, // Shift left int
    LSHL = 0x79, // Shift left long
    ISHR = 0x7a, // Arithmetic shift right int
    LSHR = 0x7b, // Arithmetic shift right long
    IUSHR = 0x7c, // Logical shift right int
    LUSHR = 0x7d, // Logical shift right long
    IAND = 0x7e, // Boolean AND int
    LAND = 0x7f, // Boolean AND long

    IOR = 0x80, // Boolean OR int
    LOR = 0x81, // Boolean OR long
    IXOR = 0x82, // Boolean XOR int
    LXOR = 0x83, // Boolean XOR long
    IINC = 0x84, // Increment local variable by constant
    I2L = 0x85, // Convert int to long
    I2F = 0x86, // Convert int to float
    I2D = 0x87, // Convert int to double
    L2I = 0x88, // Convert long to int
    L2F = 0x89, // Convert long to float
    L2D = 0x8a, // Convert long to double
    F2I = 0x8b, // Convert float to int
    F2L = 0x8c, // Convert float to long
    F2D = 0x8d, // Convert float to double
    D2I = 0x8e, // convert double to int
    D2L = 0x8f, // convert double to long

    D2F = 0x90, // convert double to float
    I2B = 0x91, // Convert int to byte
    I2C = 0x92, // Convert int to char
    I2S = 0x93, // Convert int to short
    LCMP = 0x94, // Compare long
    FCMPL = 0x95, // Compare float
    FCMPG = 0x96,
    DCMPL = 0x97, // compare double
    DCMPG = 0x98,
    IFEQ = 0x99, // Branch if int comparison with zero succeeds
    IFNE = 0x9a,
    IFLT = 0x9b,
    IFGE = 0x9c,
    IFGT = 0x9d,
    IFLE = 0x9e,
    IF_ICMPEQ = 0x9f, // Branch if int comparison succeeds

    IF_ICMPNE = 0xa0,
    IF_ICMPLT = 0xa1,
    IF_ICMPGE = 0xa2,
    IF_ICMPGT = 0xa3,
    IF_ICMPLE = 0xa4,
    IF_ACMPEQ = 0xa5, // Branch if reference comparison succeeds
    IF_ACMPNE = 0xa6,
    GOTO = 0xa7, // Branch always
    JSR = 0xa8, // Jump subroutine
    RET = 0xa9, // Return from subroutine
    TABLESWITCH = 0xaa, // Access jump table by index and jump
    LOOKUPSWITCH = 0xab, // Access jump table by key match and jump
    IRETURN = 0xac, // Return int from method
    LRETURN = 0xad, // Return long from method
    FRETURN = 0xae, // Return float from method
    DRETURE = 0xaf, // return double from method

    ARETURN = 0xb0, // return reference from method
    RETURN = 0xb1, // Return void from method
    GETSTATIC = 0xb2, // Get static field from class
    PUTSTATIC = 0xb3, // Set static field in class
    GETFIELD = 0xb4, // Fetch field from object
    PUTFIELD = 0xb5, // Set field in object
    INVOKEVIRTUAL = 0xb6, // Invoke instance method; dispatch based on class
    INVOKESPECIAL = 0xb7, // Invoke instance method; direct invocation of instance initialization
                          // methods and methods of the current class and its supertypes
    INVOKESTATIC = 0xb8, // Invoke a class (static) method
    INVOKEINTEFACE = 0xb9, // Invoke interface method
    INVOKEDYNAMIC = 0xba, // Invoke a dynamically-computed call site
    NEW = 0xbb, // Create new object
    NEWARRAY = 0xbc, // Create new array
    ANEWARRAY = 0xbd, // create new array of reference
    ARRAYLENGTH = 0xbe, // get length of array
    ATHROW = 0xbf, // throw exception or error

    CHECKCAST = 0xc0, // check whether object is of given type
    INSTANCEOF = 0xc1, // Determine if object is of given type
    MONITORENTER = 0xc2, // Enter monitor for object
    MONITOREXIT = 0xc3, // Exit monitor for object
    WIDE = 0xc4, // Extend local variable index by additional bytes
    MULTIANEWARRAY = 0xc5, // Create new multidimensional array
    IFNULL = 0xc6, // Branch if reference is null
    IFNONNULL = 0xc7, // Branch if reference not null
    GOTO_W = 0xc8, // Branch always (wide index)
    JSR_W = 0xc9, // Jump subroutine (wide index)
};

class Method;
class ClassFileInfo;
class Frame;
class Instruction {
public:
    Instruction() {};
    virtual ~Instruction(){};
    virtual void run(Frame* frame) = 0;
    virtual uint8_t codeLen() = 0;

    static const char* getOpCodeDesc(uint8_t opCode);
    static Instruction* interpreteCode(uint8_t *code);
};

class NopInstruction : public Instruction {
    public:
    NopInstruction() {}
    virtual ~NopInstruction() {}
    virtual void run(ClassFileInfo* clazz, Method *context, OperandStack *stack) {}
    virtual void run(Frame* frame) {}
    virtual uint8_t codeLen() { return 1;}
};

}

#endif