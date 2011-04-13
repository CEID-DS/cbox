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
		String addr_data=ipv4_addr+ipv6_addr+blue_addr;
		byte[] sendData = new byte[1024];
		
		BufferedReader hello = new BufferedReader(new InputStreamReader(System.in));
		try {
			DatagramSocket visibleSocket = new DatagramSocket();
			InetAddress ipAddress= InetAddress.getByName("192.168.1.64");
			sendData = addr_data.getBytes();
			DatagramPacket sendPacket = new DatagramPacket(sendData, sendData.length,ipAddress,	9876);
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
