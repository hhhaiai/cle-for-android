/*--------------------------------------------------*/
/*VirtualSociety System ServiceModuleTemplate Class Header File*/
/*CreateBy SRPLab                */
/*CreateDate: 2011-11-8  */
/*--------------------------------------------------*/
#ifndef REMOTECALLSERVER_CLASSHEADERFILE
#define REMOTECALLSERVER_CLASSHEADERFILE

#define _INCLUDE_STARLIB
#include "RemoteCallServer_VSHeader.h"

#if( VS_OS_TYPE == VS_OS_WINDOWS )
#pragma warning (disable:4819)
#pragma warning (disable:4244)
#pragma warning (disable:4996)
#pragma warning (disable:4800)
#endif

#pragma pack(4)

class ClassOfTestClass:public ClassOfSRPObject{
public:
    ClassOfTestClass();  //--Not Create a Class Object, Use WrapObject() to attach 
    ClassOfTestClass(class ClassOfSRPInterface *In_SRPInterface);  //--Create a Class Object
    ClassOfTestClass(class ClassOfSRPInterface *In_SRPInterface,void *SRPObject);  //--Create a Class Object and wrap SRPObject
    virtual VS_CHAR *SRPAPI GetSelfName();
    static class ClassOfTestClass *SRPAPI GetSRPWrap( class ClassOfSRPInterface *In_SRPInterface,void *SRPObject,VS_ULONG In_ClassLayer = 0xFFFFFFFF);
public:
    //--PubFunction Get/Put Function Define
    void Put_F_GetNumber(void * In_Value);
    void * Get_F_GetNumber();

    void Put_F_GetNumberF(void * In_Value);
    void * Get_F_GetNumberF();

    void Put_F_GetString(void * In_Value);
    void * Get_F_GetString();

    void Put_F_GetPkg(void * In_Value);
    void * Get_F_GetPkg();


#if( VS_OS_TYPE == VS_OS_WINDOW )
public:
    //--PubFunction Property Define
    __declspec(property(get=Get_F_GetNumber, put=Put_F_GetNumber)) void * F_GetNumber;
    __declspec(property(get=Get_F_GetNumberF, put=Put_F_GetNumberF)) void * F_GetNumberF;
    __declspec(property(get=Get_F_GetString, put=Put_F_GetString)) void * F_GetString;
    __declspec(property(get=Get_F_GetPkg, put=Put_F_GetPkg)) void * F_GetPkg;
#endif

public:
    //--Function Define
    VS_INT32 SRPAPI GetNumber(VS_INT32 Para);
    VS_FLOAT SRPAPI GetNumberF(VS_FLOAT Para);
    VS_CHAR * SRPAPI GetString(VS_CHAR * Para);
    VS_PARAPKGPTR SRPAPI GetPkg(VS_PARAPKGPTR Para);
};

/*--------------------------------------------------*/

#pragma pack()

#endif
