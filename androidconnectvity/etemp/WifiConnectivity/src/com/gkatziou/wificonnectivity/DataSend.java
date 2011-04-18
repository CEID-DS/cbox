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

import android.os.Environment;
import android.util.Log;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.Inet4Address;
import java.net.InetAddress;
import java.net.Socket;
import java.net.SocketException;
import java.net.UnknownHostException;

public class DataSend extends Thread{

	private String ipto;
	private String data;
	
	public DataSend(String ip,String item){
		ipto = ip;
		data = item;
	}
	
	public void run(){
		sender();
	}
	
	private void sender(){
		Log.v("TEST","send thread");
		
		int pieces;
		
		File sdCard = Environment.getExternalStorageDirectory();
		sdCard.getPath();
		String path = sdCard.getPath()+"/cBox/"+data;
		
		File file = new File(path);
		
		Log.v("TEST",String.valueOf(file.length()));
		
		if(file.length()>=1024){
            pieces = (int)(file.length()/1024);
            if (file.length()%1024!=0){
                pieces++;
                Log.v("TEST","Extra piece");
            }
            else
                Log.v("TEST","Just pieces");
        }
        else{
            pieces = 1;
                Log.v("TEST","Only one piece");
        }
		
		String mymess = null;
		byte[] message = new byte[1024];
		byte[] mybytes = new byte[1024];
		
		try {
			
			Socket connectionSocket = new Socket(ipto,9876);
			
			OutputStream outputStream = connectionSocket.getOutputStream();
			InputStream inputStream = connectionSocket.getInputStream();
			
			mymess = "<tosent>"+data+"</tosent>"+"\n"
            +"<data>"+String.valueOf(file.length())+"<data>"+"\n"
            +"<splits>"+Integer.toString(pieces)+"</splits>"+"\n";
			
			message = mymess.getBytes();
	
			
			outputStream.write(message,0,message.length);
			Log.v("TEST","Send message");
			
			FileInputStream fis = new FileInputStream(file);
			int a = 0;
			
			for(int i=0;i<pieces;i++){
				a = fis.read(mybytes,0,mybytes.length);
				outputStream.write(mybytes,0,mybytes.length);
			}
			
			 
			
		} catch (UnknownHostException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
			Log.v("TEST","not a host");
			
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
			Log.v("TEST","io");
		}
		
		
		
		/*Log.v("TEST",Integer.toString(pieces));
			
			byte[] sendData = new byte[1024];
			byte[] receiveData = new byte[1024];
			     
			String datainfo = "<tosent>"+"test.mp3"+"</tosent>\n"
			+"<data>"+String.valueOf(testdata.length())+"</data>\n"
			+"<splits>"+pieces+"</splits>";
			
			sendData = datainfo.getBytes();
			
			try {
				DatagramSocket sendSocket = new DatagramSocket();
				InetAddress IPAddress = InetAddress.getByName(ipto.replace("/",""));
				DatagramPacket sendPacket = new DatagramPacket(sendData, sendData.length,IPAddress,9876);
				DatagramPacket receivePacket = new DatagramPacket(receiveData,receiveData.length);
				sendSocket.send(sendPacket);
				//sendSocket.close();
		
				FileInputStream fis = new FileInputStream(directory+"/test.mp3");
				
				int left=(int) (testdata.length()-(pieces-1)*1024);
				
				Log.v("TEST",Integer.toString(left));
				Log.v("TEST","length "+Integer.toString(sendData.length));
				
				for(int i=0;i<pieces-1;i++){
				
					fis.read(sendData,0,sendData.length);
					
					sendPacket = new DatagramPacket(sendData, sendData.length,IPAddress,9876);
					sendSocket.send(sendPacket);
					sendSocket.receive(receivePacket);
				}

				
				byte[] leftdata = new byte[left];
				fis.read(leftdata,0,left);
				DatagramPacket leftPacket = new DatagramPacket(leftdata, leftdata.length);
				sendSocket.send(sendPacket);
				sendSocket.receive(receivePacket);
				fis.close();
				Log.v("TEST",Integer.toString(pieces));
				
			} catch (UnknownHostException e) {
				Log.v("TEST","no host");
				// TODO Auto-generated catch block
				e.printStackTrace();
			} catch (SocketException e) {
				Log.v("TEST","no socket");
				// TODO Auto-generated catch block
				e.printStackTrace();
			} catch (IOException e) {
				Log.v("TEST","no io");
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			
		}*/
		
		//Log.v("TEST",sdCard.getPath());
		
	}
}
