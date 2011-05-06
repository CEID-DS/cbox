package com.example.jconnectivity;

import java.io.FileOutputStream;
import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.SocketException;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class GetData extends Thread{
	
	Thread listen = null;
	private String tosent;
	private String data;
	private String splits;
	
	
	public GetData(){
	}
	
	public void run(){
		datareceive();
	}
	
	private void datareceive(){
		
		try {
			DatagramSocket receiverSocket = new DatagramSocket(9876);
			byte[] receiveData = new byte[1024];
			byte[] sendData = new byte[1024];
		
			while(true){
				DatagramPacket receivePacket = new DatagramPacket(receiveData, receiveData.length);
				System.out.println("to receive");
				receiverSocket.receive(receivePacket);
				DatagramPacket sendPacket = new DatagramPacket(sendData,sendData.length,receivePacket.getAddress(),receivePacket.getPort());
				String sentence = new String(receivePacket.getData());
				//System.out.println("Received "+sentence);
				
				String[] buf = sentence.split("\\r?\\n");
				
				tosent = buf[0].replace("<tosent>","").replace("</tosent>","");
				data = buf[1].replace("<data>","").replace("</data>","");
				splits = buf[2].replace("<splits>","").replace("</splits>","");
				
				//System.out.println(tosent+"\n"+data+"\n"+splits);
				
				int a = 0;
				
				Pattern p = Pattern.compile("-?\\d+");
				Matcher m = p.matcher(splits);
				while (m.find()) {
				  //System.out.println(m.group());
				  a = Integer.parseInt(m.group());
				}				
				
				int left = Integer.parseInt(data)-(a-1)*1024;
				
				
				FileOutputStream fos = new FileOutputStream(tosent);
				
				for(int i=0;i<a-1;i++){
					System.out.println("Waiting");
					receiverSocket.receive(receivePacket);
					fos.write(receivePacket.getData(),0,receivePacket.getData().length);
					receiverSocket.send(sendPacket);
				}
				
				byte[] leftdata = new byte[left];
				DatagramPacket leftPacket = new DatagramPacket(leftdata, leftdata.length);
				receiverSocket.receive(leftPacket);
				receiverSocket.send(sendPacket);
				fos.write(leftPacket.getData(),0,left);
				
				System.out.println("Done");
				fos.flush();
				fos.close();
			}
				
				
		} catch (SocketException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
			
	}
		
}
	

 