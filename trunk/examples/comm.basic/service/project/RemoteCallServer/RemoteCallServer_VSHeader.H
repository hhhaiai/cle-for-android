/*--------------------------------------------------*/
/*VirtualSociety System ServiceModuleTemplate Header File*/
/*CreateBy SRPLab                */
/*CreateDate: 2011-11-8  */
/*--------------------------------------------------*/
#ifndef REMOTECALLSERVER_HEADERFILE
#define REMOTECALLSERVER_HEADERFILE

#include "RemoteCallServer.h"
#if( VS_OS_TYPE == VS_OS_WINDOWS )

#pragma warning (disable:4819)
#pragma warning (disable:4244)
#pragma warning (disable:4996)
#pragma warning (disable:4800)
#endif

#pragma pack(4)

/*--------------------------------------------------*/
extern "C" SRPDLLEXPORT VS_ULONG VSModuleFunction_GetVersion();
extern "C" SRPDLLEXPORT void VSModuleFunction_GetModuleInfo(VS_INT8 *InfoBuf,VS_INT32 BufSize);
extern "C" SRPDLLEXPORT VS_INT32 VSModuleFunction_ModuleInit(VS_ULONG ServiceGroupID, VS_QueryServiceInterfaceProc QueryServiceInterface);
extern "C" SRPDLLEXPORT void VSModuleFunction_SetModuleHandle(VS_HANDLE hModuleHandle);
extern "C" SRPDLLEXPORT VS_INT32 VSModuleFunction_ModuleTerm();
extern "C" SRPDLLEXPORT VS_INT32 VSModuleFunction_RequestRegisterObject();

/*++++++++++++++++++++++++++++++++++++++++++++++++++*/
//----output sysrootitem name and ID
#define VSOBJNAME_TESTITEM                                                                 "TestItem"
extern VS_UUID VSOBJID_TestItem;

/*++++++++++++++++++++++++++++++++++++++++++++++++++*/
#define VSIMPORTMODULENAME_TESTMODULE                                                      "TestModule"  
extern VS_UUID VSIMPORTMODULEID_TestModule;

/*++++++++++++++++++++++++++++++++++++++++++++++++++*/
#define VSOBJNAME_REMOTECALLSERVER                                                                 "RemoteCallServer"
extern VS_UUID VSOBJID_RemoteCallServer;

/*++++++++++++++++++++++++++++++++++++++++++++++++++*/
#define VSOBJNAME_TESTCLASS                                                                        "TestClass"
extern VS_UUID VSOBJID_TestClass;
#define VSATTRDEPEND_TESTCLASS(X)  {}
#define VSATTRDEPENDNUM_TESTCLASS                                                                  0
extern VS_INT32 SRPCALLBACK TestClass_RequestRegisterObject( );

/*------Variable Index Define */
#define VSATTRNUMBER_TESTCLASS                                                                     0

/*------Local Define Function */
/*[Public Function] :     */
extern VS_UUID VSFUNCID_TestClass_GetNumber;
#define VSFUNCRETURNDEPEND_TESTCLASS_GETNUMBER(X)  {{X[0].Type=6;}}
#define VSFUNCRETURNDEPENDNUM_TESTCLASS_GETNUMBER                                                  1
#define VSFUNCPARAMDEPEND_TESTCLASS_GETNUMBER(X)  {{X[0].Type=6;}}
#define VSFUNCPARAMDEPENDNUM_TESTCLASS_GETNUMBER                                                   1

extern VS_INT32 SRPAPI TestClass_GetNumber(void *Object,VS_INT32 Para);
typedef VS_INT32 (SRPAPI *TestClass_GetNumberProc)(void *Object,VS_INT32 Para);

/*[Public Function] :     */
extern VS_UUID VSFUNCID_TestClass_GetNumberF;
#define VSFUNCRETURNDEPEND_TESTCLASS_GETNUMBERF(X)  {{X[0].Type=8;}}
#define VSFUNCRETURNDEPENDNUM_TESTCLASS_GETNUMBERF                                                 1
#define VSFUNCPARAMDEPEND_TESTCLASS_GETNUMBERF(X)  {{X[0].Type=8;}}
#define VSFUNCPARAMDEPENDNUM_TESTCLASS_GETNUMBERF                                                  1

extern VS_FLOAT SRPAPI TestClass_GetNumberF(void *Object,VS_FLOAT Para);
typedef VS_FLOAT (SRPAPI *TestClass_GetNumberFProc)(void *Object,VS_FLOAT Para);

/*[Public Function] :     */
extern VS_UUID VSFUNCID_TestClass_GetString;
#define VSFUNCRETURNDEPEND_TESTCLASS_GETSTRING(X)  {{X[0].Type=30;}}
#define VSFUNCRETURNDEPENDNUM_TESTCLASS_GETSTRING                                                  1
#define VSFUNCPARAMDEPEND_TESTCLASS_GETSTRING(X)  {{X[0].Type=30;}}
#define VSFUNCPARAMDEPENDNUM_TESTCLASS_GETSTRING                                                   1

extern VS_CHAR * SRPAPI TestClass_GetString(void *Object,VS_CHAR * Para);
typedef VS_CHAR * (SRPAPI *TestClass_GetStringProc)(void *Object,VS_CHAR * Para);

/*[Public Function] :     */
extern VS_UUID VSFUNCID_TestClass_GetPkg;
#define VSFUNCRETURNDEPEND_TESTCLASS_GETPKG(X)  {{X[0].Type=40;}}
#define VSFUNCRETURNDEPENDNUM_TESTCLASS_GETPKG                                                     1
#define VSFUNCPARAMDEPEND_TESTCLASS_GETPKG(X)  {{X[0].Type=40;}}
#define VSFUNCPARAMDEPENDNUM_TESTCLASS_GETPKG                                                      1

extern VS_PARAPKGPTR SRPAPI TestClass_GetPkg(void *Object,VS_PARAPKGPTR Para);
typedef VS_PARAPKGPTR (SRPAPI *TestClass_GetPkgProc)(void *Object,VS_PARAPKGPTR Para);

struct StructOfAttachTestClass{
};
struct StructOfTestClass{
};

/*++++++++++++++++++++++++++++++++++++++++++++++++++*/
//#define VSOBJNAME_TESTITEM                                                                         "TestItem"
//UUID VSOBJID_TestItem;
/*--------------------------------------------------*/
extern class ClassOfSRPInterface *pSRP;
extern VS_INT32 SRPProgramType;
extern VS_HANDLE SRPModuleHandle;

#pragma pack()

#endif
