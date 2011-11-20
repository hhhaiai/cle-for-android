package com.srplab.examples;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import java.util.Timer;  
import java.util.TimerTask; 
import android.os.Handler;  
import android.os.Message;

import com.srplab.www.starcore.*;

public class Test_server_twoitemActivity extends Activity {	
	StarCoreFactory starcore;
	StarSrvGroupClass SrvGroup;
	Timer timer;	
	
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        
		starcore= StarCoreFactory.GetFactory();
		StarServiceClass Service=starcore._InitSimple("RemoteCallServer","123",3008,0);
		SrvGroup = (StarSrvGroupClass)Service._Get("_ServiceGroup");

        Service._CreateSysRootItem("TestItem","",null,null);
        StarServiceItemClass SrvItem = Service._GetSysRootItem( "TestItem" );
        StarObjectClass a = Service._NewGlobal(SrvItem,"TestObject");
        a = a._Assign(new StarObjectClass(){
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
        	   public StarParaPkgClass GetPkg( StarObjectClass self,StarParaPkgClass para )    
        	   {
        	      System.out.println( "Remote Call Pkg "+(Integer)para._Get(0));
        	      StarParaPkgClass ParaPkg = SrvGroup._NewParaPkg();
        	      ParaPkg._Set(0,"asdfsaf");
        	      return ParaPkg;
        	   }        	
        });

        Service._CreateSysRootItem("TestItem2","",null,null);
        SrvItem = Service._GetSysRootItem( "TestItem2" );
        StarObjectClass b = Service._NewGlobal(SrvItem,"TestObject2");
        b = b._Assign(new StarObjectClass(){
        	   public int GetNumber( StarObjectClass self,int para )
        	   {
        	     System.out.println("Remote Call Number "+para);
        	     return para + 2;
        	   }
        	   public String GetString( StarObjectClass self,String para )
        	   {
        	     System.out.println("Remote Call String "+para);
        	     return para+"2222222";
        	   }
        	   public StarParaPkgClass GetPkg( StarObjectClass self,StarParaPkgClass para )    
        	   {
        	      System.out.println( "Remote Call Pkg "+(Integer)para._Get(0));
        	      StarParaPkgClass ParaPkg = SrvGroup._NewParaPkg();
        	      ParaPkg._Set(0,"2222222");
        	      return ParaPkg;
        	   }        	        	
        });

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