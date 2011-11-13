#include "vsopenapi.h"

static class ClassOfSRPInterface *SRPInterface;

//--define struct
struct ParaStruct{
	VS_INT32 Para1;
	VS_FLOAT Para2;
};

struct ParaClass{
	VS_INT32 Para1;
	VS_UUID Para2;
	VS_FLOAT Para3;
	struct ParaStruct Para4;
	VS_VSTRING Para5;
};

static void PrintObj(void *Object,struct ParaClass *ParaObj)
{
	SRPInterface ->Print("ParaObj.Para1=%d\n",ParaObj->Para1);
	SRPInterface ->Print("ParaObj.Para2=%s\n",SRPInterface->UuidToString(&ParaObj->Para2));
	SRPInterface ->Print("ParaObj.Para3=%f\n",ParaObj->Para3);
	SRPInterface ->Print("ParaObj.Para4.Para1=%d\n",ParaObj->Para4.Para1);
	SRPInterface ->Print("ParaObj.Para4.Para2=%f\n",ParaObj->Para4.Para2);
    SRPInterface ->Print("ParaObj.Para5=%s\n",ParaObj->Para5.Buf);
}

VS_BOOL StarCoreService_Init(class ClassOfStarCore *starcore)
{
	void *AtomicClass,*PrintObjFunction;
	class ClassOfBasicSRPInterface *BasicSRPInterface;
	
	//--init star core
	BasicSRPInterface = starcore ->GetBasicInterface();	
	BasicSRPInterface ->CreateService("","TestService",NULL,"123",0,0,0,0,0,0);
	SRPInterface = BasicSRPInterface ->GetSRPInterface("TestService","root","123");
	
	//---Create Atomic Class, for define function, no attribute 
	SRPInterface ->CreateAtomicStructSimple("ParaStruct","VS_INT32 Para1;VS_FLOAT Para2;",NULL,NULL);
	SRPInterface ->CreateAtomicObjectSimple("TestItem","ParaClass","VS_INT32 Para1;VS_UUID Para2;VS_FLOAT Para3;struct ParaStruct Para4;VS_VSTRING Para5;",NULL,NULL);


	AtomicClass = SRPInterface ->CreateAtomicObjectSimple("TestItem","TestClass",NULL,NULL,NULL);
	PrintObjFunction = SRPInterface ->CreateAtomicFunctionSimple(AtomicClass,"PrintObj","void PrintObj(VS_OBJPTR ParaObj);",NULL,NULL,VS_FALSE,VS_FALSE);
    //---Set Function Address
	SRPInterface -> SetAtomicFunction(PrintObjFunction,(void *)PrintObj);
	return VS_TRUE;
}

void StarCoreService_Term(class ClassOfStarCore *starcore)
{
	SRPInterface -> Release();
	return;
}