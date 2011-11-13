/*--------------------------------------------------*/
/*VirtualSociety System ServiceModuleTemplate Main File*/
/*CreateBy SRPLab                */
/*CreateDate: 2011-11-8  */
/*--------------------------------------------------*/
#include "RemoteCallServer_VSHeader.h"

/*++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*Function TestClass Is Extracted From ScriptFile */
/*++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*++++++++++++++++++++++++++++++++++++++++++++++++++*/
#define TESTCLASS_OBJECT_DEPENDINDEX                                                                 0x00000001
#define TESTCLASS_GETNUMBER_FUNCRETURN_DEPENDINDEX                                                   0X00000002
#define TESTCLASS_GETNUMBER_FUNCPARAM_DEPENDINDEX                                                    0X00000003
#define TESTCLASS_GETNUMBERF_FUNCRETURN_DEPENDINDEX                                                  0X00000004
#define TESTCLASS_GETNUMBERF_FUNCPARAM_DEPENDINDEX                                                   0X00000005
#define TESTCLASS_GETSTRING_FUNCRETURN_DEPENDINDEX                                                   0X00000006
#define TESTCLASS_GETSTRING_FUNCPARAM_DEPENDINDEX                                                    0X00000007
#define TESTCLASS_GETPKG_FUNCRETURN_DEPENDINDEX                                                      0X00000008
#define TESTCLASS_GETPKG_FUNCPARAM_DEPENDINDEX                                                       0X00000009

VS_INT32 SRPCALLBACK TestClass_QueryObjectInfo(VS_OBJECTMODULEINFO *VSObjectModuleInfo)
{
    VSObjectModuleInfo -> ObjectVersion = 0;
    VSObjectModuleInfo -> ObjectSubVersion = 0;
    VSObjectModuleInfo -> ObjcetBugFixVersion = 0;
    VSObjectModuleInfo -> VSVersion = 0;
    VSObjectModuleInfo -> VSSubVersion = 0;

    VSObjectModuleInfo -> CreateTime.wYear = 2011;
    VSObjectModuleInfo -> CreateTime.wMonth = 11;
    VSObjectModuleInfo -> CreateTime.wDay = 8;
    VSObjectModuleInfo -> UpdateTime.wYear = 2011;
    VSObjectModuleInfo -> UpdateTime.wMonth = 11;
    VSObjectModuleInfo -> UpdateTime.wDay = 8;

    strcpy( VSObjectModuleInfo -> AuthorityInfo, "" );
    strcpy( VSObjectModuleInfo -> LicenseInfo, "" );
    strcpy( VSObjectModuleInfo -> ExtendInfo, "" );

    return 0;
}

VS_INT32 SRPCALLBACK TestClass_RequestRegisterObject()
{
    VS_DEPENDATTRIBUTE LocalDependAttribute[512];  /*---set a large number*/
    VS_INT32 DependIndexNumber,DependIndex[64];

    //------register object depend
    VSATTRDEPEND_TESTCLASS(LocalDependAttribute);
    pSRP -> RegisterObjectDependency( VSIMPORTMODULEID_TestModule,VSOBJID_TestClass,TESTCLASS_OBJECT_DEPENDINDEX,VSOBJID_TestClass,VSMODULEDEPEND_OBJECT,VSATTRDEPENDNUM_TESTCLASS,LocalDependAttribute);

    //------register function depend
    VSFUNCRETURNDEPEND_TESTCLASS_GETNUMBER(LocalDependAttribute);
    pSRP -> RegisterObjectDependency( VSIMPORTMODULEID_TestModule,VSOBJID_TestClass,TESTCLASS_GETNUMBER_FUNCRETURN_DEPENDINDEX,VSFUNCID_TestClass_GetNumber,VSMODULEDEPEND_FUNCRETURN,VSFUNCRETURNDEPENDNUM_TESTCLASS_GETNUMBER,LocalDependAttribute);

    VSFUNCPARAMDEPEND_TESTCLASS_GETNUMBER(LocalDependAttribute);
    pSRP -> RegisterObjectDependency( VSIMPORTMODULEID_TestModule,VSOBJID_TestClass,TESTCLASS_GETNUMBER_FUNCPARAM_DEPENDINDEX,VSFUNCID_TestClass_GetNumber,VSMODULEDEPEND_FUNCPARA,VSFUNCPARAMDEPENDNUM_TESTCLASS_GETNUMBER,LocalDependAttribute);

    VSFUNCRETURNDEPEND_TESTCLASS_GETNUMBERF(LocalDependAttribute);
    pSRP -> RegisterObjectDependency( VSIMPORTMODULEID_TestModule,VSOBJID_TestClass,TESTCLASS_GETNUMBERF_FUNCRETURN_DEPENDINDEX,VSFUNCID_TestClass_GetNumberF,VSMODULEDEPEND_FUNCRETURN,VSFUNCRETURNDEPENDNUM_TESTCLASS_GETNUMBERF,LocalDependAttribute);

    VSFUNCPARAMDEPEND_TESTCLASS_GETNUMBERF(LocalDependAttribute);
    pSRP -> RegisterObjectDependency( VSIMPORTMODULEID_TestModule,VSOBJID_TestClass,TESTCLASS_GETNUMBERF_FUNCPARAM_DEPENDINDEX,VSFUNCID_TestClass_GetNumberF,VSMODULEDEPEND_FUNCPARA,VSFUNCPARAMDEPENDNUM_TESTCLASS_GETNUMBERF,LocalDependAttribute);

    VSFUNCRETURNDEPEND_TESTCLASS_GETSTRING(LocalDependAttribute);
    pSRP -> RegisterObjectDependency( VSIMPORTMODULEID_TestModule,VSOBJID_TestClass,TESTCLASS_GETSTRING_FUNCRETURN_DEPENDINDEX,VSFUNCID_TestClass_GetString,VSMODULEDEPEND_FUNCRETURN,VSFUNCRETURNDEPENDNUM_TESTCLASS_GETSTRING,LocalDependAttribute);

    VSFUNCPARAMDEPEND_TESTCLASS_GETSTRING(LocalDependAttribute);
    pSRP -> RegisterObjectDependency( VSIMPORTMODULEID_TestModule,VSOBJID_TestClass,TESTCLASS_GETSTRING_FUNCPARAM_DEPENDINDEX,VSFUNCID_TestClass_GetString,VSMODULEDEPEND_FUNCPARA,VSFUNCPARAMDEPENDNUM_TESTCLASS_GETSTRING,LocalDependAttribute);

    VSFUNCRETURNDEPEND_TESTCLASS_GETPKG(LocalDependAttribute);
    pSRP -> RegisterObjectDependency( VSIMPORTMODULEID_TestModule,VSOBJID_TestClass,TESTCLASS_GETPKG_FUNCRETURN_DEPENDINDEX,VSFUNCID_TestClass_GetPkg,VSMODULEDEPEND_FUNCRETURN,VSFUNCRETURNDEPENDNUM_TESTCLASS_GETPKG,LocalDependAttribute);

    VSFUNCPARAMDEPEND_TESTCLASS_GETPKG(LocalDependAttribute);
    pSRP -> RegisterObjectDependency( VSIMPORTMODULEID_TestModule,VSOBJID_TestClass,TESTCLASS_GETPKG_FUNCPARAM_DEPENDINDEX,VSFUNCID_TestClass_GetPkg,VSMODULEDEPEND_FUNCPARA,VSFUNCPARAMDEPENDNUM_TESTCLASS_GETPKG,LocalDependAttribute);

    //------object function
    DependIndexNumber = 3;
    DependIndex[0] = TESTCLASS_OBJECT_DEPENDINDEX;
    DependIndex[1] = TESTCLASS_GETNUMBER_FUNCRETURN_DEPENDINDEX;
    DependIndex[2] = TESTCLASS_GETNUMBER_FUNCPARAM_DEPENDINDEX;
    pSRP -> RegisterObjectFunction( VSIMPORTMODULEID_TestModule,VSOBJID_TestClass,VSFUNCID_TestClass_GetNumber,(void *)TestClass_GetNumber,DependIndexNumber,DependIndex);

    DependIndexNumber = 3;
    DependIndex[0] = TESTCLASS_OBJECT_DEPENDINDEX;
    DependIndex[1] = TESTCLASS_GETNUMBERF_FUNCRETURN_DEPENDINDEX;
    DependIndex[2] = TESTCLASS_GETNUMBERF_FUNCPARAM_DEPENDINDEX;
    pSRP -> RegisterObjectFunction( VSIMPORTMODULEID_TestModule,VSOBJID_TestClass,VSFUNCID_TestClass_GetNumberF,(void *)TestClass_GetNumberF,DependIndexNumber,DependIndex);

    DependIndexNumber = 3;
    DependIndex[0] = TESTCLASS_OBJECT_DEPENDINDEX;
    DependIndex[1] = TESTCLASS_GETSTRING_FUNCRETURN_DEPENDINDEX;
    DependIndex[2] = TESTCLASS_GETSTRING_FUNCPARAM_DEPENDINDEX;
    pSRP -> RegisterObjectFunction( VSIMPORTMODULEID_TestModule,VSOBJID_TestClass,VSFUNCID_TestClass_GetString,(void *)TestClass_GetString,DependIndexNumber,DependIndex);

    DependIndexNumber = 3;
    DependIndex[0] = TESTCLASS_OBJECT_DEPENDINDEX;
    DependIndex[1] = TESTCLASS_GETPKG_FUNCRETURN_DEPENDINDEX;
    DependIndex[2] = TESTCLASS_GETPKG_FUNCPARAM_DEPENDINDEX;
    pSRP -> RegisterObjectFunction( VSIMPORTMODULEID_TestModule,VSOBJID_TestClass,VSFUNCID_TestClass_GetPkg,(void *)TestClass_GetPkg,DependIndexNumber,DependIndex);

    //------object message process
    pSRP -> RegisterQueryObjectInfo( VSIMPORTMODULEID_TestModule,VSOBJID_TestClass,(void *)TestClass_QueryObjectInfo);

    return 0;
}

