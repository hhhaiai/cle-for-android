package com.srplab_examples.tcp_udp;

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

class TCPServer_CommInterface extends StarCommInterfaceClass{
	private StarBinBufClass BinBuf;	
	private StarSrvGroupClass SrvGroup;
    public void _MsgProc(int uMes,Object[] Msg){
     Log.d( "starcore","server receive msg "+_Toint(Msg[0]));
     if(uMes == _Getint("TCP_ONCONNECT")){
    	 Log.d("starcore","tcp connect from "+_GetIP((StarBinBufClass)Msg[3]));
     }else if(uMes == _Getint("TCP_ONREAD")){
        int Size=_TCPRecv(_Toint(Msg[0]),BinBuf,0);
        Log.d("starcore","tcp read from "+_Toint(Msg[0])+Size);
        while(Size != 0 ){
        	Log.d("starcore","receive from"+_Toint(Msg[0])+":"+(String)BinBuf._Get(0,0,"s"));
         Size=_TCPRecv(_Toint(Msg[0]),BinBuf,0);
        }
     }else if(uMes == _Getint("TCP_ONCLOSE")){
    	 Log.d("starcore","tcp connect close "+_Toint(Msg[0]));
     } 
    }   	
	public TCPServer_CommInterface(StarSrvGroupClass In_SrvGroup,StarCommInterfaceClass srcobj){
		super(srcobj);
		SrvGroup = In_SrvGroup;
		BinBuf = SrvGroup._NewBinBuf();
	}    
}  

public class TcpserverActivity extends Activity {
    /** Called when the activity is first created. */
	StarCoreFactory starcore;
	StarSrvGroupClass SrvGroup;
	MyStarCallBackClass CallBack;
	
	Timer timer = new Timer();  
	
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        
		starcore= StarCoreFactory.GetFactory();
		StarServiceClass Service=starcore._InitSimple("test","123",0,0);
		CallBack = new MyStarCallBackClass(starcore);		
		SrvGroup = (StarSrvGroupClass)Service._Get("_ServiceGroup");

		TCPServer_CommInterface CommInterface = new TCPServer_CommInterface(SrvGroup,SrvGroup._NewCommInterface());    	
        int ConnetionID = CommInterface._TCPSetupServer(100,"",3005);
        if(ConnetionID == 0 ){
        	Log.d("starcore","setup server on port 3005 fail");
          starcore._ModuleExit();
          return;
        }
        Log.d("starcore","setup server on port 3005 success");
        
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