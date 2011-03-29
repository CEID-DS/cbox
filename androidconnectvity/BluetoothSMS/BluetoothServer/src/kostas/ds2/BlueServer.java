package kostas.ds2;

import java.awt.BorderLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.JTextArea;
import javax.swing.JTextField;

public class BlueServer {

	/**
	 * @param args
	 */
	
		
	JButton send,run;
	JTextField phonenumber;
	JTextArea text;
	
	String phone,message;
	
	Thread serverThread;
	Server blue_server;
	
	public BlueServer(){
		blue_server = new Server();
		serverThread = new Thread(blue_server);
		createGUI();
	}
	
	public void createGUI(){
		
		JFrame frame = new JFrame("Bluetooth Server");
		JPanel mainPanel = new JPanel();
		mainPanel.setLayout(new BorderLayout(5,5));
		JPanel sub = new JPanel();
		sub.setLayout(new BorderLayout());
		
		run = new JButton("Run Server");
		send = new JButton("send");		
		
		phonenumber = new JTextField("Phone Number");		
		text = new JTextArea("Sms text goes here...");
		
		
		sub.add(phonenumber,BorderLayout.NORTH);
		sub.add(text,BorderLayout.CENTER);
		
		mainPanel.add(run,BorderLayout.PAGE_START);		
		mainPanel.add(send,BorderLayout.SOUTH);
		mainPanel.add(sub,BorderLayout.CENTER);
		
		
		frame.add(mainPanel);
		frame.setSize(300,200);
		frame.setVisible(true);
		
		run.addActionListener(new ActionListener() { 
			  public void actionPerformed(ActionEvent e) {
				  
				 				  
				 serverThread.start();
			    
			  } 
			} );
		
		send.addActionListener(new ActionListener() { 
			  public void actionPerformed(ActionEvent e) {
				  
				  phone = phonenumber.getText();
				  message = text.getText();
				  
				  blue_server.send(phone, message);
			    
			  } 
			} );
		
	}
	
	public static void main(String[] args) {
		// TODO Auto-generated method stub

		BlueServer server = new BlueServer();
	}

}
