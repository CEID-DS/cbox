package com.cbox;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.NetworkInterface;
import java.net.SocketException;
import java.net.UnknownHostException;
import java.util.Enumeration;
import java.util.StringTokenizer;

import android.bluetooth.BluetoothAdapter;
import android.util.Log;

public class UdpSend implements Runnable{

	private String destination;
	private Thread thread = null;
	private String data = null;
	
	public UdpSend(String gdata,String dest){
		
		data = gdata;
		if(dest.equals("broadcast")){
			destination=getBroadcastIP();
		}
		else{
			destination = dest;
		}
		
		thread = new Thread(this);
		thread.start();
		
	}
	
	public void run(){
		SendUdp();
	}
	
	private void SendUdp(){
		try {
			Log.v("TEST","sending");
			
			DatagramSocket visibleSocket = new DatagramSocket();
			InetAddress IPAddress = InetAddress.getByName(destination);
			byte[] sendData = new byte[512];
			sendData = data.getBytes();
			DatagramPacket sendPacket = new DatagramPacket(sendData, sendData.length, IPAddress, 9876);
		    visibleSocket.send(sendPacket);
		    visibleSocket.close();
		    
		    Log.v("TEST","send");
		} catch (SocketException e) {
			// TODO Auto-generated catch block
			Log.v("TEST","prob 1");
			e.printStackTrace();
		} catch (UnknownHostException e) {
			// TODO Auto-generated catch block
			Log.v("TEST","prob 2");
			e.printStackTrace();
		} catch (IOException e) {
			Log.v("TEST","prob 3");
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	
    public String getLocalIpAddress() {
    	try {
    		for (Enumeration<NetworkInterface> en = NetworkInterface.getNetworkInterfaces(); en.hasMoreElements();) {
    			NetworkInterface intf = en.nextElement();
	            for (Enumeration<InetAddress> enumIpAddr = intf.getInetAddresses(); enumIpAddr.hasMoreElements();) {
	                InetAddress inetAddress = enumIpAddr.nextElement();
	                if (!inetAddress.isLoopbackAddress()) {
	                    return inetAddress.getHostAddress().toString();
	                }
	            }
	        }
	    } catch (SocketException ex) {
		       
	    }
	    return null;
    }
	   
    public String getBluetoothMac(){
    	BluetoothAdapter bluetoothDefaultAdapter = BluetoothAdapter.getDefaultAdapter();
    	if (bluetoothDefaultAdapter != null)
    		return BluetoothAdapter.getDefaultAdapter().getAddress();
	   
    	return null;
    }
    
    public String getBroadcastIP(){
    	String myIP = getLocalIpAddress();
    	StringTokenizer tokens = new StringTokenizer(myIP,".");
    	int count = 0;
    	String broadcast = "";
    	while(count<3){
    		broadcast += tokens.nextToken()+".";
    		count++;
    	}
    	
    	return broadcast+"255";
    }
}
