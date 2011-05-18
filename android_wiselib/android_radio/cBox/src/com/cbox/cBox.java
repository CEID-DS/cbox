package com.cbox;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.Toast;

public class cBox extends Activity {
    /** Called when the activity is first created. */
    WiseLib wiseLib = new WiseLib();
    Button callnative = null;
    private String wiselibsend = null;
    
    
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        
        
        callnative = (Button) findViewById(R.id.jnibutton);
        callnative.setOnClickListener(dothejni);
        
        //UdpSend udpSend = new UdpSend();
        //udpSend.start();
        
        //UdpReceive udpReceive = new UdpReceive();
        //udpReceive.start();
        
        
    }
    
    OnClickListener dothejni = new OnClickListener() {
		
		@Override
		public void onClick(View v) {
			// TODO Auto-generated method stub
			wiselibsend = wiseLib.androidSend(this);
			Toast.makeText(getBaseContext(),wiselibsend, Toast.LENGTH_SHORT).show();
		}
	};
	
	public void UdpSendNative(String s){
		
		Toast.makeText(getBaseContext(),s,Toast.LENGTH_SHORT).show();
	
	    AlertDialog.Builder alertbox = new AlertDialog.Builder(this);
	    // set the message to display
	    alertbox.setMessage(s);
	    // add a neutral button to the alert box and assign a click listener
	    alertbox.setNeutralButton("Ok", new DialogInterface.OnClickListener() {
	        // click listener on the alert box
	        public void onClick(DialogInterface arg0, int arg1) {
	      
	        	UdpSend udpSend = new UdpSend();
	        	udpSend.start();
	        	// the button was clicked
	            // do nothing
	        }
	    });
	    // show it
	    alertbox.show();
	}
	
}