package com.cbox;

import android.app.Activity;
import android.os.Bundle;
import android.widget.Button;
import android.widget.Toast;

public class cBox extends Activity {
    /** Called when the activity is first created. */
    WiseLib wiseLib = new WiseLib();
    
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        
        
        
        
        
        UdpSend udpSend = new UdpSend();
        udpSend.start();
        
        UdpReceive udpReceive = new UdpReceive();
        udpReceive.start();
        
    }
}