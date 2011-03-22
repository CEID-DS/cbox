/* This class handles the connection between android code and
 * native code. Using a static System.loadLibrary("foo"); the
 * class loads the library libfoo.so in the .apk.
 * 
 */

package ceid_ds2.cbox.helloandroid;

public class NativeConnection {

	/** Calls the native(C++) method using JNI
	 * 
	 * @return Returns a string containing a hello world message
	 */
    public native String  stringFromJNI();

    // This method loads as soon as the instance of the class is created
    static {
    	System.loadLibrary("helloFromJNI");
    }
    


}
