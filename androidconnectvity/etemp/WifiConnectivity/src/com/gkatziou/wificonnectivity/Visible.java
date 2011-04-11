package com.gkatziou.wificonnectivity;

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

public class Visible {

	private String ipv4_addr=null;
	private String ipv6_addr=null;
	private String blue_addr=null;
	
	public Visible(){
		ipv4_addr=getLocalIpAddress();
		ipv6_addr="________________________________________";
		blue_addr=getBluetoothMac();
	}
	
	public void SetVisible(){
		try {
			DatagramSocket visibleSocket = new DatagramSocket();
			InetAddress IPAddress = InetAddress.getByName(getBroadcastIP());
			byte[] sendData ;
			String addr_string=ipv4_addr+blue_addr+ipv6_addr;
			sendData = addr_string.getBytes();
			DatagramPacket sendPacket = new DatagramPacket(sendData, sendData.length, IPAddress, 9876);
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
