package com.cbox.provider;

import android.net.Uri;
import android.provider.BaseColumns;

public class Services {

	private static final String AUTHORITY = "com.cbox.provider.CboxProvider";
	
	public static final class Service implements BaseColumns{
		public static final Uri CONTENT_URI = Uri.parse("content://"+AUTHORITY);
		
		public static final String hostname = "hostname";
		public static final String serviceType = "serviceType";
		public static final String protocol = "protocol";
		//interface is a java keyword so i replaced it with cinterface
		public static final String cinterface = "cinterface";
		public static final String port = "port";
		public static final String txtdata = "txtdata";
		public static final String ttl = "ttl";
		public static final String advertised = "advertised";
		public static final String questioned = "questioned";
	}
}
