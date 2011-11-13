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

public class Test_clientActivity extends Activity {
    /** Called when the activity is first created. */
	public static StarServiceClass Service;
	public static StarParaPkgClass Para;
	
	StarCoreFactory starcore;	
	Timer timer;  
	
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        
		starcore= StarCoreFactory.GetFactory();
		starcore._InitCore(true,true,false,true,"",0,"",0);
		MyStarCallBackClass CallBack = new MyStarCallBackClass(starcore);		
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

        Service = SrvGroup._GetService("","");
        Service._ActiveSysRootItem("TestItem");
        StarServiceItemClass SrvItem = Service._GetSysRootItem("TestItem");
        SrvItem._WaitSync();

        Para = SrvGroup._NewParaPkg();
        Para._Set(0,123);
        Para._Set(1,456.3333);
        Para._Set(2,"client request");
        ((StarObjectClass)Service._Get("TestObject"))._RemoteSend(0,Para);

        System.out.println( "Finish [ESC] exit....");       
        
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