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
			
			
			
			dataSend = new DataSend(ipv4_addr.replace("/",""),"test.mp3");
			dataSend.start();
		}
	};
}
