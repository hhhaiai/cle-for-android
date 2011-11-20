package com.srplab.examples;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import com.srplab.www.starcore.*;

public class Java_callActivity extends Activity {
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        
		StarCoreFactory starcore= StarCoreFactory.GetFactory();
		starcore._InitCore(true,true,false,true,"",0,"",0);
		StarServiceClass Service=starcore._InitSimple("test","123",0,0,"http://10.0.2.2/Test_lua.srb.bin");
//		StarServiceClass Service=starcore._InitSimple("test","123",0,0,"http://10.0.2.2/Test_py.srb.bin");
//		StarServiceClass Service=starcore._InitSimple("test","123",0,0,"http://10.0.2.2/Test_lib.srb.bin");

		StarObjectClass b = Service._GetObject("TestClass");
		StarObjectClass a = Service._GetObject("TestClass")._New();
		StarObjectClass ParaObj = Service._GetObject("ParaClass")._New();
		ParaObj._Set("Para1",124);
		ParaObj._Set("Para2",a._Get("_ID"));
		ParaObj._Set("Para3",23456.78);
		ParaObj._Set("Para4",new Object[]{999,4444.55});
		ParaObj._Set("Para5","From caller");
		a._Call("PrintObj",ParaObj);        
    }
}