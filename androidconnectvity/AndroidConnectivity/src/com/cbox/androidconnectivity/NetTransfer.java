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
import java.net.InetAddress;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.ArrayList;

import android.app.Service;
import android.content.ComponentName;
import android.content.Intent;
import android.content.ServiceConnection;
import android.os.Handler;
import android.os.IBinder;
import android.os.Message;
import android.os.Messenger;
import android.util.Log;

public class NetTransfer extends Service {

	private ServerSocket serverSocket = null;
	private String fromaddr = null;
	
	Messenger tonetListener = null;
	Message msg = null;
	
	ArrayList<ServiceClients> serviceClients = new ArrayList<ServiceClients>();
	ArrayList<Addresses> senderAddresses = new ArrayList<Addresses>();
	
	final Messenger mMessenger = new Messenger(new IncomingHandler());
	
	static final int REGISTER_CLIENT = 1;
	static final int UNREGISTER_CLIENT = 2;
	static final int SIMPLE_MESSAGE = 3;
	static final int TO_RECEIVE = 4;
	
    class IncomingHandler extends Handler {
        @Override
        public void handleMessage(Message mmsg) {
            switch (mmsg.what) {
            	case REGISTER_CLIENT:
            		
            		ServiceClients buffer = new ServiceClients();
            		buffer.messenger=mmsg.replyTo;
            		buffer.myid=mmsg.getData().getString("whois");
            		
            		if(!serviceClients.contains(buffer))
            			serviceClients.add(buffer);
            		
            		break;
            	case UNREGISTER_CLIENT:
            		Log.v("TEST","unregister client");
            		break;
            	case SIMPLE_MESSAGE:
            		Log.v("TEST","simple message");
            	case TO_RECEIVE:
            		Log.v("TEST","to receive");
            			fromaddr = mmsg.getData().getString("ip");
            			Runnable runnable = new Transfer(serverSocket, fromaddr);
            			Thread thread = new Thread(runnable);
            			thread.start();
            	default:
            		super.handleMessage(mmsg);
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
			tonetListener = new Messenger(service);
			
			msg = Message.obtain(null, NetListener.REGISTER_CLIENT);
            msg.replyTo = mMessenger;
            
			try{
				tonetListener.send(msg);
			}catch (Exception e) {
				// TODO: handle exception
			}
			
		}
	};
	
	@Override
	public IBinder onBind(Intent intent) {
		// TODO Auto-generated method stub
		return mMessenger.getBinder();
	}
		
	public void onCreate(){
		//Log.v("TEST","Netransfer oncreate");
	}
	
	public void onDestroy(){
		//Log.v("TEST","Netransfer ondestroy");
	}
	
	public void onStart(Intent intent,int startid){
		try {
			serverSocket = new ServerSocket(9876);		
			Log.v("TEST","Socket initialized");
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
	}
}
