package com.example.jconnectivity;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.SocketException;
import java.util.ArrayList;

public class NetListener extends Thread {
	
	Thread listen=null;
	ArrayList<Addresses> myaddrs = new ArrayList();
	
	public NetListener() {
		
	}
	
	@Override
	public void run() {
		waitinfo();
	}
	
	
	
	private void waitinfo(){
		
		byte[] receiveData = new byte[1024];
		
		try {
			DatagramSocket serverSocket= new DatagramSocket(9876);
			DatagramPacket receivePacket = new DatagramPacket(receiveData,receiveData.length);
			
			while(true){
				serverSocket.receive(receivePacket);
				String sentence = new String(receivePacket.getData());
				//System.out.println(sentence);
			
				Addresses temp = new Addresses();
				
				temp.ipv4_addr=sentence.substring(0,12);
				temp.blue_addr=sentence.substring(12,30);
				temp.ipv6_addr=sentence.substring(30,70);
				
				myaddrs.add(temp);
			
			}
			
		} catch (SocketException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
	}
}