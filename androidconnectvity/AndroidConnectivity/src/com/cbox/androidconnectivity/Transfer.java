package com.cbox.androidconnectivity;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.InetAddress;
import java.net.ServerSocket;
import java.net.Socket;

import android.os.Environment;
import android.os.Message;
import android.util.Log;

public class Transfer implements Runnable{

	private InetAddress myip ;
	private Socket connection; 
	private ServerSocket server;
	private String received,tosent,data,splits,path = null;
	
	public Transfer(ServerSocket serverSocket,String ip) {
		server = serverSocket;
		//myip = ip;
	}
	
	public void run() {
		// TODO Auto-generated method stub
		try {
		
			File sdCard = Environment.getExternalStorageDirectory();
			sdCard.getPath();
			
			
			byte[] message = new byte[1024];
			
			connection = server.accept();

			InputStream inputStream = connection.getInputStream();
			
			Log.v("TEST","got connection");
			
			inputStream.read(message,0,message.length);
			
			received = new String(message);
			
			received=new String(message);
			
			String[] buf = received.split("\\r?\\n");
			
			tosent = buf[0].replace("<tosent>","").replace("</tosent>","");
			data = buf[1].replace("<data>","").replace("</data>","");
			splits = buf[2].replace("<splits>","").replace("</splits>","");
			
			path = sdCard.getPath()+"/cBox/"+tosent;
			
			FileOutputStream fos = new FileOutputStream(path);
			
			for(int i=0;i<Integer.parseInt(splits)-1;i++){
			    inputStream.read(message,0,message.length);
			    fos.write(message,0,message.length);
			    
			    Log.v("TEST",Integer.toString(i));
			}
			
			inputStream.read(message,0,message.length);
			fos.write(message,0,(Integer.parseInt(splits)*1024-1)-Integer.parseInt(data));
			
			connection.close();
			
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

}
