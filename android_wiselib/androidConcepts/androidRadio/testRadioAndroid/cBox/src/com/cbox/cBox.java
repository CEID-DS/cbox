package com.cbox;

//import android.R;
//import android.R;
import android.app.Activity;
import android.app.AlertDialog;
import android.content.ComponentName;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.ServiceConnection;
import android.os.Bundle;
import android.os.Handler;
import android.os.IBinder;
import android.os.Message;
import android.os.Messenger;
import android.os.RemoteException;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.Toast;

public class cBox extends Activity {
    /** Called when the activity is first created. */
    WiseLib wiseLib = new WiseLib();
    Button jnicall = null;
    private String nativeget= null;
    Object passing = this;
    Messenger mService = null;
    final Messenger messenger = new Messenger(new IncomingHandler());
    
    
    class IncomingHandler extends Handler {
    	public void handleMessage(Message msg){
    		switch (msg.what) {
			case 3:
				Log.v("TEST","Ui received Message");
				wiseLib.androidReceive(passing,"hellooo");
				break;
    		}
    	}
    }
    
    private ServiceConnection mConnection = new ServiceConnection() {
		
		@Override
		public void onServiceDisconnected(ComponentName name) {
			// TODO Auto-generated method stub
			mService = null;
			
		}
		
		@Override
		public void onServiceConnected(ComponentName name, IBinder service) {
			mService = new Messenger(service);
			Message msg = Message.obtain(null,1);
			msg.replyTo = messenger;
			try {
				mService.send(msg);
			} catch (RemoteException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			
		}
	};
    
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        wiseLib.androidInitModel(passing);
        //UdpReceive udpReceive = new UdpReceive(wiseLib,passing);
        
        jnicall = (Button) findViewById(R.id.jnicall);
        jnicall.setOnClickListener(jnilisten);        
        
        
    }
    
    public boolean onCreateOptionsMenu(Menu menu){
    	menu.add(0,0,0,"Start");	
    	return true;
    }
    
    public boolean onOptionsItemSelected(MenuItem item){
    	switch (item.getItemId()){
    		case 0:
    			if(item.getTitle().equals("Start")){
    				item.setTitle("Stop");
    				startService(new Intent(this,Listener.class));
    				bindService(new Intent(this,Listener.class),mConnection, Context.BIND_AUTO_CREATE);
    			}
    			else if(item.getTitle().equals("Stop")){ 
    				item.setTitle("Start");
    				stopService(new Intent(this,Listener.class));
    				
    			}
    			break;
    	}
    	
    	return true;
    }
    
    OnClickListener jnilisten = new OnClickListener() {
		
		@Override
		public void onClick(View v) {
			// TODO Auto-generated method stub
			
			//UdpSend udpSend = new UdpSend("asdds","192.168.1.255");

			wiseLib.androidSend(passing);
			wiseLib.androidReceive(passing, "testingReceiveFromJava\0");
			//Toast.makeText(getBaseContext(), nativeget,Toast.LENGTH_SHORT).show();
			//wiseLib.androidReceive(passing,"hellooo");
		}
	};
	
	
	public void UdpSendNative(String s){

		final String k =s ;
		// prepare the alert box
        AlertDialog.Builder alertbox = new AlertDialog.Builder(this);
        // set the message to display
        alertbox.setMessage(s);
        // add a neutral button to the alert box and assign a click listener
        alertbox.setNeutralButton("Ok", new DialogInterface.OnClickListener() {
            // click listener on the alert box
            public void onClick(DialogInterface arg0, int arg1) {
            	UdpSend udpSend = new UdpSend(k,"broadcast");
            	//udpSend.start();
            }
        });
        // show it
        alertbox.show();
	}
	
    public void debugFromNative(String s)
    {
    	
    	// prepare the alert box
        AlertDialog.Builder alertbox = new AlertDialog.Builder(this);
        // set the message to display
        alertbox.setMessage(s);
        // add a neutral button to the alert box and assign a click listener
        alertbox.setNeutralButton("Ok", new DialogInterface.OnClickListener() {
            // click listener on the alert box
            public void onClick(DialogInterface arg0, int arg1) {
                // the button was clicked
                //Toast.makeText(getApplicationContext(), "OK button clicked", Toast.LENGTH_LONG).show();
            	//UdpSend udpSend = new UdpSend();
            	//udpSend.start();
            }
        });
        // show it
        alertbox.show();
    }
}