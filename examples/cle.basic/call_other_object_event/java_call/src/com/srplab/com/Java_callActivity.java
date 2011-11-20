package com.srplab.com;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import java.util.Hashtable;

import com.srplab.www.starcore.*;

public class Java_callActivity extends Activity {
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        
		StarCoreFactory starcore= StarCoreFactory.GetFactory();
        //StarServiceClass Service=starcore._InitSimple("test","123",0,0,"/sdcard/srplab/serverapp/AddFunction.lua?script=lua");
		StarServiceClass Service=starcore._InitSimple("test","123",0,0,"/sdcard/srplab/serverapp/AddFunction.py?script=python");
		//StarServiceClass Service=starcore._InitSimple("test","123",0,0,"/sdcard/srplab/serverapp/libAddFunction.so");
		
		StarObjectClass a = Service._GetObject("TestClass")._New();
		a = a._Assign(new StarObjectClass(){
	    	public int Add(StarObjectClass self,int x,int y)
	    	{
		    	return x+y+100;
	    	}
	    	public Object[] OnTest(StarObjectClass self, Hashtable Ev, String Args,int Type,double Val)
	    	{
	    		System.out.println("Object Event is triggered");
	    		System.out.println(Ev.get("_SrcObject"));
	    		System.out.println(Ev.get("_DesObject"));
	    		System.out.println(Ev.get("_EventID"));
	    		System.out.println(Ev.get("_ServiceGroupID"));
	    		System.out.println(".......Arg..........");
	    		System.out.println( Args );
	    		System.out.println( Type );
	    		System.out.println( Val );
		    	return new Object[]{true,"Event Result",4567,888.88};
		    } 
		});
		
		StarObjectClass b = a._New();
        System.out.println(b._Call("Add",12,34));
        Object[] Result = b._ProcessEvent("OnTest","Event From Java",12345,555.666);
        System.out.println("Event Result.........................");
        System.out.println(Result[0]);
        System.out.println(Result[1]);
        System.out.println(Result[2]);        
    }
}