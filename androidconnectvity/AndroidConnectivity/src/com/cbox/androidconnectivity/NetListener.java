package com.cbox.androidconnectivity;

import android.app.Service;
import android.content.Intent;
import android.os.Handler;
import android.os.IBinder;
import android.os.Message;
import android.os.Messenger;
import android.os.RemoteException;
import android.util.Log;

public class NetListener extends Service {

	
	
	static final int REGISTER_CLIENT = 1;
    static final int UNREGISTER_CLIENT = 2;
	//static final int MSG_SET_INT_VALUE=3;
	//static final int MSG_SET_STRING_VALUE=4;
	final Messenger myMessenger = new Messenger(new IncomingHandler());
	
	public IBinder onBind(Intent intent) {
		// TODO Auto-generated method stub
		if (Remote.class.getName().equals(intent.getAction())){
			Log.v("TEST","wut wut");
			return remote;}
		else{	
			Log.v("TEST","not not ");
			return myMessenger.getBinder();}
	}
	
	class IncomingHandler extends Handler{
		public void handleMessage(Message msg){
			switch(msg.what){
				case REGISTER_CLIENT:
					Log.v("TEST","register");
					break;
				case UNREGISTER_CLIENT:
					Log.v("TEST","unregister");
					break;
				default:
					super.handleMessage(msg);
			}
			
		}
	}
	
	private final Remote.Stub remote = new Remote.Stub() {
		
		@Override
		public int getCounter() throws RemoteException {
			// TODO Auto-generated method stub
			return 8;
		}
	};
	
    public void onCreate() {
        super.onCreate();
        Log.i("MyService", "Service Started.");
    }

    public void onDestroy(){
    	
    }
}
