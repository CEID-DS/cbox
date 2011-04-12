/******************************************************************************
* This file is part of cbox.
* 
* cbox is free software: you can redistribute it and/or modify
* it under the terms of the GNU LesserGeneral Public License as published
* by the Free Software Foundation, either version 3 of the License, or
* any later version.
* 
* Cbox is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU Lesser General Public License for more details.
* 
* You should have received a copy of the GNU Lesser General Public License
* along with cbox.  If not, see <http://www.gnu.org/licenses/>.
*******************************************************************************/

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
