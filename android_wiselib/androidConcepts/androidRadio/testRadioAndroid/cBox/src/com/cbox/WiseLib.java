package com.cbox;

public class WiseLib {

	static {
		System.loadLibrary("wiselib");
	}

	public native void androidSend(Object c);
	public native void androidReceive(Object c, String s);

}
