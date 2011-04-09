package com.gkatziou.wificonnectivity;

import android.app.Activity;
import android.content.ContentValues;
import android.content.Intent;
import android.database.Cursor;
import android.net.Uri;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.Toast;

import com.gkatziou.wificonnectivity.provider.MyNodes;

public class WifiConnectivity extends Activity {
    /** Called when the activity is first created. */
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
	   //menu.add(0,3,0,"pcon");
	   //menu.add(0,4,0,"scon");
	   return true;
   }

   public boolean onOptionsItemSelected(MenuItem item){
	   
	   switch (item.getItemId()) {
		case 0:
			startService(new Intent(this,Listener.class));
			break;
		case 1:
			Log.v("TEST","Stoping Server");
			stopService(new Intent(this,Listener.class));
			break;
		case 2:
			//Log.v("TEST","gmt");	
			Toast.makeText(getBaseContext(), visible.getLocalIpAddress(),Toast.LENGTH_SHORT).show();
			Toast.makeText(getBaseContext(),visible.getBluetoothMac(), Toast.LENGTH_SHORT).show();
			Toast.makeText(getBaseContext(), visible.getBroadcastIP(),Toast.LENGTH_SHORT ).show();
			visible.SetVisible();
			break;
		case 3:
			/*
			ContentValues values = new ContentValues();
			values.put(MyNodes.Node.ipv4_addr,"ld");
			values.put(MyNodes.Node.ipv6_addr,"xcc");
			values.put(MyNodes.Node.blue_addr,"qw");
			getContentResolver().insert(MyNodes.Node.CONTENT_URI, values);*/
			break;
		case 4:
			/*
			String columns[] = new String[]{MyNodes.Node._ID,MyNodes.Node.ipv4_addr};
			Uri myUri = MyNodes.Node.CONTENT_URI;
			Cursor cur = managedQuery(myUri, columns, null, null, null);
			Log.v("TEST",Integer.toString(cur.getColumnCount()));*/
			break;
	   }
	   
	   return true;
   }
   
   
}