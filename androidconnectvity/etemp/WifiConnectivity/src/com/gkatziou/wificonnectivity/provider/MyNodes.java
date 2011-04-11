package com.gkatziou.wificonnectivity.provider;

import android.net.Uri;
import android.provider.BaseColumns;

public class MyNodes {

	private static final String AUTHORITY = "com.gkatziou.wificonnectivity.provider.Interfaces";
	
	public static final class Node implements BaseColumns{
		public static final Uri CONTENT_URI = Uri.parse("content://com.gkatziou.wificonnectivity.provider.Interfaces");

		public static final String ipv4_addr ="ipv4_addr";
		public static final String blue_addr ="blue_addr";
		public static final String ipv6_addr ="ipv6_addr";
	}
}
