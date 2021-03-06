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

package com.kostas.bluetoothconnectivity;

import java.io.InputStream;
import java.io.OutputStream;
import java.lang.reflect.Method;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.SocketException;
import java.util.ArrayList;
import java.util.UUID;

import android.app.Service;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.ContentValues;
import android.content.Intent;
import android.os.Bundle;
import android.os.IBinder;
import android.util.Log;

import com.gkatziou.wificonnectivity.provider.MyNodes;

public class BluetoothListener extends Service{

	DatagramSocket udpsocket;
	private Thread sthread ;
	ArrayList<Addresses> myaddrs = new ArrayList<Addresses>();
	
	String bluetooth_address;
	
	// Name of the connected device
    private String mConnectedDeviceName = null;
	
	
    // Local Bluetooth adapter
    private BluetoothAdapter mBluetoothAdapter = null;
 
    
    BluetoothSocket mmSocket;
    
    private static final UUID MY_UUID = UUID.fromString("04c6093b-0000-1000-8000-00805f9b34fb");
	
	
	@Override
	public IBinder onBind(Intent intent) {
		// TODO Auto-generated method stub
		
		Bundle b = intent.getExtras();
		bluetooth_address = b.getString("key");
		
		
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
	            		            	
					//udpsocket = new DatagramSocket(9876);
					
					while(true){
						
						BluetoothDevice device = mBluetoothAdapter.getRemoteDevice(bluetooth_address);
		                // Attempt to connect to the device
						
						Method m;
		                
		                
		 				m= device.getClass().getMethod("createRfcommSocket",new Class[]{int.class});
		 				mmSocket = (BluetoothSocket)m.invoke(device, Integer.valueOf(1));
		 				mmSocket.connect();
		 				
		 				
		 				/*
		 				InputStream inStream = null;
		 				inStream = mmSocket.getInputStream();
		 				byte[] buf = new byte[60];
		 						 				 				
		 				inStream.read(buf);
		 				*/
		 				
		 				OutputStream outStream = null;
		 				outStream = mmSocket.getOutputStream();
		 				String hello = new String("Hello Bluetooth\n --my address: "+bluetooth_address);
		 				
		 				byte[] b_array = hello.getBytes();
		 				outStream.write(b_array);
		 				
						
						
						/*
					
						Addresses temp = new Addresses();
						
						udpsocket.receive(receivePacket);
						sentence = new String(receivePacket.getData());
						//both ok
						temp.ipv4_addr=receivePacket.getAddress().toString();
							//sentence.substring(0,12);
						temp.blue_addr=sentence.substring(12,30);
						temp.ipv6_addr=sentence.substring(30,70);
						
						ContentValues values = new ContentValues();
						
						values.put(MyNodes.Node.ipv4_addr,temp.ipv4_addr);
						values.put(MyNodes.Node.blue_addr,temp.blue_addr);
						values.put(MyNodes.Node.ipv6_addr,temp.ipv6_addr);
						getContentResolver().insert(MyNodes.Node.CONTENT_URI, values);
						
						myaddrs.add(temp);
						*/
						
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
