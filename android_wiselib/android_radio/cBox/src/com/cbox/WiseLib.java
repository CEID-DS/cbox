package com.cbox;

public class WiseLib {

	static {
		System.loadLibrary("wiselib");
	}

	public native int add(int v1,int v2);

}
