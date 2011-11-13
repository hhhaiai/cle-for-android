#include "vsopenapi.h"

static VS_INT32 Add(void *Object,VS_INT32 x,VS_INT32 y)
{
	return x + y;
}

VS_BOOL StarCoreService_Init(class ClassOfStarCore *starcore)
{
	void *AtomicClass,*Add_AtomicFunction;
	class ClassOfBasicSRPInterface *BasicSRPInterface;
    class ClassOfSRPInterface *SRPInterface;
	
	//--init star core
	BasicSRPInterface = starcore ->GetBasicInterface();	
	SRPInterface = BasicSRPInterface ->GetSRPInterface(BasicSRPInterface->QueryActiveService(NULL),"","");
	
	//---Create Atomic Class, for define function, no attribute 
	AtomicClass = SRPInterface ->CreateAtomicObjectSimple("TestItem","TestClass",NULL,NULL,NULL);
	Add_AtomicFunction = SRPInterface ->CreateAtomicFunctionSimple(AtomicClass,"Add","VS_INT32 Add(VS_INT32 x,VS_INT32 y);",NULL,NULL,VS_FALSE,VS_FALSE);
    //---Set Function Address
	SRPInterface -> SetAtomicFunction(Add_AtomicFunction,(void *)Add);
	SRPInterface -> Release();
	return VS_TRUE;
}

void StarCoreService_Term(class ClassOfStarCore *starcore)
{
	return;
}