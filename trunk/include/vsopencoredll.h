#include "vsopencommtype.h"
#include "vsopenapi.h"

#ifndef _VSOPENCOREDLL
#define _VSOPENCOREDLL

#ifndef SRPAPI
#define SRPAPI __cdecl
#endif

//------------------------------------------------------------------------------

#define VSCORE_REGISTERCALLBACKINFO_NAME "VSCore_RegisterCallBackInfo"
typedef void (SRPAPI *VSCore_RegisterCallBackInfoProc)( VS_MsgCallBackProc MsgCallBackProc, VS_ULONG MsgCallBackPara );
#define VSCORE_UNREGISTERCALLBACKINFO_NAME "VSCore_UnRegisterCallBackInfo"
typedef void (SRPAPI *VSCore_UnRegisterCallBackInfoProc)( VS_MsgCallBackProc MsgCallBackProc, VS_ULONG MsgCallBackPara );

#define VSCORE_INIT_NAME "VSCore_Init"
typedef VS_INT32 (SRPAPI *VSCore_InitProc)( VS_BOOL ServerFlag, VS_BOOL SRPPrintFlag, VS_CHAR *DebugInterface, VS_UINT16 PortNumberForDebug, VS_CHAR *ClientInterface, VS_UINT16 PortNumberForDirectClient, VS_STARCONFIGEX *ConfigEx );

#define VSCORE_LUAINIT_NAME "VSCore_LuaInit"
typedef VS_INT32 (SRPAPI *VSCore_LuaInitProc)( VS_INT8 *LuaFile, VS_INT8 **ErrorInfo, VS_CHAR *WorkDirectory, VS_STARCONFIGEX *ConfigEx );   //WorkDirectory maybe NULL

#define VSCORE_LUAINITBUF_NAME "VSCore_LuaInitBuf"
typedef VS_INT32 (SRPAPI *VSCore_LuaInitBufProc)( VS_INT8 *LuaBuf, VS_INT32 LuaBufSize, VS_INT8 *Name, VS_INT8 **ErrorInfo, VS_CHAR *WorkDirectory, VS_STARCONFIGEX *ConfigEx );

#define VSCORE_TERM_NAME "VSCore_Term"
typedef void (SRPAPI *VSCore_TermProc)( );

#define VSCORE_TERMEX_NAME "VSCore_TermEx"
typedef void (SRPAPI *VSCore_TermExProc)( );

#define VSCORE_HASINIT_NAME "VSCore_HasInit"
typedef VS_BOOL (SRPAPI *VSCore_HasInitProc)( );

#define VSCORE_QUERYCONTROLINTERFACE_NAME "VSCore_QueryControlInterface"
typedef class ClassOfSRPControlInterface *(SRPAPI *VSCore_QueryControlInterfaceProc)( );

#define VSCORE_GETSXMLINTERFACE_NAME "VSCore_GetSXMLInterface"
typedef class ClassOfSRPSXMLInterface *(SRPAPI *VSCore_GetSXMLInterfaceProc)( );

#define VSCORE_GETCONFIGPATH_NAME "VSCore_GetConfigPath"
typedef void (SRPAPI *VSCore_GetConfigPathProc)( VS_UINT16 ProgramRunType,VS_CHAR *Buf,VS_INT32 BufSize);

extern void SRPAPI VSCoreLib_RegisterCallBackInfo( VS_MsgCallBackProc MsgCallBackProc, VS_ULONG MsgCallBackPara );
extern void SRPAPI VSCoreLib_UnRegisterCallBackInfo( VS_MsgCallBackProc MsgCallBackProc, VS_ULONG MsgCallBackPara );
extern VS_INT32 SRPAPI VSCoreLib_Init( VS_BOOL ServerFlag, VS_BOOL SRPPrintFlag, VS_CHAR *DebugInterface, VS_UINT16 PortNumberForDebug, VS_CHAR *ClientInterface, VS_UINT16 PortNumberForDirectClient, VS_STARCONFIGEX *ConfigEx );
extern VS_INT32 SRPAPI VSCoreLib_LuaInit( VS_INT8 *LuaFile, VS_INT8 **ErrorInfo, VS_CHAR *WorkDirectory );
extern VS_INT32 SRPAPI VSCoreLib_LuaInitBuf( VS_INT8 *LuaBuf, VS_INT32 LuaBufSize, VS_INT8 *Name, VS_INT8 **ErrorInfo, VS_CHAR *WorkDirectory );
extern void SRPAPI VSCoreLib_Term( );
extern void SRPAPI VSCoreLib_TermEx( );
extern VS_BOOL SRPAPI VSCoreLib_HasInit( );
extern class ClassOfSRPControlInterface *SRPAPI VSCoreLib_QueryControlInterface( );
extern class ClassOfSRPSXMLInterface *SRPAPI VSCoreLib_GetSXMLInterface( );
extern void SRPAPI VSCoreLib_GetConfigPath( VS_UINT16 ProgramRunType,VS_CHAR *Buf,VS_INT32 BufSize);

/*--------------------------------------------------*/
#ifndef _CORELIB
#ifdef _COREDLL
extern "C" SRPDLLEXPORT void SRPAPI VSCore_RegisterCallBackInfo( VS_MsgCallBackProc MsgCallBackProc, VS_ULONG MsgCallBackPara );
extern "C" SRPDLLEXPORT void SRPAPI VSCore_UnRegisterCallBackInfo( VS_MsgCallBackProc MsgCallBackProc, VS_ULONG MsgCallBackPara );
extern "C" SRPDLLEXPORT VS_INT32 SRPAPI VSCore_Init( VS_BOOL ServerFlag, VS_BOOL SRPPrintFlag, VS_CHAR *DebugInterface, VS_UINT16 PortNumberForDebug, VS_CHAR *ClientInterface, VS_UINT16 PortNumberForDirectClient, VS_STARCONFIGEX *ConfigEx );
extern "C" SRPDLLEXPORT VS_INT32 SRPAPI VSCore_LuaInit( VS_INT8 *LuaFile, VS_INT8 **ErrorInfo, VS_CHAR *WorkDirectory, VS_STARCONFIGEX *ConfigEx );
extern "C" SRPDLLEXPORT VS_INT32 SRPAPI VSCore_LuaInitBuf( VS_INT8 *LuaBuf, VS_INT32 LuaBufSize, VS_INT8 *Name, VS_INT8 **ErrorInfo, VS_CHAR *WorkDirectory, VS_STARCONFIGEX *ConfigEx );
extern "C" SRPDLLEXPORT void SRPAPI VSCore_Term( );
extern "C" SRPDLLEXPORT void SRPAPI VSCore_TermEx( );
extern "C" SRPDLLEXPORT VS_BOOL SRPAPI VSCore_HasInit( );
extern "C" SRPDLLEXPORT class ClassOfSRPControlInterface *SRPAPI VSCore_QueryControlInterface( );
extern "C" SRPDLLEXPORT class ClassOfSRPSXMLInterface *SRPAPI VSCore_GetSXMLInterface( );
extern "C" SRPDLLEXPORT void SRPAPI VSCore_GetConfigPath( VS_UINT16 ProgramRunType,VS_CHAR *Buf,VS_INT32 BufSize);
//---lua init function
extern "C" SRPDLLEXPORT int SRPAPI luaopen_libstarcore( void *L );
#else
extern "C" SRPDLLIMPORT void SRPAPI VSCore_RegisterCallBackInfo( VS_MsgCallBackProc MsgCallBackProc, VS_ULONG MsgCallBackPara );
extern "C" SRPDLLIMPORT void SRPAPI VSCore_UnRegisterCallBackInfo( VS_MsgCallBackProc MsgCallBackProc, VS_ULONG MsgCallBackPara );
extern "C" SRPDLLIMPORT VS_INT32 SRPAPI VSCore_Init( VS_BOOL ServerFlag, VS_BOOL SRPPrintFlag, VS_CHAR *DebugInterface, VS_UINT16 PortNumberForDebug, VS_CHAR *ClientInterface, VS_UINT16 PortNumberForDirectClient, VS_STARCONFIGEX *ConfigEx );
extern "C" SRPDLLIMPORT VS_INT32 SRPAPI VSCore_LuaInit( VS_INT8 *LuaFile, VS_INT8 **ErrorInfo, VS_CHAR *WorkDirectory, VS_STARCONFIGEX *ConfigEx );
extern "C" SRPDLLIMPORT VS_INT32 SRPAPI VSCore_LuaInitBuf( VS_INT8 *LuaBuf, VS_INT32 LuaBufSize, VS_INT8 *Name, VS_INT8 **ErrorInfo, VS_CHAR *WorkDirectory, VS_STARCONFIGEX *ConfigEx );
extern "C" SRPDLLIMPORT void SRPAPI VSCore_Term( );
extern "C" SRPDLLIMPORT void SRPAPI VSCore_TermEx( );
extern "C" SRPDLLIMPORT VS_BOOL SRPAPI VSCore_HasInit( );
extern "C" SRPDLLIMPORT class ClassOfSRPControlInterface *SRPAPI VSCore_QueryControlInterface( );
extern "C" SRPDLLIMPORT class ClassOfSRPSXMLInterface *SRPAPI VSCore_GetSXMLInterface( );
extern "C" SRPDLLIMPORT void SRPAPI VSCore_GetConfigPath( VS_UINT16 ProgramRunType,VS_CHAR *Buf,VS_INT32 BufSize);
//---lua init function
extern "C" SRPDLLIMPORT int SRPAPI luaopen_libstarcore( void *L );
#endif
#else
extern void SRPAPI VSCore_RegisterCallBackInfo( VS_MsgCallBackProc MsgCallBackProc, VS_ULONG MsgCallBackPara );
extern void SRPAPI VSCore_UnRegisterCallBackInfo( VS_MsgCallBackProc MsgCallBackProc, VS_ULONG MsgCallBackPara );
extern VS_INT32 SRPAPI VSCore_Init( VS_BOOL ServerFlag, VS_BOOL SRPPrintFlag, VS_CHAR *DebugInterface, VS_UINT16 PortNumberForDebug, VS_CHAR *ClientInterface, VS_UINT16 PortNumberForDirectClient, VS_STARCONFIGEX *ConfigEx );
extern VS_INT32 SRPAPI VSCore_LuaInit( VS_INT8 *LuaFile, VS_INT8 **ErrorInfo, VS_CHAR *WorkDirectory, VS_STARCONFIGEX *ConfigEx );
extern VS_INT32 SRPAPI VSCore_LuaInitBuf( VS_INT8 *LuaBuf, VS_INT32 LuaBufSize, VS_INT8 *Name, VS_INT8 **ErrorInfo, VS_CHAR *WorkDirectory, VS_STARCONFIGEX *ConfigEx );
extern void SRPAPI VSCore_Term( );
extern void SRPAPI VSCore_TermEx( );
extern VS_BOOL SRPAPI VSCore_HasInit( );
extern class ClassOfSRPControlInterface *SRPAPI VSCore_QueryControlInterface( );
extern class ClassOfSRPSXMLInterface *SRPAPI VSCore_GetSXMLInterface( );
extern void SRPAPI VSCore_GetConfigPath( VS_UINT16 ProgramRunType,VS_CHAR *Buf,VS_INT32 BufSize);
//---lua init function
extern int SRPAPI luaopen_libstarcore( void *L );
#endif

#endif
