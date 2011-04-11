package com.gkatziou.wificonnectivity;

import com.gkatziou.wificonnectivity.provider.MyNodes;

import android.app.Activity;
import android.app.ListActivity;
import android.database.Cursor;
import android.net.Uri;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.ListView;
import android.widget.SimpleCursorAdapter;

public class Devices extends ListActivity{
	public void onCreate(Bundle savedInstanceState){
		super.onCreate(savedInstanceState);
		setContentView(R.layout.devices);
	}
	
	public void onResume(){
		super.onResume();
		fillData();
	}
	
	private void fillData(){
		String[] columns = new String[]{MyNodes.Node._ID,MyNodes.Node.ipv4_addr};
		int[] to = new int[] {R.id.id_entry,R.id.ipv4_entry};
		Uri myUri = MyNodes.Node.CONTENT_URI;
		Cursor cur = managedQuery(myUri, columns, null, null, null);
		
		SimpleCursorAdapter mAdapter = new SimpleCursorAdapter(this,R.layout.list_devices,cur,columns,to);
		
		this.setListAdapter(mAdapter);
	}
	
	protected void onListItemClick(ListView l,View v,int position,long id){
		
	}
}
