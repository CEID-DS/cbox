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

import android.app.Activity;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.os.Bundle;
import android.os.Handler;
import android.os.IBinder;
import android.os.Messenger;
import android.os.Message;
import android.os.RemoteException;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;

public class AndroidConnectivity extends Activity {

    Messenger mService = null;
    Remote sService = null ;
    MyRemoteConnection connection ;
    
    boolean mIsBound;
    final Messenger mMessenger = new Messenger(new IncomingHandler());
    
	public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        
	}
	
	public boolean onCreateOptionsMenu(Menu menu){
		super.onCreateOptionsMenu(menu);
		menu.add(0,0,0,"NetListener On");
		menu.add(0,1,0,"BlueListener On");
		//menu.add(0,1,0,"Bind NetListener Service");
		menu.add(0,2,0,"NetTransfer On");
		//menu.add(0,2,0,"Bind NetListener Interface");
		menu.add(0,3,0,"BlueTransfer On");
		
		return true;
	}
	
	
    class IncomingHandler extends Handler {
        @Override
        public void handleMessage(Message msg) {
            switch (msg.what) {
            /*case NetListener.MSG_SET_INT_VALUE:
                textIntValue.setText("Int Message: " + msg.arg1);
                break;
            case NetListener.MSG_SET_STRING_VALUE:
                String str1 = msg.getData().getString("str1");
                textStrValue.setText("Str Message: " + str1);
                break;*/
            default:
                super.handleMessage(msg);
            }
        }
    }

    private ServiceConnection myConnection = new ServiceConnection(){
        public void onServiceConnected(ComponentName className, IBinder service) {
            mService = new Messenger(service);
            try {
                Message msg = Message.obtain(null, NetListener.REGISTER_CLIENT);
                msg.replyTo = mMessenger;
                mService.send(msg);
            } catch (RemoteException e) {
                // In this case the service has crashed before we could even do anything with it
            }
        }

        public void onServiceDisconnected(ComponentName className) {
            // This is called when the connection with the service has been unexpectedly disconnected - process crashed.
            mService = null;
        }

    };
    
    class MyRemoteConnection implements ServiceConnection{

		@Override
		public void onServiceConnected(ComponentName name, IBinder service) {
			sService = Remote.Stub.asInterface(service);
		}

		@Override
		public void onServiceDisconnected(ComponentName name) {
			// TODO Auto-generated method stub
			sService=null;
		}
    	
    	
    }
	    
    void doBindService() {
        bindService(new Intent(this, NetListener.class), myConnection, Context.BIND_AUTO_CREATE);
        mIsBound = true;
    }

    void doUnbindService() {
        if (mIsBound) {
            // If we have received the service, and hence registered with it, then now is the time to unregister.
            if (mService != null) {
                try {
                    Message msg = Message.obtain(null, NetListener.UNREGISTER_CLIENT);
                    msg.replyTo = mMessenger;
                    mService.send(msg);
                } catch (RemoteException e) {
                    // There is nothing special we need to do if the service has crashed.
                }
            }
            // Detach our existing connection.
            unbindService(myConnection);
            mIsBound = false;
        }
    }

	public boolean onOptionsItemSelected(MenuItem item){
		switch(item.getItemId()){
			case 0:
				if(item.getTitle().equals("NetListener On")){
					item.setTitle("NetListener Off");
		            startService(new Intent(AndroidConnectivity.this,NetListener.class));
				}
				else if(item.getTitle().equals("NetListener Off")){
					item.setTitle("NetListener On");
					stopService(new Intent(AndroidConnectivity.this, NetListener.class));
				}
				break;
			case 1:
				if(item.getTitle().equals("BlueListener On")){
					item.setTitle("BlueListener Off");
					startService(new Intent(AndroidConnectivity.this,NetListener.class));
				}
				else if(item.getTitle().equals("BlueListener Off")){
					item.setTitle("BlueListener On");
					stopService(new Intent(AndroidConnectivity.this,NetListener.class));
				}
				break;
			case 2:
				if(item.getTitle().equals("NetTransfer On")){
					item.setTitle("NetTransfer Off");
					startService(new Intent(AndroidConnectivity.this,NetTransfer.class));
				}
				else if(item.getTitle().equals("NetTransfer Off")){
					item.setTitle("NetTransfer On");
					stopService(new Intent(AndroidConnectivity.this,NetTransfer.class));
				}
				break;
			case 3:
				if(item.getTitle().equals("BlueTransfer On")){
					item.setTitle("BlueTransfer Off");
				}
				else if(item.getTitle().equals("BlueTransfer Off")){
					item.setTitle("BlueTransfer On");
				}
				break;
			/*case 1:
				if(item.getTitle().equals("Bind NetListener Service")){
					item.setTitle("Unbind NetListener Service");
		            doBindService();
				}else if(item.getTitle().equals("Unbind NetListener Service")){
					item.setTitle("Bind NetListener Service");
					doUnbindService();					
				}
				break;*/
			/*case 2:
				if(item.getTitle().equals("Bind NetListener Interface")){
					item.setTitle("Unbind NetListener Interface");
					connection = new MyRemoteConnection();
					Intent i = new Intent();
					Log.v("TEST","test help "+com.cbox.androidconnectivity.NetListener.class.getName());
					
					i.setClassName("com.cbox.androidconnectivity","com.cbox.androidconnectivity.NetListener");
				    i.setAction(Remote.class.getName());
				    bindService(i,connection,Context.BIND_AUTO_CREATE);

				}
				else if(item.getTitle().equals("Unbind NetListener Interface")){
					item.setTitle("Bind NetListener Interface");
					unbindService(connection);
					connection=null;
				}
				break;*/
		}
		return true;
	}
    
    protected void onDestroy() {
        super.onDestroy();
        try {
            doUnbindService();
        } catch (Throwable t) {
            Log.e("MainActivity", "Failed to unbind from the service", t);
        }
    }
}