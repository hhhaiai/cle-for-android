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

public class Test_client_asyncActivity extends Activity {
	StarCoreFactory starcore;
	StarSrvGroupClass SrvGroup;
	Timer timer;
	StarObjectClass TestObject;
	StarParaPkgClass Para;
	
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        
		starcore= StarCoreFactory.GetFactory();
		starcore._InitCore(true,true,false,true,"",0,"",0);
		MyStarCallBackClass CallBack = new MyStarCallBackClass(starcore);		
        SrvGroup = starcore._CreateSrvGroup(1,starcore._Getint("VS_CLIENT_USER"));
        
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
        
        StarObjectClass TestObject = Service._GetObject("TestObject");
        TestObject = TestObject._Assign(new StarObjectClass(){
        	   public void CallBack(StarObjectClass self,int RetCode,int ServiceGroupID,int Para,Object RetValue)
        	   {
        	    System.out.println(RetCode);
        	    System.out.println(ServiceGroupID);
        	    System.out.println(Para);
        	    System.out.println(RetValue);
        	   }        	
        });
        
        Para = SrvGroup._NewParaPkg();
        Para._Set(0,123);
   
        TestObject._ARemoteCall(0,0,"CallBack","GetNumber",11,123);
        TestObject._ARemoteCall(0,0,"CallBack","GetString",11,"Hello");
        TestObject._ARemoteCall(0,0,"CallBack","GetPkg",11,Para);    
        
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