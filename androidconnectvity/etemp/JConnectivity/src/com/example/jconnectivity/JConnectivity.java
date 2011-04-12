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


package com.example.jconnectivity;

import java.awt.event.*;

import javax.swing.*;

public class JConnectivity extends JFrame{

	private static JTextField devictext=null;
	private static JPanel mainPanel=null;
	private static JMenuBar menu= null;
	private static JMenu file,scanning,visible,discovered=null;
	private JMenuItem exit,scanwifi,scanbluetooth,visiblewifi,visiblebluetooth,showdiscovered=null;

	private NetListener netListener = new NetListener();
	private Visible mvisible = new Visible();
		
	public static void main(String[] args) throws Exception{
		JConnectivity myConnectivity = new JConnectivity();
		myConnectivity.Initialize();
		myConnectivity.setVisible(true);
	}
	
	private void Initialize(){
		mainPanel=new JPanel();
		getContentPane().add(mainPanel);
		mainPanel.setLayout(null);
		devictext = new JTextField();
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
					netListener.start();
				}
				else if(scanwifi.getText().equals("Stop wifi Scan")){
					scanwifi.setText("ScanWifi");
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
				if(visiblewifi.getText().equals("Visble Wifi")){
					visiblewifi.setText("Invisible wifi");
					mvisible.broadcast();
					}
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
		
		discovered = new JMenu("Discovered Devices");
		discovered.setMnemonic(KeyEvent.VK_F);
		
		showdiscovered = new JMenuItem("Show Discovered");
		showdiscovered.addActionListener(new ActionListener() {
			
			@Override
			public void actionPerformed(ActionEvent e) {
				if(showdiscovered.getText().equals("Show Discovered")){
					//devictext.setText("you have devices");
					//devictext. 
					System.out.println(netListener.myaddrs.size());
					showdiscovered.setText("Hide Discovered");}
				else if(showdiscovered.getText().equals("Hide Discovered")){
					showdiscovered.setText("Show Discovered");}
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
		discovered.add(showdiscovered);
		menu.add(discovered);
		setJMenuBar(menu);
		setTitle("Connectivity");
		setSize(500, 400);
		setDefaultCloseOperation(EXIT_ON_CLOSE);
	}
	
}

