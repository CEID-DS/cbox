package com.cbox;

import android.app.Activity;
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
    
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        
        jnicall = (Button) findViewById(R.id.jnicall);
        jnicall.setOnClickListener(jnilisten);        
        
        
        //UdpSend udpSend = new UdpSend();
        //udpSend.start();
        
        //UdpReceive udpReceive = new UdpReceive();
        //udpReceive.start();
        
    }
    
    OnClickListener jnilisten = new OnClickListener() {
		
		@Override
		public void onClick(View v) {
			// TODO Auto-generated method stub
			
			nativeget=wiseLib.androidSend(this);
			Toast.makeText(getBaseContext(), nativeget,Toast.LENGTH_SHORT).show();
		}
	};
	
	
	public void UdpSendNative(String s){
		Toast.makeText(getBaseContext(),s,Toast.LENGTH_SHORT).show();
	}
}