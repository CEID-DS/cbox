package com.cbox;

//import android.R;
//import android.R;
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
    Button jnicall = null;
    private String nativeget= null;
    Object passing = this;
    
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        
        UdpReceive udpReceive = new UdpReceive(wiseLib,passing);
        
        jnicall = (Button) findViewById(R.id.jnicall);
        jnicall.setOnClickListener(jnilisten);        
        
        
    }
    
    OnClickListener jnilisten = new OnClickListener() {
		
		@Override
		public void onClick(View v) {
			// TODO Auto-generated method stub
			
			UdpSend udpSend = new UdpSend("asdds","192.168.1.255");
			//wiseLib.androidSend(passing);
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