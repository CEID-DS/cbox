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

import java.io.IOException;
import java.util.Vector;

import javax.bluetooth.DiscoveryListener;


import javax.bluetooth.BluetoothStateException;
import javax.bluetooth.DeviceClass;
import javax.bluetooth.DiscoveryAgent;
import javax.bluetooth.DiscoveryListener;
import javax.bluetooth.LocalDevice;
import javax.bluetooth.RemoteDevice;
import javax.bluetooth.ServiceRecord;

public class DeviceDiscovery implements DiscoveryListener,Runnable{
	
	public DeviceDiscovery(){
		
	}
	
	public void discover(){
		
		LocalDevice localDevice = null;
		try {
			localDevice = LocalDevice.getLocalDevice();
			
			System.out.println("Address: "+localDevice.getBluetoothAddress());
			System.out.println("Name: "+localDevice.getFriendlyName());
			//find devices
			DiscoveryAgent agent = localDevice.getDiscoveryAgent();
			System.out.println("Starting device inquiry...");
			
			agent.startInquiry(DiscoveryAgent.GIAC, this);
			
		} catch (BluetoothStateException e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
		}
		
		
		
		try {
			synchronized(lock){
				lock.wait();
			}
		}
		catch (InterruptedException e) {
			e.printStackTrace();
		}
		System.out.println("Device Inquiry Completed. ");
		//print all devices in vecDevices
		int deviceCount=vecDevices.size();
		if(deviceCount <= 0){
			System.out.println("No Devices Found .");
		}
		else{
			//print bluetooth device addresses and names in the format [ No. address (name) ]
			System.out.println("Bluetooth Devices: ");
			for (int i = 0; i <deviceCount; i++) {
				RemoteDevice remoteDevice=(RemoteDevice)vecDevices.elementAt(i);
				try {
					System.out.println((i+1)+". "+remoteDevice.getBluetoothAddress()+" ("+remoteDevice.getFriendlyName(true)+")");
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}
		}
		
		
	}
	
	//object used for waiting
	private static Object lock=new Object();
	//vector containing the devices discovered
	private static Vector vecDevices=new Vector();
	//main method of the application
	
	
	/**
	 * This call back method will be called for each discovered bluetooth devices.
	 */
	public void deviceDiscovered(RemoteDevice btDevice, DeviceClass cod) {
		System.out.println("Device discovered: "+btDevice.getBluetoothAddress());
		//add the device to the vector
		if(!vecDevices.contains(btDevice)){
			vecDevices.addElement(btDevice);
		}
	}
	//no need to implement this method since services are not being discovered
	public void servicesDiscovered(int transID, ServiceRecord[] servRecord) {
	}
	//no need to implement this method since services are not being discovered
	public void serviceSearchCompleted(int transID, int respCode) {
	}
	/**
	 * This callback method will be called when the device discovery is
	 * completed.
	 */
	public void inquiryCompleted(int discType) {
		synchronized(lock){
			lock.notify();
		}
		switch (discType) {
			case DiscoveryListener.INQUIRY_COMPLETED :
				System.out.println("INQUIRY_COMPLETED");
				break;
			case DiscoveryListener.INQUIRY_TERMINATED :
				System.out.println("INQUIRY_TERMINATED");
				break;
			case DiscoveryListener.INQUIRY_ERROR :
				System.out.println("INQUIRY_ERROR");
				break;
			default :
				System.out.println("Unknown Response Code");
				break;
		}
	}//end method

	@Override
	public void run() {
		// TODO Auto-generated method stub
		discover();
		
	}

	
}
