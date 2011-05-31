package com.cbox;

public class MainLib {

	static {
		System.loadLibrary("mainLib");
	}

	public native void initialize(Object c);
	public native void testMethod(Object c); //just for testing purposes
	public native void androidReceive(Object c, String s);

}
