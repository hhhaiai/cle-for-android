# Common Language Extension for Android #

CLE is a middleware to support mixed programming of multiple languages. Any language of c/c++, lua, python, c#, java can access most classes, functions, variables, or modules of another language directly, which makes existing codes or libraries to be reused in products developed with other languages easily. Developers can write codes using the preferred language and then use them in another language application. CLE is simple, which is a single core share library plus share libraries corresponding to each script language.

From version 2.0, CLE is released as shareware. Current version is 2.0.3, which helps java in calling lua, c/c++, and python(need SL4A). CLE is very easy to use and greatly simplify the programming for java with different other languages. With the help of CLE, programmers can written c/c++ share modules without any knowledge about JNI, can call lua or python and provide callback functions of java to these languages easily.

CLE supports android version android 2.1/2.2/3.0/3.2/4.0/4.1/4.2/4.3/4.4.

# Dynamic Web Server for Android #

http://code.google.com/p/dynamic-web-server-for-android/

# Version 2.2.0 is released #

1. support int64 and double

2. support delphi,c++builder,xe6,xe7.

3. add json support

4. fix bugs of version 2.1.0

http://www.srplab.com/data/starcore_for_android.2.2.0.zip

# Version 2.1.0 is released #

1. lua engine is upgraded to version 5.2.3

2. support ruby 193, ruby syntax, most built-in classes, except thread, process, socket, io, mutex, etc.

3. support delphi

4. add c interface

5. add more java interface for callback

6. add x86 image for android

7. fix bugs of version 2.0.6

http://www.srplab.com/data/starcore_for_android.2.1.0.zip


# Version 2.0.6 is released #

1. Support android 4.3 or higher


# Version 2.0.5 is released #

1. Lua engine is upgraded to 5.2.2

2. Version 2.0.5 is a bug fix version of 2.0.4

# Using CLE #

  1. copy starcore\_android\_rX.X.jar to the libs directory of the project.
  1. copy libstar\_java.so, libstarcore.so and libstarpy.so to libs directory, as follows:
> > http://www.srplab.com/Images/android/android_libs.JPG
  1. Using the following code to init cle
```
import com.srplab.www.starcore.*;

StarCoreFactory starcore;
StarServiceClass Service;

StarCoreFactoryPath.StarCoreCoreLibraryPath = "/data/data/"+getPackageName()+"/lib";
StarCoreFactoryPath.StarCoreShareLibraryPath = "/data/data/"+getPackageName()+"/lib";
StarCoreFactoryPath.StarCoreOperationPath = "/data/data/"+getPackageName()+"/files";

starcore= StarCoreFactory.GetFactory();             
Service=starcore._InitSimple("test","123",0,0,"");
```
  1. Build native share library using CLE
> > Write "Android.mk" and build with ndk-build
```
LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

# Here we give our module name and sourcefile(s)
LOCAL_CFLAGS += -Wno-write-strings -DENV_ANDROID
LOCAL_CPPFLAGS += -Wno-write-strings -fexceptions -DENV_ANDROID
LOCAL_LDFLAGS += -Wno-write-strings -DENV_ANDROID

LOCAL_C_INCLUDES += cle_files/include

#--------source file
MODULE_CXXSRCS := XXX.cpp

LOCAL_SRC_FILES := ${MODULE_CXXSRCS}
LOCAL_LDLIBS := cle_files/libs/armeabi/libstarlib.a

LOCAL_MODULE  := Code

include $(BUILD_SHARED_LIBRARY) 
```

# Samples #

  * Calling lua script file
> > This example shows how to call lua files in java. The lua file is simple.
```
function Add(x,y)
    return x+y
end    
```
> > The file is packed into zip format and stored in the assets directory of the project.
> > Java code:
```
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        try{
        	AssetManager assetManager = getAssets();
        	InputStream dataSource = assetManager.open("test.zip");
        	StarCoreFactoryPath.Install(dataSource, "/data/data/"+getPackageName()+"/files", false);
        }
        catch(IOException e ){
        }            	        
        /*----init starcore----*/
        StarCoreFactoryPath.StarCoreCoreLibraryPath = "/data/data/"+getPackageName()+"/lib";
        StarCoreFactoryPath.StarCoreShareLibraryPath = "/data/data/"+getPackageName()+"/lib";
	StarCoreFactory starcore= StarCoreFactory.GetFactory();
	StarServiceClass Service=starcore._InitSimple("test","123",0,0);
	StarSrvGroupClass SrvGroup = (StarSrvGroupClass)Service._Get("_ServiceGroup");
		
	Service._DoFile("lua", "/data/data/"+getPackageName()+"/files/test.lua", "");
	SrvGroup._InitRaw("lua",Service);
	StarObjectClass lua = Service._ImportRawContext("lua","",false,"");
        int result = lua._Callint("Add", 123,456);
        System.out.println("result = "+result);
    }
```


  * Calling python script file
> > This example shows how to call python files in java. The python file is simple.
```
def Add(x,y) :
    return x+y      
```
> > The file is packed into zip format and stored in the assets directory of the project.
> > Java code:
```
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        try{
        	AssetManager assetManager = getAssets();
        	InputStream dataSource = assetManager.open("test.zip");
        	StarCoreFactoryPath.Install(dataSource, "/data/data/"+getPackageName()+"/files", true);
        }
        catch(IOException e ){
        }            	        
        /*----init starcore----*/
        StarCoreFactoryPath.StarCoreCoreLibraryPath = "/data/data/"+getPackageName()+"/lib";
        StarCoreFactoryPath.StarCoreShareLibraryPath = "/data/data/"+getPackageName()+"/lib";
	StarCoreFactory starcore= StarCoreFactory.GetFactory();
	StarServiceClass Service=starcore._InitSimple("test","123",0,0);
	StarSrvGroupClass SrvGroup = (StarSrvGroupClass)Service._Get("_ServiceGroup");
		
	Service._DoFile("python", "/data/data/"+getPackageName()+"/files/test.py", "");
	SrvGroup._InitRaw("python",Service);
	StarObjectClass python = Service._ImportRawContext("python","",false,"");
        int result = python._Callint("Add", 123,456);
        System.out.println("result = "+result);
    }
```

  * Lua accesses android GUI element
> > This example shows how to access android java class using lua directly. The example uses lua script to create a button on screen and capture it's onclick event. The lua code is show as follow:
```
--prepare lua environment
SrvGroup=_GetSrvGroup()
Service=SrvGroup:_GetService("","")

function main(activity) 
    --create absolute layout
    local AbsoluteLayout = Service:_ImportRawContext("java","android/widget/AbsoluteLayout",true,""); 
    local abslayout=AbsoluteLayout(activity);
    activity:setContentView(abslayout);
    
    --create button
    local Button = Service:_ImportRawContext("java","android/widget/Button",true,"");
    local btn1 = Button(activity);
    btn1:setText("Hello World");
	
    Toast = Service:_ImportRawContext("java","android/widget/Toast","");

    --create event proxy
    local btnevent = Service:_New();
    function btnevent:onClick(view)
        print( view )
        Toast:makeText(activity, "Is clicked", Toast.LENGTH_SHORT ):show()
    end
    local btnevent_proxy = Service:_NewRawProxy("java",btnevent,nil,"android.view.View$OnClickListener",0);
    btn1:setOnClickListener( btnevent_proxy );
    	
    --create layout
    local ViewGroup_LayoutParams = Service:_ImportRawContext("java","android/view/ViewGroup$LayoutParams",true,"");
    local LayoutParams = Service:_ImportRawContext("java","android/widget/AbsoluteLayout$LayoutParams",true,"");
	
    --add button to layout
    local lp1 = LayoutParams(ViewGroup_LayoutParams.WRAP_CONTENT,ViewGroup_LayoutParams.WRAP_CONTENT,0,100)
    abslayout:addView(btn1, lp1 );	
end	
```
> > Java code:
```
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        
        /*----load test.lua----*/
        String luastring = null;
        try{
        	AssetManager assetManager = getAssets();
        	InputStream dataSource = assetManager.open("test.lua");
        	int size=dataSource.available();
        	byte[] buffer=new byte[size]; 
        	dataSource.read(buffer); 
        	dataSource.close();
        	luastring=new String(buffer);
        }
        catch(IOException e ){
        }            	        
        /*----init starcore----*/
        StarCoreFactoryPath.StarCoreCoreLibraryPath = "/data/data/"+getPackageName()+"/lib";
        StarCoreFactoryPath.StarCoreShareLibraryPath = "/data/data/"+getPackageName()+"/lib";
	StarCoreFactory starcore= StarCoreFactory.GetFactory();
	StarServiceClass Service=starcore._InitSimple("test","123",0,0);
	StarSrvGroupClass SrvGroup = (StarSrvGroupClass)Service._Get("_ServiceGroup");
	Service._CheckPassword(false);
		
	/*----run lua code----*/
	Service._RunScript("lua", luastring, "", "");
	SrvGroup._InitRaw("lua",Service);
	StarObjectClass lua = Service._ImportRawContext("lua","",false,"");
	/*---wrap activity object for lua---*/
	StarObjectClass activityobj = Service._New();
	activityobj._AttachRawObject(this,false);
	/*---call lua raw function----*/
	lua._Call("main", activityobj);
    }
```


  * Python accesses android GUI element
> > This example shows how to access android java class using python directly. The example uses python script to create a button on screen and capture it's onclick event. The python code is show as follow:
```
#--prepare lua environment
SrvGroup=libstarpy._GetSrvGroup()
Service=SrvGroup._GetService("","")
Toast = Service._ImportRawContext("java","android/widget/Toast",True,"");

def main(activity) :
    #--create absolute layout
    AbsoluteLayout = Service._ImportRawContext("java","android/widget/AbsoluteLayout",True,""); 
    abslayout=AbsoluteLayout(activity);
    activity.setContentView(abslayout);
    
    #--create button
    print(Service)
    Button = Service._ImportRawContext("java","android/widget/Button",True,"");
    btn1 = Button(activity);
    btn1.setText("Hello World");		
    
    #--create event proxy
    btnevent = Service._New();
    def btnevent_onClick(self,view) :
        print( view )
        Toast.makeText(activity, "Is clicked", Toast.LENGTH_SHORT ).show()
    btnevent.onClick = btnevent_onClick
    btnevent_proxy = Service._NewRawProxy("java",btnevent,"","android.view.View$OnClickListener",0);
    btn1.setOnClickListener( btnevent_proxy );
    
    #--create layout
    ViewGroup_LayoutParams = Service._ImportRawContext("java","android/view/ViewGroup$LayoutParams",True,"");
    LayoutParams = Service._ImportRawContext("java","android/widget/AbsoluteLayout$LayoutParams",True,"");
    
    #--add button to layout
    lp1 = LayoutParams(ViewGroup_LayoutParams.WRAP_CONTENT,ViewGroup_LayoutParams.WRAP_CONTENT,0,100)
    abslayout.addView(btn1, lp1 );	
```
> > Java code:
```
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        
        /*----load test.py----*/
        String pythonstring = null;
        try{
        	AssetManager assetManager = getAssets();
        	InputStream dataSource = assetManager.open("test.py");
        	int size=dataSource.available();
        	byte[] buffer=new byte[size]; 
        	dataSource.read(buffer); 
        	dataSource.close();
        	pythonstring=new String(buffer);
        }
        catch(IOException e ){
        }            	        
        /*----init starcore----*/
        StarCoreFactoryPath.StarCoreCoreLibraryPath = "/data/data/"+getPackageName()+"/lib";
        StarCoreFactoryPath.StarCoreShareLibraryPath = "/data/data/"+getPackageName()+"/lib";        
	StarCoreFactory starcore= StarCoreFactory.GetFactory();
	StarServiceClass Service=starcore._InitSimple("test","123",0,0);
	StarSrvGroupClass SrvGroup = (StarSrvGroupClass)Service._Get("_ServiceGroup");
	Service._CheckPassword(false);
		
	/*----run python code----*/
	Service._RunScript("python", pythonstring, "", "");
	SrvGroup._InitRaw("python",Service);
	StarObjectClass python = Service._ImportRawContext("python","",false,"");
	/*---wrap activity object for python---*/
	StarObjectClass activityobj = Service._New();
	activityobj._AttachRawObject(this,false);
	/*---call lua python function----*/
	python._Call("main", activityobj);
    }
```

  * More information and details, please refer to http://www.srplab.com

# History #

## Version 2.0.4 ##

1. Version 2.0.4 is a bug fix version of 2.0.3

## Version 1.80.1 ##

1.lua engine update to 5.2.1

2.add more lua interface functions

3.new interface function _Super_

4.new interface functions RegGetObjectCallBack/RegLuaFuncFilter/IsRegLuaFunc

5.interface function RegLuaFunc, supports general match with input FuncName equals to NULL

6.new platform ios is supported

## Version 1.6.1 ##

1. _ProcessEvent function supports object as input parameter;_

2. Fix bug for java when assign large amounts of functions to object;

## Version 1.5.1 ##

1. add inteface functions for java, _Callbool_Callint _Calldouble_Callobject, which returns value with corresponding type;

2. add inteface functions for loading and unloading share library dynamically;

3. add inteface functions for group management, _NewGroup,_FreeGroup,_GroupAdd,etc. Objects can be added into group and freed;_

## Version 1.4.1 ##

1. new script function : _SUnlockGC and_Version;

2. new c/c++ function: DetachCurrentThread;

3. solve integer scope problem for lua script;

4. add static variable StarCoreCoreLibraryPath to starcore\_android\_r5.jar(for android version);

5. correct python module path for sl4a;

## Version 1.3.1 ##

1. Embedded lua engine is upgraded to version 5.2

2. float type as function parameters and return value for java is supported

3. exception when free cle objects in java GC procedure is solved

4. net install function is included in starcore\_android\_r4.jar, you need not include  starcore\_netinst\_r3.jar in your project from now on. The function is used as follows:

