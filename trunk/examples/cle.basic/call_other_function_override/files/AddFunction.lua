require "libstarcore"
Service = libstarcore._InitSimple("AddFunctionService","123",0,0);
Obj=Service:_New("TestClass");
function Obj:Add(x,y)
  return x+y;
end