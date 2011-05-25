package com.cbox;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.SocketException;

import android.os.Looper;
import android.util.Log;

public class UdpReceive implements Runnable{

	Thread thread;
	
	private DatagramSocket serverSocket = null;
	private DatagramPacket receivePacket = null;
	private String sentence = null;
	private Object passing = null;
	private WiseLib wiseLib = null;
	
	
	public UdpReceive(WiseLib mywiseLib,Object object){
		Log.v("TEST","Starting Receiver");
		thread = new Thread(this);
		thread.start();
		
		wiseLib = mywiseLib;
		passing = object;
	}
	
	public void run(){
		
		byte[] receiveData = new byte[512];
		receivePacket = new DatagramPacket(receiveData,receiveData.length);		
		try {
			serverSocket = new DatagramSocket(9876);
			
			Looper.myLooper().prepare();
			
			while(true){
				serverSocket.receive(receivePacket);
				sentence = new String(receivePacket.getData());
				Log.v("TEST","Received "+sentence);
		
				
				wiseLib.androidReceive(passing,"hellooo");
			

				Looper.myLooper().loop();
			}
			
			
		} catch (SocketException e) {
			Log.v("TEST","sock error");
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IOException e) {
			Log.v("TEST","io error");
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		serverSocket.close();
	}
	
}
