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

package ceid.ds2.cBox.twitterProxy;

import java.util.List;

import twitter4j.Status;
import twitter4j.Twitter;
import twitter4j.TwitterFactory;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.ScrollView;
import android.widget.TextView;

public class cBoxTwitterProxy extends Activity {
	
	private static Twitter twitter = null;
	private static Twitter authenticatedTwitter = null;
	private static EditText updateStatusEditText = null;
	private static Button updateStatusButton = null;
	
	/**
	 * Create an EditText for user to insert the Twitter Update.
	 * Create a button, when it is pressed the update that the
	 * EditText contain is send to twitter.com.
	 */
	public void sendUpdateStatus()	{
		updateStatusEditText = (EditText) findViewById(R.id.updateStatusEditText);
    	updateStatusButton = (Button) findViewById(R.id.updateStatusButton);
    	updateStatusButton.setOnClickListener(new View.OnClickListener() {
            public void onClick(View view) {
            	try{
            		Status status = authenticatedTwitter.updateStatus(updateStatusEditText.getText().toString());
            		updateStatusEditText.setText("");
            	} catch (Exception e) {
            		TextView tv = (TextView) findViewById(R.id.authenticationTextView);
            		tv.append("\n[cBoxTwitterProxy]" + e.getMessage());
            	}
            }
        });
	}
	
	/**
	 * Create GUI for the list of Account's statuses retrieved from twitter.com
	 */
	public void displayAccountStatuses()	{
    	try {
    		ScrollView sv = (ScrollView)findViewById(R.id.scrollView1);
    		LinearLayout ll = new LinearLayout(this);
    		ll.setOrientation(LinearLayout.VERTICAL);
    		sv.addView(ll);
    		
    		// Retrieve account's tweets and print to view
    		List<Status> statuses = authenticatedTwitter.getHomeTimeline();
    		
    		for (Status status : statuses) {
    			TextView tvScroll = new TextView(this);
    			tvScroll.setText(status.getUser().getName() + ":" + status.getText());
    			ll.addView(tvScroll);
    		}
    		
    	} catch (Exception e) {
    		TextView tve = (TextView) findViewById(R.id.authenticationTextView);
    		tve.append("\n[cBoxTwitterProxy]" + e.getMessage());
    	} 
	}
	
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
         
        TextView tv = (TextView) findViewById(R.id.authenticationTextView);
        
        twitter = new TwitterFactory().getInstance();
        Authentication a = new Authentication(this);
        
        authenticatedTwitter = a.authenticate(twitter);
        if( authenticatedTwitter != null ) {
        	
        	// Create interface, and send input status
        	sendUpdateStatus();
        	
        	// Get account's statuses
        	displayAccountStatuses();
        }
        else 
        	tv.append("failure");
    }
}
