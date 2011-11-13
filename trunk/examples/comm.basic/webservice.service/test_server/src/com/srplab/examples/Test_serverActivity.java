package com.srplab.examples;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import java.util.Timer;  
import java.util.TimerTask; 
import android.os.Handler;  
import android.os.Message;

import com.srplab.www.starcore.*;

class MyStarCallBackClass extends StarCallBackClass{  	
  	public Object[] CallBack( int ServiceGroupID, int uMes, Object wParam, Object lParam )
  	{
  		if( uMes == _Getint("MSG_VSDISPMSG") || uMes == _Getint("MSG_VSDISPLUAMSG") ){
  		    Log.d("starcore",(String)wParam);
  		}
  		if( uMes == _Getint("MSG_DISPMSG") || uMes == _Getint("MSG_DISPLUAMSG") ){
  			Log.d("starcore",(String)wParam);
  		}
  		return null;
  	}
  	MyStarCallBackClass(StarCoreFactory starcore){super(starcore);starcore._RegMsgCallBack(this,"CallBack");}
}	

public class Test_serverActivity extends Activity {
    /** Called when the activity is first created. */
	StarCoreFactory starcore;
	Timer timer;
	
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        
		starcore= StarCoreFactory.GetFactory();
		starcore._InitCore(true,true,false,true,"",0,"",0);
		MyStarCallBackClass CallBack = new MyStarCallBackClass(starcore);		
		StarSrvGroupClass SrvGroup = starcore._GetSrvGroup(0);
        SrvGroup._ImportServiceWithPath("/sdcard/srplab/serverapp","RemoteCallServer",true);
        SrvGroup._CreateService( "","WebServiceCallServer", "123",5,0,0,0,0,0,"B07427AF-3C8B-4e88-9F06-535831EF46EF" );
        StarServiceClass Service = SrvGroup._GetService("root","123");

        Service._CreateSysRootItem("TestItem","",null,null);
        StarServiceItemClass SrvItem = Service._GetSysRootItem( "TestItem" );
        
        SrvGroup._SetWebServerPort(null,3008,100,100);
        
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