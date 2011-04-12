package com.example.jconnectivity;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.SocketException;
import java.util.ArrayList;
//import java.util.UUID;

import javax.bluetooth.DiscoveryAgent;
import javax.bluetooth.LocalDevice;
import javax.bluetooth.UUID;

import javax.microedition.io.Connector;
import javax.microedition.io.StreamConnection;
import javax.microedition.io.StreamConnectionNotifier;

public class BluetoothListener extends Thread {
	
	Thread listen=null;
	ArrayList<Addresses> myaddrs = new ArrayList();
	Addresses temp = new Addresses();
	
	LocalDevice local = null;
	
	StreamConnectionNotifier notifier;
	StreamConnection connection;
	
	public BluetoothListener() {
		
	}
	
	@Override
	public void run() {
		waitinfo();
	}
	
	
	
	private void waitinfo(){
		
		byte[] receiveData = new byte[1024];
		
			// retrieve the local Bluetooth device object
			local = null;		
			connection = null;
			
			// setup the server to listen for connection
			try {
				local = LocalDevice.getLocalDevice();
				local.setDiscoverable(DiscoveryAgent.GIAC);
				
				UUID uuid = new UUID("04c6093b00001000800000805f9b34fb", false);
				//UUID uuid = new UUID("04c6093b-0000-1000-8000-00805f9b34fb", false);
				System.out.println(uuid.toString());
				
	            String url = "btspp://localhost:" + uuid.toString() + ";name=RemoteBluetooth";
	            notifier = (StreamConnectionNotifier)Connector.open(url);
			} catch (Exception e) {
	            e.printStackTrace();
	            return;
	        }
			
			// waiting for connection
			while(true) {
				try {
					System.out.println("waiting for connection...");
		            connection = notifier.acceptAndOpen();
		            
		            InputStream inStream = connection.openInputStream();
		            
		            inStream.read(receiveData);
		            
		            System.out.println(new String(receiveData));
					
		            	            
		            
				} catch (Exception e) {
					e.printStackTrace();
					//return;
				}
			}
		
	}
}
