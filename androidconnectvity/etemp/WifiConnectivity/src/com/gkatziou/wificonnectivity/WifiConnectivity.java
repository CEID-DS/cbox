package com.gkatziou.wificonnectivity;

import java.security.PublicKey;
import java.util.Enumeration;
import java.net.InetAddress;
import java.net.NetworkInterface;
import java.net.SocketException;
import java.net.UnknownHostException;
import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.TextView;
import android.widget.Toast;

public class WifiConnectivity extends Activity {
    /** Called when the activity is first created. */
    private TextView mprog;
    private Visible visible = new Visible();
    
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);               
        
   }
   
   public boolean onCreateOptionsMenu(Menu menu) {
	   super.onCreateOptionsMenu(menu);
	   menu.add(0,0,0,"Start Server");
	   menu.add(0,1,0,"End Server");
	   menu.add(0,2,0,"Send Id");
	   return true;
   }

   public boolean onOptionsItemSelected(MenuItem item){
	   
	   switch (item.getItemId()) {
		case 0:
			startService(new Intent(this,Listener.class));
			break;
		case 1:
			//Log.v("TEST","Stoping Server");
			stopService(new Intent(this,Listener.class));
			break;
		case 2:
			//Log.v("TEST","gmt");	
			Toast.makeText(getBaseContext(), visible.getLocalIpAddress(),Toast.LENGTH_SHORT).show();
			Toast.makeText(getBaseContext(),visible.getBluetoothMac(), Toast.LENGTH_SHORT).show();
			Toast.makeText(getBaseContext(), visible.getBroadcastIP(),Toast.LENGTH_SHORT ).show();
			visible.SetVisible();
			break;
	   }
	   
	   return true;
   }
}