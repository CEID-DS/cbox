package com.cbox;

public class WiseLib {

	static {
		System.loadLibrary("wiselib");
	}

	public native String androidSend(Object c);

}
