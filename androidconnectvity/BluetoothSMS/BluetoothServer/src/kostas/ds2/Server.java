package kostas.ds2;

import java.io.IOException;
import java.io.OutputStream;

import javax.bluetooth.DiscoveryAgent;
import javax.bluetooth.LocalDevice;
import javax.bluetooth.UUID;
import javax.microedition.io.Connector;
import javax.microedition.io.StreamConnection;
import javax.microedition.io.StreamConnectionNotifier;

public class Server implements Runnable{
	
	LocalDevice local = null;
	
	StreamConnectionNotifier notifier;
	StreamConnection connection;
	
	public Server(){
		
	}
	
	public void connect(){
		
		// retrieve the local Bluetooth device object
		local = null;		
		connection = null;
		
		// setup the server to listen for connection
		try {
			local = LocalDevice.getLocalDevice();
			local.setDiscoverable(DiscoveryAgent.GIAC);
			
			UUID uuid = new UUID("04c6093b00001000800000805f9b34fb", false);
			//UUID uuid = new UUID("04c6093b-0000-1000-8000-00805f9b34fb", false);
			System.out.println(uuid.toString());
			
            String url = "btspp://localhost:" + uuid.toString() + ";name=RemoteBluetooth";
            notifier = (StreamConnectionNotifier)Connector.open(url);
		} catch (Exception e) {
            e.printStackTrace();
            return;
        }
		
		// waiting for connection
		while(true) {
			try {
				System.out.println("waiting for connection...");
	            connection = notifier.acceptAndOpen();
	            	            
	            
			} catch (Exception e) {
				e.printStackTrace();
				//return;
			}
		}
		
		
	}
	
	public void send(String phone,String message){
		
		try {
			OutputStream outStream = connection.openOutputStream();
			outStream.write(phone.getBytes());
			outStream.write(message.getBytes());
			
			
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		
	}

	@Override
	public void run() {
		// TODO Auto-generated method stub
		connect();
		
	}

}
