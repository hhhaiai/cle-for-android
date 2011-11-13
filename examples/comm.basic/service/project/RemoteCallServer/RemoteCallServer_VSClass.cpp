/*--------------------------------------------------*/
/*VirtualSociety System ServiceModuleTemplate Class Body File*/
/*CreateBy SRPLab                */
/*CreateDate: 2011-11-8  */
/*--------------------------------------------------*/
#include "RemoteCallServer_VSClass.h"
ClassOfTestClass::ClassOfTestClass()
{
}

ClassOfTestClass::ClassOfTestClass(class ClassOfSRPInterface *In_SRPInterface)
{
    if( In_SRPInterface == NULL )
        return;
    WrapObject(In_SRPInterface,VS_TRUE,VS_FALSE,In_SRPInterface -> MallocObjectL(&VSOBJID_TestClass,0,NULL));
}

ClassOfTestClass::ClassOfTestClass(class ClassOfSRPInterface *In_SRPInterface,void *SRPObject)
{
    if( In_SRPInterface == NULL )
        return;
    WrapObject(In_SRPInterface,VS_FALSE,VS_FALSE,SRPObject);
}

VS_CHAR *ClassOfTestClass::GetSelfName()
{
    static VS_CHAR Buf[64];
    strcpy( Buf, "TestClass" );
    return Buf;
}

class ClassOfTestClass *ClassOfTestClass::GetSRPWrap( class ClassOfSRPInterface *In_SRPInterface,void *SRPObject,VS_ULONG In_ClassLayer)
{
    class ClassOfTestClass *ClassObject;

    if( SRPObject == NULL || In_SRPInterface == NULL )
        return NULL;
    if( In_ClassLayer == 0xFFFFFFFF )
        In_ClassLayer = In_SRPInterface -> GetLayer( SRPObject );
    ClassObject = (class ClassOfTestClass *)In_SRPInterface -> GetAppClass( SRPObject, In_ClassLayer );
    if( ClassObject != NULL && strcmp( ((class ClassOfSRPObject *)ClassObject) -> GetSelfName(), "TestClass" ) == 0 )
        return ClassObject;
    ClassObject = new class ClassOfTestClass();
    ClassObject -> WrapObject(In_SRPInterface,VS_FALSE,VS_TRUE,SRPObject, In_ClassLayer);
    return ClassObject;
}

void ClassOfTestClass::Put_F_GetNumber(void * In_Value)
{
    ThisSRPInterface -> CreateOVLFunction(ThisSRPObject,&VSFUNCID_TestClass_GetNumber,In_Value,NULL);
}
void * ClassOfTestClass::Get_F_GetNumber()
{
    return NULL;
}

void ClassOfTestClass::Put_F_GetNumberF(void * In_Value)
{
    ThisSRPInterface -> CreateOVLFunction(ThisSRPObject,&VSFUNCID_TestClass_GetNumberF,In_Value,NULL);
}
void * ClassOfTestClass::Get_F_GetNumberF()
{
    return NULL;
}

void ClassOfTestClass::Put_F_GetString(void * In_Value)
{
    ThisSRPInterface -> CreateOVLFunction(ThisSRPObject,&VSFUNCID_TestClass_GetString,In_Value,NULL);
}
void * ClassOfTestClass::Get_F_GetString()
{
    return NULL;
}

void ClassOfTestClass::Put_F_GetPkg(void * In_Value)
{
    ThisSRPInterface -> CreateOVLFunction(ThisSRPObject,&VSFUNCID_TestClass_GetPkg,In_Value,NULL);
}
void * ClassOfTestClass::Get_F_GetPkg()
{
    return NULL;
}


VS_INT32 SRPAPI ClassOfTestClass::GetNumber(VS_INT32 Para)
{
    return (VS_INT32 )ThisSRPInterface -> Call(ThisSRPObject,"GetNumber",Para);
}
VS_FLOAT SRPAPI ClassOfTestClass::GetNumberF(VS_FLOAT Para)
{
    return (VS_FLOAT)ThisSRPInterface -> FCall(ThisSRPObject,"GetNumberF",Para);
}
VS_CHAR * SRPAPI ClassOfTestClass::GetString(VS_CHAR * Para)
{
    return (VS_CHAR * )ThisSRPInterface -> Call(ThisSRPObject,"GetString",Para);
}
VS_PARAPKGPTR SRPAPI ClassOfTestClass::GetPkg(VS_PARAPKGPTR Para)
{
    return (VS_PARAPKGPTR )ThisSRPInterface -> Call(ThisSRPObject,"GetPkg",Para);
}

/*--------------------------------------------------*/
