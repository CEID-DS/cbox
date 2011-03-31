package com.gkatziou.wificonnectivity;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketException;

import android.app.Service;
import android.content.Intent;
import android.os.IBinder;
import android.util.Log;
import android.widget.Toast;

public class Listener extends Service {
	
	DatagramSocket udpsocket;
	private Thread sthread = null;
	
	
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
	            byte[] sendData = new byte[1024];
	            
	            try {
	            	Log.v("TEST","asd");
	            	
					udpsocket = new DatagramSocket(9876);
					
					while(true){
						DatagramPacket receivePacket = new DatagramPacket(receiveData,receiveData.length);
						udpsocket.receive(receivePacket);
						String sentence = new String(receivePacket.getData());
						Log.v("TEST",sentence);
					}
					
					
				} catch (SocketException e) {
					// TODO Auto-generated catch block
					Log.v("TEST","fail");
					e.printStackTrace();
				} catch (Exception e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
					Log.v("TEST","fail");
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
		sthread.stop();
		Log.v("TEST","Listener Stop");
	}
	
	public void onStart(Intent intent ,int startid){
		Log.v("TEST","onstart");
		sthread.start();
	}
	
	
}
