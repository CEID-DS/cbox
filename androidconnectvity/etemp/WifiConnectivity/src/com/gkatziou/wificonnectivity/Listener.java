package com.gkatziou.wificonnectivity;

import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.SocketException;
import java.util.ArrayList;
import android.app.Service;
import android.content.Intent;
import android.os.IBinder;
import android.util.Log;

public class Listener extends Service {
	
	DatagramSocket udpsocket;
	private Thread sthread ;
	ArrayList<Addresses> myaddrs = new ArrayList();
	Addresses temp = new Addresses();
	
	@Override
	public IBinder onBind(Intent intent) {
		// TODO Auto-generated method stub
		return null;
	}
	
	private void makeServer(){
		sthread = new Thread(){
			public void run(){
				Log.v("TEST","Server start");
	            byte[] receiveData = new byte[1024];
	            DatagramPacket receivePacket = new DatagramPacket(receiveData,receiveData.length);
	            String sentence = null;
	            
	            try {
	            	//InetAddress servAddr = InetAddress.getByName("0.0.0.0");
	            	Log.v("TEST","The inet");
	            		            	
					udpsocket = new DatagramSocket(9876);
					
					while(true){
					
						udpsocket.receive(receivePacket);
						sentence = new String(receivePacket.getData());		
						temp.ipv4_addr=sentence.substring(0,12);
						temp.blue_addr=sentence.substring(12,30);
						temp.ipv6_addr=sentence.substring(30,70);
						
						myaddrs.add(temp);
						
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
		};
		
	}
	
	public void onCreate(){
		super.onCreate();
		Log.v("TEST","Listener Started");
		makeServer();
	}
	
	public void onDestroy(){
		super.onDestroy();
		udpsocket.close();
		sthread.stop();
		Log.v("TEST","Listener Stop");
	}
	
	public void onStart(Intent intent ,int startid){
		Log.v("TEST","onstart");
		sthread.start();
	}
}

class Addresses{
	public String ipv4_addr=null;
	public String blue_addr=null;
	public String ipv6_addr=null;
}