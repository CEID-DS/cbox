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

import com.gkatziou.wificonnectivity.provider.MyNodes;

import android.app.Activity;
import android.app.ListActivity;
import android.content.Intent;
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
		super.onListItemClick(l, v, position, id);
		Intent dev_id = new Intent(Devices.this,Device.class);
		dev_id.putExtra("position",String.valueOf(getListView().getItemIdAtPosition(position)));
		Log.v("TEST",String.valueOf(getListView().getItemIdAtPosition(position)));
		startActivity(dev_id);
		
	}
}
