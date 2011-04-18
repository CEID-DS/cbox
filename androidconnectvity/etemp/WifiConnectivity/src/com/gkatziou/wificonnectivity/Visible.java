/******************************************************************************
* This file is part of cbox.
* 
* cbox is free software: you can redistribute it and/or modify
* it under the terms of the GNU LesserGeneral Public License as published
* by the Free Software Foundation, either version 3 of the License, or
* any later version.
* 
* Cbox is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU Lesser General Public License for more details.
* 
* You should have received a copy of the GNU Lesser General Public License
* along with cbox.  If not, see <http://www.gnu.org/licenses/>.
*******************************************************************************/

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
			String addr_string="<ipv4_addr>"+ipv4_addr+"</ipv4_addr>"+"\n"
			+"<blue_addr>"+blue_addr+"</blue_addr>"+"\n"
			+"<ipv6_addr>"+ipv6_addr+"</ipv6_addr>"+"\n";
			
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
