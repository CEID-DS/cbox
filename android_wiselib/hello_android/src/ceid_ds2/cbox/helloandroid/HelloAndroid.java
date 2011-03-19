package ceid_ds2.cbox.helloandroid;

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
import android.widget.EditText;
import android.widget.TextView;

/** Created an activity that performs a classic "Hello World" */
public class HelloAndroid extends Activity {
	
	//global variables
	Button showTimeButton;	//when pressed shows the text(retrieved by C++)
	TextView defaultText;	//the default text which never changes
	TextView retrievedText;	//the dynamic text which is retrieved by C++
	CharSequence outputString = "Hello World";


    /** Called when the activity is first created. */
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
				retrievedText.append("\t\t\t" + outputString + "\n");
			}
        });
        


    }
    
	/** when application enters pause mode reset the text */
    public void onPause(Bundle savedInstanceState) {
    	retrievedText.setText("");
    }
    
    /** this method that created a menu according to res/menu/menu.xml file */
    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        MenuInflater inflater = getMenuInflater();
        inflater.inflate(R.menu.menu, menu);
        return true;
    }
    /** this method handles the user input on menu */
    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle item selection
        switch (item.getItemId()) {
        case R.id.newText: //getting a new text from user input
            getNewText();
            return true;
        case R.id.exit:
            exitApp();
            return true;
        default:
            return super.onOptionsItemSelected(item);
        }
    }
    /** this method sets the string that will appear (default "Hello World") */
    /*public void setOutputString(String s)
    {
    	this.outputString=s;
    }*/
    
    /** this method creates a dialog for user input in order to get a new string to appear
     *  every time the user presses ShowTime
     */
    public void getNewText()
    {
    	//creating an alertDialog
    	AlertDialog.Builder alert = new AlertDialog.Builder(this);

    	//setting the asking title and message that will appear
    	alert.setTitle("New Text");
    	alert.setMessage("Enter a new text to appear");

    	// Set an EditText view to get user input 
    	final EditText input = new EditText(this);
    	alert.setView(input);

    	//registering the dialog to an event listener 
    	alert.setPositiveButton("Ok", new DialogInterface.OnClickListener() {
    	//implementing the event handler
    	public void onClick(DialogInterface dialog, int whichButton) {
    	  outputString =  input.getText();
    	  	
    	  }
    	});
    	
    	alert.setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
    	  public void onClick(DialogInterface dialog, int whichButton) {
    	    // Canceled.
    	  }
    	});

    	alert.show();
    	
    }
    /** This method that is closes the application */
    public void exitApp()
    {
    	finish();
    }

 

}