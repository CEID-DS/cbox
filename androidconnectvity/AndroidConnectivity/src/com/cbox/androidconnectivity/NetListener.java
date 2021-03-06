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

package com.cbox.androidconnectivity;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketException;
import java.util.ArrayList;

import android.app.Service;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.os.Bundle;
import android.os.Handler;
import android.os.IBinder;
import android.os.Message;
import android.os.Messenger;
import android.os.RemoteException;
import android.util.Log;

public class NetListener extends Service {
	
	DatagramSocket datagramSocket;
	private Thread thread = null;
	Messenger tonetTransfer = null;
	Message msg = null;

	ArrayList<ServiceClients> serviceClients = new ArrayList<ServiceClients>();
	ArrayList<Addresses> addresses = new ArrayList<Addresses>();
	
	static final int REGISTER_CLIENT = 1;
    static final int UNREGISTER_CLIENT = 2;
    static final int SIMPLE_MESSAGE = 3;
	final Messenger mMessenger = new Messenger(new IncomingHandler());
	
	public IBinder onBind(Intent intent) {
		
		return mMessenger.getBinder();
	}
	
	class IncomingHandler extends Handler{
		public void handleMessage(Message mmsg){
			switch(mmsg.what){
				case REGISTER_CLIENT:
					Log.v("TEST","register");
					break;
				case UNREGISTER_CLIENT:
					Log.v("TEST","unregister");
					break;
				case SIMPLE_MESSAGE:
					Log.v("TEST","simple message");
					break;
				default:
					super.handleMessage(msg);
			}
			
		}
	}
	
	private ServiceConnection mConnection = new ServiceConnection() {
		
		@Override
		public void onServiceDisconnected(ComponentName name) {
			// TODO Auto-generated method stub
			
		}
		
		@Override
		public void onServiceConnected(ComponentName name, IBinder service) {
			tonetTransfer = new Messenger(service);
			
			Bundle b = new Bundle();
			b.putString("whois", "NetListener");
			
			msg = Message.obtain(null,NetTransfer.REGISTER_CLIENT);
			msg.setData(b);
			msg.replyTo = mMessenger;
			
			try {
				tonetTransfer.send(msg);
			} catch (RemoteException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
	};
	
    public void onCreate() {
        super.onCreate();
    	makelistener();
    }

    public void onDestroy(){
    	Log.v("TEST","Netlisten ondestroy");


		msg = Message.obtain(null,NetTransfer.UNREGISTER_CLIENT);
		msg.replyTo = mMessenger;
		
		try {
			tonetTransfer.send(msg);
		} catch (RemoteException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
    	
    	unbindService(mConnection);
    	//datagramSocket.close();
    	thread.stop();
    }
    
    public void onStart(Intent intent,int startid){
    	Log.v("TEST","Netlisten onStart");
    	bindService(new Intent(this,NetTransfer.class), mConnection, Context.BIND_AUTO_CREATE);
    	thread.start();
    }
    
    private void datamessage(InetAddress inetAddress,int id){

    	Bundle b = new Bundle();

    	msg = Message.obtain(null,id);
    	
    	b.putString("ip", inetAddress.toString().replace("/",""));

		msg.setData(b);
    	
    	msg.replyTo = mMessenger;	
    	
    	try {
    		tonetTransfer.send(msg);
    	}catch(RemoteException e){
    		e.printStackTrace();
    	}
    }
    
    private void makelistener(){
    	thread = new Thread() {
    		
    		byte[] receiveData = new byte[1024];
    		DatagramPacket receivePacket = new DatagramPacket(receiveData, receiveData.length);
    		String bufferstring=null;;
    		
			@Override
			public void run() {
				try {
					datagramSocket = new DatagramSocket(9876);
					
					while(true){
						Log.v("TEST","thread");
						 
						datagramSocket.receive(receivePacket);
						bufferstring= new String(receivePacket.getData());
						if(bufferstring.lastIndexOf("<datamessage/>")!=-1)
							datamessage(receivePacket.getAddress(),NetTransfer.TO_RECEIVE);
						else if(bufferstring.lastIndexOf("<requiredata/>")!=-1)
							datamessage(receivePacket.getAddress(),NetTransfer.TO_SENT);
					}
					
				} catch (SocketException e) {
					Log.v("TEST","problem");
					e.printStackTrace();
				} catch (IOException e) {
					Log.v("TEST","problem");
					e.printStackTrace();
				}
			}
		};
    }
    
    
}
