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
  	public MyStarCallBackClass(StarCoreFactory starcore){super(starcore);starcore._RegMsgCallBack(this,"CallBack");}
}

class TCPClient_CommInterface extends StarCommInterfaceClass{
	private StarBinBufClass BinBuf;	
	private StarSrvGroupClass SrvGroup;
	private int Index;
    public void _MsgProc(int uMes,Object[] Msg){
     if(uMes == _Getint("TCP_ONCONNECT")){
        if(_Toint(Msg[4]) == 0){
           Log.d("starcore","tcp connect success ");
        }
     }else if(uMes == _Getint("TIMER" )){
        BinBuf._Clear();
        BinBuf._Set(0,0,"s",String.format("test data %1$d....................",Index));

        _TCPSend(_Getint("ConnetionID"),BinBuf,0,true);
        Log.d("starcore",String.format("test data %1$d....................",Index));
        Index = Index + 1;
     }else if(uMes == _Getint("TCP_ONCLOSE")){
        Log.d("starcore","tcp connect close "+_Toint(Msg[0]));
     }
    }
	public TCPClient_CommInterface(StarSrvGroupClass In_SrvGroup,StarCommInterfaceClass srcobj){
		super(srcobj);
		SrvGroup = In_SrvGroup;
		BinBuf = SrvGroup._NewBinBuf();
		Index = 0;
	}    
} 

public class TcpclientActivity extends Activity {
    /** Called when the activity is first created. */
	StarCoreFactory starcore;
	Timer timer;  
	
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        
		starcore= StarCoreFactory.GetFactory();
		StarServiceClass Service=starcore._InitSimple("test","123",0,0);
		MyStarCallBackClass CallBack = new MyStarCallBackClass(starcore);		
		StarSrvGroupClass SrvGroup = (StarSrvGroupClass)Service._Get("_ServiceGroup");

		TCPClient_CommInterface CommInterface = new TCPClient_CommInterface(SrvGroup,SrvGroup._NewCommInterface());    	
        int ConnetionID = CommInterface._TCPSetupClient(100,"10.0.2.2",3005);

        if(ConnetionID == 0 ){
          Log.d("starcore","setup client on port 3005 fail");
          starcore._ModuleExit();
          return;
        }
        Log.d("starcore","setup client on port 3005 success");
        CommInterface._SetupTimer(100,0);
        CommInterface._Set("ConnetionID",ConnetionID);
        
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