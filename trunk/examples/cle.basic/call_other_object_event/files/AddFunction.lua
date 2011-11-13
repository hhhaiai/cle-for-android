require "libstarcore"
Service = libstarcore._InitSimple("AddFunctionService","123",0,0);
AtomicClass,ErrorInfo = Service:_CreateAtomicObjectSimple("TestItem","TestClass","","");
AtomicFunction,ErrorInfo = Service:_CreateAtomicFunctionSimple(AtomicClass,"Add","VS_INT32 Add(VS_INT32 x,VS_INT32 y);","",false,false);
AtomicOutEvent = Service:_CreateAtomicOutEvent(AtomicClass,"OnTest","","",true);

Obj = Service:_AtomicToObject(AtomicClass);
function Obj:Add(x,y)
  return x+y;
end
function Obj:OnTest(Ev, Args,Type,Val)
	print("Object Event is triggered");
	print(Ev._SrcObject);
	print(Ev._DesObject);
	print(Ev._EventID);
	print(Ev._ServiceGroupID);
	print(".......Arg..........");
	print( Args );
	print( Type );
	print( Val );
    return true,"Event Result",4567,888.88;
end    
