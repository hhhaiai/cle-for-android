package com.srplab_examples.tcp_udp;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import java.util.Timer;  
import java.util.TimerTask; 
import android.os.Handler;  
import android.os.Message;

import com.srplab.www.starcore.*;

class UDPClient_CommInterface extends StarCommInterfaceClass{
	private StarBinBufClass BinBuf,BinBuf_IP;	
	private StarSrvGroupClass SrvGroup;
	private int Index;
    public void _MsgProc(int uMes,Object[] Msg){
      int Size = 0;
      if(uMes == _Getint("TIMER") ){
        BinBuf._Clear();
        BinBuf._Set(0,0,"s",String.format("test data %1$d....................",Index));
        _UDPSend(_Getint("ConnetionID"),BinBuf,BinBuf_IP);
        System.out.println(String.format("test data %1$d....................",Index));
        Index = Index + 1;
      }
      if(uMes == _Getint("UDP_ONREAD") ){
        Size=_UDPRecv(_Toint(Msg[0]),BinBuf,BinBuf_IP);
        while(Size != 0 ){
         System.out.println("receive from"+_Toint(Msg[0])+":"+(String)BinBuf._Get(0,0,"s")+_GetIP(BinBuf_IP)+_GetPort(BinBuf_IP));
         Size=_UDPRecv(_Toint(Msg[0]),BinBuf,BinBuf_IP);
        }
      }
    }
	public UDPClient_CommInterface(StarSrvGroupClass In_SrvGroup,StarCommInterfaceClass srcobj){
		super(srcobj);
		SrvGroup = In_SrvGroup;
		BinBuf = SrvGroup._NewBinBuf();
		BinBuf_IP = SrvGroup._NewBinBuf();
		_UDPSetSockAddr("10.0.2.2",3005,BinBuf_IP);
		Index = 0;
	}    
}  

public class UdpclientActivity extends Activity {
    /** Called when the activity is first created. */
	StarCoreFactory starcore;
	Timer timer;  
	
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        
		starcore= StarCoreFactory.GetFactory();
		StarServiceClass Service=starcore._InitSimple("test","123",0,0);
		StarSrvGroupClass SrvGroup = (StarSrvGroupClass)Service._Get("_ServiceGroup");

		UDPClient_CommInterface CommInterface = new UDPClient_CommInterface(SrvGroup,SrvGroup._NewCommInterface());    	
        int ConnetionID = CommInterface._UDPSetupClient(100);

        if(ConnetionID == 0 ){
          System.out.println("setup client on port 3005 fail");
          starcore._ModuleExit();
          return;
        }
        System.out.println("setup client on port 3005 success");
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