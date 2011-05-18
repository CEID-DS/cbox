package com.cbox;

import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.SocketException;

import android.content.ContentValues;
import android.location.Address;
import android.util.Log;

public class UdpReceive extends Thread{

	DatagramSocket udpSocket;
	
	public UdpReceive(){
		Log.v("TEST","you made me udpreceiver");
	}
	
	public void run(){
		makeServer();
	}
	
	private void makeServer(){
		Log.v("TEST","Server start");
        byte[] receiveData = new byte[1024];
        DatagramPacket receivePacket = new DatagramPacket(receiveData,receiveData.length);
        String sentence = null;
        
        try {
        	//InetAddress servAddr = InetAddress.getByName("0.0.0.0");
        	Log.v("TEST","The inet");
        		
        	udpSocket = new DatagramSocket(9876);
			
			while(true){
			
				udpSocket.receive(receivePacket);
				sentence = new String(receivePacket.getData());
				
				
				Log.v("TEST",sentence);
			}
			
			
		} catch (SocketException e) {
			// TODO Auto-generated catch block
			Log.v("TEST","sock fail");
			e.printStackTrace();
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
			Log.v("TEST","exeption fail");
		}

	}
	
}
