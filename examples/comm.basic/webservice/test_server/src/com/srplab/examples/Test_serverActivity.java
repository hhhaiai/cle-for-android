package com.srplab.examples;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import java.util.Timer;  
import java.util.TimerTask; 
import android.os.Handler;  
import android.os.Message;

import com.srplab.www.starcore.*;

public class Test_serverActivity extends Activity {
    /** Called when the activity is first created. */
	StarCoreFactory starcore;
	Timer timer;
	
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        
		starcore= StarCoreFactory.GetFactory();
		StarServiceClass Service=starcore._InitSimple("WebServiceCallServer","123",0,0);
		StarSrvGroupClass SrvGroup = (StarSrvGroupClass)Service._Get("_ServiceGroup");
		SrvGroup._SetWebServerPort(null,3008,100,100);

        Service._CreateSysRootItem("TestItem","",null,null);
        StarServiceItemClass SrvItem = Service._GetSysRootItem( "TestItem" );
        
        Object[] AtomicClassArray = Service._CreateAtomicObjectSimple("TestItem","TestClass","","");
        Service._CreateAtomicFunctionSimple(Service._Toint(AtomicClassArray[0]),"GetNumber","VS_INT32 GetNumber(VS_INT32 Para);","",false,false);
        Service._CreateAtomicFunctionSimple(Service._Toint(AtomicClassArray[0]),"GetString","VS_CHAR *GetString(VS_CHAR *Para);","",false,false);
        
        StarObjectClass ObjClass = Service._AtomicToObject(Service._Toint(AtomicClassArray[0]));
        ObjClass._Assign(new StarObjectClass(){
        	   public int GetNumber( StarObjectClass self,int para )
        	   {
        	     System.out.println("Remote Call Number "+para);
        	     return para + 1;
        	   }
        	   public String GetString( StarObjectClass self,String para )
        	   {
        	     System.out.println("Remote Call String "+para);
        	     return para+"asdfsaf";
        	   }        	
        });
        StarObjectClass a = Service._GetObject("TestClass")._New("TestObject");
        a._Set("_WebServiceFlag",true);
        a._SLockGC();

        System.out.println("Server Start ok....");
        
        timer = new Timer();
        
        final Handler handler = new Handler()  
        {  
            @Override  
            public void handleMessage(Message msg)  
            {  
                while( starcore._SRPDispatch(false) == true );
            }
        };
        timer.scheduleAtFixedRate(new TimerTask()  
        {  
            @Override  
            public void run()  
            {  
                Message mesasge = new Message();  
                mesasge.what = 0;  
                handler.sendMessage(mesasge);  
            }  
        }, 0, 10);        
    }
}