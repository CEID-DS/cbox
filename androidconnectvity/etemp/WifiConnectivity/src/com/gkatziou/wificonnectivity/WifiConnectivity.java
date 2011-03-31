package com.gkatziou.wificonnectivity;

import java.security.PublicKey;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;

public class WifiConnectivity extends Activity {
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        
        
   }
   
   public boolean onCreateOptionsMenu(Menu menu) {
	   super.onCreateOptionsMenu(menu);
	   menu.add(0,0,0,"Start Server");
	   menu.add(0,1,0,"End Server");
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
	   }
	   
	   return true;
   }
   
}