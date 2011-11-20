package com.srplab.examples;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import java.util.Timer;  
import java.util.TimerTask; 
import android.os.Handler;  
import android.os.Message;
import java.util.Hashtable;

import com.srplab.www.starcore.*;

public class Test_serverActivity extends Activity {
    /** Called when the activity is first created. */
	StarCoreFactory starcore;
	StarSrvGroupClass SrvGroup;
	
	Timer timer;  
	
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        
		starcore= StarCoreFactory.GetFactory();
		StarServiceClass Service=starcore._InitSimple("RemoteCallServer","123",3008,0);
		SrvGroup = (StarSrvGroupClass)Service._Get("_ServiceGroup");

        System.out.println( Service );
        Service._CreateSysRootItem("TestItem","",null,null);
        StarServiceItemClass SrvItem = Service._GetSysRootItem( "TestItem" );

        StarObjectClass a = Service._NewGlobal(SrvItem);
        a._Set("_Name","TestObject");
   	
        a = a._Assign(new StarObjectClass(){
            public Object[] _OnRemoteSend(Hashtable Ev){
            	StarParaPkgClass Para;
            	
            	Para = (StarParaPkgClass)Ev.get("_Arg0");
            	System.out.println("Para1= "+Para._Get(0));
            	System.out.println("Para2= "+Para._Get(1));
            	System.out.println("Para3= "+Para._Get(2));
            	return null;
            }        	        	
        });
   
        System.out.println( "Server Start ok....");      
        
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