package com.cbox;

import android.app.Activity;
import android.os.Bundle;
import android.widget.Toast;

public class cBox extends Activity {
    /** Called when the activity is first created. */
    WiseLib wiseLib = new WiseLib();
    
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        //Toast.makeText(getBaseContext(),Integer.toString(wiseLib.add(1,2)),Toast.LENGTH_SHORT).show();
    }
}