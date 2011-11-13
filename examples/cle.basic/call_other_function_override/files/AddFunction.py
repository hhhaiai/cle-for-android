import starpy
Service = starpy._InitSimple("AddFunctionService","123",0,0);
Obj=Service._New("TestClass");
def Obj_Add(self,x,y) :
    return x+y;
Obj.Add = Obj_Add;