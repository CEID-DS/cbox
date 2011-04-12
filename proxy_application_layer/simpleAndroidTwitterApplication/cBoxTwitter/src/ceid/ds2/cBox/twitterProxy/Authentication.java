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

/**
 * <b>Implmentation of OAuthentication for Twitter Proxy</b>
 */
package ceid.ds2.cBox.twitterProxy;

import twitter4j.Twitter;
import twitter4j.TwitterException;
import twitter4j.TwitterFactory;
import twitter4j.auth.AccessToken;
import twitter4j.auth.RequestToken;
import twitter4j.conf.ConfigurationBuilder;

import java.io.*;

import java.util.Properties;

import ceid.ds2.cBox.twitterProxy.R;

import android.app.Activity;
import android.content.Context;
import android.widget.TextView;

/**
 * @author Nick Palaghias
 *
 */
public class Authentication {
	
	private static final String PROPERTIES_FILE = "twitter4j.properties";
	private static Activity currentActivity = null;
	private static Properties consumerProperties = null;    
    private static RequestToken requestToken = null;
    private static AccessToken accessToken = null;

    /**
     * Constructor
     */
	public Authentication(Activity a)	{
		consumerProperties = new Properties();
		currentActivity = a;
	}
	
	/**
	 * Get consumer properties from properties file
	 * @return boolean: true if success
	 */
	private boolean getConsumerProperties() {
		FileOutputStream output = null;
		FileInputStream input = null;
		TextView tv = (TextView) currentActivity.findViewById(R.id.authenticationTextView);
		
		try {
			// Read consumer properties from file
			input = currentActivity.openFileInput(PROPERTIES_FILE);
			consumerProperties.load(input);
	        input.close();
		} catch (FileNotFoundException e) {	
		} catch (IOException ioe) {
			tv.append(ioe.getMessage());
		}
		
		try {	
			// Consumer Key and Secret are not set
			if (consumerProperties.getProperty("oauth.consumerKey") == null
                    && consumerProperties.getProperty("oauth.consumerSecret") == null) {
				
	    		// Set Consumer Key and Secret
	    		consumerProperties.setProperty("oauth.consumerKey", "wXYz9UhqIaJbbtY2CyeA");
	    		consumerProperties.setProperty("oauth.consumerSecret", "nhEs7Ups99gXv6tFfIrpnE2LQ1ukoscurGgvfZljSjw");
	    		
	    		// Write properties to file
	            output = currentActivity.openFileOutput(PROPERTIES_FILE, Context.MODE_PRIVATE);
	            consumerProperties.store(output, PROPERTIES_FILE);
	            
	            output.close();
			}

        	return true;
		} catch (IOException e)	{
			tv.append(e.getMessage());
		} 
			
		return false;
		
	}
	
	/**
	 * Store access token to properties file
	 */
	  private static void storeAccessToken(){
		  try {
			  FileOutputStream output = currentActivity.openFileOutput(PROPERTIES_FILE, 
					  												   Context.MODE_PRIVATE);

			  // Set Access Token and Access Token Secret
			  consumerProperties.setProperty("oauth.accessToken", accessToken.getToken());
		  	  consumerProperties.setProperty("oauth.accessTokenSecret", accessToken.getTokenSecret());
          
			  consumerProperties.setProperty("oauth.accessToken", "276870861-1EHdvbBP9U70q04Hcxf73Bc3a41Sj97lD9aA1896");
		  	  consumerProperties.setProperty("oauth.accessTokenSecret", "k4AQjYhlDUiGYdZnQBqXtEWo8Rkhlr3ZANKiGX5jXI");
		  	  // Store Access Token
		  	  consumerProperties.store(output, PROPERTIES_FILE);
		  	  
		  	  // Close Stream
		  	  output.close();
		  } catch (IOException e) {
			  TextView tv = (TextView) currentActivity.findViewById(R.id.authenticationTextView);
			  tv.append(e.getMessage());
		  }
	  }
	  
	/**
	 * Get access token from Twitter
	 * @return boolean: true if success
	 */
	private boolean getAccessToken(Twitter twitter)	{
		TextView tv = (TextView) currentActivity.findViewById(R.id.authenticationTextView);
		
        try	{
        	// Set Authentication Properties
        	twitter.setOAuthConsumer(consumerProperties.getProperty("oauth.consumerKey"), 
        							 consumerProperties.getProperty("oauth.consumerSecret"));
        	// Get Request Token
        	requestToken = twitter.getOAuthRequestToken();
        	
        	while (accessToken == null)	{
        		try	{
        			tv.append(requestToken.getAuthorizationURL());
        			        			
        			accessToken = twitter.getOAuthAccessToken();
        	    } catch (TwitterException te) {   	    	
        			tv.append(te.getMessage());
        	    }
        	}
        	// Store Access Token to properties files
        	storeAccessToken();
        	
        	return true;
        } catch (Exception e)	{
        	tv.append(e.getMessage());
        	return false;
        } 
	}
	

	/**
	 * Check if access token has been retrieved and is stored at Android Storage
	 * 
	 * @param twitter Initial Twitter instance
	 * @return Authorized Twitter instance
	 */
	private Twitter checkAccessTokenExists(Twitter twitter) {
		FileInputStream input = null;

		try {
			// Load keys and secrets
			input = currentActivity.openFileInput(PROPERTIES_FILE);
			consumerProperties.load(input);
			
			// If access token and access token secret exist
			if (consumerProperties.getProperty("oauth.accessToken") != null
             && consumerProperties.getProperty("oauth.accessTokenSecret") != null) {
				
				// Create access token
				accessToken = new AccessToken( consumerProperties.getProperty("oauth.accessToken"),
        				 					   consumerProperties.getProperty("oauth.accessTokenSecret"));
				

				// Create authorized twitter instance, to sign in
				ConfigurationBuilder confbuilder = new ConfigurationBuilder();
				confbuilder.setOAuthAccessToken(accessToken.getToken()) 
												.setOAuthAccessTokenSecret(accessToken.getTokenSecret()) 
												.setOAuthConsumerKey(consumerProperties.getProperty("oauth.consumerKey")) 
												.setOAuthConsumerSecret(consumerProperties.getProperty("oauth.consumerSecret"));
				Twitter t = new TwitterFactory(confbuilder.build()).getInstance();
				
				input.close();
				return t;
			}
			input.close();
		} catch (Exception e) {
			TextView tv = (TextView) currentActivity.findViewById(R.id.authenticationTextView);
			tv.append("\n[checkAccessTokenExists]" + e.getMessage());
		} 
		return null;
	}
	
	/**
	 * Given an Twitter instance it checks if Access Token was
	 * retrieved. If not it implements OAuth authentication.
	 * 
	 * CAUTION: URL cannot be access through mobile device, so
	 * access token must have been retrieved in advance.
	 * 
	 * @param twitter Initial Twitter instance
	 * @return Authorized Twitter instance
	 */
	public Twitter authenticate(Twitter twitter)	{	
		
		Twitter authenticatedTwitter = checkAccessTokenExists(twitter);
		
		if( authenticatedTwitter != null ){
			// Initialize Consumer Properties
			if( ! getConsumerProperties())
				return null;
			
			// Get Access Token
			if( ! getAccessToken(twitter) )
				return null;
		}

		return authenticatedTwitter;
	}
}

