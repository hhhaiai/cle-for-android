/*--------------------------------------------------*/
/*VirtualSociety System ServiceModuleTemplate Main File*/
/*CreateBy SRPLab                */
/*CreateDate: 2011-11-8  */
/*--------------------------------------------------*/
#include "RemoteCallServer_VSHeader.h"

class ClassOfSRPInterface *pSRP;
VS_INT32 SRPProgramType;
VS_HANDLE SRPModuleHandle = NULL;

/*--------------------------------------------------*/
VS_ULONG VSModuleFunction_GetVersion()
{
    return 0x0;
}

/*--------------------------------------------------*/
void VSModuleFunction_GetModuleInfo(VS_INT8 *InfoBuf,VS_INT32 BufSize)
{
    strncpy(InfoBuf,"VS Module",BufSize);
    InfoBuf[BufSize - 1] = 0;
    return;
}

static void VSModuleFunction_GetUserInfoCallBack(VS_UUID ServiceID, VS_INT8 *ServiceName, VS_INT8 *UserNameBuf, VS_INT8 *UserPassBuf)
{
    strncpy( UserNameBuf, "", 128 );
    strncpy( UserPassBuf, "", 128 );
}

void VSModuleFunction_ErrorHandler(VS_CHAR *ErrorInfo)
{
    pSRP -> Print( ErrorInfo );
}

VS_INT32 VSModuleFunction_ModuleInit(VS_ULONG ServiceGroupID, VS_QueryServiceInterfaceProc QueryServiceInterface)
{
    VS_UINT8 MainVersion,SubVersion;
    VS_UINT16 BuildVersion;

    pSRP = QueryServiceInterface( ServiceGroupID, VSOBJID_RemoteCallServer, (VS_GetUserInfoCallBackProc)VSModuleFunction_GetUserInfoCallBack );
    if( pSRP == NULL )
        return -1;
    pSRP -> GetVersion(&MainVersion,&SubVersion,&BuildVersion);
    if( BuildVersion != 1 ){
        pSRP -> Print("this module is not compatible with starcore [%d.%d.%d]",MainVersion,SubVersion,BuildVersion);
        pSRP -> Release();
        return -1;
    }
    SRPProgramType = pSRP -> GetProgramType();
    pSRP -> SetExceptHandler( (VS_ExceptHandlerProc)VSModuleFunction_ErrorHandler );
    return 0;
}

void VSModuleFunction_SetModuleHandle(VS_HANDLE hModuleHandle)
{
    SRPModuleHandle = hModuleHandle;
    return;
}

/*--------------------------------------------------*/
VS_INT32 VSModuleFunction_ModuleTerm()
{
    if( pSRP != NULL )
        pSRP -> Release();
    return 0;
}

/*--------------------------------------------------*/
VS_INT32 VSModuleFunction_RequestRegisterObject( )
{
    TestClass_RequestRegisterObject( );
    return 0;
}

