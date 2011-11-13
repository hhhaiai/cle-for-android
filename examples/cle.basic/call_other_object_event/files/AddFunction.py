import starpy
Service = starpy._InitSimple("AddFunctionService","123",0,0);
Service._CreateAtomicSysRootItem("TestItem","");
AtomicClass,ErrorInfo = Service._CreateAtomicObjectSimple("TestItem","TestClass","","");
AtomicFunction,ErrorInfo = Service._CreateAtomicFunctionSimple(AtomicClass,"Add","VS_INT32 Add(VS_INT32 x,VS_INT32 y);","",False,False);
#define output event
AtomicOutEvent = Service._CreateAtomicOutEvent(AtomicClass,"OnTest","","",True);
Obj = Service._AtomicToObject(AtomicClass);
def Obj_Add(self,x,y) :  
    return x+y;
Obj.Add = Obj_Add;
# event 
def Obj_OnTest(self, Ev, Args,Type,Val) :
    print("Object Event is triggered");
    print(Ev["_SrcObject"]);
    print(Ev["_DesObject"]);
    print(Ev["_EventID"]);
    print(Ev["_ServiceGroupID"]);
    print(".......Arg..........");
    print( Args );
    print( Type );
    print( Val );
    return True,"Event Result",4567,888.88;
Obj.OnTest = Obj_OnTest;    
