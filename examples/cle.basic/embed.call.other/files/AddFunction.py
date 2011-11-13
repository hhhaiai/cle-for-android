SrvGroup = starpy._GetSrvGroup()
Service = SrvGroup._GetService("","")
Obj=Service._New("TestClass");
def Obj_Add(self,x,y) :
    return x+y+self.Value;  # Value is defined by caller
Obj.Add = Obj_Add;
Obj.ChildValue = 200;