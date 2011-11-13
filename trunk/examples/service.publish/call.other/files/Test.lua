require "libstarcore"
Service = libstarcore._InitSimple("TestService","123",0,0);
--define struct
Service:_CreateAtomicStructSimple("ParaStruct","VS_INT32 Para1;VS_FLOAT Para2;","");
Service:_CreateAtomicObjectSimple("ServiceItem","ParaClass","VS_INT32 Para1;VS_UUID Para2;VS_FLOAT Para3;struct ParaStruct Para4;VS_VSTRING Para5;","");

Obj=Service:_New("TestClass");
function Obj:PrintObj(ParaObj)
  print("ParaObj.Para1=",ParaObj.Para1);
  print("ParaObj.Para2=",ParaObj.Para2);
  print("ParaObj.Para3=",ParaObj.Para3);
  print("ParaObj.Para4.Para1=",ParaObj.Para4.Para1);
  print("ParaObj.Para4.Para2=",ParaObj.Para4.Para2);
  print("ParaObj.Para5=",ParaObj.Para5);
end
