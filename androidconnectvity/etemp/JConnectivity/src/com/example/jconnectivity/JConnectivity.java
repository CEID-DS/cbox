package com.example.jconnectivity;

import java.awt.event.*;

import javax.swing.*;

public class JConnectivity extends JFrame{

	private static JPanel mainPanel=null;
	private static JMenuBar menu= null;
	private static JMenu file,scanning,visible=null;
	private JMenuItem exit,scanwifi,scanbluetooth,visiblewifi,visiblebluetooth=null;
	private Thread netListener = null;
	
	//private NetListener netListener = new NetListener();
	
	public static void main(String[] args) throws Exception{
		JConnectivity myConnectivity = new JConnectivity();
		myConnectivity.Initialize();
		myConnectivity.setVisible(true);
	}
	
	private void Initialize(){
		mainPanel=new JPanel();
		getContentPane().add(mainPanel);
		mainPanel.setLayout(null);
		menu=new JMenuBar();
		
		file = new JMenu("File");
		file.setMnemonic(KeyEvent.VK_F);
		
		exit = new JMenuItem("Exit");
		exit.addActionListener(new ActionListener() {
			
			@Override
			public void actionPerformed(ActionEvent e) {
				System.exit(0);
			}
		});
		
		scanning=new JMenu("Scanning");
		scanning.setMnemonic(KeyEvent.VK_F);
		
		scanwifi = new JMenuItem("ScanWifi");
		scanwifi.addActionListener(new ActionListener() {
			
			@Override
			public void actionPerformed(ActionEvent e) {
				
				if(scanwifi.getText().equals("ScanWifi")){
					scanwifi.setText("Stop wifi Scan");
					netListener = new Thread(new NetListener());
					netListener.start();
				}
				else if(scanwifi.getText().equals("Stop wifi Scan")){
					scanwifi.setText("ScanWifi");
					netListener.stop();
					netListener=null;
				}
			}
		});
		
		scanbluetooth = new JMenuItem("ScanBluetooth");
		scanbluetooth.addActionListener(new ActionListener() {
			
			@Override
			public void actionPerformed(ActionEvent e) {
				if(scanbluetooth.getText().equals("ScanBluetooth"))
					scanbluetooth.setText("Stop bluetooth scan");
				else if(scanbluetooth.getText().equals("Stop bluetooth scan"))
					scanbluetooth.setText("ScanBluetooth");
			}
		});
		
		visible = new JMenu("Visible");
		visible.setMnemonic(KeyEvent.VK_F);
		
		visiblewifi = new JMenuItem("Visble Wifi");
		visiblewifi.addActionListener(new ActionListener() {
			
			@Override
			public void actionPerformed(ActionEvent e) {
				if(visiblewifi.getText().equals("Visble Wifi"))
					visiblewifi.setText("Invisible wifi");
				else if(visiblewifi.getText().equals("Invisible wifi"))
					visiblewifi.setText("Visble Wifi");
			}
		});
		
		visiblebluetooth = new JMenuItem("Visible Bluetooth");
		visiblebluetooth.addActionListener(new ActionListener() {
			
			@Override
			public void actionPerformed(ActionEvent e) {
				if(visiblebluetooth.getText().equals("Visible Bluetooth"))
					visiblebluetooth.setText("Invisible bluetooth");
				else if(visiblebluetooth.getText().equals("Invisible bluetooth"))
					visiblebluetooth.setText("Visible Bluetooth");
			}
		});
		
		file.add(exit);
		menu.add(file);
		scanning.add(scanwifi);
		scanning.add(scanbluetooth);
		menu.add(scanning);
		visible.add(visiblewifi);
		visible.add(visiblebluetooth);
		menu.add(visible);
		setJMenuBar(menu);
		setTitle("Connectivity");
		setSize(500, 400);
		setDefaultCloseOperation(EXIT_ON_CLOSE);
	}
	
}
