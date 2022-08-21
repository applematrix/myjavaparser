package com.myvm.test;

public class SimpleClass {
    private int mValue1 = 1;
    private int mValue2 = 2;

    public SimpleClass() {
    }

    public int sum() {
        return mValue1 + mValue2;
    }

    public int sum1(int input1) {
        return mValue1 + input1;
    }
	
	public static void main(String args[]) {
		SimpleClass obj = new SimpleClass();
		obj.sum();
	}
}
