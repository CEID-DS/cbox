package kostas.ds2.bluetoothsms;






import java.io.IOException;
import java.io.InputStream;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.UUID;

import android.app.Activity;
import android.app.PendingIntent;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Intent;
import android.os.Bundle;
import android.telephony.SmsManager;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

public class BluetoothSms extends Activity {
    /** Called when the activity is first created. */
	
	// Intent request codes
    private static final int REQUEST_CONNECT_DEVICE = 1;
    private static final int REQUEST_ENABLE_BT = 2;
	
	public static final String DEVICE_NAME = "device_name";
    public static final String TOAST = "toast";
	
    // Message types sent from the BluetoothChatService Handler
    public static final int MESSAGE_STATE_CHANGE = 1;
    public static final int MESSAGE_READ = 2;
    public static final int MESSAGE_WRITE = 3;
    public static final int MESSAGE_DEVICE_NAME = 4;
    public static final int MESSAGE_TOAST = 5;
    
	// Name of the connected device
    private String mConnectedDeviceName = null;
	
	
    // Local Bluetooth adapter
    private BluetoothAdapter mBluetoothAdapter = null;
 
    
    BluetoothSocket mmSocket;
    
    private static final UUID MY_UUID = UUID.fromString("04c6093b-0000-1000-8000-00805f9b34fb");

    
    TextView textv;
    
    @Override
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
        
        Button scan = (Button) findViewById(R.id.Button01);
        scan.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
            	Intent serverIntent = new Intent( BluetoothSms.this, DeviceListActivity.class);
                startActivityForResult(serverIntent, REQUEST_CONNECT_DEVICE);
            	
            	
            	
            }
        });

        textv = (TextView) findViewById(R.id.TextView01);
        
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
                
                

                Method m;
                
                try {
 				m= device.getClass().getMethod("createRfcommSocket",new Class[]{int.class});
 				mmSocket = (BluetoothSocket)m.invoke(device, Integer.valueOf(1));
 				mmSocket.connect();
 				
 				
 				
 				InputStream inStream = null;
 				inStream = mmSocket.getInputStream();
 				byte[] buf = new byte[60];		
 				
 				 				
 				inStream.read(buf);
 				
 				byte[] pnumber = new byte[10];
 				byte[] msg = new byte[60];
 				
 				for(int i=0;i<buf.length;i++){
 					if(i<10){
 						pnumber[i] = buf[i];
 					}
 					else{
 						msg[i-10] = buf[i];
 					}
 				}
 				
 				String phonenumber = new String(pnumber);
 				String message = new String(msg);
 				
 				//Toast.makeText(getBaseContext(),new String(buf), Toast.LENGTH_SHORT).show();
 				textv.setText("Server is going to send sms to:\n" + phonenumber);
 				
 				textv.setText(textv.getText()+"\nmessage:\n"+ message);
 				
 				
 				mmSocket.close();
 				
 				
 				sendSms(phonenumber,message);
 				
 				
 			} catch (SecurityException e) {
 				// TODO Auto-generated catch block
 				e.printStackTrace();
 			} catch (NoSuchMethodException e) {
 				// TODO Auto-generated catch block
 				e.printStackTrace();
 			} catch (IllegalArgumentException e) {
 				// TODO Auto-generated catch block
 				e.printStackTrace();
 			} catch (IllegalAccessException e) {
 				// TODO Auto-generated catch block
 				e.printStackTrace();
 			} catch (InvocationTargetException e) {
 				// TODO Auto-generated catch block
 				e.printStackTrace();
 			} catch (IOException e) {
 				// TODO Auto-generated catch block
 				e.printStackTrace();
 			}
 				
                
               

                
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
    
    public void sendSms(String number,String message){
    	
    	 PendingIntent pi = PendingIntent.getActivity(this, 0,
    	            new Intent(this, BluetoothSms.class), 0); 
    	 
    	 SmsManager sms = SmsManager.getDefault();
    	 sms.sendTextMessage(number, null, message, pi, null); 
    }
    
}