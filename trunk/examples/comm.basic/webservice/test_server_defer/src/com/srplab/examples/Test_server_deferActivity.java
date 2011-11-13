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

public class Test_server_deferActivity extends Activity {
    /** Called when the activity is first created. */
    StarObjectClass a;
    StarServiceClass Service;
    StarCoreFactory starcore;
    Timer timer;
	
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        
		starcore= StarCoreFactory.GetFactory();
		Service=starcore._InitSimple("WebServiceCallServer","123",0,0);
		MyStarCallBackClass CallBack = new MyStarCallBackClass(starcore);		
		StarSrvGroupClass SrvGroup = (StarSrvGroupClass)Service._Get("_ServiceGroup");
		SrvGroup._SetWebServerPort(null,3008,100,100);

        Service._CreateSysRootItem("TestItem","",null,null);
        StarServiceItemClass SrvItem = Service._GetSysRootItem( "TestItem" );
        
        Object[] AtomicClassArray = Service._CreateAtomicObjectSimple("TestItem","TestClass","","");
        Service._CreateAtomicFunctionSimple(Service._Toint(AtomicClassArray[0]),"GetNumber","VS_INT32 GetNumber(VS_INT32 Para);","",false,false);
        Service._CreateAtomicFunctionSimple(Service._Toint(AtomicClassArray[0]),"GetString","VS_CHAR *GetString(VS_CHAR *Para);","",false,false);
        
        StarObjectClass ObjClass = Service._AtomicToObject(Service._Toint(AtomicClassArray[0]));
        ObjClass = ObjClass._Assign(new StarObjectClass(){
        	   public int GetNumber( StarObjectClass self,int para )
        	   {
        	     System.out.println("Remote Call Number "+para);
        	     if(self._Getint("SaveRemoteCallID") == 0 ){
        	        self._SetDeferRspFlag();
        	        self._Set("SaveRemoteCallID",self._Get("_RemoteCallID"));
        	        self._Set("SaveRemoteClientID",self._Get("_RemoteID"));
        	        self._Set("SaveRemoteCallName",self._Get("_RemoteCallName"));
        	        self._Set("SaveRemoteSourceTag",self._Get("_RemoteSourceTag"));
        	        self._Set("SaveRemoteRara",para);
        	        return 0;
        	     }else{           	
        	        return para + 1;
        	     }
        	   }
        	   public String GetString( StarObjectClass self,String para )
        	   {
        	     System.out.println("Remote Call String "+para);
        	     return para+"asdfsaf";
        	   }        	
        });        
        a = Service._GetObject("TestClass")._New("TestObject");
        a._Set("_WebServiceFlag",true);
        a._Set("SaveRemoteCallID",0);
        
        System.out.println("Start service ok...");
        
        timer = new Timer();
        
        final Handler handler = new Handler()  
        {  
            @Override  
            public void handleMessage(Message msg)  
            {  
        	    if(a._Getint("SaveRemoteCallID") != 0){
            		a._RemoteCallRsp(a._Getint("SaveRemoteClientID"),a._Getint("SaveRemoteCallID"),a._GetStr("SaveRemoteCallName"),a._Getint("SaveRemoteSourceTag"),0,null,Service._Getint("TYPE_INT32"),a._Getint("SaveRemoteRara") + 1);
            		a._Set("SaveRemoteCallID",0);
        	    }       	            	
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