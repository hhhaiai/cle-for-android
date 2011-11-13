require "libstarcore"

if libstarcore._InitCore(true,true,false,true,"",0,"",0) == false then
    return
end

SrvGroup = libstarcore:_GetSrvGroup()
--Create service
SrvGroup:_CreateService( "","RemoteCallServer", "123",5,0,0,0,0,0,"5D0465E1-4203-4d44-9860-8B56C4790BC2" )    
Service = SrvGroup:_GetService("root","123")

--Create service item(object group)
Service:_CreateSysRootItem("TestItem","")
Service:_ActiveSysRootItem("TestItem")
SrvItem = Service:_GetSysRootItem( "TestItem" )
--Create Atomic Class, for define function
AtomicClass = Service:_CreateAtomicObjectSimple("TestItem","TestClass","","3547400A-AFCF-4434-8341-D4FF93D73AAE");
Service:_CreateAtomicFunctionSimple(AtomicClass,"GetNumber","VS_INT32 GetNumber(VS_INT32 Para);","5859F990-57FE-4af7-86B6-9E47CED15444",false,false);
Service:_CreateAtomicFunctionSimple(AtomicClass,"GetNumberF","VS_FLOAT GetNumberF(VS_FLOAT Para);","DFDCBA63-39F3-4984-877A-614A38FCA0F6",false,false);
Service:_CreateAtomicFunctionSimple(AtomicClass,"GetString","VS_CHAR *GetString(VS_CHAR *Para);","0DA48468-A90E-4351-BB38-7BDD520451FE",false,false);
Service:_CreateAtomicFunctionSimple(AtomicClass,"GetPkg","VS_PARAPKGPTR GetPkg(VS_PARAPKGPTR Para);","9C8BFB8F-6C48-4e32-A89A-D41DE3A9627E",false,false);
Service:_CreateAtomicModule("TestModule",0,"0E63CE93-7C1C-4a41-857A-5824E1482023");
Service:_Save("..\\..\\script");

print("save service to ..\\..\\script")

SrvGroup:_ClearService()
libstarcore._ModuleExit()