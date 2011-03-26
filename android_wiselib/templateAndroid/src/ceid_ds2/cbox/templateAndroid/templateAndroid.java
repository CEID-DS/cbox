/* This program has 2 features.
 * 1.It can run a (mathematical) benchmark using either SDK code (running in delvik VM)
 * or in native code. If you test it you will notice that NDK is .. than SDK.
 * 2.You can test native code C++ templates.
 */


package ceid_ds2.cbox.templateAndroid;

import android.app.Activity;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.view.View.OnClickListener;
import android.os.Bundle;
import android.widget.Button;
//import android.widget.EditText;
import android.widget.TextView;

/* Created an activity that performs the features */
public class templateAndroid extends Activity {
	
	//loading the class that handles native methods
	CalcNativeClass nc = new CalcNativeClass();
	
	//global variables
	Menu menu;
	Button showTimeButton;	//when pressed shows the text(retrieved by C++)
	TextView defaultText;	//the default text which never changes
	TextView retrievedText;	//the dynamic text which is retrieved by C++
	String FLAG = "SDK";	//flag that is used in button's listener
	long time;	//used to measure the elapsed time of the benchmark


    /* Called when the activity is first created. */
	@Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        //sets the layout according to res/layout/main.xml file
        setContentView(R.layout.main);

        //retrieving the values from menu.xml layout
        showTimeButton = (Button) findViewById(R.id.ShowTime);	
        defaultText = (TextView) findViewById(R.id.DefaultText);
        retrievedText = (TextView) findViewById(R.id.RetrievedText);       

        //registering an event listener
        showTimeButton.setOnClickListener( new OnClickListener() {
			public void onClick(View v) {
				if(FLAG.equals("SDK"))
				{
					retrievedText.setText("\nCalculation by SDK\n");
					time = System.currentTimeMillis(); //getting current time
					String s=Double.toString(doCalculation()); //calling templateAndroid method
					time = System.currentTimeMillis() - time; //retrieving the elapsed time
					retrievedText.append(s + "\n" + time);
				}
				else if(FLAG.equals("NDK"))
				{
					retrievedText.setText("\nCalculation by NDK\n");
					time = System.currentTimeMillis(); //getting current time
					String s=Double.toString(nc.calcsFromJNI());
					time = System.currentTimeMillis() - time; //retrieving the elapsed time
					retrievedText.append(s + "\n" + time);
				}
				else if(FLAG.equals("TEMPLATES"))
				{
					double templateRes=nc.playWithTemplateJNI(1, 2, 3, 4); //calling native method
					retrievedText.setText("\nResult from NDK: " + templateRes + "\n"); //printing the result
				}
				else retrievedText.setText("Wrong command\n");
			}
        });



    }
    
	/* when application enters pause mode reset the text */
    public void onPause(Bundle savedInstanceState) {
    	retrievedText.setText("");
    }
    
    /** this method that created a menu according to res/menu/menu.xml file */
    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
    	this.menu=menu;
        MenuInflater inflater = getMenuInflater();
        inflater.inflate(R.menu.menu, menu);
        return true;
    }
    /* this method handles the user input on menu */
    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle item selection
        switch (item.getItemId()) {  
        case R.id.androidText:	//benchmark using android SDK
        	showTimeButton.setText("Do calculations using SDK");
        	//outputString="Hello World";
        	FLAG="SDK";
        	return true;
        case R.id.nativeText:	//benchmark using android NDK
        	showTimeButton.setText("Do calculations using NDK");
        	//outputString=nc.stringFromJNI();
        	FLAG="NDK";
        	return true;
        case R.id.doubleStack: //interfering with c++ templates in native code
        	showTimeButton.setText("Interfere with templates");
        	FLAG="TEMPLATES";
        	return true;
        case R.id.exit:		//exit the application
            exitApp();
            return true;
        default:
            return super.onOptionsItemSelected(item);
        }
    }

    
    /* This method  kills the application */
    public void exitApp()
    {
    	finish();
    }
    /* this method is a mathematical benchmarks
     * It does numerous calculations so that to
     * slow down the system more than a second.
     */
    public double doCalculation()
    {
    	double x = 45.0 / (180.0 / Math.PI); 
    	double result = 0.5; 
    	for (int i = 0; i < 100000; i++)
    	{
    		for(int j=0; j< 100; j++)
    		{
	    		result += Math.pow(Math.cos(x), 2.0); 
	    		result -= Math.pow(Math.sin(x), 2.0); 
	    		result = Math.log(result); 
	    		result = Math.exp(result);
    		}
    	} 
    	return result; 
    }


    


 

}