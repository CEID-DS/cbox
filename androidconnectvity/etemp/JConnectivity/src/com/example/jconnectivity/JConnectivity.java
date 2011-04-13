package com.example.jconnectivity;

import java.awt.Dimension;
import java.awt.event.*;

import javax.swing.*;
import javax.swing.event.ListSelectionEvent;
import javax.swing.event.ListSelectionListener;

public class JConnectivity extends JFrame{

	private static JTextField devictext=null;
	private static JPanel mainPanel=null;
	private static JMenuBar menu= null;
	private static JMenu file,scanning,visible,discovered,transferdata=null;
	private JMenuItem exit,scanwifi,scanbluetooth,visiblewifi,visiblebluetooth,showdiscovered,getdata,sendData=null;
	private JList devlist=null;
	private static JButton blist =null;
	private DefaultListModel model = new DefaultListModel();
	private JScrollPane pane =null;
	
	private NetListener netListener = new NetListener();
	private Visible mvisible = new Visible();
	private GetData getData = new GetData();	
	private Addresses mybuf=null;
	
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
					//System.out.println(netListener.myaddrs.size());
					showdiscovered.setText("Hide Discovered");}
				else if(showdiscovered.getText().equals("Hide Discovered")){
					showdiscovered.setText("Show Discovered");}
			}
		});
		
		transferdata = new JMenu("Transfer data");
		transferdata.setMnemonic(KeyEvent.VK_F);
		
		sendData = new JMenuItem("Send Data");
		sendData.addActionListener(new ActionListener() {
			
			@Override
			public void actionPerformed(ActionEvent e) {

				for(int i=0;i<netListener.myaddrs.size();i++){
					//System.out.println(netListener.myaddrs.get(i).ipv4_addr);
					if(!model.contains(netListener.myaddrs.get(i).ipv4_addr)){
						model.addElement(netListener.myaddrs.get(i).ipv4_addr);
					}
					
				}
				devlist=new JList(model);
				pane = new JScrollPane();
				pane.getViewport().add(devlist);
				pane.setVisible(false);
			}
		});
		
		getdata = new JMenuItem("Get Data");
		getdata.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				//System.out.println(netListener.myaddrs.size());
				if(getdata.getText().equals("Get Data")){
					getdata.setText("End Transfer");
					getData.start();}
				else if(getdata.getText().equals("End Transfer")){
					getdata.setText("Get Data");
					getData.stop();}

			}
		});
		
		
		
		
		devlist=new JList(model);
		devlist.addListSelectionListener(listSelectionListener);
		
		pane = new JScrollPane();
		pane.getViewport().add(devlist);
		pane.setBounds(10,10,200,30);
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
		transferdata.add(sendData);
		transferdata.add(getdata);
		menu.add(transferdata);
		setJMenuBar(menu);
		mainPanel.add(pane);
		setTitle("Connectivity");
		setSize(500, 400);
		setDefaultCloseOperation(EXIT_ON_CLOSE);
	}
	
	ListSelectionListener listSelectionListener = new ListSelectionListener() {
		
		@Override
		public void valueChanged(ListSelectionEvent e) {
			// TODO Auto-generated method stub
			e.getValueIsAdjusting();
			JList list = (JList) e.getSource();
			boolean adjust=e.getValueIsAdjusting();
			if(!adjust){
				int selections[] = list.getSelectedIndices();
				Object selectionValues[] = list.getSelectedValues();
				list.getSelectedValue();
				System.out.println(list.getSelectedValue());
			}
		}
	};
}

