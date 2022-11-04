package com.myvm.test;

public class Sum {
    private int mValue1 = 1;
    private int mValue2 = 2;

    public Sum() {
    }

    public int sum() {
        return mValue1 + mValue2;
    }

    public int sum1(int input1) {
        return mValue1 + input1;
    }
	
	public static void main(String args[]) {
		Sum obj = new Sum();
		obj.sum();
	}
}
