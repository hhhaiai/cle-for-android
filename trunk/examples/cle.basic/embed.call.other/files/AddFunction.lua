SrvGroup = libstarcore._GetSrvGroup()
Service = SrvGroup:_GetService("","")
Obj=Service:_New("TestClass");
function Obj:Add(x,y)
  return x+y+self.Value;  --Value is defined by caller
end
Obj.ChildValue = 200;