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

package com.cbox.androidconnectivity;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.OutputStream;
import java.net.Socket;
import java.net.UnknownHostException;

import android.os.Environment;
import android.util.Log;

public class Transmit implements Runnable{

	private Socket connection;
	private String myip;
	private String myfile;
	private int pieces;
	
	public Transmit(String ip,String file) {
		myip = ip;
		myfile = Environment.getExternalStorageDirectory().getPath()+"/cBox/"+file;
	}
	
	@Override
	public void run() {
		Log.v("TEST","Running");
		
		byte[] message = new byte[1024];
		String mymes=null;
		
		File file = new File(myfile);
		
		if(file.length()>=1024){
			pieces = (int)(file.length()/1024);
			if(file.length()%1024!=0){
				pieces++;
			}
		}else{
			pieces=1;
		}
		
		mymes = "<tosent>"+file.getName()+"</tosent>"+"\n"
				+"<data>"+String.valueOf(file.length())+"</data>"+"\n"
				+"<splits>"+Integer.toString(pieces)+"</splits>"+"\n";
			
		Log.v("TEST","host "+myip);
		
		try {
			FileInputStream fis = new FileInputStream(myfile);
			connection = new Socket(myip,9876);	
			
			OutputStream outputStream = connection.getOutputStream();
		
			message = mymes.getBytes();
			
			outputStream.write(message,0,message.length);
			
			int a=0;
			
			for(int i=0;i<pieces-1;i++){
				a=fis.read(message,0,message.length);
				outputStream.write(message,0,message.length);
				
				Log.v("TEST",Integer.toString(i));
			}
			//a=(int) (file.length()-1024*(pieces-1));
			fis.read(message,0,a);
			outputStream.write(message,0,message.length);
			
			connection.close();
		} catch (FileNotFoundException e) {
			// TODO Auto-generated catch block
			Log.v("TEST","file prob");
			e.printStackTrace();
		} catch (UnknownHostException e) {
			Log.v("TEST","host prob");
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IOException e) {
			Log.v("TEST","io prob");
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	
}
