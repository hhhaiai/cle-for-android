#include "vsopenapi.h"

static class ClassOfSRPInterface *SRPInterface;

static VS_INT32 Add(void *Object,VS_INT32 x,VS_INT32 y)
{
	return x + y;
}

static VS_INT32 EventProc(VS_ULONG FunctionChoice,void *EventPara)
{
	VS_EVENTPARAM *Event;
	VS_PARAPKGPTR ParaPkg;
	VS_EVENTPARAM_RUNPARAM *ResponsePara;

	Event = (VS_EVENTPARAM *)EventPara;
	printf("Object Event is triggered\n");
	printf("%s\n",SRPInterface->GetName(Event->SrcObject));
	printf("%s\n",SRPInterface->GetName(Event->DesObject));
	printf("%s\n",SRPInterface->UuidToString(&Event->EventID));
	printf(".......Arg..........\n");
	printf("%s\n",Event ->RequestParam->ParaPkg->GetStr(0));
	printf("%d\n",Event ->RequestParam->ParaPkg->GetInt(1));
	printf("%f\n",Event ->RequestParam->ParaPkg->GetFloat(2));
	ParaPkg = SRPInterface ->GetParaPkgInterface();
	ParaPkg ->InsertStr(0,"Event Result");
	ParaPkg ->InsertInt(1,4567);
	ParaPkg ->InsertFloat(2,888.88);
	ResponsePara = SRPInterface ->GetResponseBuf();
	ResponsePara ->ParaPkg = ParaPkg;
	SRPInterface ->AttachResponseBuf(Event,ResponsePara);
	return VSEVENTMANAGER_STOP;  // return Value
}


VS_BOOL StarCoreService_Init(class ClassOfStarCore *starcore)
{
	void *AtomicClass,*Add_AtomicFunction,*AtomicEvent;
	class ClassOfBasicSRPInterface *BasicSRPInterface;
	VS_BOOL Result;
	
	//--init star core
	BasicSRPInterface = starcore ->GetBasicInterface();	
	BasicSRPInterface ->CreateService("","AddFunctionService",NULL,"123",0,0,0,0,0,0);
	SRPInterface = BasicSRPInterface ->GetSRPInterface("AddFunctionService","root","123");
	
	//---Create Atomic Class, for define function, no attribute 
	AtomicClass = SRPInterface ->CreateAtomicObjectSimple("TestItem","TestClass",NULL,NULL,NULL);
	Add_AtomicFunction = SRPInterface ->CreateAtomicFunctionSimple(AtomicClass,"Add","VS_INT32 Add(VS_INT32 x,VS_INT32 y);",NULL,NULL,VS_FALSE,VS_FALSE);
	AtomicEvent = SRPInterface -> CreateAtomicOutEvent(AtomicClass,"OnTest",NULL,"",true);
    //---Set Function Address
	SRPInterface -> SetAtomicFunction(Add_AtomicFunction,(void *)Add);
	VS_UUID EventID;
	SRPInterface ->GetAtomicID(AtomicEvent,&EventID);
	Result = SRPInterface -> RegEventFunction(NULL,&EventID,SRPInterface->AtomicToObject(AtomicClass),(void *)EventProc,0);	
	return VS_TRUE;
}

void StarCoreService_Term(class ClassOfStarCore *starcore)
{
	SRPInterface -> Release();
	return;
}