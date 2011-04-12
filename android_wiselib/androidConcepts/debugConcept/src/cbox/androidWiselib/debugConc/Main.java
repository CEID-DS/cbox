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

/*This class tests the debug concept of wiselib in the android environment.
 * Method public void testNative(String s) has to be included in every object that needs
 * the debug feature enabled.
 */

package cbox.androidWiselib.debugConc;

import java.lang.Object;
import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.view.View.OnClickListener;
import android.os.Bundle;
import android.widget.Button;
import android.widget.TextView;


public class Main extends Activity {
	
	//loading the class that handles native methods
	NativeConnection nc = new NativeConnection();
	
	//global variables
	Menu menu;
	Button showTimeButton;	//when pressed calls the native methods
	TextView defaultText;	//the default text which never changes
	TextView retrievedText;	//the dynamic text which is retrieved by C++
	CharSequence outputString = "Press menu to select the concept"; //the default hello message
	String FLAG = "DEBUG";	//flag that is used in button's listener
	Object passing=this;	//variable that holds the object instance to pass it in native code


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
				if(FLAG.equalsIgnoreCase("DEBUG"))
				{
					//calling a native method that tests the debug concept
					//passing an object argument is mandatory since the native class
					//is declared in different class
					nc.debugConceptJNI(passing);
				}
				else retrievedText.setText("Wrong command\n");
			}
        });



    }
    
	/* when application enters pause mode reset the text */
    public void onPause(Bundle savedInstanceState) {
    	//retrievedText.setText("");
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
        case R.id.androidText:	//testing debug feature
        	showTimeButton.setText("Test debug concept");
        	FLAG="DEBUG";
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
    
    /**
     * Simple method that is called through JNI from native method testDebug()
     */
    public void testNative(String s)
    {
    	
    	// prepare the alert box
        AlertDialog.Builder alertbox = new AlertDialog.Builder(this);
        // set the message to display
        alertbox.setMessage(s);
        // add a neutral button to the alert box and assign a click listener
        alertbox.setNeutralButton("Ok", new DialogInterface.OnClickListener() {
            // click listener on the alert box
            public void onClick(DialogInterface arg0, int arg1) {
                // the button was clicked
                //Toast.makeText(getApplicationContext(), "OK button clicked", Toast.LENGTH_LONG).show();
            }
        });
        // show it
        alertbox.show();



    }
    

    


 

}
