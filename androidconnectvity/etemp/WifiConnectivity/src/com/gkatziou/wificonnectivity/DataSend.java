package com.gkatziou.wificonnectivity;

import android.os.Environment;
import android.util.Log;
import java.io.File;
import java.io.FileWriter;
import java.io.InputStream;
import java.io.OutputStream;

public class DataSend extends Thread{

	private String ipto;
	
	public DataSend(String ip){
		ipto = ip;
	}
	
	public void run(){
		sender();
	}
	
	private void sender(){
		Log.v("TEST","send thread");
		
		File sdCard = Environment.getExternalStorageDirectory();
		sdCard.getPath();
		String directory = sdCard.getPath()+"cBox";
		
		File cBoxdir = new File(directory);
		
		if(!cBoxdir.exists()){
			cBoxdir.mkdirs();
		}else{
			
		}
		
		//Log.v("TEST",sdCard.getPath());
		
	}
}
