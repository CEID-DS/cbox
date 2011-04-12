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

package com.gkatziou.Pcsms;

import java.io.*;
import java.net.*;
import java.awt.event.*;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.JTextArea;
import javax.swing.SwingWorker;

public class Sendsms extends JFrame{

	private static JFrame maiFrame = null;
	private static JTextArea phonetext = null;
	private static JTextArea smstext = null;
	private static JButton sendbutton = null;
	
	public static void main(String args[]) throws Exception{
				
		Sendsms MySms = new Sendsms();
		MySms.Initialize();
		MySms.setVisible(true);
		
	}
	private void Initialize(){
	
		JPanel panel = new JPanel();
		getContentPane().add(panel);
		panel.setLayout(null);
		
		JButton sendButton = new JButton("toSend");
		sendButton.setBounds(10,200,80,30);
		sendButton.addActionListener(new ActionListener(){
			
			@Override
			public void actionPerformed(ActionEvent e) {
				byte[] receiveData = new byte[1024];
			    byte[] sendData = new byte[1024];
			    String sentence = ""; 
			    
				try {
					 DatagramSocket serverSocket = new DatagramSocket(9876); 
		             DatagramPacket receivePacket = new DatagramPacket(receiveData, receiveData.length);
		             serverSocket.receive(receivePacket);
		             sentence = new String( receivePacket.getData());
		             System.out.println("RECEIVED: " + sentence);
		             InetAddress IPAddress = receivePacket.getAddress();
		             int port = receivePacket.getPort();
		             sendData = phonetext.getText().getBytes();
		             System.out.println(phonetext.getText());		        
						
		             DatagramPacket sendPacket =
		             new DatagramPacket(sendData, sendData.length, IPAddress, port);
		             serverSocket.send(sendPacket);
		             
		             sendData = smstext.getText().getBytes();
		            
		             System.out.println(smstext.getText());		        
						
		             sendPacket =new DatagramPacket(sendData, sendData.length, IPAddress, port);
		             
		             serverSocket.send(sendPacket);
		             
		             serverSocket.close();
				} catch (SocketException e1) {
					// TODO Auto-generated catch block
					e1.printStackTrace();
				} catch (UnknownHostException e1) {
					// TODO Auto-generated catch block
					e1.printStackTrace();
				} catch (IOException e1) {
					// TODO Auto-generated catch block
					e1.printStackTrace();
				}

	        }
		});
		
		JButton recvButton = new JButton("Recv");
		recvButton.setBounds(100,200,70,30);
		recvButton.addActionListener(new ActionListener() {
			
			@Override
			public void actionPerformed(ActionEvent e) {
				// TODO Auto-generated method stub
				/*PackWork hate = new PackWork();
				try{
					hate.execute();
				}catch (Exception e1){
					e1.printStackTrace();
				}*/
				
			}
		});
		
		//panel.add(recvButton);
		panel.add(sendButton);
		
		phonetext = new JTextArea();
		phonetext.setText("Give the mobile number");
		phonetext.setBounds(10,10,200,30);
		panel.add(phonetext);
		
		smstext = new JTextArea();
		smstext.setText("Give the message");
		smstext.setBounds(10,100,200,30);
		panel.add(smstext);
				
		
		setTitle("Send sms");
		setSize(400,300);
		setDefaultCloseOperation(EXIT_ON_CLOSE);
	}
	
	/*class PackWork extends SwingWorker{

		@Override
		protected Object doInBackground() throws Exception {
			// TODO Auto-generated method stub
		    
			System.out.println("lala");
	    

	        
		    
		    /*InetAddress IPAddress = InetAddress.getByName("192.168.1.255");
	        String sentence = "Sms";
	        
	        sendData = sentence.getBytes();
	        DatagramPacket sendPacket = new DatagramPacket(sendData, sendData.length, IPAddress, 9876);
	        clientSocket.send(sendPacket);
		    DatagramPacket receivePacket = new DatagramPacket(receiveData, receiveData.length);
		    serverSocket.receive(receivePacket);
		    String sentence = new String( receivePacket.getData());
		               
		    System.out.println("RECEIVED: " + sentence);
		    InetAddress IPAddress = receivePacket.getAddress();
		    int port = receivePacket.getPort();
		    String capitalizedSentence = phonetext.getText()+smstext.getText();
		    sendData = capitalizedSentence.getBytes();
		    DatagramPacket sendPacket =
		    new DatagramPacket(sendData, sendData.length, IPAddress, port);
		    serverSocket.send(sendPacket);
			serverSocket.close();
		    return sendPacket;	        	        
		}
		
	}*/
}
