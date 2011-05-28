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

package com.cbox;

import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.SocketException;
import java.util.ArrayList;
import android.app.Service;
import android.content.ContentValues;
import android.content.Intent;
import android.os.Handler;
import android.os.IBinder;
import android.os.Message;
import android.os.Messenger;
import android.os.RemoteException;
import android.util.Log;


public class Listener extends Service {
	
	DatagramSocket udpsocket;
	private Thread sthread ;
	
	//ArrayList<Messenger> mClients = new ArrayList<Messenger>();
	Messenger mClients;
	final Messenger messenger = new Messenger(new IncomingHandler());
	
	
	@Override
	public IBinder onBind(Intent intent) {
		// TODO Auto-generated method stub
		return messenger.getBinder();
	}
	
	class IncomingHandler extends Handler{
		public void handleMessage(Message msg){
			switch(msg.what){
				case 1:
					mClients=msg.replyTo;
					Log.v("TEST","got mes");
					break;
			}
		}
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
						
						ContentValues values = new ContentValues();
												
						Log.v("TEST",sentence);
						sendMessageToUI();
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
	
	private void sendMessageToUI(){
		Message msg = Message.obtain(null,3);
		try {
			mClients.send(msg);
		} catch (RemoteException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
}
