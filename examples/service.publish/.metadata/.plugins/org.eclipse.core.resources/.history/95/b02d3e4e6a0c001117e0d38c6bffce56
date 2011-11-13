package com.srplab.examples;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
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

public class Java_callActivity extends Activity {
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        
		StarCoreFactory starcore= StarCoreFactory.GetFactory();
		starcore._InitCore(true,true,false,true,"",0,"",0);
		MyStarCallBackClass CallBack = new MyStarCallBackClass(starcore);
//		StarServiceClass Service=starcore._InitSimple("test","123",0,0,"/sdcard/srplab/serverapp/embed/Test.lua?script=lua");
//		StarServiceClass Service=starcore._InitSimple("test","123",0,0,"/sdcard/srplab/serverapp/embed/Test.py?script=python");
		StarServiceClass Service=starcore._InitSimple("test","123",0,0,"/sdcard/srplab/serverapp/embed/libTest.so");

		StarObjectClass b = Service._GetObject("TestClass");
		StarObjectClass a = Service._GetObject("TestClass")._New();
		StarObjectClass ParaObj = Service._GetObject("ParaClass")._New();
		ParaObj._Set("Para1",124);
		ParaObj._Set("Para2",a._Get("_ID"));
		ParaObj._Set("Para3",23456.78);
		ParaObj._Set("Para4",new Object[]{999,4444.55});
		ParaObj._Set("Para5","From caller");
		a._Call("PrintObj",ParaObj);        
    }
}