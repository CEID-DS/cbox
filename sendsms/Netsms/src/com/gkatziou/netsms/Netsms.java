package com.gkatziou.netsms;

import java.io.*;
import java.net.*;

import android.app.Activity;
import android.app.PendingIntent;
import android.os.Bundle;
import android.content.Intent;
import android.telephony.gsm.SmsManager;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;
import java.net.InetAddress;
import android.util.Log;

@SuppressWarnings("deprecation")
public class Netsms extends Activity {

	private static final String TAG="UDP";
	
	Button servbut;
	
	/** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);  
        setContentView(R.layout.main);

        servbut = (Button)findViewById(R.id.servbut);

        servbut.setOnClickListener(new View.OnClickListener() {
        	
        	public void onClick(View v){

        		//String ip=phone.getText().toString();      	
        		
        		try {

        		      BufferedReader inFromUser =
        		          new BufferedReader(new InputStreamReader(System.in));
        		       DatagramSocket clientSocket = new DatagramSocket();
        		       InetAddress IPAddress = InetAddress.getByName("192.168.1.255");
        		       byte[] sendData = new byte[1024];
        		       byte[] receiveData = new byte[1024];
        		       String sentence = "Sms";
        		       sendData = sentence.getBytes();
        		       DatagramPacket sendPacket = new DatagramPacket(sendData, sendData.length, IPAddress, 9876);
        		       clientSocket.send(sendPacket);
        		       DatagramPacket receivePacket = new DatagramPacket(receiveData, receiveData.length);
        		       clientSocket.receive(receivePacket);
        		       String phonenum = new String(receivePacket.getData());
        		       receiveData = new byte[1024];
        		       receivePacket = new DatagramPacket(receiveData, receiveData.length);
        		       clientSocket.receive(receivePacket);
        		       String sms = new String(receivePacket.getData());
        		       
        		    
        		       clientSocket.close();
        		       
        		       //String temp[];
        		       //temp=modifiedSentence.split("\\n");
        		       
        			        			
        		       Toast.makeText(getBaseContext(),phonenum, Toast.LENGTH_SHORT).show();
        		       Toast.makeText(getBaseContext(),sms, Toast.LENGTH_SHORT).show();
        		       sendSMS(phonenum, sms);
					return;
        		} catch (SocketException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
        		
        		
        		
        	}
        	
        });
        
        /*sendsms.setOnClickListener(new View.OnClickListener() {
			
			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				
				String itsphone= phone.getText().toString();
				String itsmessage = message.getText().toString();
		
				sendSMS(itsphone, itsmessage);
				
				Toast.makeText(getBaseContext(),"The number "+ 
						itsphone+" \n"+"The message "+itsmessage+"\n",Toast.LENGTH_SHORT).show();
			}
		});*/
    }

    public void sendSMS(String phoneNumber,String message){
    	PendingIntent pi = PendingIntent.getActivity(this, 0,
            new Intent(this, Netsms.class), 0);                
            SmsManager sms = SmsManager.getDefault();
            sms.sendTextMessage(phoneNumber, null, message, pi, null); 
    }
}
