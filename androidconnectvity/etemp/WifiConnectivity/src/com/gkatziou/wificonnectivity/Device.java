package com.gkatziou.wificonnectivity;

import android.app.Activity;
import android.database.Cursor;
import android.net.Uri;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import com.gkatziou.wificonnectivity.provider.MyNodes;

public class Device extends Activity{

	private TextView mydev;
	private String dev_id;
	private String ipv4_addr;
	private DataSend dataSend;
	
	Button sendData;
	
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.device);  
                
        dev_id=getIntent().getExtras().getString("position");
        
        mydev = (TextView) findViewById(R.id.dev);
        mydev.setText(dev_id);

        String[] columns = new String[]{MyNodes.Node._ID,MyNodes.Node.ipv4_addr};
        Uri myUri = MyNodes.Node.CONTENT_URI;
        
        Cursor cur = managedQuery(myUri, columns,"_id"+"="+dev_id ,null ,null); 
        
        cur.getColumnCount();
        
        cur.moveToFirst();
        Log.v("TEST",cur.getColumnName(0));
        Log.v("TEST",Integer.toString(cur.getCount()));
        //Log.v("TEST",Integercur.getColumnCount());
        
        
        ipv4_addr = cur.getString(1);
        Log.v("TEST",ipv4_addr.replace("/",""));
        
        
        sendData = (Button)findViewById(R.id.button1);
        sendData.setOnClickListener(sendlisten);
        
	}
    
    
    private OnClickListener sendlisten = new OnClickListener() {
		
		@Override
		public void onClick(View v) {
			Toast.makeText(getApplicationContext(), "sad", Toast.LENGTH_LONG).show();
			
			dataSend = new DataSend(ipv4_addr);
			dataSend.start();
		}
	};
}
