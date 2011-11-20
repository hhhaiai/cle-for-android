package com.srplab.examples;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import java.util.Timer;  
import java.util.TimerTask; 
import android.os.Handler;  
import android.os.Message;

import com.srplab.www.starcore.*;

public class Test_clientActivity extends Activity {
	StarCoreFactory starcore;
	StarSrvGroupClass SrvGroup;
	
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        
		StarCoreFactory starcore= StarCoreFactory.GetFactory();
		starcore._InitCore(true,true,false,true,"",0,"",0);
        StarSrvGroupClass SrvGroup = starcore._CreateSrvGroup(1,starcore._Getint("VS_CLIENT_USER"));
        
        int ret = SrvGroup._SConnect("","10.0.2.2",3008,"","",null);
        if(ret == 0 ){
            System.out.println( "Fail To Connect..." );
   	        SrvGroup._ClearService();
            starcore._ModuleExit();
            return;
        }
        SrvGroup._WaitServiceSync(0);
        System.out.println( "Success To Connect..." );

        StarServiceClass Service = SrvGroup._GetService("","");
        Service._ActiveSysRootItem("TestItem");
        StarServiceItemClass SrvItem = Service._GetSysRootItem("TestItem");
        SrvItem._WaitSync();

        Object[] RetResult = Service._GetObject("TestObject")._SRemoteCall(0,0,"GetNumber",123);
        System.out.println( RetResult[0]);
        System.out.println( RetResult[1]);
        Object[] RetResult1 = Service._GetObject("TestObject")._SRemoteCall(0,0,"GetString","Hello");
        System.out.println( RetResult1[0]);
        System.out.println( RetResult1[1]);

        StarParaPkgClass Para = SrvGroup._NewParaPkg();
        Para._Set(0,123);
        Object[] RetResult2 = Service._GetObject("TestObject")._SRemoteCall(0,0,"GetPkg",Para);
        System.out.println( RetResult2[0]);
        System.out.println( ((StarParaPkgClass)RetResult2[1])._Get(0));        
    }
}