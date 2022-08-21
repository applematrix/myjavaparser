package com.myvm.test;

public class SimpleClass {
    private int operand1 = 1;
    private int operand2 = 2;

    public SimpleClass() {
    }

    public int sum() {
        return operand1 + operand2;
    }

    public int sum1(int input1) {
        return operand1 + input1;
    }
}
