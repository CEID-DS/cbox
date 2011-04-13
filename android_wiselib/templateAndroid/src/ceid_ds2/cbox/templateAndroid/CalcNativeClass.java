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

package ceid_ds2.cbox.templateAndroid;

public class CalcNativeClass {

	/** 
	 * Calls the native(C++) method using JNI and runs the benchmark
	 * @return Returns the result of the benchmark
	 */
    public native double calcsFromJNI();
    
    /**
     * Creates two calc templates and does some operations among the templates
     * @param in1 first integer
     * @param in2 second integer
     * @param d1 first double
     * @param d2 second double
     * @return
     */
    public native double playWithTemplateJNI(int in1, int in2, double d1, double d2);
    
    /**
     * Native method that calls and executes the Java method testNative() through JNI
     * and also doubles the value of templateAndroid class variable "test".
     * @param c the actual instance(object in java language) of templateAndroid class
     * @returns a string specifying that it doubled test variable
     */
    public native String nativeVMPlay(Object c);

    // This method loads as soon as the instance of the class is created
    static {
    	System.loadLibrary("calcsFromJNI");
    }
    

    


}
