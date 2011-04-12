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

package com.example.jconnectivity;

import java.awt.image.BufferedImage;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.ObjectInputStream.GetField;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketException;
import java.net.UnknownHostException;

import javax.bluetooth.BluetoothStateException;
import javax.bluetooth.LocalDevice;

public class Visible {

	private String ipv4_addr=null;
	private String ipv6_addr=null;
	private String blue_addr=null;
	
	public Visible(){
		ipv4_addr=getLocalIpAddress();
		ipv6_addr="________________________________________";
		blue_addr=getBluetoothMac();
	}

	public void broadcast(){
		System.out.println(ipv4_addr+"\n"+ipv6_addr+"\n"+blue_addr);
		byte[] sendData = new byte[1024];
		
		BufferedReader hello = new BufferedReader(new InputStreamReader(System.in));
		try {
			DatagramSocket visibleSocket = new DatagramSocket();
			InetAddress ipAddress= InetAddress.getByName("192.168.1.255");
			sendData = "asdasd".getBytes();
			DatagramPacket sendPacket = new DatagramPacket(sendData, sendData.length,ipAddress,9876);
			visibleSocket.send(sendPacket);
			visibleSocket.close();
			
		} catch (SocketException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (UnknownHostException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

		
	}
	
	public String getLocalIpAddress(){
		
		try {
		    InetAddress addr = InetAddress.getLocalHost();
		    
		    /*
		    byte[] ipAddr = addr.getAddress();

		    // Convert to dot representation
		    String ipAddrStr = "";
		    for (int i=0; i<ipAddr.length; i++) {
		        if (i > 0) {
		            ipAddrStr += ".";
		        }
		        ipAddrStr += ipAddr[i]&0xFF;
		    }*/
		    
		    return addr.getAddress().toString();
		    
		} catch (UnknownHostException e) {
		}
		
		return null;
	}
	
	public String getLocalIp6Address(){
		
		return null;
	}
	
	public  String getBluetoothMac(){
		
		LocalDevice localDevice;
		try {
			localDevice = LocalDevice.getLocalDevice();
			return localDevice.getBluetoothAddress();
		} catch (BluetoothStateException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		return null;
	}

	public String getBroadcastIP(){
		return null;
	}
}
