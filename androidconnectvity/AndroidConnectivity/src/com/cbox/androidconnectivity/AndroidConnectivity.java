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
		menu.add(0,0,0,"Start NetListener Service");
		menu.add(0,1,0,"Bind NetListener Service");
		menu.add(0,2,0,"Bind NetListener Interface");
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
	    
    /*private void sendMessageToService(int intvaluetosend) {
        if (mIsBound) {
            if (mService != null) {
                try {
                    Message msg = Message.obtain(null, NetListener.MSG_SET_INT_VALUE, intvaluetosend, 0);
                    msg.replyTo = mMessenger;
                    mService.send(msg);
                } catch (RemoteException e) {
                }
            }
        }
    }*/
    
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
				if(item.getTitle().equals("Start NetListener Service")){
					item.setTitle("Stop NetListener Service");
		            startService(new Intent(AndroidConnectivity.this,NetListener.class));
				}else if(item.getTitle().equals("Stop NetListener Service")){
					item.setTitle("Start NetListener Service");
					//doUnbindService();
					stopService(new Intent(AndroidConnectivity.this, NetListener.class));
				}
				break;
			case 1:
				if(item.getTitle().equals("Bind NetListener Service")){
					item.setTitle("Unbind NetListener Service");
		            doBindService();
				}else if(item.getTitle().equals("Unbind NetListener Service")){
					item.setTitle("Bind NetListener Service");
					doUnbindService();					
				}
				break;
			case 2:
				if(item.getTitle().equals("Bind NetListener Interface")){
					item.setTitle("Unbind NetListener Interface");
					connection = new MyRemoteConnection();
					Intent i = new Intent();
					i.setClassName("com.cbox.androidconnectivity", com.cbox.androidconnectivity.NetListener.class.getName());
				    i.setAction(Remote.class.getName());
				    boolean ret = bindService(i,connection,Context.BIND_AUTO_CREATE);
				}
				else if(item.getTitle().equals("Unbind NetListener Interface")){
					item.setTitle("Bind NetListener Interface");
					unbindService(connection);
					connection=null;
				}
				break;
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