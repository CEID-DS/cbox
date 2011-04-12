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
	Addresses temp = new Addresses();
	
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
				System.out.println(sentence);
				
				temp.ipv4_addr=sentence;
				
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

