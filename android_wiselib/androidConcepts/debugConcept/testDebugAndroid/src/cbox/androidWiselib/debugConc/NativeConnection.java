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


/* This class handles the connection between android code and
 * native code. Using a static System.loadLibrary("foo"); the
 * class loads the library libfoo.so in the .apk.
 * 
 */

package cbox.androidWiselib.debugConc;

public class NativeConnection {

	/** Calls the native(C++) method using JNI
	 * 
	 * @return Returns a string containing a hello world message
	 */
    public native String  debugConceptJNI(Object c);

    // This method loads as soon as the instance of the class is created
    static {
    	System.loadLibrary("debugTest");
    }
    

    

    


}