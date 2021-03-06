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
	StarObjectClass b;
	
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        
		starcore= StarCoreFactory.GetFactory();
		StarServiceClass Service=starcore._InitSimple("RemoteCallServer","123",3008,0);
		StarSrvGroupClass SrvGroup = (StarSrvGroupClass)Service._Get("_ServiceGroup");

        Service._CreateSysRootItem("TestItem","",null,null);
        StarServiceItemClass SrvItem = Service._GetSysRootItem( "TestItem" );

        Service._CreateAtomicStructSimple("ParaStruct","VS_INT32 Para1;VS_FLOAT Para2;","");
        Service._CreateAtomicObjectSimple("TestItem","ParaObject","VS_INT32 Para1;VS_UUID Para2;VS_FLOAT Para3;struct ParaStruct Para4;VS_VSTRING Para5;","");

        StarObjectClass a = Service._NewGlobal(SrvItem,"TestObject");
        a = a._Assign(new StarObjectClass(){
        	   public StarObjectClass GetRemoteObject( StarObjectClass self,StarObjectClass para )
        	   {
        	    b._Set("Para1",123 + _Toint(para._Get("Para1")));
        	    b._Set("Para2","1E2929C6-7DDA-468f-BBAD-E303A1B3C826");
        	    b._Set("Para3",456.0 + _Todouble(para._Get("Para3")));
        	    b._Set("Para4",new Object[]{((StarStructClass)para._Get("Para4"))._Getint("Para1") + 234,((StarStructClass)para._Get("Para4"))._Getdouble("Para2")+567.0});
        	    b._Set("Para5","server return");
        	    return b;
        	   }        	
        });
        
        b = Service._GetObject("ParaObject")._New();

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