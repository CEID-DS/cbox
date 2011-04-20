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
