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

package com.kostas.bluetoothconnectivity;




import java.io.IOException;
import java.io.InputStream;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;



import com.gkatziou.wificonnectivity.provider.MyNodes;

import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.ContentValues;
import android.content.Intent;
import android.database.Cursor;
import android.net.Uri;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.Toast;

public class BluetoothConnectivity extends Activity {
    /** Called when the activity is first created. */
private Visible visible = new Visible();
    
	//Intent request codes
	private static final int REQUEST_CONNECT_DEVICE = 1;
	private static final int REQUEST_ENABLE_BT = 2;
	
	// Name of the connected device
    private String mConnectedDeviceName = null;
	
	
    // Local Bluetooth adapter
    private BluetoothAdapter mBluetoothAdapter = null;
 
    
    BluetoothSocket mmSocket;

    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        
        // Get local Bluetooth adapter
        mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();

        // If the adapter is null, then Bluetooth is not supported
        if (mBluetoothAdapter == null) {
            Toast.makeText(this, "Bluetooth is not available", Toast.LENGTH_LONG).show();
            finish();
            return;
        }
   }
   
   public void onDestroy(){
	   super.onDestroy();
	   this.finish();
   }
    
   public boolean onCreateOptionsMenu(Menu menu) {
	   super.onCreateOptionsMenu(menu);
	   menu.add(0,0,0,"Start Server");
	   menu.add(0,1,0,"End Server");
	   menu.add(0,2,0,"Send Id");
	   menu.add(0,5,0,"Devices");
	   menu.add(0,6,0,"Close");
	   return true;
   }

   public boolean onOptionsItemSelected(MenuItem item){
	   
	   switch (item.getItemId()) {
		case 0:
			
			Intent device_scan = new Intent( this, DeviceListActivity.class);
            startActivityForResult(device_scan, REQUEST_CONNECT_DEVICE);
			
			
			
			break;
		case 1:
			Log.v("TEST","Stoping Server");
			stopService(new Intent(this,BluetoothListener.class));
			break;
		case 2:
			//Log.v("TEST","gmt");	
			Toast.makeText(getBaseContext(), visible.getLocalIpAddress(),Toast.LENGTH_SHORT).show();
			Toast.makeText(getBaseContext(),visible.getBluetoothMac(), Toast.LENGTH_SHORT).show();
			Toast.makeText(getBaseContext(), visible.getBroadcastIP(),Toast.LENGTH_SHORT ).show();
			visible.SetVisible();
			break;
		case 3:
			
			ContentValues values = new ContentValues();
			values.put(MyNodes.Node.ipv4_addr,"ld");
			values.put(MyNodes.Node.ipv6_addr,"xcc");
			values.put(MyNodes.Node.blue_addr,"qw");
			getContentResolver().insert(MyNodes.Node.CONTENT_URI, values);
			break;
		case 4:
			
			String columns[] = new String[]{MyNodes.Node._ID,MyNodes.Node.ipv4_addr};
			Uri myUri = MyNodes.Node.CONTENT_URI;
			Cursor cur = managedQuery(myUri, columns, null, null, null);
			if(cur.moveToFirst()){
				do{
					Log.v("TEST","lala");
				}while(cur.moveToNext());
			}
			//Log.v("TEST",Integer.toString(cur.getColumnCount()));
			break;
		case 5:
				//Intent devices = new Intent(BluetoothConnectivity.this,Devices.class);
				//startActivity(devices);
			break;
		case 6:
				this.finish();
			break;
	   }
	   
	   return true;
   }
   
   public void onActivityResult(int requestCode, int resultCode, Intent data) {
       switch (requestCode) {
       case REQUEST_CONNECT_DEVICE:
           // When DeviceListActivity returns with a device to connect
           if (resultCode == Activity.RESULT_OK) {
               // Get the device MAC address
               String address = data.getExtras()
                                    .getString(DeviceListActivity.EXTRA_DEVICE_ADDRESS);
               // Get the BLuetoothDevice object
               BluetoothDevice device = mBluetoothAdapter.getRemoteDevice(address);
               // Attempt to connect to the device
               
               Intent intent = new Intent(this, BluetoothListener.class);
               Bundle b = new Bundle();

               // see Bundle.putInt, etc.
               // Bundle.putSerializable for full Objects (careful there)
               b.putString("key", address);  
               intent.putExtras(b);
                              
               
               startService(intent);
              

               
           }
           break;
       case REQUEST_ENABLE_BT:
           // When the request to enable Bluetooth returns
           if (resultCode == Activity.RESULT_OK) {
               // Bluetooth is now enabled, so set up a chat session
               
           } else {
               // User did not enable Bluetooth or an error occured
               Toast.makeText(this, R.string.bt_not_enabled_leaving, Toast.LENGTH_SHORT).show();
               finish();
           }
       }
   }
   
   
}
