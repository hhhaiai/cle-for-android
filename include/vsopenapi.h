#ifndef EXTERNALAPPLICATIONAPI
#define EXTERNALAPPLICATIONAPI

#include "vsopencommtype.h"
extern "C"{
	#include "vs_shell.h"
}
#include "vsopensyseventdef.h"
#include "vscoreshell.h"

typedef void (SRPAPI *VSModuleFunction_QueryObjectInfoProc)(VS_OBJECTMODULEINFO *VSObjectModuleInfo);
typedef  VS_BOOL (*VS_SRPMessageProcessHookProc)( VS_HWND hWnd,VS_ULONG message, VS_ULONG wParam, VS_ULONG lParam );  //--Returns true that processed the message, otherwise no
typedef void (SRPAPI *VS_TimerProc)(void *Object,VS_ULONG TimerID,VS_ULONG Para1,VS_ULONG Para2,VS_ULONG Para3,VS_ULONG Para4);
typedef class ClassOfSRPInterface *(SRPAPI *VS_QueryServiceInterfaceProc)( VS_ULONG ServiceGroupID, VS_UUID ServiceID,VS_GetUserInfoCallBackProc CallBackProc );
typedef void (SRPAPI *VSModuleFunction_ScriptCompileHookProc)(void *L, void *Object, VS_ULONG Para,VS_CHAR *ScriptName, VS_CHAR *ScriptBuf);
typedef void (SRPAPI *VS_ExceptHandlerProc)(VS_CHAR *ErrorInfo);

//---------trigger by object attribute change
typedef VS_BOOL (SRPAPI *VS_ObjectBeforeChangeNotifyProc)(void *Object,VS_ULONG Para,OBJECTATTRIBUTEINDEX AttributeIndex,VS_INT8 *NewValue,VS_INT32 DebugEditFlag);  //--Returns true to allow change, or not allowed to change
typedef void (SRPAPI *VS_ObjectChangeNotifyProc)(void *Object,VS_ULONG Para,OBJECTATTRIBUTEINDEX AttributeIndex,VS_ATTRIBUTEINDEXMAP *AttributeIndexMap);
typedef void (SRPAPI *VS_ObjectChangeNotifyExProc)(void *Object,void *DesObject,VS_ULONG Para,OBJECTATTRIBUTEINDEX AttributeIndex,VS_ATTRIBUTEINDEXMAP *AttributeIndexMap);
typedef void (SRPAPI *VS_ObjectReMallocNotifyProc)(void *Object,VS_ULONG Para,void *NewObject); //---NewObject == NULL free
typedef void (SRPAPI *VS_ObjectIDChangeNotifyProc)(void *Object,VS_ULONG Para,VS_UUID *NewObjectID);
typedef void (SRPAPI *VS_ObjectFreeNotifyProc)(void *Object,VS_ULONG Para);

//---------trigger by object namevalue change
typedef void (SRPAPI *VS_ObjectNameValueChangeNotifyProc)(void *Object,VS_ULONG Para,VS_CHAR *Name,VS_ULONG NameHashValue);

typedef void (SRPAPI *VS_QueryObjectStaticDataProc)(VS_UUID *ObjectID,VS_ULONG UniqueDataUnitID,VS_STATICID *DataVersion,VS_ULONG *DataSize, VS_UINT8 *StaticSaveFlag, VS_BOOL AutoDownLoad, VS_BOOL *ContinueDefaultProcess,VS_STATICID *LocalDataVersion,VS_CHAR *Token,VS_BOOL *CachedToDiskFlag,VS_ULONG Para);
typedef void (SRPAPI *VS_SaveObjectStaticDataProc)(VS_UUID *ObjectID,VS_ULONG UniqueDataUnitID,VS_STATICID Version,VS_ULONG DataSize,VS_INT8 *DataBuf, VS_UINT8 StaticSaveFlag, VS_BOOL *ContinueDefaultProcess,VS_BOOL *CachedToDiskFlag,VS_ULONG Para);
typedef void (SRPAPI *VS_ClearObjectStaticDataProc)(VS_UUID *ObjectID,VS_ULONG UniqueDataUnitID,VS_BOOL *ContinueDefaultProcess);  //object is free

//---------Lua disp info
typedef void (SRPAPI *VS_LuaInfoDispProc)(VS_CHAR *DispInfo,VS_ULONG Para);

//---------object register lua value function
typedef VS_BOOL (SRPAPI *VS_LuaGetValueProc)(void *Object,VS_CHAR *Name,VS_ULONG Para);    //---lua's _GetIndex,if return VS_TRUE,then the value is on stacktop
typedef VS_BOOL (SRPAPI *VS_LuaSetValueProc)(void *Object,VS_CHAR *Name,VS_INT32 Index,VS_ULONG Para);    //---lua's _NewIndex
typedef VS_INT32 (SRPAPI *VS_LuaUserDataGCProc)(void *L);

//---------sysrootitem change sync on client side
typedef void (SRPAPI *VS_ClientSysRootItemChangeToSyncProc)(void *SysRootItem,VS_ULONG ClientID,VS_ULONG SyncGroupIndex,VS_ULONG Para);

typedef VS_INT32 (SRPAPI *VS_IndexCompareProc)(void *NodeBuf1,void *NodeBuf2);  //-- -1 Buf1 < Buf2,  0 Buf1 == Buf2,  1 Buf1 > Buf2
//---only support 1 ~ 3 VS_LONG Key

//---------info disp window
typedef void (SRPAPI *VS_MessageBoxProc)(void *Object,VS_CHAR *Caption,VS_CHAR *Info);

//---------set or get env info,Env Type is EnvClassTag，defined by app
struct StructOfVSRunEnv_Para{
    VS_INT32 BufSize;
    VS_INT8  Buf[1];
};
struct StructOfVSRunEnv{
    VS_ULONG EnvClassTag;  //--Env Type  0,0x80000000-0xFFFFFFFF is reserved
    VS_UUID  SrcObjectID;
    struct StructOfVSRunEnv_Para *RequestPara;       //--Request
    struct StructOfVSRunEnv_Para *ResponsePara;      //--Response
};

//env/parentobject <-> [RunEnvCallBack]object[RunEnvCallBack]  <-> childobject
typedef VS_BOOL (SRPAPI *VS_RunEnvCallBackProc)(void *Object,VS_ULONG Para,struct StructOfVSRunEnv *EnvInfo);

//---------response of remotecall
typedef void (SRPAPI *VS_RemoteCallResponseProc)(void *Object,VS_ULONG Para,VS_ULONG RetCode,VS_ULONG RetValue);

//---------when load, map object attribute name
typedef VS_CHAR *(SRPAPI *VS_LoadMapAttrNameProc)(void *Object,VS_CHAR *LoadAttrName);

//---------gc function
typedef void (SRPAPI *VS_GCProc)( VS_ULONG Para );

//---------sharelib callback function
typedef VS_ULONG (SRPAPI *VS_MsgCallBackProc)( VS_ULONG ServiceGroupID, VS_ULONG uMsg, VS_ULONG wParam, VS_ULONG lParam, VS_BOOL &IsProcessed, VS_ULONG Para );

//---------wait event callback
typedef void (SRPAPI *VS_WaitEventCallBackProc)(void *SrcObject,void *EventParam,void *Object,VS_ULONG Para);

//---------Dispatch callback
typedef void (SRPAPI *VS_SRPDispatchCallBackProc)(VS_ULONG Para);

//---------Redirect To Url Info Call Back, the first para is type,defined as follow:
#define SRPENVREDIRECT_DISPINFO   0
#define SRPENVREDIRECT_START      1
#define SRPENVREDIRECT_FAIL       2
#define SRPENVREDIRECT_ABORT      3
//---------if type is DISPINFO，then the second para is string, the third para is max download size(MaxLength), the third para is current down size(CurLength)
typedef void (SRPAPI *RedirectToUrl_InfoProc)( class ClassOfBasicSRPInterface *BasicSRPInterface, VS_ULONG Para, VS_CHAR *Url, class ClassOfSRPParaPackageInterface *ParaPkg );
typedef void (SRPAPI *VS_WebDownInfoProc)( VS_ULONG Para, VS_ULONG uMes, VS_CHAR *FileName, VS_ULONG MaxSize, VS_ULONG CurSize );
#define SRPENVSTART_FROMDIR             ((VS_UINT8)0x00)
#define SRPENVSTART_FROMSINGLE          ((VS_UINT8)0x01)
#define SRPENVSTART_FROMCHILDDIR        ((VS_UINT8)0x10)
#define SRPENVSTART_FROMCHILDSINGLE     ((VS_UINT8)0x11)

#define SRPLOADPROCESS_OK        0
#define SRPLOADPROCESS_BUSY      -1
#define SRPLOADPROCESS_DISABLE   -2
#define SRPLOADPROCESS_FAIL      -3

//--------------------------------------------------------------------------
#define SRPLUAEDITMODULECONFIG_SCRIPTCONSOLE 0x00000001
#define SRPLUAEDITMODULECONFIG_PROJECT       0x00000002
#define SRPLUAEDITMODULECONFIG_SRPDOC        0x00000004

#define SRPLUAEDITMODULE_OPEN "SRPLuaEditModule_Open"
typedef VS_BOOL (SRPAPI *SRPLuaEditModule_OpenProc)( class ClassOfSRPControlInterface *ControlInterface, VS_HWND ParentWnd,VS_BOOL CloseEnable, VS_INT32 Type, VS_ULONG Config ); //--Type=0 ServerExe  =1 ServerDll =2 clientexe ==3 clientdll other debugserver
extern "C" SRPDLLEXPORT VS_BOOL SRPAPI SRPLuaEditModule_Open(class ClassOfSRPControlInterface *ControlInterface, VS_HWND ParentWnd,VS_BOOL CloseEnable, VS_INT32 Type, VS_ULONG Config );

#define SRPLUAEDITMODULE_DISP "SRPLuaEditModule_Disp"
typedef void (SRPAPI *SRPLuaEditModule_DispProc)( VS_CHAR *Info );
extern "C" SRPDLLEXPORT void SRPAPI SRPLuaEditModule_Disp( VS_CHAR *Info );

#define SRPLUAEDITMODULE_HELP "SRPLuaEditModule_Help"
typedef void (SRPAPI *SRPLuaEditModule_HelpProc)( VS_INT32 Type, VS_CHAR *HelpInfo ); //Type=0 help info =1 Examples project
extern "C" SRPDLLEXPORT void SRPAPI SRPLuaEditModule_Help( VS_INT32 Type,VS_CHAR *HelpInfo ); //Type=0 help info =1 Examples project

#define SRPLUAEDITMODULE_CLOSE "SRPLuaEditModule_Close"
typedef void (SRPAPI *SRPLuaEditModule_CloseProc)( );
extern "C" SRPDLLEXPORT void SRPAPI SRPLuaEditModule_Close( );

//==============================================================================
//
//==============================================================================
struct VSImportServiceDef {
    VS_CHAR	ServiceName[DEFAULT_NAMELENGTH];
};
typedef struct VSImportServiceDef VSImportServiceDef;

struct VSPublicServiceDef {
    VS_CHAR	ServiceName[DEFAULT_NAMELENGTH];
    VS_BOOL NeedUpdateFlag;
    VS_CHAR ServiceInfo[256];
};
typedef struct VSPublicServiceDef VSPublicServiceDef;

typedef void (SRPAPI *SRPBuild_QueryPublicServiceCallBackProc)( VS_BOOL Result, VS_ULONG Para, struct VSPublicServiceDef *PublicServiceList );

typedef void (SRPAPI *SRPBuild_PrintProc)( VS_ULONG Para, VS_CHAR *Info );

#define VS_DEFAULTDEPENDSERVICE_DOWNURL "http:\\\\www.srplab.com\\dependservice\\s1"
#define VS_DEFAULTDEPENDSERVICE_LOCALDOWNURL "http:\\\\127.0.0.1\\service"            
#define VS_DEFAULTDEPENDSERVICE_CLIENTPATH "\\web\\www.srplab.com\\dependservice\\s1"
#define VS_DEFAULTSERVICE_BLANK "BLANK.SRP"

typedef void (SRPAPI *SRPParse_PrintProc)( VS_ULONG Para, VS_CHAR *Info );

//---windowless site callback option
typedef void (SRPAPI *SRPWindowless_DrawProc)( VS_UUID *ObjectID, VS_ULONG Para, void *hDC, void *rcBounds, void *rcInvalid );
typedef VS_BOOL (SRPAPI *SRPWindowless_MessageProc)( VS_UUID *ObjectID, VS_ULONG Para, VS_ULONG uMes,VS_ULONG wParam,VS_ULONG LParam,VS_ULONG *Result );
typedef VS_BOOL (SRPAPI *SRPWindowless_GetDropTargetProc)( VS_UUID *ObjectID, VS_ULONG Para, void **DropTarget );
struct VSWindowlessSiteCallBackInfo{
    SRPWindowless_DrawProc DrawProc;
    SRPWindowless_MessageProc MessageProc;
    SRPWindowless_GetDropTargetProc GetDropTargetProc;
};

//==============================================================================
// Script--
//==============================================================================
typedef VS_BOOL (SRPAPI *VSScript_PreCompileProc)(VS_ULONG Para,VS_INT8 *ScriptBuf,VS_INT32 ScriptBufSize, VS_CHAR *Name, VS_CHAR **ErrorInfo);
typedef VS_BOOL (SRPAPI *VSScript_DoBufferProc)( VS_ULONG Para,VS_INT8 *ScriptBuf,VS_INT32 ScriptBufSize, VS_CHAR *Name, VS_CHAR **ErrorInfo, VS_CHAR *WorkDirectory );
typedef VS_BOOL (SRPAPI *VSScript_DoFileProc)( VS_ULONG Para,VS_CHAR *FileName, VS_BOOL IsUTF8, VS_CHAR **ErrorInfo, VS_CHAR *WorkDirectory );
//--
typedef void (SRPAPI *VSScript_CompileObjectScriptProc)(VS_ULONG Para,class ClassOfSRPInterface *SRPInterface,void *Object, VS_CHAR *ScriptName, VS_CHAR *ScriptBuf,VS_INT32 ScriptBufSize);
//--if success, return luacfunction on the top of lua stack, else , return nil on the top od lua stack
//     can use LuaPushClosure to record some para, to aid subsequent call
//              Has the following parameters in the UpValue: ServiceGroupID, Para, FuncName,…,  Not more than 8
//     should use LuaNewUserData, LuaSetUserDataGC, to capture release info to free buffer compile data
typedef void (SRPAPI *VSScript_CleanUpProc)( VS_ULONG Para, VS_BOOL ClearSrvGroupFlag );    // if ClearSrvGroupFlag == VS_FALSE, should reserve service group 0,and delete  all other service groups; if ClearSrvGroupFlag == VS_TRUE, delete all service groups;
typedef void (SRPAPI *StarCoreScript_TermProc)( );

typedef void (SRPAPI *VSScript_FreeScriptObjectProc)( VS_ULONG Para, VS_ULONG ScriptObject );

struct StructOfVSScriptContext{
    VSScript_PreCompileProc     PreCompileProc;
    VSScript_DoBufferProc       DoBufferProc;
    VSScript_DoFileProc         DoFileProc;
    //--object embed script
    VSScript_CompileObjectScriptProc CompileObjectScriptProc;
    //--cleanup
    VSScript_CleanUpProc        CleanUpProc;
    VS_BOOL                     OnLineScriptFlag;   //--true, canbe used for telnet, else
    VS_INT8                     Reserved1[3];
    //---
    VS_ULONG                    Reserved[16];  //--reserved,set to 0
};


//==============================================================================
//                   first part，the function need to define and implement in dll
//==============================================================================
#define VSFUNCTIONNAME_MODULEFUNCTION_GETVERSION "VSModuleFunction_GetVersion"
typedef VS_ULONG (*VSModuleFunction_GetVersionProc)();

#define VSFUNCTIONNAME_MODULEFUNCTION_GETMODULEINFO "VSModuleFunction_GetModuleInfo"
typedef void (*VSModuleFunction_GetModuleInfoProc)(VS_INT8 *InfoBuf,VS_INT32 BufSize);

#define VSFUNCTIONNAME_MODULEFUNCTION_MODULEINIT "VSModuleFunction_ModuleInit"
typedef VS_INT32 (*VSModuleFunction_ModuleInitProc)(VS_ULONG ServiceGroupID, VS_QueryServiceInterfaceProc QueryServiceInterface);
//----Function returns 0 for success, otherwise fail

#define VSFUNCTIONNAME_MODULEFUNCTION_SETMODULEHANDLE "VSModuleFunction_SetModuleHandle"
typedef void (*VSModuleFunction_SetModuleHandleProc)(VS_HANDLE hModuleHandle);

#define VSFUNCTIONNAME_MODULEFUNCTION_MODULETERM "VSModuleFunction_ModuleTerm"
typedef VS_INT32 (*VSModuleFunction_ModuleTermProc)();

#define VSFUNCTIONNAME_MODULEFUNCTION_REQUESTREGISTEROBJECT "VSModuleFunction_RequestRegisterObject"
typedef VS_INT32 (*VSModuleFunction_RequestRegisterObjectProc)();

//==============================================================================
typedef struct StructOfVSWindowStyle{
    VS_BOOL SystemMenuFlag;
    VS_BOOL MinimizeFlag;
    VS_BOOL MaximizeFlag;
    VS_BOOL ShowBorderFlag;
    VS_BOOL SizeableFlag;
}VSWINDOWSTYLE;

typedef struct StructOfVSWindowPosition{
    VS_INT32 X;
    VS_INT32 Y;
    VS_INT32 nWidth;
    VS_INT32 nHeight;
}VSWINDOWPOSITION;

#define VS_RUMFROMURL_NORESTART    0
#define VS_RUMFROMURL_RESTART      1
#define VS_RUMFROMURL_WAITRESTART  2  //If the new services rely on the services need to download, then uninstall the current service

//==============================================================================
//            Mutex Interface
//==============================================================================
class ClassOfSRPLockInterface{
public:
    ClassOfSRPLockInterface(){};
    virtual ~ClassOfSRPLockInterface(){};
    virtual void SRPAPI Release() = 0;
    virtual void SRPAPI Lock() = 0;
    virtual void SRPAPI UnLock() = 0;
};

//==============================================================================
//            Comm interface, Only Support C/C++, Run In Thread
//==============================================================================
struct StructOfSRPCommMessage{
	VS_UINT16  MsgClass;              //  User type, you can not use the parameter
    VS_UINT16  OperateCode;           //  Operation code, which is defined to separate the different layers
    void       *PrivateBuf;             //  Used By Kernel
    VS_INT8  Buf[1];
};

#define SRPCOMM_MSG_START         0x0400

#define SRPCOMM_TCP_ONCLOSE       SRPCOMM_MSG_START + 0x0020
#define SRPCOMM_TCP_ONCONNECT     SRPCOMM_MSG_START + 0x0021
#define SRPCOMM_TCP_ONREAD        SRPCOMM_MSG_START + 0x0022
#define SRPCOMM_TCP_ONWRITE       SRPCOMM_MSG_START + 0x0023
#define SRPCOMM_UDP_ONREAD        SRPCOMM_MSG_START + 0x0026
#define SRPCOMM_UDP_ONWRITE       SRPCOMM_MSG_START + 0x0027   

#define SRPCOMM_HTTP_ONSTART      SRPCOMM_MSG_START + 0x0030
#define SRPCOMM_HTTP_ONREAD       SRPCOMM_MSG_START + 0x0031
#define SRPCOMM_HTTP_ONWRITE      SRPCOMM_MSG_START + 0x0032
#define SRPCOMM_HTTP_ONFINISH     SRPCOMM_MSG_START + 0x0033
#define SRPCOMM_HTTP_ONERROR      SRPCOMM_MSG_START + 0x0034
#define SRPCOMM_HTTP_ONREQUEST    SRPCOMM_MSG_START + 0x0035
/*-------Http Get Related message--------------
    NETCOMM_ABSLAYER_HTTP_ONSTART
    NETCOMM_ABSLAYER_HTTP_ONREAD      (RSP DATA)
    NETCOMM_ABSLAYER_HTTP_ONERROR
    NETCOMM_ABSLAYER_HTTP_ONFINISH    (RSP DATA)

    NetComm_AbsLayer_HttpRecv
    NetComm_AbsLayer_HttpRelease
------------------------------------------------*/

/*-------Http Post Related message--------------
    NETCOMM_ABSLAYER_HTTP_ONWRITE
    NETCOMM_ABSLAYER_HTTP_ONSTART
    NETCOMM_ABSLAYER_HTTP_ONREAD       (RSP DATA)
    NETCOMM_ABSLAYER_HTTP_ONERROR
    NETCOMM_ABSLAYER_HTTP_ONFINISH     (RSP DATA)

    NetComm_AbsLayer_HttpSend
    NetComm_AbsLayer_HttpRecv
    NetComm_AbsLayer_HttpRelease
------------------------------------------------*/

/*-------Http Server Related message--------------
    NETCOMM_ABSLAYER_HTTP_ONREQUEST
    NETCOMM_ABSLAYER_HTTP_ONWRITE
    NETCOMM_ABSLAYER_HTTP_ONFINISH

    NetComm_AbsLayer_HttpSend
    NetComm_AbsLayer_HttpRelease
------------------------------------------------*/
#define SRPCOMM_HTTP_ONWEBSERVERUNREG  SRPCOMM_MSG_START + 0x0036   // unreg web server, not realmessage

//-----timer Message  use SetupTimer to create
#define SRPCOMM_TIMER             SRPCOMM_MSG_START + 0x0040

#define SRPCOMM_APPMSG_START        0x8000
#define SRPCOMM_APPMSG_CLASS_START  0x8000

struct StructOfSRPComm_TCPOnConnect{
	VS_ULONG ServerConnectionID;
	VS_ULONG ConnectionID;
	VS_ULONG ConnectionGroup;
	SOCKADDR_IN LocalSockAddr;
	SOCKADDR_IN PeerSockAddr;
	VS_ULONG Result;          // ==0  Successful, otherwise fail
};
struct StructOfSRPComm_TCPOnClose{  //Need Call TCPRelease
	VS_ULONG ConnectionID;
	VS_ULONG ConnectionGroup;
};
struct StructOfSRPComm_TCPOnRead{
	VS_ULONG ConnectionID;
	VS_ULONG ConnectionGroup;
};
struct StructOfSRPComm_TCPOnWrite{
	VS_ULONG ConnectionID;
	VS_ULONG ConnectionGroup;
};

struct StructOfSRPComm_UDPOnRead{
	VS_ULONG ConnectionID;
	VS_ULONG ConnectionGroup;
};
struct StructOfSRPComm_UDPOnWrite{
	VS_ULONG ConnectionID;
	VS_ULONG ConnectionGroup;
};
//-------
struct StructOfSRPComm_HttpOnStart{
	 VS_ULONG ConnectionID;
     VS_ULONG ConnectionGroup;
     VS_ULONG FileSize;             //valid at download
     VS_CHAR  *ResponseHeader;      //valid at http
};
struct StructOfSRPComm_HttpOnRead{
	 VS_ULONG ConnectionID;
     VS_ULONG ConnectionGroup;
};
struct StructOfSRPComm_HttpOnWrite{
	 VS_ULONG ConnectionID;
     VS_ULONG ConnectionGroup;
};
struct StructOfSRPComm_HttpOnFinish{
	 VS_ULONG ConnectionID;
     VS_ULONG ConnectionGroup;
     VS_ULONG ResidualSize;        //--if not zero, must be valid data
     VS_INT8  *ResidualData;     
};

struct StructOfSRPComm_HttpOnError{
	 VS_ULONG ConnectionID;
     VS_ULONG ConnectionGroup;
     VS_BOOL  LocalErrorFlag;      //error info is local create    
     VS_INT8  Reserved[3];
     VS_CHAR  *ErrorInfo;          //may be null
};

#define VS_HTTPREQUEST_GET               0
#define VS_HTTPREQUEST_POST              1
#define VS_HTTPREQUEST_UNKNOWN           2

struct StructOfSRPComm_HttpOnRequest{
	 VS_ULONG ConnectionID;
     VS_ULONG ConnectionGroup;
     SOCKADDR_IN PeerSockAddr;
     VS_UINT8 RequestType;
     VS_UINT8 BoundaryNumber;
     VS_UINT8 Reserved[2];
     VS_CHAR  ServiceGroupIndexOrName[40];   //http://XXX/__ServiceGroupIndex/XXX;  http://XXX/__ServiceGroupName/XXX
     VS_ULONG FileSize;
     VS_CHAR  *FileName;
     VS_CHAR  *ContentType;
     VS_CHAR  *Cookie;
     VS_ULONG  *BoundaryInfo;    // start from requestbody, Offset,Length,Offset1,Length1...
     VS_CHAR  *RequestHeader;    //valid at http, may be NULL for local request
     VS_CHAR  *RequestBody;      //valid at http
};

struct StructOfSRPComm_Timer{
	 VS_ULONG TimerID;
     VS_ULONG ConnectionGroup;
};

//--------------------------------------------------------
#define VS_COMM_INVALIDCONNECTION 0xFFFFFFFF

typedef VS_BOOL (SRPAPI *VS_KernelMsgProc)(class ClassOfSRPCommInterface *CommInterface,struct StructOfSRPCommMessage *Mes,VS_ULONG Para); //--return value is ignore
typedef VS_BOOL (SRPAPI *VS_WebServerMsgProc)(VS_HANDLE MsgHandle,class ClassOfSRPCommInterface *CommInterface,struct StructOfSRPCommMessage *Mes,VS_ULONG Para,void *AttachBuf,VS_BOOL *ContinueFlag);  //--return true if it process the request
typedef VS_BOOL (SRPAPI *VS_ManagerMsgProc)(VS_HANDLE MsgHandle,VS_BOOL IsHttpRequest,class ClassOfSRPCommInterface *CommInterface,struct StructOfSRPCommMessage *Mes,class ClassOfSRPSXMLInterface *SXMLInterface,VS_BOOL *ContinueFlag);     //--return true if it process the request, SXMLInterface is body

class ClassOfSRPCommInterface{
public:
    ClassOfSRPCommInterface(){};
    virtual ~ClassOfSRPCommInterface(){};
    virtual void SRPAPI Release() = 0;

    //---thread
	//---thread MsgQueue Interface
    virtual VS_HANDLE SRPAPI CreateMsgQueue(VS_INT32 ControlMsgSize,VS_INT32 DataMsgSize) = 0;
	virtual void SRPAPI DeleteMsgQueue(VS_HANDLE MsgHandle) = 0;
	virtual VS_INT8 *SRPAPI GetControlMsgBuf(VS_HANDLE MsgHandle) = 0;  //  Does not return NULL, if no buf, then wait
	virtual VS_INT8 *SRPAPI GetDataMsgBuf(VS_HANDLE MsgHandle) = 0;     //  May return NULL, if not, then return NULL
	virtual void SRPAPI FreeMsgBuf(VS_HANDLE MsgHandle,VS_INT8 *MsgBuf) = 0;
	virtual VS_INT32 SRPAPI AddMsgToQueue(VS_HANDLE MsgHandle,VS_INT8 *MsgBuf) = 0;  //According to sources different, were inserted into a different queue
	virtual VS_INT8 *SRPAPI GetMsgFromQueue(VS_HANDLE MsgHandle,VS_INT32 WaitMark) = 0;   //  == 0 Not Wait  , == 1 Wait

    //---kernel
    virtual VS_HANDLE SRPAPI GetKernelMsgQueue() = 0;
    virtual VS_UINT16 SRPAPI AllocKernelMsgClass() = 0;
    virtual void SRPAPI FreeKernelMsgClass(VS_UINT16 MsgClass) = 0;
    virtual VS_BOOL SRPAPI RegKernelMsgProc(VS_UINT16 MsgClass,VS_KernelMsgProc KernelMsgProc,VS_ULONG Para) = 0;
    virtual void SRPAPI UnRegKernelMsgProc(VS_UINT16 MsgClass,VS_KernelMsgProc KernelMsgProc,VS_ULONG Para) = 0;
    virtual VS_BOOL SRPAPI RegWebServerMsgProc(VS_WebServerMsgProc WebServerMsgProc,VS_ULONG Para,VS_BOOL RunInKernel,VS_ULONG BufSize) = 0;
    virtual VS_BOOL SRPAPI UnRegWebServerMsgProc(VS_WebServerMsgProc WebServerMsgProc,VS_ULONG Para) = 0;
	//If UnRegWebServerMsgProc return VS_TRUE, you need to loop wait SRPCOMM_HTTP_ONWEBSERVERUNREG, call SRPDispatch
    //If the deal SRPCOMM_HTTP_ONREQUEST, return TRUE, ContinueFlag = VS_TRUE, then use the function to release the connection, can not call HttpRelease, otherwise the memory will leak
    //                                          ContinueFlag=VS_FALSE,Kernel Responsible for releasing the connection
    virtual void SRPAPI WebServerRelease(VS_ULONG ConnectionID) = 0;  

	//---TCP/UDP Interface
	virtual VS_ULONG SRPAPI TCPSetupServer(VS_HANDLE MsgHandle,VS_INT32 BufferPkgNum,VS_INT8 *LocalServerName,VS_UINT16 PortNumber,VS_ULONG ConnectionGroup,VS_UINT16 MsgClass,SOCKADDR_IN *SocketAddr) = 0;
	virtual VS_ULONG SRPAPI TCPSetupClient(VS_HANDLE MsgHandle,VS_INT32 BufferPkgNum,VS_INT8 *ServerName,VS_UINT16 PortNumber,VS_ULONG ConnectionGroup,VS_UINT16 MsgClass) = 0;
	virtual VS_INT32 SRPAPI TCPSend(VS_ULONG ConnectionID,VS_INT32 Length,VS_INT8 *FrameBuf,VS_BOOL MoreData) = 0;
	virtual VS_INT32 SRPAPI TCPRecv(VS_ULONG ConnectionID,VS_INT32 Length,VS_INT8 *FrameBuf) = 0;    // If Length is equal to 0, returns the current size of the cache data
    virtual VS_INT32 SRPAPI TCPRecvLine(VS_ULONG SocketIDOfClient,VS_INT32 Length,VS_CHAR *FrameBuf,VS_INT32 *ModeDataSize) = 0;  /* \r\n  */
    virtual VS_INT32 SRPAPI TCPPreview(VS_ULONG SocketIDOfClient,VS_INT32 Length,VS_INT8 *FrameBuf) = 0;  // Copy data to  buf, but not discard
    virtual VS_INT32 SRPAPI TCPDisacrd(VS_ULONG SocketIDOfClient,VS_INT32 Length) = 0;  // Disca++rd data， if length ==0  Discard All Data
	virtual void SRPAPI TCPRelease(VS_ULONG ConnectionID) = 0;
	
	virtual VS_ULONG SRPAPI UDPSetupServer(VS_HANDLE MsgHandle,VS_INT32 BufferPkgNum,VS_INT8 *LocalServerName,VS_UINT16 PortNumber,VS_ULONG ConnectionGroup,VS_UINT16 MsgClass,SOCKADDR_IN *SocketAddr) = 0;
	virtual VS_ULONG SRPAPI UDPSetupClient(VS_HANDLE MsgHandle,VS_INT32 BufferPkgNum,VS_ULONG ConnectionGroup,VS_UINT16 MsgClass) = 0;
	virtual VS_INT32 SRPAPI UDPSend(VS_ULONG ConnectionID,VS_INT32 Length,VS_INT8 *FrameBuf,VSSOCKADDR_IN *SockAddr) = 0;
	virtual VS_BOOL SRPAPI UDPRecv(VS_ULONG ConnectionID,VS_INT32 *Length,VS_INT8 *FrameBuf,VSSOCKADDR_IN *SockAddr) = 0;  //SockAddr source address
	virtual void SRPAPI UDPRelease(VS_ULONG ConnectionID) = 0;
	virtual VS_BOOL SRPAPI UDPSetSockAddr(VS_CHAR *Name,VS_UINT16 Port,VSSOCKADDR_IN *SockAddr) = 0;

//---Url : http://www.test.com:8080?ACCE=;BOUN=(13Bytes)/XXX/XXX FileName=XXX      ACCE is Accept  COOK is Cookie
//          ftp://www.test.com:8080?TYPE=I/A;USER=;PASS=;ACCT=;FWHOST=;FWPORT=;FWUSER=;FWPASS=;FWTYPE= /XXX/XXX FileName=XXX
//---valid at http:  VS_CHAR *ContentType,VS_ULONG ContentLength,VS_CHAR *RequestHeader,VS_BOOL MultiPartFlag;
//---                VS_ULONG ContentLength : used at download
//                   VS_BOOL MultiPartFlag  : used at upload
//                   VS_CHAR *RequestHeader : if not NULL, all these para is invalid
    virtual VS_ULONG SRPAPI HttpDownLoad( VS_HANDLE MsgHandle,VS_CHAR *Url,VS_CHAR *FileName,VS_ULONG ConnectionGroup,VS_UINT16 MsgClass ) = 0;
    virtual VS_ULONG SRPAPI HttpUpLoad( VS_HANDLE MsgHandle,VS_CHAR *Url,VS_CHAR *FileName,VS_ULONG FileSize,VS_CHAR *ContentType,VS_BOOL MultiPartFlag,VS_CHAR *SaveFileName,VS_ULONG ConnectionGroup,VS_UINT16 MsgClass ) = 0;
    virtual VS_ULONG SRPAPI HttpDownLoadEx( VS_HANDLE MsgHandle,VS_CHAR *Url,VS_CHAR *FileName,VS_CHAR *RequestHeader,VS_ULONG ConnectionGroup,VS_UINT16 MsgClass ) = 0;
    virtual VS_ULONG SRPAPI HttpUpLoadEx( VS_HANDLE MsgHandle,VS_CHAR *Url,VS_CHAR *FileName,VS_ULONG FileSize,VS_CHAR *RequestHeader,VS_ULONG ConnectionGroup,VS_UINT16 MsgClass ) = 0;
    virtual VS_INT32 SRPAPI HttpSend(VS_ULONG ConnectionID,VS_INT32 Length,VS_INT8 *FrameBuf,VS_BOOL MoreData) = 0;
    virtual VS_INT32 SRPAPI HttpRecv(VS_ULONG ConnectionID,VS_INT32 Length,VS_INT8 *FrameBuf) = 0;        // If Length is equal to 0, returns the current size of the cache data
    virtual void SRPAPI HttpRelease( VS_ULONG ConnectionID ) = 0;
    //---http simple server interface
    virtual VS_ULONG SRPAPI HttpServer( VS_HANDLE MsgHandle,VS_INT8 *LocalServerName,VS_UINT16 PortNumber,VS_ULONG ConnectionGroup,VS_UINT16 MsgClass,SOCKADDR_IN *SocketAddr, VS_ULONG MaxPostSize ) = 0;  // MaxPostSize: KBytes
    virtual void SRPAPI FormatRspHeader(VS_CHAR *RspInfo,VS_CHAR *ServerInfo,VS_CHAR *Connection,VS_CHAR *ContentType,VS_INT32 ContentLength,VS_CHAR *Buf) = 0;
    /*FormatRspHeader("200 OK","Microsoft-IIS/5.1","Close","text/html",268,Buf);*/
    virtual VS_BOOL SRPAPI ParsePara(VS_CHAR *Info,VS_CHAR *ParaName,VS_CHAR *ParaBuf,VS_INT32 ParaBufSize) = 0;
    virtual VS_INT32 SRPAPI GetResponseCode(VS_INT8 *Response,VS_CHAR *RspInfo,VS_INT32 RspInfoSize) = 0;
    virtual VS_BOOL SRPAPI GetResponseStr(VS_INT8 *Response,VS_CHAR *Title,VS_CHAR *Buf,VS_INT32 BufSize) = 0;
    /*-- Title
      Date:
      Connection:
      Content-Type:
      Content-Length:
      and so on
    */
    virtual VS_INT8 *SRPAPI GetResponseBody(VS_INT8 *Response) = 0;

    //----Para=XXX&Para2=XXX
    virtual VS_ULONG SRPAPI HttpLocalRequest(VS_HANDLE MsgHandle,VS_ULONG ConnectionGroup,VS_UINT16 MsgClass,VS_UINT8 RequestType,VS_UINT8 BoundaryNumber,VS_ULONG FileSize,VS_CHAR  *FileName,VS_CHAR  *ContentType,VS_CHAR  *Cookie,VS_ULONG  *BoundaryInfo,VS_CHAR  *RequestHeader,VS_CHAR  *RequestBody) = 0;
    virtual VS_ULONG SRPAPI HttpLocalRequestEx(VS_HANDLE MsgHandle,VS_ULONG ConnectionGroup,VS_UINT16 MsgClass,VS_CHAR *HtmlRequestPlain) = 0;
/*  source
    NETCOMM_ABSLAYER_HTTP_ONREAD     (RSP DATA)
    NETCOMM_ABSLAYER_HTTP_ONFINISH   (RSP DATA)
    Interface : SetWebServerPort("",0,0);  or do not call SetWebServerPort
*/

    //---Timer  Interval number of 10ms
    virtual VS_ULONG SRPAPI SetupTimer(VS_INT32 Interval,VS_INT32 NumberOfValid,VS_HANDLE MsgHandle,VS_ULONG ConnectionGroup,VS_UINT16 MsgClass) = 0;
    virtual void SRPAPI KillTimer(VS_ULONG TimerID) = 0;
    //--
    virtual class ClassOfSRPCommInterface *SRPAPI Dup() = 0;

    //--register as soon as possible, is global, only register one proc
    //--typedef VS_BOOL (SRPAPI *VS_ManagerMsgProc)(VS_BOOL IsHttpRequest,class ClassOfSRPCommInterface *CommInterface,struct StructOfSRPCommMessage *Mes,VS_BOOL *ContinueFlag);
    //--   IsHttpRequest==false  ContinueFlag is NULL and return value is ignore
    //--
    virtual void SRPAPI HookManagerMsgProc(VS_ManagerMsgProc ManagerMsgProc) = 0;
    virtual VS_BOOL SRPAPI UnHookManagerMsgProc() = 0;
	//If UnRegWebServerMsgProc return VS_TRUE, you need to loop wait SRPCOMM_HTTP_ONWEBSERVERUNREG, call SRPDispatch
    virtual VS_ULONG SRPAPI SetupManagerTimer(VS_INT32 Interval,VS_INT32 NumberOfValid,VS_ULONG ConnectionGroup,VS_UINT16 MsgClass) = 0;

	//0.61.1 Version
    virtual void SRPAPI FormatRspHeaderEx(VS_CHAR *RspInfo,VS_CHAR *ServerInfo,VS_CHAR *Connection,VS_CHAR *ContentType,VS_INT32 ContentLength,VS_CHAR *ExtendInfo,VS_CHAR *Buf) = 0;
	//----ItemIndex start from 0, for to retrieve the same name item
    virtual VS_BOOL SRPAPI Http_GetHeaderItem(VS_INT8 *Header,VS_INT32 ItemIndex,VS_CHAR *ItemName,VS_CHAR *RetBuf,VS_INT32 RetBufSize) = 0;  //ItemName maybe NULL
    virtual VS_BOOL SRPAPI Http_GetHeaderSubItem(VS_CHAR *Item,VS_INT32 SubItemIndex,VS_CHAR *SubItemName,VS_CHAR *RetBuf,VS_INT32 RetBufSize ) = 0; //seperated by ';',SubItemName maybe NULL
    virtual VS_BOOL SRPAPI Http_GetNVValue(VS_CHAR *Buf,VS_CHAR *Name,VS_CHAR *RetBuf,VS_INT32 RetBufSize) = 0;  //seperated by ';', Name=value
    virtual void SRPAPI Http_TimeToHttpTime(VS_TIME_T *Time,VS_CHAR *HttpTimeBuf) = 0;
    virtual void SRPAPI Http_HttpTimeToTime(VS_CHAR *HttpTimeBuf,VS_TIME_T *Time) = 0;
    virtual void SRPAPI Http_SetCookie(VS_CHAR *Domain,VS_CHAR *Path,VS_CHAR *Cookie,VS_BOOL Secure) = 0;
    virtual void SRPAPI Http_ClearCookie(VS_CHAR *Domain,VS_CHAR *Path,VS_CHAR *Cookie) = 0;
    virtual VS_CHAR *SRPAPI Http_GetMediaType(VS_CHAR *FileName) = 0;

    virtual void SRPAPI Http_SetMaxPostSize(VS_ULONG ConnectionID,VS_ULONG MaxPostSize) = 0;  // MaxPostSize: KBytes, ConnectionID ==0 represents kernel webserver
    virtual VS_INT8 *SRPAPI Http_GetMultiPart(VS_INT8 *RequestBody,VS_UINT8 Index,VS_UINT8 BoundaryNumber,VS_ULONG  *BoundaryInfo,VS_ULONG *RetMultiPartBodySize,VS_CHAR *RetBuf,VS_INT32 RetBufSize) = 0;  //--return point to multipart body

    virtual void SRPAPI LuaPushHttpOnRequest(VS_ULONG ServiceGroupIndex,struct StructOfSRPComm_HttpOnRequest *HttpOnRequest) = 0;    //push a table on lua stack

    virtual VS_BOOL SRPAPI IsTCPConnect(VS_ULONG ConnectionID) = 0;
    virtual VS_BOOL SRPAPI IsHttpConnect(VS_ULONG ConnectionID) = 0;

    //--the following will wait finish, RetBinBuf maybe NULL
    virtual VS_BOOL SRPAPI FileDownLoad( VS_CHAR *Url,VS_CHAR *LocalFileName,VS_BOOL WaitFlag,VS_WebDownInfoProc CallBackProc, VS_ULONG Para) = 0;
    virtual VS_BOOL SRPAPI FileUpLoad( VS_CHAR *Url,VS_CHAR *LocalFileName,VS_CHAR *RemoteFileName,class ClassOfSRPBinBufInterface *RetBinBuf,VS_BOOL MultiPartFlag,VS_CHAR *ContentType,VS_BOOL WaitFlag,VS_WebDownInfoProc CallBackProc, VS_ULONG Para ) = 0;                        //RemoteFileName valid for  MultiPartFlag == VS_TRUE
    virtual VS_BOOL SRPAPI BufDownLoad( VS_CHAR *Url,class ClassOfSRPBinBufInterface *BinBuf,VS_BOOL WaitFlag,VS_WebDownInfoProc CallBackProc, VS_ULONG Para ) = 0;
    virtual VS_BOOL SRPAPI BufUpLoad( VS_CHAR *Url,class ClassOfSRPBinBufInterface *BinBuf,VS_CHAR *RemoteFileName,class ClassOfSRPBinBufInterface *RetBinBuf,VS_BOOL MultiPartFlag,VS_CHAR *ContentType,VS_BOOL WaitFlag,VS_WebDownInfoProc CallBackProc, VS_ULONG Para ) = 0;        //RemoteFileName valid for  MultiPartFlag == VS_TRUE

    virtual void SRPAPI AddRef() = 0;
    virtual VS_ULONG SRPAPI GetRef() = 0;
};

/*---------------GET response-----------------
		HTTP/1.1 200 OK
		Server: Microsoft-IIS/5.1
		Date: Mon, 11 Oct 2010 09:08:38 GMT
		Content-Type: text/html
		Accept-Ranges: bytes
		Content-Length: 268

		HTTP/1.1 404 Object Not Found
		Server: Microsoft-IIS/5.1
		Date: Mon, 11 Oct 2010 09:14:08 GMT
		Connection: close
		Content-Length: 3780
		Content-Type: text/html
----------------------------------------*/
/*---------------POST response-----------------
		HTTP/1.1 100 Continue
		Server: Microsoft-IIS/5.1
		Date: Mon, 11 Oct 2010 09:11:21 GMT

		HTTP/1.1 200 OK
		Server: Microsoft-IIS/5.1
		Date: Mon, 11 Oct 2010 09:11:21 GMT
		Connection: close
		Content-type: text/html

*/

//==============================================================================
//            XML interface
//==============================================================================
class ClassOfSRPSXMLInterface{
public:
    ClassOfSRPSXMLInterface(){};
    virtual ~ClassOfSRPSXMLInterface(){};
    virtual void SRPAPI Release() = 0;
    //--
    virtual VS_BOOL SRPAPI LoadFromFile(VS_CHAR *FileName,VS_CHAR **ErrorInfo) = 0;
    virtual VS_BOOL SRPAPI LoadFromBuf(VS_INT8 *Buf,VS_CHAR **ErrorInfo) = 0;
    virtual VS_BOOL SRPAPI SaveToFile(VS_CHAR *FileName) = 0;
    virtual VS_BOOL SRPAPI SaveToBuf(class ClassOfSRPBinBufInterface *BinBuf) = 0;
    //--Read Function
    virtual VS_CHAR *SRPAPI GetStandalone() = 0;
    virtual VS_CHAR *SRPAPI GetVersion() = 0;
    virtual VS_CHAR *SRPAPI GetEncoding() = 0;

    virtual void *SRPAPI FindElement(VS_CHAR *Value) = 0;
    virtual void *SRPAPI FindElementEx(void *ParentElement,VS_CHAR *Value) = 0;
    virtual void *SRPAPI FirstElement(void *ParentElement) = 0;
    virtual void *SRPAPI NextElement(void *Element) = 0;
    virtual void *SRPAPI ParentElement(void *Element) = 0;
    virtual VS_CHAR *SRPAPI GetElement(void *Element) = 0;
    virtual void SRPAPI GetElementEx(void *Element,VS_CHAR *Buf,VS_INT32 BufSize) = 0;
    virtual VS_BOOL GetNs(void *Element,VS_CHAR *nsName,VS_INT32 nsNameBufSize,VS_CHAR **nsValue) = 0;
    virtual VS_CHAR *SRPAPI GetNsValue(void *Element,VS_CHAR *nsName) = 0;
    virtual void SRPAPI SetNs(void *Element,VS_CHAR *nsName,VS_CHAR *nsValue) = 0;

    virtual void *SRPAPI FindAttribute(void *Element,VS_CHAR *Name) = 0;
    virtual void *SRPAPI FirstAttribute(void *Element) = 0;
    virtual void *SRPAPI NextAttribute(void *Attribute) = 0;
    virtual VS_CHAR *SRPAPI GetAttributeName(void *Attribute) = 0;
    virtual VS_CHAR *SRPAPI GetAttributeValue(void *Attribute) = 0;

    virtual VS_CHAR *SRPAPI GetSingleText(void *Element) = 0;
    virtual void *SRPAPI FirstText(void *Element) = 0;
    virtual void *SRPAPI NextText(void *Text) = 0;
    virtual VS_CHAR *SRPAPI GetText(void *Text) = 0;

    //--Create Function
    virtual void SRPAPI SetDeclaration(VS_CHAR *Version,VS_CHAR *Encoding,VS_CHAR *Standalone) = 0;
    virtual void SRPAPI RemoveDeclaration() = 0;

    virtual void *SRPAPI InsertElementBefore(void *ParentElement,void *Element,VS_CHAR *Value) = 0;
    virtual void *SRPAPI InsertElementAfter(void *ParentElement,void *Element,VS_CHAR *Value) = 0;
    virtual void SRPAPI RemoveElement(void *Element) = 0;
    virtual void SRPAPI SetElement(void *Element,VS_CHAR *Value) = 0;

    virtual void *SRPAPI InsertTextBefore(void *ParentElement,void *Text,VS_CHAR *Value,VS_BOOL CDataFlag) = 0;
    virtual void *SRPAPI InsertTextBeforeEx(void *ParentElement,void *Text,VS_BOOL CDataFlag,VS_CHAR *Format,...) = 0;
    virtual void *SRPAPI InsertTextAfter(void *ParentElement,void *Text,VS_CHAR *Value,VS_BOOL CDataFlag) = 0;
    virtual void *SRPAPI InsertTextAfterEx(void *ParentElement,void *Text,VS_BOOL CDataFlag,VS_CHAR *Format,...) = 0;
    virtual void SRPAPI RemoveText(void *Text) = 0;
    virtual void SRPAPI SetText(void *Text,VS_CHAR *Value,VS_BOOL CDataFlag) = 0;
    virtual void SRPAPI SetTextEx(void *Text,VS_BOOL CDataFlag,VS_CHAR *Format,...) = 0;    // small 1024

    virtual void *SRPAPI InsertCommentBefore(void *ParentElement,void *Comment,VS_CHAR *Value) = 0;
    virtual void *SRPAPI InsertCommentBeforeEx(void *ParentElement,void *Comment,VS_CHAR *Format,...) = 0;
    virtual void *SRPAPI InsertCommentAfter(void *ParentElement,void *Comment,VS_CHAR *Value) = 0;
    virtual void *SRPAPI InsertCommentAfterEx(void *ParentElement,void *Comment,VS_CHAR *Format,...) = 0;
    virtual void SRPAPI RemoveComment(void *Comment) = 0;
    virtual void SRPAPI SetComment(void *Comment,VS_CHAR *Value) = 0;
    virtual void SRPAPI SetCommentEx(void *Comment,VS_CHAR *Format,...) = 0;   // small 1024

    virtual void SRPAPI SetAttribute(void *Element,VS_CHAR *Name,VS_CHAR *Value) = 0;
    virtual void SRPAPI SetAttributeEx(void *Element,VS_CHAR *Name,VS_CHAR *Format,...) = 0;   // small 1024
    virtual void SRPAPI RemoveAttribute(void *Element,VS_CHAR *Name) = 0;

    virtual VS_INT32 SRPAPI GetRow(void *Node) = 0;
    virtual VS_INT32 SRPAPI GetCol(void *Node) = 0;

    virtual class ClassOfSRPSXMLInterface *SRPAPI Dup() = 0;

    //--SrcElement may be in this interface or other interface
    virtual VS_BOOL SRPAPI Copy(class ClassOfSRPSXMLInterface *SrcSXML) = 0;
    virtual void *SRPAPI CopyElementBefore(void *ParentElement,void *Element,void *SrcElement) = 0;
    virtual void *SRPAPI CopyElementAfter(void *ParentElement,void *Element,void *SrcElement) = 0;
    virtual VS_BOOL SRPAPI CopyChild(void *DesElement,void *SrcElement) = 0;

    virtual VS_BOOL SRPAPI LoadFromAnsiBuf(VS_INT8 *Buf,VS_CHAR **ErrorInfo) = 0;
    virtual VS_BOOL SRPAPI SaveToAnsiBuf(class ClassOfSRPBinBufInterface *BinBuf) = 0;

    virtual void SRPAPI AddRef() = 0;
    virtual VS_ULONG SRPAPI GetRef() = 0;
};

//==============================================================================
//            interface to access and release of CLE core
//==============================================================================
class ClassOfSRPControlInterface{
public:
    ClassOfSRPControlInterface(){};
    virtual ~ClassOfSRPControlInterface(){};
    virtual void SRPAPI Release() = 0;

    //--------CLE global lock
    virtual void SRPAPI SRPLock( ) = 0;
    virtual void SRPAPI SRPUnLock( ) = 0;

    virtual VS_UINT16 SRPAPI GetOsType( ) = 0;

    //--------Global flow control
    virtual  VS_BOOL SRPAPI SRPDispatch(VS_BOOL WaitFlag) = 0;
    virtual  VS_BOOL SRPAPI SRPIdle() = 0;
    virtual  void SRPAPI SRPAppActive() = 0;
    virtual  void SRPAPI SRPAppDeactive() = 0;

    virtual void SRPAPI ProcessError(VS_INT32 AlarmLevel,VS_CHAR *SourceName,VS_INT32 LineIndex,VS_CHAR *format,...) = 0;
    virtual void SRPAPI ProcessErrorVar(VS_INT32 AlarmLevel,VS_CHAR *SourceName,VS_INT32 LineIndex,VS_CHAR *format,va_list argList) = 0;
    virtual void SRPAPI ProcessLuaError(VS_INT32 AlarmLevel,VS_CHAR *SourceName,VS_INT32 LineIndex,VS_CHAR *format,...) = 0;
    virtual void SRPAPI ProcessLuaErrorVar(VS_INT32 AlarmLevel,VS_CHAR *SourceName,VS_INT32 LineIndex,VS_CHAR *format,va_list argList) = 0;

    virtual VS_BOOL SRPAPI IsAppActive(void) = 0;

    virtual void SRPAPI CaptureLuaDisp(VS_LuaInfoDispProc DispProc,VS_ULONG Para) = 0;    //---Capture the output of Lua
    virtual void SRPAPI ReleaseLuaDisp(VS_LuaInfoDispProc DispProc,VS_ULONG Para) = 0;    //---Release capture the output of Lua
    virtual VS_BOOL SRPAPI PreCompile(VS_CHAR *ScriptInterface,VS_INT8 *ScriptBuf,VS_INT32 ScriptBufSize, VS_CHAR *Name, VS_CHAR **ErrorInfo) = 0;
    virtual VS_BOOL SRPAPI OpenLuaEdit(VS_CHAR *Module,VS_ULONG Config,VS_BOOL CloseEnable) = 0;
    virtual void SRPAPI LuaEditDisp(VS_CHAR *Info) = 0;
    virtual void SRPAPI CloseLuaEdit() = 0;
    virtual void SRPAPI ClearService( ) = 0;
    virtual VS_BOOL SRPAPI DoBuffer( VS_CHAR *ScriptInterface,VS_INT8 *ScriptBuf,VS_INT32 ScriptBufSize,VS_BOOL IsUTF8,VS_CHAR *Name ) = 0;     //ScriptInterfaces length < 16
    virtual void SRPAPI PostDoBuffer( VS_CHAR *ScriptInterface,VS_INT8 *ScriptBuf,VS_INT32 ScriptBufSize,VS_BOOL IsUTF8,VS_CHAR *Name ) = 0; //ScriptInterfaces length < 16
    virtual VS_BOOL SRPAPI DoFile( VS_CHAR *ScriptInterface,VS_CHAR *FileName, VS_CHAR **ErrorInfo, VS_CHAR *WorkDirectory, VS_BOOL IsUTF8 ) = 0;                            //ScriptInterfaces length < 16
    virtual VS_BOOL SRPAPI PostDoFile( VS_CHAR *ScriptInterface,VS_CHAR *FileName, VS_CHAR **ErrorInfo, VS_CHAR *WorkDirectory, VS_BOOL IsUTF8 ) = 0;                        //ScriptInterfaces length < 16

    virtual void SRPAPI LuaEditHelp(VS_INT32 Type,VS_CHAR *HelpInfo) = 0;   //Type=0 help info =1 Examples project

    virtual class ClassOfBasicSRPInterface *SRPAPI QueryBasicInterface( VS_ULONG ServiceGroupID ) = 0;
    virtual class ClassOfBasicSRPInterface *SRPAPI CreateBasicInterface( VS_ULONG ServiceGroupID, VS_UINT16 ProgramRunType ) = 0;
    virtual void SRPAPI DeleteBasicInterface( VS_ULONG ServiceGroupID ) = 0;
    virtual VS_ULONG SRPAPI QueryFirstServiceGroup( ) = 0;
    virtual VS_ULONG SRPAPI QueryNextServiceGroup( ) = 0;

    virtual void *SRPAPI GetLuaStack( ) = 0;
    virtual VS_ULONG SRPAPI LuaGetTableInt( void *L, VS_INT32 Index, VS_CHAR *ValueName ) = 0;
    virtual VS_ULONG SRPAPI LuaGetInt( void *L, VS_INT32 Index ) = 0;
    virtual VS_INT32 SRPAPI LuaUpValueIndex( void *L, VS_INT32 Index ) = 0;
    virtual VS_ULONG SRPAPI GetObjectServiceGroupID( void *Object ) = 0;

    virtual void SRPAPI GetUrl(VS_CHAR *Buf,VS_INT32 BufSize) = 0;

    virtual void SRPAPI SetProgramType(VS_UINT16 Type) = 0;
    virtual VS_UINT16 SRPAPI GetProgramType() = 0;

    virtual VS_BOOL SRPAPI SRPBuild_QueryPublicService(SRPBuild_QueryPublicServiceCallBackProc QueryPublicServiceCallBackProc,VS_ULONG CallBackPara,VS_BOOL FillUpdateInfo,SRPBuild_PrintProc PrintProc,VS_ULONG Para) = 0;
    virtual VS_BOOL SRPAPI SRPBuild_QueryPublicServiceEx(VS_CHAR *Url,SRPBuild_QueryPublicServiceCallBackProc QueryPublicServiceCallBackProc,VS_ULONG CallBackPara,VS_BOOL FillUpdateInfo,SRPBuild_PrintProc PrintProc,VS_ULONG Para) = 0;
    virtual VS_BOOL SRPAPI SRPBuild_Start(VS_CHAR *Name,VS_CHAR *Path,VS_BOOL SingleFlag,VS_BOOL ForceToDownLoadPublicService,SRPBuild_PrintProc PrintProc,VS_ULONG Para,struct VSPublicServiceDef *PublicServiceList,VS_BOOL ExeFileFlag,VS_CHAR *ScriptInterface,VS_UINT16 SupportOsType) = 0;
    virtual void SRPAPI SRPBuild_InsertServiceFile(VS_CHAR *DiskFileName,VS_CHAR *OutFileName,VS_BOOL StartFileFlag,VS_BOOL ToUTF8,VS_UINT16 SupportOsType) = 0;
    virtual void SRPAPI SRPBuild_InsertDependFile(VS_CHAR *Path,VS_CHAR *DependName) = 0;
    virtual void SRPAPI SRPBuild_InsertStaticDataFile(VS_CHAR *DiskFileName,VS_CHAR *OutFileName,VS_BOOL ToUTF8) = 0;
    virtual void SRPAPI SRPBuild_InsertDynaDataFile(VS_CHAR *DiskFileName,VS_CHAR *OutFileName,VS_BOOL ToUTF8) = 0;
    virtual VS_BOOL SRPAPI SRPBuild_Execute() = 0;

    virtual VS_BOOL SRPAPI StringToUuid(VS_INT8 *String,VS_UUID *Uuid) = 0;
    virtual VS_INT8 *SRPAPI UuidToString(VS_UUID *Uuid) = 0;
    virtual VS_BOOL SRPAPI MD5ToUuid(VS_INT8 *String,VS_UUID *Uuid) = 0;
    virtual VS_INT8 *SRPAPI UuidToMD5(VS_UUID *Uuid) = 0;
    virtual VS_INT8 *SRPAPI GetMD5(VS_INT8 *Buf,VS_INT32 BufSize) = 0;

    virtual void SRPAPI CreateUuid(VS_UUID *UuidPtr) = 0;

    virtual class ClassOfSRPSXMLInterface *SRPAPI GetSXMLInterface() = 0;
	virtual class ClassOfSRPCommInterface *SRPAPI GetCommInterface() = 0;

    //class ClassOfCoreShellInterface
	virtual void *SRPAPI GetCoreShellInterface() = 0;

    //script inteface
	virtual VS_BOOL SRPAPI RegScriptInterface(VS_CHAR *ScriptInterface,struct StructOfVSScriptContext *ScriptContext,VS_ULONG Para,StarCoreScript_TermProc TermProc) = 0;
	virtual VS_BOOL SRPAPI UnRegScriptInterface(VS_CHAR *ScriptInterface,struct StructOfVSScriptContext *ScriptContext,VS_ULONG Para,StarCoreScript_TermProc TermProc) = 0;
    virtual VS_BOOL SRPAPI ActiveScriptInterface(VS_CHAR *ScriptInterface,VS_BOOL *OnLineScriptFlag,void *VirtualMachine) = 0;   //VirtualMachine may be NULL
    virtual VS_CHAR *SRPAPI FirstScriptInterface(VS_QUERYRECORD *QueryRecord) = 0;
    virtual VS_CHAR *SRPAPI NextScriptInterface(VS_QUERYRECORD *QueryRecord) = 0;

    //temp file
    virtual VS_BOOL SRPAPI RegTempFile(VS_CHAR *TempFileName,VS_CHAR *OriFileName) = 0;     /*clear when process not exist*/
    virtual VS_CHAR *SRPAPI GetRegTempFile(VS_CHAR *OriFileName,VS_CHAR *Buf,VS_INT32 BufSize) = 0;   /*get temp filename of other process*/
    virtual void SRPAPI UnRegTempFile(VS_CHAR *TempFileName) = 0;   /*clear when process not exist,temp file will be delete by starcore*/

    virtual void SRPAPI GetConfigResult(VS_BOOL *DebugCfgResult,VS_BOOL *DirectClientCfgResult,VS_BOOL *TelnetCfgResult,VS_BOOL *WebServerCfgResult) = 0;
    virtual VS_CHAR *SRPAPI GetConfigEnvTag( ) = 0;

    virtual class ClassOfSRPControlInterface *SRPAPI Dup() = 0;
    virtual void *SRPAPI QueryInterface( VS_UUID *InterfaceID ) = 0;

    virtual void SRPAPI AddRef() = 0;
    virtual VS_ULONG SRPAPI GetRef() = 0;

    virtual void SRPAPI SetLogFile(VS_CHAR *FileName,VS_BOOL LogAll) = 0;

    virtual void SRPAPI GetSystemRegCode(VS_CHAR *Buf) = 0;  //Buf size should > 64
    virtual VS_BOOL SRPAPI SetRegisterCode(VS_CHAR *Buf,VS_BOOL Single) = 0;
    virtual VS_BOOL SRPAPI IsRegistered() = 0;

	virtual VS_ULONG SRPAPI GetHashValue(void *Key,VS_ULONG Length,VS_ULONG InitValue) = 0;

    //--script load starcore sharelib record, for current process
    virtual void SRPAPI ScriptStarcoreRecord(VS_CHAR *ScriptInterface,VS_HANDLE SharelibHandle) = 0;
    virtual void SRPAPI ScriptStarcoreUnRecord(VS_CHAR *ScriptInterface,VS_HANDLE SharelibHandle) = 0;
    virtual VS_BOOL SRPAPI ScriptStarcoreIsRecord(VS_CHAR *ScriptInterface) = 0;

    //win32 function special
    virtual VS_ULONG SRPAPI FindWindow(VS_CHAR *Caption) = 0;     //valid for windows
    virtual VS_BOOL SRPAPI WinMsgLoop(VS_BOOL *QuitFlag) = 0;     //return VS_TRUE, there has winmsg being processed,return QuitFlag == VS_TRUE， receive WM_QUIT MSG

    //Script object record
    virtual void SRPAPI RegScriptObject(VS_ULONG ScriptObject,VSScript_FreeScriptObjectProc FreeScriptObjectProc,VS_ULONG Para) = 0;
    virtual void SRPAPI UnRegScriptObject(VS_ULONG ScriptObject,VSScript_FreeScriptObjectProc FreeScriptObjectProc,VS_ULONG Para) = 0;
    virtual void SRPAPI ClearScriptObject(VSScript_FreeScriptObjectProc FreeScriptObjectProc,VS_ULONG Para) = 0;

    virtual VS_BOOL SRPAPI PreAuthorize(VS_CHAR *ServiceName,VS_UUID *ServiceID,VS_CHAR *Buf,VS_BOOL Single) = 0;

    //locale, valid for android
    virtual void SRPAPI SetLocale(VS_CHAR *Lang) = 0;
    virtual VS_CHAR *SRPAPI GetLocale() = 0;
};

class ClassOfBasicSRPInterface{
public:
    ClassOfBasicSRPInterface(){};
    virtual ~ClassOfBasicSRPInterface(){};
    virtual void SRPAPI Release() = 0;

    virtual VS_UINT16 SRPAPI GetOsType( ) = 0;

    virtual void SRPAPI Print(VS_CHAR *format,...) = 0;
    virtual void SRPAPI PrintVar(VS_CHAR *format,va_list argList) = 0;
    virtual void SRPAPI PrintLua(VS_CHAR *format,...) = 0;
    virtual void SRPAPI PrintLuaVar(VS_CHAR *format,va_list argList) = 0;
    virtual void SRPAPI SetPrintToLua(VS_BOOL PrintFlag) = 0;
    virtual void SRPAPI MessageBox(VS_CHAR *Caption,VS_CHAR *format,...) = 0;
    virtual void SRPAPI MessageBoxVar(VS_CHAR *Caption,VS_CHAR *format,va_list argList) = 0;
    virtual void SRPAPI ProcessError(VS_INT32 AlarmLevel,VS_CHAR *SourceName,VS_INT32 LineIndex,VS_CHAR *format,...) = 0;
    virtual void SRPAPI ProcessErrorVar(VS_INT32 AlarmLevel,VS_CHAR *SourceName,VS_INT32 LineIndex,VS_CHAR *format,va_list argList) = 0;
    virtual void SRPAPI ProcessLuaError(VS_INT32 AlarmLevel,VS_CHAR *SourceName,VS_INT32 LineIndex,VS_CHAR *format,...) = 0;
    virtual void SRPAPI ProcessLuaErrorVar(VS_INT32 AlarmLevel,VS_CHAR *SourceName,VS_INT32 LineIndex,VS_CHAR *format,va_list argList) = 0;
    virtual void SRPAPI CaptureLuaDisp(VS_LuaInfoDispProc DispProc,VS_ULONG Para) = 0;    //---Capture the output of Lua
    virtual void SRPAPI ReleaseLuaDisp(VS_LuaInfoDispProc DispProc,VS_ULONG Para) = 0;    //---release Capture the output of Lua

    virtual void SRPAPI SetDefaultPath(VS_CHAR *DefaultPath) = 0;
    virtual void SRPAPI GetDefaultPath(VS_CHAR *DefaultPath,VS_INT32 BufSize) = 0;
    virtual VS_BOOL SRPAPI DefaultPathIsSet() = 0;

    virtual VS_ULONG SRPAPI Connect(VS_CHAR *ServerInterface,VS_CHAR *ServerName,VS_UINT16 ServerPortNumber,VS_INT32 RetrySecond,class ClassOfSRPParaPackageInterface *ParaPkg,VS_ClientConnectCallBackProc ClientConnectCallBack,VS_ULONG Para,VS_CHAR *LoginName,VS_CHAR *LoginPassword) = 0;
    virtual VS_ULONG SRPAPI ConnectEx(VS_CHAR *ServiceName,VS_INT32 RetrySecond,class ClassOfSRPParaPackageInterface *ParaPkg,VS_ClientConnectCallBackProc ClientConnectCallBack,VS_ULONG Para,VS_CHAR *LoginName,VS_CHAR *LoginPassword) = 0;
    virtual VS_ULONG SRPAPI SConnect(VS_CHAR *ServerInterface,VS_CHAR *ServerName,VS_UINT16 ServerPortNumber,class ClassOfSRPParaPackageInterface *ParaPkg,VS_CHAR *LoginName,VS_CHAR *LoginPassword) = 0; /*---sync connect--*/
    virtual VS_ULONG SRPAPI SConnectEx(VS_CHAR *ServiceName,class ClassOfSRPParaPackageInterface *ParaPkg,VS_CHAR *LoginName,VS_CHAR *LoginPassword) = 0; /*---sync connect--*/
    virtual class ClassOfSRPInterface *SRPAPI Connect2( VS_CHAR *ServerInterface,VS_CHAR *ServerName,VS_UINT16 ServerPortNumber,class ClassOfSRPParaPackageInterface *ParaPkg,VS_CHAR *LoginName,VS_CHAR *LoginPassword, VS_CHAR *SysRootItemName ) = 0;
    virtual class ClassOfSRPInterface *SRPAPI ConnectEx2( VS_CHAR *ServiceName,class ClassOfSRPParaPackageInterface *ParaPkg,VS_CHAR *LoginName,VS_CHAR *LoginPassword, VS_CHAR *SysRootItemName ) = 0;
    virtual void SRPAPI DisConnectEx(VS_ULONG ConnectionID) = 0;
    virtual void SRPAPI DisConnect() = 0;
    virtual VS_BOOL SRPAPI IsConnect() = 0;

    virtual void *SRPAPI GetLua( ) = 0;
    virtual VS_BOOL SRPAPI DoBuffer( VS_CHAR *ScriptInterface,VS_INT8 *ScriptBuf,VS_INT32 ScriptBufSize,VS_BOOL IsUTF8,VS_CHAR *Name ) = 0;   //ScriptInterfaces length < 16

    virtual VS_BOOL SRPAPI LuaToBool( VS_INT32 Index ) = 0;
    virtual VS_CHAR *SRPAPI LuaToString( VS_INT32 Index ) = 0;
    virtual VS_DOUBLE SRPAPI LuaToNumber( VS_INT32 Index ) = 0;
    virtual VS_INT32 SRPAPI LuaToInt( VS_INT32 Index ) = 0;
    virtual VS_BOOL SRPAPI LuaIsBool( VS_INT32 Index ) = 0;
    virtual VS_BOOL SRPAPI LuaIsString( VS_INT32 Index ) = 0;
    virtual VS_BOOL SRPAPI LuaIsNumber( VS_INT32 Index ) = 0;
    virtual void SRPAPI LuaGetGlobal( VS_CHAR *Name ) = 0;
    virtual void SRPAPI LuaGetSrvGroupTable( VS_CHAR *Name ) = 0;  //support XXX.XXX
    virtual void SRPAPI LuaPop( VS_INT32 Index ) = 0;
    virtual void SRPAPI LuaPushBool( VS_BOOL Value ) = 0;
    virtual void SRPAPI LuaPushString( VS_CHAR *Value ) = 0;
    virtual void SRPAPI LuaPushNumber( VS_DOUBLE Value ) = 0;
    virtual void SRPAPI LuaPushInt( VS_INT32 Value ) = 0;
    virtual void SRPAPI LuaPushNil( ) = 0;
    virtual void SRPAPI LuaSetGlobal( VS_CHAR *Name ) = 0;
    virtual void SRPAPI LuaSetSrvGroupTable( VS_CHAR *Name ) = 0;  //support XXX.XXX
    virtual VS_BOOL SRPAPI LuaIsNil( VS_INT32 Index ) = 0;
    virtual void SRPAPI LuaNewTable( ) = 0;
    virtual VS_BOOL SRPAPI LuaIsTable( VS_INT32 Index ) = 0;
    virtual VS_INT32  SRPAPI LuaGetTop( ) = 0;
    virtual VS_INT32 SRPAPI LuaObjLen(VS_INT32 TableIndex) = 0;
    virtual void SRPAPI LuaPushLString( VS_CHAR *Value, VS_ULONG Len ) = 0;

    virtual VS_BOOL SRPAPI LuaPushParaPkg( class ClassOfSRPParaPackageInterface *ParaPkg, VS_BOOL AutoRelease ) = 0;
    virtual VS_BOOL SRPAPI LuaPushQueryRecord( VS_QUERYRECORD *QueryRecord, VS_BOOL AutoRelease ) = 0;
    virtual class ClassOfSRPParaPackageInterface *SRPAPI LuaToParaPkg( VS_INT32 Index ) = 0;
    virtual VS_QUERYRECORD *SRPAPI LuaToQueryRecord( VS_INT32 Index ) = 0;
    virtual VS_BOOL SRPAPI LuaIsParaPkg( VS_INT32 Index ) = 0;
    virtual VS_BOOL SRPAPI LuaIsQueryRecord( VS_INT32 Index ) = 0;

    virtual VS_BOOL SRPAPI LuaPushBinBuf( class ClassOfSRPBinBufInterface *BinBuf, VS_BOOL AutoRelease ) = 0;
    virtual class ClassOfSRPBinBufInterface *SRPAPI LuaToBinBuf( VS_INT32 Index ) = 0;
    virtual VS_BOOL SRPAPI LuaIsBinBuf( VS_INT32 Index ) = 0;

    virtual VS_BOOL SRPAPI LuaPushSXml( class ClassOfSRPSXMLInterface *SXml, VS_BOOL AutoRelease ) = 0;
    virtual class ClassOfSRPSXMLInterface *SRPAPI LuaToSXml( VS_INT32 Index ) = 0;
    virtual VS_BOOL SRPAPI LuaIsSXml( VS_INT32 Index ) = 0;

    virtual VS_BOOL SRPAPI LuaPushFunctionPara( class ClassOfSRPFunctionParaInterface *FunctionPara, VS_BOOL AutoRelease ) = 0;
    virtual class ClassOfSRPFunctionParaInterface *SRPAPI LuaToFunctionPara( VS_INT32 Index ) = 0;
    virtual VS_BOOL SRPAPI LuaIsFunctionPara( VS_INT32 Index ) = 0;

    virtual VS_BOOL SRPAPI LuaPushCommInterface( class ClassOfSRPCommInterface *CommInterface, VS_BOOL AutoRelease ) = 0;
    virtual class ClassOfSRPCommInterface *SRPAPI LuaToCommInterface( VS_INT32 Index ) = 0;
    virtual VS_BOOL SRPAPI LuaIsCommInterface( VS_INT32 Index ) = 0;

    virtual void SRPAPI ClearLuaGlobal( ) = 0;
    virtual VS_BOOL SRPAPI PreCompile(VS_CHAR *ScriptInterface,VS_INT8 *ScriptBuf,VS_INT32 ScriptBufSize, VS_CHAR *Name, VS_CHAR **ErrorInfo) = 0;
    virtual void SRPAPI GCCollect() = 0;

    virtual void SRPAPI PostDoBuffer( VS_CHAR *ScriptInterface,VS_INT8 *ScriptBuf,VS_INT32 ScriptBufSize,VS_BOOL IsUTF8,VS_CHAR *Name ) = 0;  //ScriptInterfaces length < 16
    virtual void SRPAPI PostDoBufferEx( VS_CHAR *ScriptInterface,VS_INT8 *ScriptBuf,VS_INT32 ScriptBufSize,VS_LuaInfoDispProc DispProc,VS_ULONG Para,VS_BOOL IsUTF8,VS_CHAR *Name ) = 0; //ScriptInterfaces length < 16
    virtual VS_BOOL SRPAPI DoFile( VS_CHAR *ScriptInterface,VS_CHAR *FileName, VS_CHAR **ErrorInfo, VS_CHAR *WorkDirectory, VS_BOOL IsUTF8 ) = 0;                             //ScriptInterfaces length < 16
    virtual VS_BOOL SRPAPI PostDoFile( VS_CHAR *ScriptInterface,VS_CHAR *FileName, VS_CHAR **ErrorInfo, VS_CHAR *WorkDirectory, VS_BOOL IsUTF8 ) = 0;                         //ScriptInterfaces length < 16

    virtual void SRPAPI LuaRegHook( void *FuncAddr ) = 0;
    virtual void SRPAPI LuaUnRegHook( void *FuncAddr ) = 0;

//---Callback function to collect garbage
    virtual VS_BOOL SRPAPI RegGCProc( VS_GCProc GCProc, VS_ULONG Para ) = 0;
    virtual void SRPAPI UnRegGCProc( VS_GCProc GCProc, VS_ULONG Para ) = 0;

    virtual VS_ULONG SRPAPI GetServiceGroupID( ) = 0;

    virtual VS_CHAR *SRPAPI QueryFirstService( VS_UUID *RetUuid ) = 0;
    virtual VS_CHAR *SRPAPI QueryNextService( VS_UUID *RetUuid ) = 0;
    virtual VS_CHAR *SRPAPI QueryActiveService( VS_UUID *RetUuid ) = 0;
    virtual VS_CHAR *SRPAPI GetServiceName(VS_UUID *ServiceID) = 0;
    virtual VS_BOOL SRPAPI GetServiceID(VS_CHAR *ServiceName,VS_UUID *ServiceID) = 0;
    virtual VS_BOOL SRPAPI IsValidUser(VS_CHAR *UserName,VS_CHAR *UserPass) = 0;

    virtual VS_BOOL SRPAPI ImportServiceEx( VS_UUID *ServiceID, VS_BOOL LoadRunModule ) = 0;
    virtual VS_BOOL SRPAPI ImportServiceWithPath( VS_CHAR *ServicePath,VS_CHAR *ServiceName, VS_BOOL LoadRunModule ) = 0;
    virtual VS_BOOL SRPAPI ImportService( VS_CHAR *ServiceName, VS_BOOL LoadRunModule ) = 0;
    virtual VS_CHAR *SRPAPI ImportDynaService( VS_CHAR *Url ) = 0;
    virtual VS_BOOL SRPAPI CreateService( VS_CHAR *ServicePath,VS_CHAR *ServiceName,VS_UUID *ServiceID,VS_CHAR *RootPass,VS_INT32 FrameInterval,VS_INT32 NetPkgSize,VS_INT32 UploadPkgSize,VS_INT32 DownloadPkgSize,VS_INT32 DataUpPkgSize,VS_INT32 DataDownPkgSize) = 0;
    virtual VS_BOOL SRPAPI CreateServiceEx( VS_CHAR *ServicePath,VS_CHAR *ServiceName,VS_UUID *ServiceID,VS_CHAR *RootPass,VS_INT32 FrameInterval,VS_INT32 NetPkgSize,VS_INT32 UploadPkgSize,VS_INT32 DownloadPkgSize,VS_INT32 DataUpPkgSize,VS_INT32 DataDownPkgSize) = 0;
    virtual VS_BOOL SRPAPI LoadServiceEx( VS_UUID *ServiceID,VS_CHAR *UserName, VS_CHAR *UserPass, VS_BOOL LoadRunModule ) = 0;
    virtual VS_BOOL SRPAPI LoadServiceWithPath( VS_CHAR *ServicePath,VS_CHAR *ServiceName,VS_CHAR *UserName, VS_CHAR *UserPass, VS_BOOL LoadRunModule ) = 0;
    virtual VS_BOOL SRPAPI LoadService( VS_CHAR *ServiceName,VS_CHAR *UserName, VS_CHAR *UserPass, VS_BOOL LoadRunModule ) = 0;
    virtual VS_BOOL SRPAPI ExportServiceHeader( VS_CHAR *ServiceName,VS_CHAR *Path ) = 0;
    virtual VS_BOOL SRPAPI ExportServiceDefine( VS_CHAR *ServiceName,VS_CHAR *FileName ) = 0;
    virtual void SRPAPI ClearService( ) = 0;
    virtual void SRPAPI ClearServiceEx( ) = 0;

    //--webservice
    virtual VS_BOOL SRPAPI RegisterServer( VS_CHAR *ServiceName ) = 0;
    virtual void SRPAPI AllocCooperator( VS_CHAR *ServiceName ) = 0;
    virtual void SRPAPI FreeCooperator( VS_CHAR *ServiceName ) = 0;
    virtual void SRPAPI GetServerUrlInfo( struct StructOfVSServerUrlInfo *ServerUrlInfo) = 0;
    virtual void SRPAPI WebServiceRefresh()=0;
    virtual VS_BOOL SRPAPI GetWSDL(VS_ULONG WSDLVersion,VS_CHAR *WebServiceHost,class ClassOfSRPBinBufInterface *BinBuf)=0;

    virtual class ClassOfSRPInterface *SRPAPI XmlToService(class ClassOfSRPSXMLInterface *SXMLInterface,VS_CHAR *DataPath,VS_CHAR *SegmentName,SRPParse_PrintProc PrintProc,VS_ULONG Para) = 0;

    virtual class ClassOfSRPInterface *SRPAPI GetSRPInterfaceEx( VS_UUID *ServiceID, VS_CHAR *UserName, VS_CHAR *UserPass ) = 0;
    virtual class ClassOfSRPInterface *SRPAPI GetSRPInterface( VS_CHAR *ServiceName, VS_CHAR *UserName, VS_CHAR *UserPass ) = 0;
    virtual class ClassOfSRPInterface *SRPAPI GetSRPInterface2( VS_CHAR *ServiceName, VS_GetUserInfoCallBackProc CallBackProc ) = 0;
    virtual class ClassOfSRPInterface *SRPAPI GetSRPInterfaceEx2( VS_UUID *ServiceID, VS_GetUserInfoCallBackProc CallBackProc ) = 0;

    //--------Dependency handling, server-side call
    virtual VS_BOOL SRPAPI ChangeDepend( VS_UUID *OldDependServiceID,VS_UUID *NewDependServiceID,VS_CHAR *NewServiceName ) = 0;
    virtual VS_BOOL SRPAPI AddDepend( VS_UUID *DependServiceID,VS_CHAR *NewServiceName ) = 0;

    //---System objects and events
    virtual VS_EVENTPARAM_RUNPARAM *SRPAPI GetRequestBuf() = 0;
    virtual void SRPAPI FreeResponseBuf(VS_EVENTPARAM_RUNPARAM *ResponseParam) = 0;
    virtual void SRPAPI FreeRequestBuf(VS_EVENTPARAM_RUNPARAM *RequestParam) = 0;
    virtual VS_EVENTPARAM_RUNPARAM *SRPAPI ProcessSysObjectEvent(VS_UUID *EventID,VS_EVENTPARAM_RUNPARAM *RequestParam) = 0;
    virtual void *SRPAPI GetSysObject() = 0;

    //--------
    virtual VS_CHAR *SRPAPI GetName(VS_UUID *ObjectID) = 0;
    virtual VS_INT8 *SRPAPI GetMD5(VS_INT8 *Buf,VS_INT32 BufSize) = 0;
    virtual VS_ULONG SRPAPI GetTickCount() = 0;

    virtual void SRPAPI GetID(void *Object,VS_UUID *UuidPtr) = 0;
    virtual VS_BOOL SRPAPI RegObjectIDChangeNotify(VS_ObjectIDChangeNotifyProc ChangeNotifyProc,VS_ULONG Para) = 0;
    virtual void SRPAPI UnRegObjectIDChangeNotify(VS_ObjectIDChangeNotifyProc ChangeNotifyProc,VS_ULONG Para) = 0;
    virtual VS_BOOL SRPAPI RegObjectFreeNotify(VS_ObjectFreeNotifyProc FreeNotifyProc,VS_ULONG Para) = 0;
    virtual void SRPAPI UnRegObjectFreeNotify(VS_ObjectFreeNotifyProc FreeNotifyProc,VS_ULONG Para) = 0;

//---------registry query callback function
    virtual VS_CHAR *SRPAPI GetRegStr(VS_CHAR *SubKey,VS_CHAR *ValueName,VS_CHAR *DefaultValue) = 0;
    virtual VS_ULONG SRPAPI GetRegInt(VS_CHAR *SubKey,VS_CHAR *ValueName,VS_ULONG DefaultValue) = 0;

//---------timer callback function
    virtual VS_ULONG SRPAPI SetupTimer(VS_INT32 Ticket,VS_TimerProc FunctionAddr,VS_ULONG Para1,VS_ULONG Para2,VS_ULONG Para3,VS_ULONG Para4 ) = 0;
    virtual void SRPAPI KillTimer(VS_ULONG TimerID) = 0;
    
    //---Raw Socket function
    virtual VS_ULONG SRPAPI SetupSocketServer( VS_CHAR *Interface, VS_UINT16 PortNumber, VS_ULONG *LinkInterfaceStatus,VS_SockAcceptCallBackProc CallBackProc,VS_ULONG Para) = 0;  //--返回ConnectionID
    virtual VS_ULONG SRPAPI SetupSocketClient( VS_CHAR *ServerInterface, VS_CHAR *ServerName, VS_UINT16 PortNumber, VS_SockEventCallBackProc CallBackProc,VS_ULONG Para ) = 0;
    virtual void SRPAPI CloseSocketConnect( VS_ULONG ConnectionID ) = 0;
    virtual VS_BOOL SRPAPI SocketSend( VS_ULONG MachineID, class ClassOfSRPParaPackageInterface *ParaPkg, VS_BOOL Assure ) = 0;
    virtual VS_BOOL SRPAPI SocketSendBin( VS_ULONG MachineID, VS_INT32 BinDataSize, VS_INT8 *BinDataBuf, VS_BOOL Assure ) = 0;
    virtual VS_ULONG SRPAPI SetupSocketTimer( VS_ULONG MachineID, VS_INT32 Ticket,VS_INT32 Counter ) = 0;
    virtual void SRPAPI KillSocketTimer( VS_ULONG MachineID, VS_ULONG TimerIndex ) = 0;

    //--------Data server address
    virtual VS_BOOL SRPAPI SetDataServerAddr(VS_BOOL DirectConnect,VS_CHAR *DataServerInterface,VS_CHAR *DataServerName,VS_UINT16 DataServerPort,VS_CHAR *LocalDataServerInterface,VS_UINT16 LocalDataServerPort) = 0;
    virtual void SRPAPI RegQueryStaticDataProc( VS_QueryObjectStaticDataProc Proc,VS_ULONG Para) = 0;
    virtual void SRPAPI UnRegQueryStaticDataProc( VS_QueryObjectStaticDataProc Proc,VS_ULONG Para) = 0;
    virtual void SRPAPI RegSaveStaticDataProc( VS_SaveObjectStaticDataProc Proc,VS_ULONG Para) = 0;
    virtual void SRPAPI UnRegSaveStaticDataProc( VS_SaveObjectStaticDataProc Proc,VS_ULONG Para) = 0;
    virtual void SRPAPI RegClearStaticDataProc( VS_ClearObjectStaticDataProc Proc,VS_ULONG Para) = 0;
    virtual void SRPAPI UnRegClearStaticDataProc( VS_ClearObjectStaticDataProc Proc,VS_ULONG Para) = 0;
    virtual void SRPAPI SetServerPara(VS_INT32 MaxClientNumber,VS_INT32 MaxDataServerConnectionNumber,VS_INT32 DataServerOverTime) = 0;

    //--------Statistics
    virtual void SRPAPI QuyeryStatisticInfo(void *Machine,VS_STATISTICINFO *InfoBuf) = 0;
    virtual VS_BOOL SRPAPI RegFileCallBack( VS_UUID *ServiceID,VS_FileUpDownLoadCallBackProc FileCallBackProc, VS_ULONG Para ) = 0;
    virtual void SRPAPI UnRegFileCallBack( VS_UUID *ServiceID,VS_FileUpDownLoadCallBackProc FileCallBackProc, VS_ULONG Para ) = 0;
    virtual VS_BOOL SRPAPI RegFileReqCallBack( VS_FileUpDownLoadRequestCallBackProc FileCallBackProc, VS_ULONG Para ) = 0;
    virtual void SRPAPI UnRegFileReqCallBack( VS_FileUpDownLoadRequestCallBackProc FileCallBackProc, VS_ULONG Para ) = 0;
    virtual void SRPAPI GetFileInfo( VS_UPDOWNFILEINFO *InfoPtr ) = 0;

    virtual VS_BOOL SRPAPI StringToUuid(VS_INT8 *String,VS_UUID *Uuid) = 0;
    virtual VS_INT8 *SRPAPI UuidToString(VS_UUID *Uuid) = 0;

//---Check the type of the current program: server, debug the server, the client
    virtual VS_UINT16 SRPAPI GetProgramType( ) = 0;
    virtual VS_BOOL SRPAPI IsDefaultServer() = 0;  //---If is the server, then is default or not
    virtual VS_BOOL SRPAPI IsWindowVisible() = 0;  //---Management window is visible
    virtual void SRPAPI HideWindow() = 0;       //---hide Management window
    virtual void SRPAPI ShowWindow() = 0;       //---show Management window
    virtual void SRPAPI SetWindowCaption(VS_CHAR *Caption) = 0;   //---Set the display window caption
    virtual void SRPAPI ExitVSSystem(VS_CHAR *ErrorInfo) = 0;            //---Exit the program
    virtual VS_BOOL SRPAPI IsAppActive() = 0;       //---Program is active
    virtual void SRPAPI SetIdleActive(VS_BOOL CreateFlag) = 0;    //---Is produced in a non-active state IDLE event
    virtual void SRPAPI GetVersion(VS_UINT8 *MainVersion,VS_UINT8 *SubVersion,VS_UINT16 *BuildVersion) = 0;
    virtual void SRPAPI GetVersionInfo(VS_CHAR *InfoBuf,VS_INT32 InfoBufSize) = 0;
    virtual VS_HWND SRPAPI GetWindowHandle(  ) = 0;
    virtual void SRPAPI GetWindowSize( VS_INT32 *Width, VS_INT32 *Height ) = 0;
    virtual void SRPAPI SetColor( VS_COLOR Text,VS_COLOR Explane,VS_COLOR ObjName,VS_COLOR AttrType,VS_COLOR Number,VS_COLOR Error ) = 0;
    virtual void SRPAPI SetBkColor( VS_COLOR BkColor ) = 0;
    virtual void SRPAPI ShowStatusMenu( VS_BOOL MenuShowFlag, VS_BOOL StatusShowFlag ) = 0;

//-----Client window function (to provide a simple output)
    virtual VS_HWND SRPAPI GetClientWndHandle(  ) = 0;
    virtual void SRPAPI GetClientWndSize( VS_INT32 *Width, VS_INT32 *Height ) = 0;
    virtual void SRPAPI SetClientWndSize( VS_INT32 Width, VS_INT32 Height ) = 0;  //---Set client window
    virtual void SRPAPI SetClientWndFocus( VS_HWND hWnd, VS_BOOL NeedAction ) = 0;
    virtual void SRPAPI KillClientWndFocus( VS_HWND hWnd, VS_BOOL NeedAction ) = 0;
    virtual void SRPAPI ClearClientWnd( ) = 0;
    virtual void SRPAPI HideClientWnd( ) = 0;
    virtual void SRPAPI ShowClientWnd( ) = 0;
    virtual void SRPAPI SetClientBkColor( VS_COLOR BkColor ) = 0;

    virtual VS_BOOL SRPAPI GetKeyState(VS_INT32 Key) = 0;
    virtual void SRPAPI SetWindowStyle( VSWINDOWSTYLE *Style  ) = 0;
    virtual void SRPAPI MoveWindow(VSWINDOWPOSITION *Position,VS_BOOL RepaintFlag) = 0;
    virtual void SRPAPI GetWindowPos(VSWINDOWPOSITION *Position) = 0;
    virtual void SRPAPI SetWindowStatus(VS_INT32 Status) = 0;  //--0 normal  1  minimize  other maximize

//-----Operating environment information (for complex output)
    virtual void SRPAPI RegRunEnv_FromParentCallBack( VS_RunEnvCallBackProc CallBack,VS_ULONG Para) = 0;
    virtual void SRPAPI UnRegRunEnv_FromParentCallBack( VS_RunEnvCallBackProc CallBack,VS_ULONG Para) = 0;
    virtual VS_BOOL SRPAPI RunEnvToChild(VS_UUID *ObjectID,struct StructOfVSRunEnv *RunEnvInfo) = 0;
    virtual VS_BOOL SRPAPI RunEnvToParent(struct StructOfVSRunEnv *RunEnvInfo) = 0;

//-----Message processing function hook
    virtual void SRPAPI SetMessageHook(VS_SRPMessageProcessHookProc HookProc) = 0;
    virtual VS_SRPMessageProcessHookProc SRPAPI GetMessageHook( ) = 0;

//---------Determine whether the synchronization service
    virtual VS_BOOL SRPAPI IsInSync( ) = 0;
    virtual VS_BOOL SRPAPI IsServiceSync() = 0;  /*--Service is synchronous--*/
    virtual VS_BOOL SRPAPI WaitServiceSync(VS_INT32 WaitTimeMs) = 0;  /*--Waiting service synchronization--*/

    virtual  VS_BOOL SRPAPI SRPIdle() = 0;
    virtual  void SRPAPI SRPAppActive() = 0;
    virtual  void SRPAPI SRPAppDeactive() = 0;

//---------Trigger a hyperlink
    virtual void SRPAPI HyperLink(VS_CHAR *HyperLink,VS_BOOL CreateNewWindow) = 0;
//---------Trigger a external defined event
    virtual void SRPAPI AppEvent(VS_ULONG EventID,VS_CHAR *EventInfo) = 0;

//---------Set exception handler
    virtual void SRPAPI SetExceptHandler(VS_ExceptHandlerProc ExceptHandlerProc) = 0;

//----Index related
    virtual void *SRPAPI CreateIndex_Nor(VS_INT32 KeyNumber,VS_UINT16 HashTableBits) = 0;  // KeyNumber == 1 ==2 ==3
    virtual void *SRPAPI CreateIndexCmp_Nor(VS_INT32 KeyNumber,VS_UINT16 HashTableBits,VS_IndexCompareProc CompareProc) = 0;  // KeyNumber == 1 ==2 ==3
    virtual void *SRPAPI CreateIDIndex_Nor(VS_UINT16 HashTableBits) = 0;
    virtual void *SRPAPI CreateIDIndexEx_Nor(VS_UINT16 HashTableBits) = 0;     //---UUID + DWORD
    virtual void *SRPAPI CreateIndex_Dbg(VS_INT32 KeyNumber,VS_UINT16 HashTableBits,VS_CHAR *FileName,VS_INT32 LineNumber) = 0;  // KeyNumber == 1 ==2 ==3
    virtual void *SRPAPI CreateIndexCmp_Dbg(VS_INT32 KeyNumber,VS_UINT16 HashTableBits,VS_IndexCompareProc CompareProc,VS_CHAR *FileName,VS_INT32 LineNumber) = 0;  // KeyNumber == 1 ==2 ==3
    virtual void *SRPAPI CreateIDIndex_Dbg(VS_UINT16 HashTableBits,VS_CHAR *FileName,VS_INT32 LineNumber) = 0;
    virtual void *SRPAPI CreateIDIndexEx_Dbg(VS_UINT16 HashTableBits,VS_CHAR *FileName,VS_INT32 LineNumber) = 0;     //---UUID + DWORD
    virtual void SRPAPI InsertOneKey(void *IndexContext,VS_ULONG MainKey,VS_INT8 *Buf) = 0;
    virtual VS_INT8 *SRPAPI FindOneKey(void *IndexContext,VS_ULONG MainKey) = 0;
    virtual VS_INT8 *SRPAPI DelOneKey(void *IndexContext,VS_ULONG MainKey) = 0;
    virtual VS_INT8 *SRPAPI QueryFirstOneKey(void *IndexContext,VS_QUERYRECORD *QueryRecord,VS_ULONG *MainKey) = 0;
    virtual VS_INT8 *SRPAPI QueryNextOneKey(void *IndexContext,VS_QUERYRECORD *QueryRecord,VS_ULONG *MainKey) = 0;
    virtual VS_INT8 *SRPAPI QueryFirstOneKeyA(void *IndexContext,VS_QUERYRECORD *QueryRecord,VS_ULONG *MainKey) = 0;
    virtual VS_INT8 *SRPAPI QueryNextOneKeyA(void *IndexContext,VS_QUERYRECORD *QueryRecord,VS_ULONG *MainKey) = 0;
    virtual void SRPAPI InsertTwoKey(void *IndexContext,VS_ULONG MainKey,VS_ULONG SecondKey,VS_INT8 *Buf) = 0;
    virtual VS_INT8 *SRPAPI FindTwoKey(void *IndexContext,VS_ULONG MainKey,VS_ULONG SecondKey) = 0;
    virtual VS_INT8 *SRPAPI DelTwoKey(void *IndexContext,VS_ULONG MainKey,VS_ULONG SecondKey) = 0;
    virtual VS_INT8 *SRPAPI QueryFirstTwoKey(void *IndexContext,VS_QUERYRECORD *QueryRecord,VS_ULONG *MainKey,VS_ULONG *SecondKey) = 0;
    virtual VS_INT8 *SRPAPI QueryNextTwoKey(void *IndexContext,VS_QUERYRECORD *QueryRecord,VS_ULONG *MainKey,VS_ULONG *SecondKey) = 0;
    virtual VS_INT8 *SRPAPI QueryFirstTwoKeyA(void *IndexContext,VS_QUERYRECORD *QueryRecord,VS_ULONG *MainKey,VS_ULONG *SecondKey) = 0;
    virtual VS_INT8 *SRPAPI QueryNextTwoKeyA(void *IndexContext,VS_QUERYRECORD *QueryRecord,VS_ULONG *MainKey,VS_ULONG *SecondKey) = 0;
    virtual VS_INT8 *SRPAPI QueryFirstTwoKey_F(void *IndexContext,VS_QUERYRECORD *QueryRecord,VS_ULONG MainKey,VS_ULONG *SecondKey) = 0;
    virtual VS_INT8 *SRPAPI QueryNextTwoKey_F(void *IndexContext,VS_QUERYRECORD *QueryRecord,VS_ULONG MainKey,VS_ULONG *SecondKey) = 0;
    virtual VS_INT8 *SRPAPI QueryFirstTwoKeyA_F(void *IndexContext,VS_QUERYRECORD *QueryRecord,VS_ULONG MainKey,VS_ULONG *SecondKey) = 0;
    virtual VS_INT8 *SRPAPI QueryNextTwoKeyA_F(void *IndexContext,VS_QUERYRECORD *QueryRecord,VS_ULONG MainKey,VS_ULONG *SecondKey) = 0;
    virtual void SRPAPI InsertThreeKey(void *IndexContext,VS_ULONG MainKey,VS_ULONG SecondKey,VS_ULONG ThirdKey,VS_INT8 *Buf) = 0;
    virtual VS_INT8 *SRPAPI FindThreeKey(void *IndexContext,VS_ULONG MainKey,VS_ULONG SecondKey,VS_ULONG ThirdKey) = 0;
    virtual VS_INT8 *SRPAPI DelThreeKey(void *IndexContext,VS_ULONG MainKey,VS_ULONG SecondKey,VS_ULONG ThirdKey) = 0;
    virtual VS_INT8 *SRPAPI QueryFirstThreeKey(void *IndexContext,VS_QUERYRECORD *QueryRecord,VS_ULONG *MainKey,VS_ULONG *SecondKey,VS_ULONG *ThirdKey) = 0;
    virtual VS_INT8 *SRPAPI QueryNextThreeKey(void *IndexContext,VS_QUERYRECORD *QueryRecord,VS_ULONG *MainKey,VS_ULONG *SecondKey,VS_ULONG *ThirdKey) = 0;
    virtual VS_INT8 *SRPAPI QueryFirstThreeKeyA(void *IndexContext,VS_QUERYRECORD *QueryRecord,VS_ULONG *MainKey,VS_ULONG *SecondKey,VS_ULONG *ThirdKey) = 0;
    virtual VS_INT8 *SRPAPI QueryNextThreeKeyA(void *IndexContext,VS_QUERYRECORD *QueryRecord,VS_ULONG *MainKey,VS_ULONG *SecondKey,VS_ULONG *ThirdKey) = 0;
    virtual VS_INT8 *SRPAPI QueryFirstThreeKey_F(void *IndexContext,VS_QUERYRECORD *QueryRecord,VS_ULONG MainKey,VS_ULONG *SecondKey,VS_ULONG *ThirdKey) = 0;
    virtual VS_INT8 *SRPAPI QueryNextThreeKey_F(void *IndexContext,VS_QUERYRECORD *QueryRecord,VS_ULONG MainKey,VS_ULONG *SecondKey,VS_ULONG *ThirdKey) = 0;
    virtual VS_INT8 *SRPAPI QueryFirstThreeKeyA_F(void *IndexContext,VS_QUERYRECORD *QueryRecord,VS_ULONG MainKey,VS_ULONG *SecondKey,VS_ULONG *ThirdKey) = 0;
    virtual VS_INT8 *SRPAPI QueryNextThreeKeyA_F(void *IndexContext,VS_QUERYRECORD *QueryRecord,VS_ULONG MainKey,VS_ULONG *SecondKey,VS_ULONG *ThirdKey) = 0;
    virtual VS_INT8 *SRPAPI QueryFirstThreeKey_S(void *IndexContext,VS_QUERYRECORD *QueryRecord,VS_ULONG MainKey,VS_ULONG SecondKey,VS_ULONG *ThirdKey) = 0;
    virtual VS_INT8 *SRPAPI QueryNextThreeKey_S(void *IndexContext,VS_QUERYRECORD *QueryRecord,VS_ULONG MainKey,VS_ULONG SecondKey,VS_ULONG *ThirdKey) = 0;
    virtual VS_INT8 *SRPAPI QueryFirstThreeKeyA_S(void *IndexContext,VS_QUERYRECORD *QueryRecord,VS_ULONG MainKey,VS_ULONG SecondKey,VS_ULONG *ThirdKey) = 0;
    virtual VS_INT8 *SRPAPI QueryNextThreeKeyA_S(void *IndexContext,VS_QUERYRECORD *QueryRecord,VS_ULONG MainKey,VS_ULONG SecondKey,VS_ULONG *ThirdKey) = 0;
    virtual void SRPAPI InsertIDKey(void *IndexContext,VS_UUID *UuidKey,VS_INT8 *Buf ) = 0;
    virtual VS_INT8 *SRPAPI FindIDKey(void *IndexContext,VS_UUID *UuidKey) = 0;
    virtual VS_INT8 *SRPAPI DelIDKey(void *IndexContext,VS_UUID *UuidKey) = 0;
    virtual VS_INT8 *SRPAPI QueryFirstIDKey(void *IndexContext,VS_QUERYRECORD *QueryRecord,VS_UUID *UuidKey) = 0;
    virtual VS_INT8 *SRPAPI QueryNextIDKey(void *IndexContext,VS_QUERYRECORD *QueryRecord,VS_UUID *UuidKey) = 0;
    virtual VS_INT8 *SRPAPI QueryFirstIDKeyA(void *IndexContext,VS_QUERYRECORD *QueryRecord,VS_UUID *UuidKey) = 0;
    virtual VS_INT8 *SRPAPI QueryNextIDKeyA(void *IndexContext,VS_QUERYRECORD *QueryRecord,VS_UUID *UuidKey) = 0;
    virtual void SRPAPI InsertIDKeyEx(void *IndexContext,VS_UUID *UuidKey,VS_ULONG ExKey,VS_INT8 *Buf) = 0;
    virtual VS_INT8 *SRPAPI FindIDKeyEx(void *IndexContext,VS_UUID *UuidKey,VS_ULONG ExKey) = 0;
    virtual VS_INT8 *SRPAPI DelIDKeyEx(void *IndexContext,VS_UUID *UuidKey,VS_ULONG ExKey) = 0;
    virtual VS_INT8 *SRPAPI QueryFirstIDKeyEx(void *IndexContext,VS_QUERYRECORD *QueryRecord,VS_UUID *UuidKey,VS_ULONG *ExKey) = 0;
    virtual VS_INT8 *SRPAPI QueryNextIDKeyEx(void *IndexContext,VS_QUERYRECORD *QueryRecord,VS_UUID *UuidKey,VS_ULONG *ExKey) = 0;
    virtual VS_INT8 *SRPAPI QueryFirstIDKeyEx_F(void *IndexContext,VS_QUERYRECORD *QueryRecord,VS_UUID *UuidKey,VS_ULONG *ExKey) = 0;
    virtual VS_INT8 *SRPAPI QueryNextIDKeyEx_F(void *IndexContext,VS_QUERYRECORD *QueryRecord,VS_UUID *UuidKey,VS_ULONG *ExKey) = 0;
    virtual VS_INT8 *SRPAPI QueryFirstIDKeyExA(void *IndexContext,VS_QUERYRECORD *QueryRecord,VS_UUID *UuidKey,VS_ULONG *ExKey) = 0;
    virtual VS_INT8 *SRPAPI QueryNextIDKeyExA(void *IndexContext,VS_QUERYRECORD *QueryRecord,VS_UUID *UuidKey,VS_ULONG *ExKey) = 0;
    virtual VS_INT32  SRPAPI GetKeyNumber(void *IndexContext) = 0;
    virtual void SRPAPI DelAllKey(void *IndexContext) = 0;
    virtual void SRPAPI DestoryIndex(void *IndexContext) = 0;
    virtual VS_ULONG SRPAPI GetHashValue(void *Key,VS_ULONG Length,VS_ULONG InitValue) = 0;

//----Memory management related
    virtual void *SRPAPI CreateMemory_Nor(VS_INT32 ItemSize) = 0;
    virtual void *SRPAPI CreateMemory_Dbg(VS_INT32 ItemSize,VS_CHAR *FileName,VS_INT32 LineNumber) = 0;
    virtual void *SRPAPI GetMemoryPtr_Nor(void *MemoryContext) = 0;
    virtual void *SRPAPI GetMemoryPtr_Dbg(void *MemoryContext,VS_CHAR *FileName,VS_INT32 LineNumber) = 0;
    virtual void *SRPAPI QueryFirstMemoryPtr(void *MemoryContext,VS_QUERYRECORD *QueryRecord) = 0;
    virtual void *SRPAPI QueryNextMemoryPtr(void *MemoryContext,VS_QUERYRECORD *QueryRecord) = 0;
    virtual void SRPAPI FreeMemoryPtr(void *MemoryContext,void *Ptr) = 0;
    virtual void SRPAPI ClearMemory(void *MemoryContext) = 0;
    virtual void SRPAPI DestoryMemory(void *MemoryContext) = 0;
    virtual void *SRPAPI Malloc_Nor(VS_INT32 MemorySize) = 0;
    virtual void *SRPAPI Malloc_Dbg(VS_INT32 MemorySize,VS_CHAR *FileName,VS_INT32 LineNumber) = 0;
    virtual void SRPAPI Free(void *MemoryPtr) = 0;
    virtual void SRPAPI GetMemoryUsed( VS_ULONG *KernelAllocSize,VS_ULONG *DataAllocSize,VS_ULONG *AppAllocSize, VS_ULONG *ScriptMemoryUsed ) = 0;

//------------------------------------------------------------------------------
//-----State machine related functions
    virtual VS_ULONG SRPAPI GetMachineID(void *Machine) = 0;
    virtual void *SRPAPI FindMachine(VS_ULONG MachineID) = 0;

//--------
    virtual VS_BOOL SRPAPI MD5ToUuid(VS_INT8 *String,VS_UUID *Uuid) = 0;
    virtual VS_INT8 *SRPAPI UuidToMD5(VS_UUID *Uuid) = 0;
    virtual VS_INT8 *SRPAPI StringToUtf8(VS_INT8 *String) = 0;
    virtual VS_INT8 *SRPAPI Utf8ToString(VS_INT8 *String) = 0;

    virtual VS_BOOL SRPAPI ProgramRestart( ) = 0;

    virtual VS_BOOL SRPAPI HttpDownLoad( VS_UUID *AttachObjectID, VS_CHAR *ServerUrl,VS_CHAR *ClientPath,VS_CHAR *FileName, VS_FileUpDownLoadCallBackProc CallBackProc, VS_UUID *ObjectID, VS_ULONG Para, VS_BOOL SaveFileFlag ) = 0;
    virtual void SRPAPI HttpDownLoadAbort(  ) = 0;
    virtual void SRPAPI RegWebDownFunction(VS_WebDownInfoProc CallBackProc,VS_ULONG Para) = 0;
    virtual void SRPAPI UnRegWebDownFunction(VS_WebDownInfoProc CallBackProc,VS_ULONG Para) = 0;
    virtual void SRPAPI WebDownPrint(VS_ULONG uMes, VS_CHAR *FileName, VS_ULONG MaxLength, VS_ULONG CurLength) = 0;

    virtual VS_BOOL SRPAPI CanSetStaticData(void *Object,VS_ULONG DataSize) = 0;
    virtual VS_BOOL SRPAPI SetStaticData( VS_UUID *ObjectID,VS_ULONG UniqueDataUnitID,VS_ULONG DataSize,VS_INT8 *DataBuf,VS_STATICID *RetDataVersion) = 0;

    virtual void *SRPAPI GetEnvDependCheckInfo(VS_CHAR *ServiceName,VS_ULONG *Size) = 0;  //--Get Service Depend Check Info
    virtual void SRPAPI SetEnvDependCheckInfo(VS_ULONG Size,void *Buf) = 0;  //--Set Service Depend Check Info
    virtual VS_UINT8 SRPAPI GetEnvStartType(void) = 0;  //--Get Env Start Type
    virtual class ClassOfSRPParaPackageInterface *SRPAPI GetEnvPara(void) = 0;  //--Get Env Para Info
    virtual void SRPAPI SetEnvPara(class ClassOfSRPParaPackageInterface *Para) = 0;  //--Set Env Para Info
    virtual void SRPAPI SetEnvInputPara(class ClassOfSRPParaPackageInterface *Para) = 0;  //--Set Env Input Para Info
    virtual class ClassOfSRPParaPackageInterface *SRPAPI GetEnvInputPara(void) = 0;  //--Get Env Input Para Info
    virtual VS_CHAR *SRPAPI GetEnvParentUrl(void) = 0;      //--
    virtual void SRPAPI SetEnvCurrentUrl(VS_CHAR *Url) = 0;
    virtual VS_INT32 SRPAPI RedirectToUrlRequest(VS_CHAR *Url,class ClassOfSRPParaPackageInterface *ParaPkg,RedirectToUrl_InfoProc CallBackProc,VS_ULONG Para,VS_CHAR *WorkDirectory,VS_CHAR *ChildTermScript) = 0;
    virtual void SRPAPI RedirectToUrlFail(VS_CHAR *Url) = 0;
    virtual void SRPAPI SetRedirectToUrlInfo(class ClassOfSRPParaPackageInterface *ParaPkg) = 0;      //--
    virtual void SRPAPI RedirectToUrlAbort() = 0;

    virtual VS_BOOL SRPAPI OpenLuaEdit(VS_CHAR *Module,VS_ULONG Config,VS_BOOL CloseEnable) = 0;
    virtual void SRPAPI LuaEditDisp(VS_CHAR *Info) = 0;
    virtual void SRPAPI CloseLuaEdit() = 0;
    virtual void SRPAPI LuaEditHelp(VS_INT32 Type,VS_CHAR *HelpInfo) = 0;

    virtual void SRPAPI RegDllCallBack( VS_MsgCallBackProc MsgCallBackProc, VS_ULONG MsgCallBackPara ) = 0;
    virtual void SRPAPI UnRegDllCallBack( VS_MsgCallBackProc MsgCallBackProc, VS_ULONG MsgCallBackPara ) = 0;

    virtual void SRPAPI CreateUuid(VS_UUID *UuidPtr) = 0;
    virtual void SRPAPI GetSRPTempPath(VS_ULONG BufSize,VS_CHAR *Buf) = 0;

//---------Compression decompression function, use the ZLIB
    virtual VS_BOOL SRPAPI Compress(VS_UINT8 *dest,VS_ULONG *destLen,VS_UINT8 *source,VS_ULONG sourceLen ) = 0;
    virtual VS_BOOL SRPAPI UnCompress(VS_UINT8 *dest,VS_ULONG *destLen,VS_UINT8 *source,VS_ULONG sourceLen ) = 0;

    virtual class ClassOfSRPMemoryFileInterface *SRPAPI CreateMemoryFile() = 0;
    virtual class ClassOfSRPMemoryFileInterface *SRPAPI GetEnvMemoryFile() = 0;
    virtual void SRPAPI SetEnvMemoryFile(class ClassOfSRPMemoryFileInterface *MemoryFile) = 0;

    virtual void SRPAPI GetUrl(VS_CHAR *Buf,VS_INT32 BufSize) = 0;
    virtual VS_BOOL SRPAPI ToAbsoluteUrl(VS_CHAR *InputUrl,VS_CHAR *OutputUrl,VS_INT32 OutputUrlBufSize ) = 0;
    virtual VS_INT32 SRPAPI RunFromUrl(VS_CHAR *Url,VS_INT8 RestartFlag,VS_BOOL WaitFlag) = 0;
    virtual VS_INT32 SRPAPI RunFromBuf(VS_CHAR *Buf,VS_ULONG BufSize,VS_INT8 RestartFlag,VS_BOOL WaitFlag) = 0;
    virtual VS_BOOL SRPAPI IsLoadServiceBusy() = 0;
    virtual VS_BOOL SRPAPI IsLoadServiceIdle() = 0;
    virtual void SRPAPI SetDepend(VS_CHAR *ServiceName,VS_BOOL DefaultUrlFlag) = 0;

    virtual VS_BOOL SRPAPI GetServicePathByName(VS_CHAR *ServiceName,VS_CHAR *ServicePath,VS_ULONG ServicePathSize) = 0;
    virtual void SRPAPI InsertSearchPath(VS_CHAR *SearchPath) = 0;
    virtual void SRPAPI ClearSearchPath() = 0;
    virtual VS_CHAR *SRPAPI FirstSearchPath(VS_QUERYRECORD *QueryRecord) = 0;
    virtual VS_CHAR *SRPAPI NextSearchPath(VS_QUERYRECORD *QueryRecord) = 0;

    virtual VS_BOOL SRPAPI GetSaveFile( VS_CHAR *Caption, VS_CHAR *Filter, VS_ULONG FilterIndex, VS_CHAR *DefExt, VS_CHAR *FileNameBuf,VS_INT32 FileNameBufSize) = 0;
    virtual VS_BOOL SRPAPI GetOpenFile( VS_CHAR *Caption, VS_CHAR *Filter, VS_ULONG FilterIndex, VS_CHAR *DefExt, VS_CHAR *FileNameBuf,VS_INT32 FileNameBufSize) = 0;

    virtual void SRPAPI SaveServiceStaticData(VS_UUID *ServiceID) = 0;
    virtual void SRPAPI GetStaticVersion( VS_ULONG DataSize,VS_INT8 *DataBuf,VS_STATICID *RetDataVersion) = 0;

    //---------------------------------------------------
    //---SRPDoc interface
    virtual void *SRPAPI GetSysDocClass() = 0;
    virtual void *SRPAPI FirstDoc(VS_QUERYRECORD *QueryRecord,VS_CHAR **DocName) = 0;
    virtual void *SRPAPI NextDoc(VS_QUERYRECORD *QueryRecord,VS_CHAR **DocName) = 0;
    virtual void SRPAPI RegisterDoc(void *DocObject,VS_CHAR *DocName) = 0;
    virtual void SRPAPI UnRegisterDoc(void *DocObject) = 0;
    virtual VS_EVENTPARAM_RUNPARAM *SRPAPI ProcessSysDocEvent(VS_UUID *DocObjectID,VS_UUID *EventID,VS_EVENTPARAM_RUNPARAM *RequestParam) = 0;
    virtual VS_BOOL SRPAPI RegDocEventFunction(VS_UUID *DocObjectID,VS_UUID *EventID, void *FuncAddr,VS_ULONG Para) = 0;
    virtual void SRPAPI UnRegDocEventFunction(VS_UUID *DocObjectID,VS_UUID *EventID, void *FuncAddr,VS_ULONG Para ) = 0;

    //---------------------------------------------------
    //---CopyToClipBoard
    virtual void SRPAPI ToClipBoard(VS_CHAR *Info) = 0;
    virtual VS_CHAR *SRPAPI FromClipBoard() = 0;

    //---Windowless Option
    virtual VS_BOOL SRPAPI IsWindowlessSite( ) = 0;
    virtual VS_BOOL SRPAPI IsWindowlessTransparent( ) = 0;
    virtual void SRPAPI RegWindowlessSiteCallBack(struct VSWindowlessSiteCallBackInfo *CallBackInfo, VS_UUID *ObjectID, VS_ULONG Para ) = 0;
    virtual void SRPAPI UnRegWindowlessSiteCallBack(struct VSWindowlessSiteCallBackInfo *CallBackInfo, VS_UUID *ObjectID, VS_ULONG Para ) = 0;
    virtual void SRPAPI Windowless_Draw( void *hDC, void *rcBounds, void *rcInvalid ) = 0;
    virtual VS_BOOL SRPAPI Windowless_Message( VS_ULONG uMes,VS_ULONG wParam,VS_ULONG LParam,VS_ULONG *Result ) = 0;
    virtual VS_BOOL SRPAPI Windowless_GetDropTarget( void **DropTarget ) = 0;
    virtual void SRPAPI Windowless_Redraw( VS_BOOL fErase ) = 0;
    virtual void SRPAPI Windowless_GetDC( void **hDC,VS_RECT *rEct ) = 0;
    virtual void SRPAPI Windowless_ReleaseDC( void *hDC ) = 0;

    virtual VS_BOOL SRPAPI SetClientPort(VS_CHAR *ClientInterface,VS_UINT16 ClientPortNumber) = 0;
    virtual VS_BOOL SRPAPI SetDebugPort(VS_CHAR *DebugInterface,VS_UINT16 DebugPortNumber) = 0;
    virtual VS_BOOL SRPAPI SetTelnetPort(VS_UINT16 TelnetPortNumber) = 0;
    virtual VS_BOOL SRPAPI SetOutputPort(VS_CHAR *OutputHost,VS_UINT16 OutputPortNumber) = 0;
    virtual VS_BOOL SRPAPI SetWebServerPort(VS_CHAR *WebServerHost,VS_UINT16 WebServerPortNumber,VS_INT32 ConnectionNumber,VS_ULONG PostSize) = 0;

    virtual VS_UUID *SRPAPI GetVSObjectID(VS_INT32 Which) = 0;

//---------Access control interface
    virtual class ClassOfSRPControlInterface *SRPAPI GetSRPControlInterface() = 0;
    virtual class ClassOfSRPFunctionParaInterface *SRPAPI GetFunctionParaInterface() = 0;
    virtual class ClassOfSRPParaPackageInterface *SRPAPI GetParaPkgInterface() = 0;
    virtual class ClassOfSRPLockInterface *SRPAPI GetSRPLockInterface(void) = 0;
    virtual class ClassOfSRPSXMLInterface *SRPAPI GetSXMLInterface() = 0;
    virtual class ClassOfSRPBinBufInterface *SRPAPI GetSRPBinBufInterface(void) = 0;
	virtual class ClassOfSRPCommInterface *SRPAPI GetCommInterface() = 0;
    virtual class ClassOfSRPFileDiskInterface *SRPAPI GetFileDiskInterface() = 0;

    virtual void SRPAPI GetSRPConfigPath(VS_ULONG BufSize,VS_CHAR *Buf) = 0;

    //temp file
    virtual VS_BOOL SRPAPI RegTempFile(VS_CHAR *TempFileName,VS_CHAR *OriFileName) = 0;     /*clear when process not exist*/
    virtual VS_CHAR *SRPAPI GetRegTempFile(VS_CHAR *OriFileName,VS_CHAR *Buf,VS_INT32 BufSize) = 0;   /*get temp filename of other process*/
    virtual void SRPAPI UnRegTempFile(VS_CHAR *TempFileName) = 0;   /*clear when process not exist,temp file will be delete by starcore*/

    virtual void SRPAPI GetConfigResult(VS_BOOL *DebugCfgResult,VS_BOOL *DirectClientCfgResult,VS_BOOL *TelnetCfgResult,VS_BOOL *WebServerCfgResult) = 0;
    virtual VS_CHAR *SRPAPI GetConfigEnvTag( ) = 0;

    //---SRPDispatchCallBack
    virtual void SRPAPI RegDispatchCallBack(VS_SRPDispatchCallBackProc CallBack,VS_ULONG Para) = 0;
    virtual void SRPAPI UnRegDispatchCallBack(VS_SRPDispatchCallBackProc CallBack,VS_ULONG Para) = 0;

    virtual class ClassOfBasicSRPInterface *SRPAPI Dup() = 0;
    virtual void *SRPAPI QueryInterface( VS_UUID *InterfaceID ) = 0;

    virtual VS_BOOL SRPAPI LockLuaTable( ) = 0;
    virtual VS_BOOL SRPAPI UnLockLuaTable( ) = 0;

    virtual VS_UUID *SRPAPI GetIDEx(void *Object) = 0;
    virtual VS_BOOL SRPAPI IsRootService() = 0;

    virtual void SRPAPI AddRef() = 0;
    virtual VS_ULONG SRPAPI GetRef() = 0;

    virtual void SRPAPI GetConfig(class ClassOfSRPSXMLInterface *XmlInterface) = 0;
    virtual void SRPAPI GetConfigHost( VS_CHAR *Buf,VS_INT32 BufSize ) = 0;

    virtual VS_CHAR *SRPAPI LuaToLString(VS_INT32 index,VS_ULONG *len) = 0;
    virtual VS_BOOL SRPAPI LuaIsInt( VS_INT32 Index ) = 0;
};
//==============================================================================
//            Binary buffer user interface
//==============================================================================
#define SRPBINTYPE_BOOL   ((VS_UINT8)'z')
#define SRPBINTYPE_BYTE   ((VS_UINT8)'b')
#define SRPBINTYPE_UBYTE  ((VS_UINT8)'B')
#define SRPBINTYPE_CHAR   ((VS_UINT8)'c')
#define SRPBINTYPE_SHORT  ((VS_UINT8)'h')
#define SRPBINTYPE_USHORT ((VS_UINT8)'H')
#define SRPBINTYPE_INT    ((VS_UINT8)'i')
#define SRPBINTYPE_UINT   ((VS_UINT8)'I')
#define SRPBINTYPE_LONG   ((VS_UINT8)'l')
#define SRPBINTYPE_ULONG  ((VS_UINT8)'L')
#define SRPBINTYPE_FLOAT  ((VS_UINT8)'d')          //double
#define SRPBINTYPE_STRING     ((VS_UINT8)'s')      //With the end of the 0
#define SRPBINTYPE_STRINGEX   ((VS_UINT8)'S')     //Without the end of the 0
#define SRPBINTYPE_UNICODE    ((VS_UINT8)'u')      //With the end of the 0
#define SRPBINTYPE_UNICODEEX  ((VS_UINT8)'U')     //Without the end of the 0
#define SRPBINTYPE_UTF8       ((VS_UINT8)'t')      //With the end of the 0
#define SRPBINTYPE_UTF8EX     ((VS_UINT8)'T')     //Without the end of the 0
#define SRPBINTYPE_BIN    ((VS_UINT8)'r')      //Binary

class ClassOfSRPBinBufInterface{
public:
    ClassOfSRPBinBufInterface(){};
    virtual ~ClassOfSRPBinBufInterface(){};
    virtual void SRPAPI Release() = 0;

    virtual void SRPAPI Init(VS_UINT32 BufSize) = 0;  //--Set the maximum buffer, and if equal to 0, is automatically extended
    virtual VS_UINT32 SRPAPI GetSize() = 0;
    virtual VS_UINT32 SRPAPI GetOffset() = 0;
    virtual VS_INT8  *SRPAPI GetBuf() = 0;
    virtual void SRPAPI Clear() = 0;
    virtual void SRPAPI ClearEx( VS_UINT32 Offset, VS_UINT32 Length ) = 0;
    virtual VS_BOOL SRPAPI Set(VS_UINT32 Offset,VS_UINT32 Length,VS_INT8 *Buf) = 0;
    virtual VS_BOOL SRPAPI Get(VS_UINT32 Offset,VS_UINT32 Length,VS_INT8 *Buf) = 0;
    virtual VS_INT8 *SRPAPI GetBufPtr(VS_UINT32 Offset) = 0;
    virtual VS_INT8 *SRPAPI GetBufPtrEx(VS_UINT32 Offset,VS_UINT32 Length) = 0;
    virtual VS_BOOL SRPAPI SetOffset(VS_UINT32 Offset) = 0;
    virtual VS_BOOL SRPAPI Fill(VS_UINT32 Offset,VS_UINT32 Length,VS_UINT8 Value) = 0;
    virtual VS_BOOL SRPAPI Expand(VS_INT32 NewBufSize) = 0;

    virtual VS_BOOL SRPAPI PackObject(void *Object) = 0;
    virtual VS_BOOL SRPAPI UnPackObject(void *Object) = 0;

    virtual VS_BOOL SRPAPI ToUTF8(void) = 0;
    virtual VS_BOOL SRPAPI ToAnsi(void) = 0;

    virtual class ClassOfSRPBinBufInterface *SRPAPI Dup() = 0;

    virtual void SRPAPI Print(VS_UINT32 Offset,VS_CHAR *format,...) = 0;                //insert   result is not zero end,MaxLength is 10240
    virtual void SRPAPI PrintVar(VS_UINT32 Offset,VS_CHAR *format,va_list argList) = 0; //insert   result is not zero end,MaxLength is 10240
    virtual void SRPAPI Insert(VS_UINT32 Offset,VS_UINT32 Length,VS_INT8 *Buf) = 0;
    virtual VS_INT32 SRPAPI FindStr(VS_UINT32 Offset,VS_CHAR *Str) = 0;  //return the offset from input offset
    virtual VS_INT32 SRPAPI FindStri(VS_UINT32 Offset,VS_CHAR *Str) = 0;  //return the offset from input offset

    virtual VS_BOOL SRPAPI SetLightBuf(VS_UINT32 Length,VS_INT8 *Buf) = 0;  //clear current data, light buffer only read,not support write
    virtual VS_BOOL SRPAPI IsLightBuf() = 0;

    virtual void SRPAPI AddRef() = 0;
    virtual VS_ULONG SRPAPI GetRef() = 0;

    virtual VS_BOOL SRPAPI AnsiToUnicode(VS_CHAR *Code,VS_INT32 BytesPerChar) = 0;          //Code ignored on window,, maybe UCS2 UCS4 UTF-16 UTF-32 UTF-16BE UTF-16LE UTF-32BE UTF-32LE
    virtual VS_BOOL SRPAPI UnicodeToAnsi(VS_CHAR *Code,VS_INT32 BytesPerChar) = 0;   //BytesPerChar should be 2 on windows

    virtual VS_ULONG SRPAPI WriteFromMemoryFile(class ClassOfSRPInterface *SRPInterface,VS_UINT32 Offset,VS_CHAR *FileName) = 0;

	virtual VS_INT8 *SRPAPI GetMD5() = 0;
	virtual VS_ULONG SRPAPI GetHashValue() = 0;
};

//==============================================================================
//            Packaged object interfaces and parameters
//==============================================================================
#define SRPPARATYPE_INVALID   0     //--invalid
#define SRPPARATYPE_INT       1     //--integer
#define SRPPARATYPE_FLOAT     2     //--double
#define SRPPARATYPE_BIN       3
#define SRPPARATYPE_CHARPTR   4     //--string
#define SRPPARATYPE_TIME      5     //--datetime
#define SRPPARATYPE_BOOL      6     //--bool

class ClassOfSRPParaPackageInterface{
public:
    ClassOfSRPParaPackageInterface(){};
    virtual ~ClassOfSRPParaPackageInterface(){};
    virtual void SRPAPI Release() = 0;
//-----
    virtual VS_INT32 SRPAPI GetNumber() = 0;
    virtual void SRPAPI Clear() = 0;

    virtual VS_BOOL SRPAPI InsertInt(VS_INT32 Index,VS_INT32 Para) = 0;
    virtual VS_BOOL SRPAPI InsertFloat(VS_INT32 Index,VS_DOUBLE Para) = 0;
    virtual VS_BOOL SRPAPI InsertStr(VS_INT32 Index,VS_CHAR *Para) = 0;
    virtual VS_BOOL SRPAPI InsertBin(VS_INT32 Index,VS_INT8 *Para,VS_INT32 Size) = 0;
    virtual VS_BOOL SRPAPI InsertTime(VS_INT32 Index,VS_TIME *hTime) = 0;

    virtual VS_BOOL SRPAPI InsertEmpty(VS_INT32 Index) = 0;

    virtual VS_BOOL SRPAPI ExChange(VS_INT32 DesIndex,VS_INT32 SrcIndex) = 0;
    virtual void SRPAPI Del(VS_INT32 Index) = 0;

    virtual VS_INT32 SRPAPI GetType(VS_INT32 Index) = 0;   //---begin with 0
    virtual VS_INT32 SRPAPI GetInt(VS_INT32 Index) = 0;    //---From 0
    virtual VS_DOUBLE SRPAPI GetFloat(VS_INT32 Index) = 0;   //---From 0
    virtual VS_CHAR *SRPAPI GetStr(VS_INT32 Index) = 0;      //---From 0
    virtual VS_INT8 *SRPAPI GetBin(VS_INT32 Index,VS_INT32 *Length) = 0;      //---From 0
    virtual VS_BOOL SRPAPI GetTime(VS_INT32 Index,VS_TIME *hTime) = 0;      //---From 0

    virtual class ClassOfSRPParaPackageInterface *SRPAPI Dup() = 0;
    virtual VS_BOOL SRPAPI AppendFrom(class ClassOfSRPParaPackageInterface *ParaPkg) = 0;

    //-----for transmission and group
    virtual void SRPAPI SetChangeFlag(VS_INT32 Index) = 0;   //---From 0
    virtual void SRPAPI ClearChangeFlag(VS_INT32 Index) = 0;   //---From 0
    virtual void SRPAPI SetChangeFlagEx() = 0;   //---All items marked change
    virtual void SRPAPI ClearChangeFlagEx() = 0;   //---Clear all items marked changes
    virtual VS_BOOL SRPAPI IsChangeFlag(VS_INT32 Index) = 0;   //---From 0 ，To determine whether changes
    virtual VS_BOOL SRPAPI IsChangeFlagEx() = 0;   //---Determine whether there is a change item
    virtual VS_INT8 *SRPAPI SaveChangeToBuf( VS_BOOL CompressFlag,VS_INT32 *RetSize )=0;
    virtual VS_INT8 *SRPAPI SaveChangeToBufEx( VS_BOOL CompressFlag,VS_INT32 *RetSize )=0;
    virtual VS_BOOL SRPAPI LoadChangeFromBuf( VS_INT32 BufSize, VS_INT8 *Buf ) = 0;   //----restore the contents of the buffer, return the actual length

    virtual void SRPAPI FreeBuf( VS_INT8 *Buf ) = 0;

    virtual void SRPAPI AddRef() = 0;
    virtual VS_ULONG SRPAPI GetRef() = 0;

    virtual VS_BOOL SRPAPI InsertBool(VS_INT32 Index,VS_BOOL Para) = 0;
    virtual VS_BOOL SRPAPI GetBool(VS_INT32 Index) = 0;    //---From 0

};

//==============================================================================
//            Function call parameters encapsulated interfaces, Max Para Number Is 64
//            Each parameter occupies 4 bytes (32 bits), the parameter packages and objects, then pass a pointer
//  The input parameter types supported:VS_BOOL，VS_INT8, VS_UINT8, VS_INT16, VS_UINT16,VS_INT32,VS_UINT32,VS_LONG,VS_ULONG, VS_FLOAT,VSTYPE_CHARPTR,VSTYPE_PARAPKGPTR,VS_OBJPTR
//  Return type:  VS_BOOL，VS_INT32，VS_FLOAT，VS_CHARPTR，VS_PARAPKGPTR，VS_OBJPTR
//==============================================================================
class ClassOfSRPFunctionParaInterface{
public:
    ClassOfSRPFunctionParaInterface(){};
    virtual ~ClassOfSRPFunctionParaInterface(){};
    virtual void SRPAPI Release() = 0;

    virtual void SRPAPI Clear() = 0;
//-----
    virtual VS_INT32 SRPAPI GetNumber() = 0;
    virtual VS_UINT8 SRPAPI GetType(VS_INT32 Index) = 0;   //---From 0
    virtual VS_ULONG SRPAPI GetValue(VS_INT32 Index) = 0;   //---From 0

    virtual VS_BOOL SRPAPI SetValue(VS_INT32 Index,VS_UINT8 In_Type,VS_ULONG In_Para) = 0;   //---From 0
    virtual VS_BOOL SRPAPI Call(void *Object,VS_UUID *FunctionID,VS_ULONG *RetValue,VS_UINT8 *RetType) = 0;

    virtual class ClassOfSRPFunctionParaInterface *SRPAPI Dup() = 0;

    virtual void SRPAPI AddRef() = 0;
    virtual VS_ULONG SRPAPI GetRef() = 0;
};


//==============================================================================
//--used in QueryFirstAtomicInfo and QueryNextAtomicInfo function
#define SRPATOMICQUERYTYPE_SYSROOTITEM   0      //--Para1:SysRootItemname address，Para2:DependSysRootItem address，other invalid
#define SRPATOMICQUERYTYPE_MACRO         1      //--Para1:MacroName addreee，other invalid
#define SRPATOMICQUERYTYPE_MACROITEM     2      //--Para1:Macro atomicobject，Para2 ret: MacroItemName address，Para3 ret: MacroItemValue address，other invalid
#define SRPATOMICQUERYTYPE_MODULE        3      //--Para1 ret: ModuleName address，other invalid
#define SRPATOMICQUERYTYPE_EDITMODULE    4      //--Para1 ret:ModuleName address，other invalid
#define SRPATOMICQUERYTYPE_STRUCT        5      //--Para1 ret:StructName address，other invalid
#define SRPATOMICQUERYTYPE_OBJECT        6      //--Para1:parent atomicobject，Para2 : parent property global index，Para３ ret: class atomicobject，Para４ ret:ObjectName address，other invalid
#define SRPATOMICQUERYTYPE_ATTACHATTRIBUTE  7   //--Para1:parent atomicobject，Para2 ret:AttributeName address，Para3 ret:type，Para４ ret:length，Para5 ret:struct atomicobject
#define SRPATOMICQUERYTYPE_ATTRIBUTE     8      //--Para1:parent atomicobject，Para2 ret:AttributeName address，Para3 ret:type，Para４ ret:length度，Para5 ret:struct atomicobject
#define SRPATOMICQUERYTYPE_FUNCRETATTRIBUTE   9      //--Para1:parent atomicobject，Para2 ret:AttributeName address，Para3 ret:type，Para４ ret:length，Para5 ret:struct atomicobject
#define SRPATOMICQUERYTYPE_FUNCPARAATTRIBUTE  10      //--Para1:parent atomicobject，Para2 ret:AttributeName address，Para3 ret:type，Para４ ret:length，Para5 ret:struct atomicobject
#define SRPATOMICQUERYTYPE_STRUCTATTRIBUTE  11      //--Para1:parent atomicobject，Para2 ret:AttributeName address，Para3 ret:type，Para４ ret:length，Para5 ret:struct atomicobject
#define SRPATOMICQUERYTYPE_SCRIPT        12      //--Para1:parent atomicobject，Para2 ret:ScriptName address，Para3 ret:ScriptBuf，other invalid
#define SRPATOMICQUERYTYPE_FUNCTION      13      //--Para1:parent atomicobject，Para2 ret:回FunctionName address，Para3 ret:0 general function, 1 lua function，Para４ ret: overloaded function atomicobject，other invalid
#define SRPATOMICQUERYTYPE_INEVENT       14      //--Para1:parent atomicobject，Para2 ret:InEventName address，Para3 ret:OutEventName，other invalid
#define SRPATOMICQUERYTYPE_OUTEVENT      15      //--Para1:parent atomicobject，Para2 ret:OutEventName address，other invalid

//---RemoteCallAttach
struct StructOfVSRemoteCallRequestAttach_WebService{
    struct StructOfSRPComm_HttpOnRequest *HttpRequest;
    class ClassOfSRPSXMLInterface *SoapInfo;
    VS_CHAR *OperationName;
    struct{
        VS_CHAR  *ContentType;
        VS_ULONG MimeDataSize;
        VS_INT8  *MimeDataBuf;
    }MimeData;
    VS_CHAR  *MimeContentType;
};
struct StructOfVSRemoteCallResponseAttach_WebService{
    class ClassOfSRPSXMLInterface *SoapInfo;
    struct{
        VS_ULONG MimeDataSize;
        VS_INT8  *MimeDataBuf;
    }MimeData;
    VS_CHAR  *MimeContentType;
};

class ClassOfSRPInterface{
public:
    ClassOfSRPInterface(){};
    virtual ~ClassOfSRPInterface(){};
    virtual void SRPAPI Release() = 0;
//-----
    virtual VS_INT32 SRPAPI GetObjectRegisterInfo(VS_UUID ObjectID,VS_INT8 *InBuf,VS_INT32 InBufLength,VS_INT8 *OutBuf,VS_INT32 OutBufSize) = 0;
    virtual void SRPAPI RegisterObjectDependency(VS_UUID ModuleID,VS_UUID ObjectID,VS_INT32 DependIndex,VS_UUID DependObjectID,VS_INT32 DependType,VS_INT32 DependItemNumber,VS_DEPENDATTRIBUTE *DependItemList) = 0;
    virtual void SRPAPI RegisterObjectFunction(VS_UUID ModuleID,VS_UUID ObjectID,VS_UUID FunctionID,void *CallBackPtr,VS_INT32 DependIndexNumber,VS_INT32 *DependIndex) = 0;
    virtual void SRPAPI RegisterObjectSysEvent(VS_UUID ModuleID,VS_UUID ObjectID,void *CallBackPtr,VS_INT32 DependIndexNumber,VS_INT32 *DependIndex) = 0;
    virtual void SRPAPI RegisterObjectSysEditEvent(VS_UUID ModuleID,VS_UUID ObjectID,void *CallBackPtr,VS_INT32 DependIndexNumber,VS_INT32 *DependIndex) = 0;
    virtual void SRPAPI RegisterObjectInEvent(VS_UUID ModuleID,VS_UUID ObjectID,VS_UUID InEventID,void *CallBackPtr,VS_INT32 DependIndexNumber,VS_INT32 *DependIndex) = 0;
    virtual void SRPAPI RegisterQueryObjectInfo(VS_UUID ModuleID,VS_UUID ObjectID,void *CallBackPtr) = 0;

    virtual VS_INT32  SRPAPI RegisterDynamicModule( VS_UUID ModuleID, VSModuleFunction_ModuleInitProc ModuleInitProc, VSModuleFunction_ModuleTermProc ModuleTermProc, VSModuleFunction_RequestRegisterObjectProc RequestRegisterObjectProc) = 0;

    virtual VS_UINT16 SRPAPI GetOsType( ) = 0;

//------------------------------------------------------------------------------
    virtual VS_BOOL SRPAPI StringToUuid(VS_INT8 *String,VS_UUID *Uuid) = 0;
    virtual VS_INT8 *SRPAPI UuidToString(VS_UUID *Uuid) = 0;

//------------------------------------------------------------------------------
    virtual void *SRPAPI GetParent(void *Object) = 0;
    virtual OBJECTATTRIBUTEINDEX SRPAPI GetIndex(void *Object) = 0;   //the index of the parent object
    virtual VS_UINT16 SRPAPI GetOrder(void *Object) = 0;   //the order in parent object queue
    virtual void *SRPAPI GetClass(void *Object) = 0;
    virtual void SRPAPI GetClassID(void *Object,VS_UUID *UuidPtr) = 0;
    virtual void SRPAPI GetID(void *Object,VS_UUID *UuidPtr) = 0;
    virtual void *SRPAPI GetObject(VS_UUID *ObjectID) = 0;
    virtual void *SRPAPI GetObjectEx(void *ParentObject,VS_CHAR *Name) = 0;
    virtual void *SRPAPI GetObjectEx2(VS_CHAR *ServiceName,VS_CHAR *Name) = 0;
    virtual void *SRPAPI GetSRPObject(VS_UUID *ObjectID) = 0;   //--Equivalent with GetObject
    virtual void *SRPAPI GetSRPObjectEx(void *ParentObject,VS_CHAR *Name) = 0;   //--Equivalent with GetObjectEx
    virtual void *SRPAPI GetPrevEx(void *Object) = 0;
    virtual void *SRPAPI GetNextEx(void *Object) = 0;
    virtual void *SRPAPI QueryFirst(void *VSObject) = 0;
    virtual void *SRPAPI QueryFirstChild(void *Object,OBJECTATTRIBUTEINDEX AttributeIndex) = 0;
    virtual void *SRPAPI QueryNext(void *Object) = 0;
    virtual void *SRPAPI QueryPrev(void *Object) = 0;
    virtual void *SRPAPI QueryFirstEx(VS_QUERYRECORD *QueryRecord) = 0;
    virtual void *SRPAPI QueryNextEx(VS_QUERYRECORD *QueryRecord) = 0;
    virtual VS_BOOL SRPAPI IsObject(void *Object) = 0;
    virtual void *SRPAPI QueryFirstActiveChild(void *Object,VS_ULONG *Context) = 0;
    virtual void *SRPAPI QueryNextActiveChild(VS_ULONG *Context) = 0;
    virtual VS_BOOL SRPAPI IsObjectInActiveSet(void *Object) = 0;
    virtual VS_BOOL SRPAPI GetWebServiceFlag(void *Object) = 0;
    virtual VS_BOOL SRPAPI SetWebServiceFlag(void *Object,VS_BOOL WebServiceFlag) = 0;

//------------------------------------------------------------------------------
    virtual void *SRPAPI QueryFirstInst(VS_QUERYRECORD *QueryRecord,void *ClassObject) = 0;
    virtual void *SRPAPI QueryNextInst(VS_QUERYRECORD *QueryRecord,void *ClassObject) = 0;
    virtual void *SRPAPI QueryFirstInstEx(VS_QUERYRECORD *QueryRecord,VS_UUID *ObjectClassID) = 0;
    virtual void *SRPAPI QueryNextInstEx(VS_QUERYRECORD *QueryRecord,VS_UUID *ObjectClassID) = 0;
    virtual void SRPAPI QueryInstClose(VS_QUERYRECORD *QueryRecord) = 0;
    virtual VS_CHAR *SRPAPI GetName(void *Object) = 0;
    virtual void SRPAPI SetName(void *Object,VS_CHAR *Name) = 0;
    virtual VS_BOOL SRPAPI IsInst(VS_UUID *ObjectClassID,void *Object) = 0;
    virtual VS_BOOL SRPAPI IsDirectInst(VS_UUID *ObjectClassID,void *Object) = 0;
    virtual VS_BOOL SRPAPI IsChild(void *ParentObject,void *Object) = 0;
    virtual void SRPAPI GetObjectSysRootItemID(void *Object,VS_UUID *UuidPtr) = 0;
    virtual void *SRPAPI GetObjectSysRootItem(void *Object) = 0;
    virtual VS_BOOL SRPAPI IsThisService(void *Object) = 0;
    virtual VS_BOOL SRPAPI IsActiveServiceObject(void *Object) = 0;
    virtual VS_BOOL SRPAPI IsThisClient(void *Object) = 0;
    virtual VS_ULONG SRPAPI GetClientID(void *Object) = 0;  //--Available to clients, server and debugger to return 0.
    virtual VS_BOOL SRPAPI ExportObjectHeader( void *Object, VS_CHAR *FileName ) = 0;

//----object save and load
    virtual void SRPAPI SetSaveFlag(void *Object,VS_UINT8 SaveFlag) = 0;
    virtual VS_UINT8 SRPAPI GetSaveFlag(void *Object) = 0;
    virtual VS_INT8 *SRPAPI SaveToBuf( void *Object, VS_INT32 *ObjectSize, VS_CHAR *Password, VS_UINT8 SaveFlag, VS_BOOL SaveNameValue ) = 0;
    virtual VS_BOOL SRPAPI SaveToFile( void *Object, VS_CHAR *FileName, VS_CHAR *Password, VS_UINT8 SaveFlag, VS_BOOL SaveNameValue ) = 0;
    virtual VS_BOOL SRPAPI LoadFromBuf( void *Object, VS_INT8 *Buf, VS_INT32 BufSize, VS_CHAR *Password, VS_BOOL LoadAsLocal,VS_BOOL LoadNameValue,VS_BOOL UpdateFlag ) = 0;
    virtual void SRPAPI DeferLoadFromBuf( void *Object, VS_INT8 *Buf, VS_INT32 BufSize, VS_CHAR *Password, VS_BOOL LoadAsLocal,VS_BOOL LoadNameValue,VS_BOOL UpdateFlag ) = 0;
    virtual VS_BOOL SRPAPI LoadFromFile( void *Object, VS_CHAR *FileName, VS_CHAR *Password, VS_BOOL LoadAsLocal,VS_BOOL LoadNameValue,VS_BOOL UpdateFlag, VS_BOOL StaticDataUseFile ) = 0;
    virtual void SRPAPI DeferLoadFromFile( void *Object, VS_CHAR *FileName, VS_CHAR *Password, VS_BOOL LoadAsLocal,VS_BOOL LoadNameValue,VS_BOOL UpdateFlag, VS_BOOL StaticDataUseFile ) = 0;
    virtual void SRPAPI ResetLoad( void *Object ) = 0;
    virtual void SRPAPI RegLoadMapAttrNameProc( VS_LoadMapAttrNameProc LoadMapAttrNameProc ) = 0;

//----objec control
    virtual VS_BOOL SRPAPI IsLocalControl( void *Object ) = 0;
    virtual VS_BOOL SRPAPI IsRemoteCreate( void *Object ) = 0;
    virtual void SRPAPI SetIDInParent( void *Object,VS_UINT16 IDInParent ) = 0;
    virtual VS_UINT16 SRPAPI GetIDInParent( void *Object ) = 0;
    virtual void *SRPAPI GetChildByID( void *Object,OBJECTATTRIBUTEINDEX AttributeIndex,VS_UINT16 IDInParent ) = 0;

//----get or set object function
    virtual VS_BOOL SRPAPI GetFunctionID(void *Object,VS_CHAR *FuntionName,VS_UUID *FunctionID) = 0;
    virtual VS_BOOL SRPAPI GetInEventID(void *Object,VS_CHAR *InEventName,VS_UUID *InEventID) = 0;
    virtual VS_BOOL SRPAPI GetOutEventID(void *Object,VS_CHAR *OutEventName,VS_UUID *OutEventID) = 0;
    virtual VS_CHAR *SRPAPI GetOutEventName(VS_UUID *OutEventID) = 0;    

    virtual void *SRPAPI GetFunction(VS_UUID *FunctionID,VS_BOOL *GlobalFunctionFlag = NULL) = 0;
    virtual void *SRPAPI GetFunctionEx(void *Object,VS_UUID *FunctionID,VS_BOOL *GlobalFunctionFlag = NULL) = 0;  //---Can be a parent class definition, the object is overloaded
    virtual void *SRPAPI GetOriFunction(VS_UUID *FunctionID,VS_BOOL *GlobalFunctionFlag = NULL) = 0;  //---Get overloaded function
    virtual void SRPAPI SetFunction(VS_UUID *FunctionID,void *FuncAddress) = 0;  //----Set the object handler address, use requires careful
    virtual void SRPAPI SetInEvent(VS_UUID *InEventID,void *InEventAddress) = 0;  //----Set the event object address, use requires careful
    virtual void *SRPAPI GetSysEvent(void *Object, VS_ULONG *Para) = 0;  //----Get the address of object system events
    virtual void SRPAPI SetSysEvent(void *Object,void *SysEventAddress, VS_ULONG Para) = 0;  //----Set the address of object system events，use requires careful

    virtual void SRPAPI SetSysEventMask(void *Object,VS_ULONG EventMask, VSSystemEvent_EventProc EventProc) = 0;
    virtual VS_ULONG SRPAPI GetSysEventMask(void *Object, VSSystemEvent_EventProc EventProc ) = 0;
    virtual void SRPAPI SetChildEventMask(void *Object,VS_ULONG ClassLayer,VS_ULONG EventMask ) = 0;
    virtual VS_ULONG SRPAPI GetChildEventMask(void *Object,VS_ULONG ClassLayer ) = 0;

//----Overloaded functions and events Object
    virtual VS_BOOL SRPAPI CreateOVLFunction(void *Object,VS_UUID *OriginFunctionID,void *FuncAddress,VS_UUID *NewFunctionID) = 0;
    virtual VS_BOOL SRPAPI CreateOVLInEvent(void *Object,VS_UUID *OutEventID,void *InEventAddress,VS_UUID *NewInEventID) = 0;

//----Objects embedded script, and run
    virtual VS_ULONG SRPAPI EventIDToDWORD(VS_UUID *OutEventID) = 0;
    virtual VS_ULONG SRPAPI EventNameToDWORD(VS_UUID *OutEventID) = 0;
    virtual VS_BOOL SRPAPI CreateNameScript( void *Object, VS_CHAR *ScriptName,VS_CHAR *ScriptBuf ) = 0;  //---
    virtual VS_BOOL SRPAPI CreateNameScriptEx( void *Object, VS_CHAR *ScriptName,VS_CHAR *FileName ) = 0;  //---
    virtual void SRPAPI DeleteNameScript( void *Object, VS_CHAR *ScriptName ) = 0;  //---
    virtual void SRPAPI ChangeScriptName( void *Object, VS_CHAR *ScriptName, VS_CHAR *NewScriptName) = 0;
    virtual VS_CHAR *SRPAPI GetNameScript( void *Object, VS_CHAR *ScriptName) = 0;
    virtual VS_CHAR *SRPAPI QueryFirstNameScript( void *Object,VS_CHAR **ScriptPtr ) = 0;
    virtual VS_CHAR *SRPAPI QueryNextNameScript( VS_CHAR **ScriptPtr ) = 0;
    virtual VS_BOOL SRPAPI ExecNameScript( void *Object, VS_CHAR *ScriptName, VS_INT32 nArgs,VS_INT32 nOutArgs ) = 0;  //---
    virtual VS_BOOL SRPAPI ExecNameScriptEx( void *Object, VS_CHAR *ScriptName, VS_INT32 nArgs,VS_INT32 nOutArgs ) = 0;  //---not process Hook
    virtual VS_BOOL SRPAPI ExecScript(void *Object,VS_CHAR *FuncName,VS_INT8 *ScriptBuf,VS_INT32 ScriptBufSize,VS_INT32 nArgs,VS_INT32 nOutArgs) = 0;  //--run script fragment
    virtual VS_BOOL SRPAPI ExecScriptEx(void *Object,VS_CHAR *FuncName,VS_CHAR *FileName,VS_INT32 nArgs,VS_INT32 nOutArgs) = 0;  //--run script fragment
    virtual void SRPAPI ForceReCompile( void *Object, VS_CHAR *ScriptName ) = 0;
    virtual void SRPAPI RegCompileFunc( VSModuleFunction_ScriptCompileHookProc HookProc,VS_ULONG Para) = 0;

//----object remotecall
    virtual void SRPAPI RemoteCall(VS_ULONG ClientID,void *Object,VS_UUID *FunctionID,...) = 0;
    virtual void SRPAPI RemoteCallVar(VS_ULONG ClientID,void *Object,VS_UUID *FunctionID,va_list argList) = 0;
    virtual void SRPAPI RemoteCallEx(VS_ULONG ExcludeClientID,void *Object,VS_UUID *FunctionID,...) = 0;
    virtual void SRPAPI RemoteCallExVar(VS_ULONG ExcludeClientID,void *Object,VS_UUID *FunctionID,va_list argList) = 0;
    virtual VS_ULONG SRPAPI GetRemoteID(void *Object) = 0;
    virtual VS_ULONG SRPAPI GetRemoteCallID(void *Object) = 0;
    virtual VS_UINT16 SRPAPI GetRemoteSourceTag(void *Object) = 0;
    virtual void *SRPAPI GetRemoteAttach(void *Object) = 0;
    virtual VS_CHAR *SRPAPI GetRemoteCallName(void *Object) = 0;
    virtual VS_BOOL SRPAPI IsRemoteCall(void *Object) = 0;
    virtual VS_ULONG SRPAPI SRemoteCall(VS_ULONG WaitTime,VS_ULONG ClientID,VS_ULONG *RetCode,void *Object,VS_UUID *FunctionID,...) = 0;
    virtual VS_ULONG SRPAPI SRemoteCallVar(VS_ULONG WaitTime,VS_ULONG ClientID,VS_ULONG *RetCode,void *Object,VS_UUID *FunctionID,va_list argList) = 0;
    virtual VS_BOOL SRPAPI ARemoteCall(VS_ULONG WaitTime,VS_ULONG ClientID,VS_RemoteCallResponseProc CallBackProc,VS_ULONG Para,void *Object,VS_UUID *FunctionID,...) = 0;
    virtual VS_BOOL SRPAPI ARemoteCallVar(VS_ULONG WaitTime,VS_ULONG ClientID,VS_RemoteCallResponseProc CallBackProc,VS_ULONG Para,void *Object,VS_UUID *FunctionID,va_list argList) = 0;
    virtual void SRPAPI SetDeferRspFlag(void *Object) = 0;
    virtual void SRPAPI SetRetCode(void *Object,VS_ULONG RemoteRetCode) = 0;
    virtual void SRPAPI SetRemoteRspAttach(void *Object,void *RemoteAttach) = 0;
    virtual void SRPAPI RemoteCallRsp(void *Object,VS_ULONG ClientID,VS_ULONG RemoteCallID,VS_CHAR *RemoteCallName,VS_UINT16 RemoteSourceTag,VS_ULONG RetCode,VS_UINT8 RetType,VS_ULONG RetValue, void *RemoteAttach) = 0;
    virtual VS_BOOL SRPAPI FillSoapRspHeader(class ClassOfSRPSXMLInterface *SXMLInterface) = 0;

//----client tag
    virtual void SRPAPI SetPrivateTag(VS_ULONG ClientPrivateTag) = 0;
    virtual VS_ULONG SRPAPI GetRemotePrivateTag(void *Object) = 0;

//----set and read the local data of Object
    virtual VS_ULONG SRPAPI GetLayer( void *Object ) = 0;
    virtual void SRPAPI SetPrivateValue( void *Object,VS_ULONG ClassLayer,VS_ULONG Index, VS_ULONG Value  ) = 0;
    virtual VS_BOOL SRPAPI GetPrivateValue( void *Object,VS_ULONG ClassLayer,VS_ULONG Index, VS_ULONG *Value, VS_ULONG DefaultValue  ) = 0;
    virtual void *SRPAPI MallocPrivateBuf( void *Object,VS_ULONG ClassLayer,VS_ULONG Index, VS_INT32 BufSize  ) = 0;
    virtual void *SRPAPI GetPrivateBuf( void *Object,VS_ULONG ClassLayer,VS_ULONG Index, VS_INT32 *BufSize  ) = 0;
    virtual void SRPAPI FreePrivate( void *Object,VS_ULONG ClassLayer,VS_ULONG Index ) = 0;

//----set and read the global data of Object
    virtual VS_BOOL SRPAPI SetNameIntValue( void *Object,VS_CHAR *Name, VS_INT32 Value, VS_BOOL LocalChange  ) = 0;
    virtual VS_BOOL SRPAPI GetNameIntValue( void *Object,VS_CHAR *Name, VS_INT32 *Value, VS_INT32 DefaultValue  ) = 0;
    virtual VS_BOOL SRPAPI SetNameFloatValue( void *Object,VS_CHAR *Name, VS_DOUBLE Value, VS_BOOL LocalChange  ) = 0;
    virtual VS_BOOL SRPAPI GetNameFloatValue( void *Object,VS_CHAR *Name, VS_DOUBLE *Value, VS_DOUBLE DefaultValue  ) = 0;
    virtual VS_BOOL SRPAPI SetNameBinValue( void *Object,VS_CHAR *Name, VS_INT8 *Value, VS_UINT16 ValueSize, VS_BOOL LocalChange  ) = 0;
    virtual VS_INT8 *SRPAPI GetNameBinValue( void *Object,VS_CHAR *Name, VS_UINT16 *ValueSize ) = 0;
    virtual VS_BOOL SRPAPI SetNameStrValue( void *Object,VS_CHAR *Name, VS_CHAR *Value, VS_BOOL LocalChange  ) = 0;
    virtual VS_CHAR *SRPAPI GetNameStrValue( void *Object,VS_CHAR *Name, VS_CHAR *DefaultValue  ) = 0;
    virtual VS_BOOL SRPAPI SetNameTimeValue( void *Object,VS_CHAR *Name, VS_TIME *Value, VS_BOOL LocalChange  ) = 0;
    virtual VS_BOOL SRPAPI GetNameTimeValue( void *Object,VS_CHAR *Name, VS_TIME *Value, VS_TIME *DefaultValue  ) = 0;
    virtual void SRPAPI FreeNameValue( void *Object,VS_CHAR *Name ) = 0;  //---删除
    virtual void SRPAPI FreeAllNameValue( void *Object ) = 0;
    virtual VS_UINT8 SRPAPI GetNameValueType( void *Object,VS_CHAR *Name ) = 0;
    virtual VS_CHAR *SRPAPI QueryFirstNameValue(void *Object, VS_ULONG *Context, VS_UINT8 *Type ) = 0;
    virtual VS_CHAR *SRPAPI QueryNextNameValue(void *Object, VS_ULONG *Context, VS_UINT8 *Type ) = 0;

    virtual VS_BOOL SRPAPI RegNameValueChangeCallBack(void *Object,VS_ObjectNameValueChangeNotifyProc ObjectNameValueChangeNotifyProc,VS_ULONG Para) = 0;
    virtual void SRPAPI UnRegNameValueChangeCallBack(void *Object,VS_ObjectNameValueChangeNotifyProc ObjectNameValueChangeNotifyProc,VS_ULONG Para) = 0;

//---Determine whether the synchronization objects and services item
    virtual VS_UINT8 SRPAPI GetSyncStatus( void *Object) = 0;
    virtual VS_UINT8 SRPAPI GetSyncGroupStatus( VS_UUID *SysRootItemID,VS_SYNCGROUP GroupIndex ) = 0;
    virtual VS_UINT8 SRPAPI GetSyncGroupStatusEx( VS_CHAR *SysRootItemName,VS_SYNCGROUP GroupIndex ) = 0;
    virtual void SRPAPI SetSyncGroup(void *Object,VS_SYNCGROUP GroupIndex) = 0;  //----This function is only valid on the server side
    virtual void SRPAPI GetSyncGroup(void *Object,VS_SYNCGROUP *GroupIndex) = 0;  //----This function is only valid on the server side

//---Service-related functions
    virtual void *SRPAPI GetActiveService() = 0;
    virtual void *SRPAPI GetService() = 0;
    virtual VS_INT32 SRPAPI GetActiveServicePath(VS_INT8 *Buf,VS_INT32 BufSize) = 0;
    virtual VS_CHAR *SRPAPI GetActiveServiceName() = 0;
    virtual VS_INT32 SRPAPI GetServicePath(VS_INT8 *Buf,VS_INT32 BufSize) = 0;
    virtual VS_CHAR *SRPAPI GetServiceName() = 0;
    virtual VS_INT32  SRPAPI GetServiceInterval( ) = 0;  //--Return result unit : 10ms
    virtual void SRPAPI GetServiceID(VS_UUID *UuidPtr) = 0;
    virtual void SRPAPI StartVSService(VS_UUID *ServiceID) = 0;
    virtual void SRPAPI ExitVSService() = 0;   //---this function can call the server side
    virtual void SRPAPI SaveService(VS_CHAR *Path) = 0;
    virtual VS_BOOL SRPAPI IsServiceChange() = 0;
    virtual VS_BOOL SRPAPI IsServiceActive() = 0;
    virtual void SRPAPI GetServiceInfo(VS_SERVICEINFO *ServiceInfo) = 0;
    virtual VS_BOOL SRPAPI QueryFirstDepend( VS_QUERYRECORD *QueryRecord,VS_UUID *ServiceID,VS_UUID *RetUuid,VS_CHAR **RetServiceName ) = 0;
    virtual VS_BOOL SRPAPI QueryNextDepend( VS_QUERYRECORD *QueryRecord,VS_UUID *RetUuid,VS_CHAR **RetServiceName ) = 0;
    virtual VS_BOOL SRPAPI IsOsSupport( VS_UINT16 ProgramRunType, VS_UINT16 OsType ) = 0;
    virtual VS_ULONG SRPAPI GetFrameTicket( ) = 0;
    virtual VS_BOOL SRPAPI ExportModule( VS_CHAR *XmlCfgFile, VS_CHAR **ErrorInfo ) = 0;
    virtual VS_ULONG SRPAPI GetServiceGroupID( ) = 0;

//---
    virtual void SRPAPI Print(VS_CHAR *format,...) = 0;
    virtual void SRPAPI PrintVar(VS_CHAR *format,va_list argList) = 0;
    virtual void SRPAPI PrintLua(VS_CHAR *format,...) = 0;
    virtual void SRPAPI PrintLuaVar(VS_CHAR *format,va_list argList) = 0;
    virtual void SRPAPI SetPrintToLua(VS_BOOL PrintFlag) = 0;
    virtual void SRPAPI MessageBox(VS_CHAR *Caption,VS_CHAR *format,...) = 0;
    virtual void SRPAPI MessageBoxVar(VS_CHAR *Caption,VS_CHAR *format,va_list argList) = 0;
    virtual void SRPAPI RegMessageBoxFunction(void *Object,VS_MessageBoxProc MessageBoxProc) = 0;
    virtual void SRPAPI UnRegMessageBoxFunction(void *Object,VS_MessageBoxProc MessageBoxProc) = 0;
    virtual void SRPAPI ProcessError(VS_INT32 AlarmLevel,VS_CHAR *SourceName,VS_INT32 LineIndex,VS_CHAR *format,...) = 0;
    virtual void SRPAPI ProcessErrorVar(VS_INT32 AlarmLevel,VS_CHAR *SourceName,VS_INT32 LineIndex,VS_CHAR *format,va_list argList) = 0;
    virtual void SRPAPI ProcessLuaError(VS_INT32 AlarmLevel,VS_CHAR *SourceName,VS_INT32 LineIndex,VS_CHAR *format,...) = 0;
    virtual void SRPAPI ProcessLuaErrorVar(VS_INT32 AlarmLevel,VS_CHAR *SourceName,VS_INT32 LineIndex,VS_CHAR *format,va_list argList) = 0;
    virtual VS_BOOL SRPAPI IsBeingTrace(void *Object) = 0;
    virtual void SRPAPI Trace(VS_INT8 *SourceName,VS_INT32 BinaryBufSize,VS_INT8 *BinaryBuf,VS_CHAR *format,...) = 0;
    virtual void SRPAPI TraceVar(VS_INT8 *SourceName,VS_INT32 BinaryBufSize,VS_INT8 *BinaryBuf,VS_CHAR *format,va_list argList) = 0;
    virtual void SRPAPI CaptureLuaDisp(VS_LuaInfoDispProc DispProc,VS_ULONG Para) = 0;    //---Capture the output of Lua
    virtual void SRPAPI ReleaseLuaDisp(VS_LuaInfoDispProc DispProc,VS_ULONG Para) = 0;    //---release Capture the output of Lua

//------------------------------------------------------------------------------
//-----Services, data storage table
    virtual void SRPAPI InsertToSDT(void *Object) = 0;
    virtual void SRPAPI DelFromSDT(void *Object) = 0;
    virtual void *SRPAPI QueryFirstFromSDT(VS_QUERYRECORD *QueryRecord) = 0;
    virtual void *SRPAPI QueryNextFromSDT(VS_QUERYRECORD *QueryRecord) = 0;
    virtual void *SRPAPI QueryFirstInstFromSDT(VS_QUERYRECORD *QueryRecord,VS_UUID *ObjectClassID) = 0;
    virtual void *SRPAPI QueryNextInstFromSDT(VS_QUERYRECORD *QueryRecord,VS_UUID *ObjectClassID) = 0;

//----
    virtual void *SRPAPI MallocStaticObject(void *ParentObject,OBJECTATTRIBUTEINDEX AttributeIndex,VS_UUID *ObjectClassID,VS_INT32 AttachBufSize,void *AttachBuf) = 0;
    virtual void *SRPAPI MallocStaticObjectEx(VS_UUID *ObjectID,void *ParentObject,OBJECTATTRIBUTEINDEX AttributeIndex,VS_UUID *ObjectClassID,VS_INT32 AttachBufSize,void *AttachBuf) = 0;
    virtual void *SRPAPI MallocGlobalObject(void *ParentObject,OBJECTATTRIBUTEINDEX AttributeIndex,VS_UUID *ObjectClassID,VS_INT32 AttachBufSize,void *AttachBuf,VS_ULONG ClientID) = 0;
    virtual void *SRPAPI MallocGlobalObjectEx(VS_UUID *ObjectID,void *ParentObject,OBJECTATTRIBUTEINDEX AttributeIndex,VS_UUID *ObjectClassID,VS_INT32 AttachBufSize,void *AttachBuf,VS_ULONG ClientID) = 0;
    virtual void *SRPAPI MallocObject(void *ParentObject,OBJECTATTRIBUTEINDEX AttributeIndex,VS_UUID *ObjectClassID,VS_INT32 AttachBufSize,void *AttachBuf) = 0;
    virtual void *SRPAPI MallocObjectEx(VS_UUID *ObjectID,void *ParentObject,OBJECTATTRIBUTEINDEX AttributeIndex,VS_UUID *ObjectClassID,VS_INT32 AttachBufSize,void *AttachBuf) = 0;
    virtual void *SRPAPI MallocObjectL(VS_UUID *ObjectClassID,VS_INT32 AttachBufSize,void *AttachBuf) = 0;
    virtual void *SRPAPI MallocObjectLEx(VS_UUID *ObjectID,VS_UUID *ObjectClassID,VS_INT32 AttachBufSize,void *AttachBuf) = 0;
    virtual void *SRPAPI MallocClientObject(void *ParentObject,OBJECTATTRIBUTEINDEX AttributeIndex,VS_UUID *ObjectClassID,VS_INT32 AttachBufSize,void *AttachBuf,VS_ULONG ClientID) = 0;
    virtual void *SRPAPI MallocClientObjectEx(VS_UUID *ObjectID,void *ParentObject,OBJECTATTRIBUTEINDEX AttributeIndex,VS_UUID *ObjectClassID,VS_INT32 AttachBufSize,void *AttachBuf,VS_ULONG ClientID) = 0;
    virtual VS_BOOL SRPAPI WaitMalloc(void *Object) = 0;
    virtual VS_ULONG SRPAPI GetOPPermission() = 0;
    virtual VS_BOOL SRPAPI CopyObject(void *Object,void *SrcObject) = 0;

    virtual void SRPAPI FreeObject(void *Object) = 0;
    virtual void SRPAPI DeferFreeObject(void *Object) = 0;
    virtual VS_BOOL SRPAPI IsObjectInFree(void *Object) = 0;
    virtual void SRPAPI ChangeLocal(void *Object,OBJECTATTRIBUTEINDEX AttributeIndex,VS_INT8 *NewValue) = 0;
    virtual void SRPAPI ChangeObject(void *Object,OBJECTATTRIBUTEINDEX AttributeIndex,VS_INT8 *NewValue) = 0;
    virtual void SRPAPI ChangeParent(void *Object,void *ParentObject,OBJECTATTRIBUTEINDEX AttributeIndex) = 0;
    virtual void SRPAPI MarkChange(void *Object,OBJECTATTRIBUTEINDEX AttributeIndex) = 0;
    virtual VS_BOOL SRPAPI RegBeforeChangeCallBack(void *Object,VS_ObjectBeforeChangeNotifyProc ObjectBeforeChangeNotifyProc,VS_ULONG Para,VS_BOOL ChildNotify) = 0;
    virtual VS_BOOL SRPAPI RegChangeCallBack(void *Object,VS_ObjectChangeNotifyProc ObjectChangeNotifyProc,VS_ULONG Para,VS_BOOL ChildNotify) = 0;
    virtual VS_BOOL SRPAPI RegReMallocCallBack(void *Object,VS_ObjectReMallocNotifyProc ObjectReMallocNotifyProc,VS_ULONG Para) = 0;
    virtual void SRPAPI UnRegBeforeChangeCallBack(void *Object,VS_ObjectBeforeChangeNotifyProc ObjectBeforeChangeNotifyProc,VS_ULONG Para) = 0;
    virtual void SRPAPI UnRegChangeCallBack(void *Object,VS_ObjectChangeNotifyProc ObjectChangeNotifyProc,VS_ULONG Para) = 0;
    virtual void SRPAPI UnRegReMallocCallBack(void *Object,VS_ObjectReMallocNotifyProc ObjectReMallocNotifyProc,VS_ULONG Para) = 0;
    virtual VS_BOOL SRPAPI RegObjectIDChangeNotify(VS_ObjectIDChangeNotifyProc ChangeNotifyProc,VS_ULONG Para) = 0;
    virtual void SRPAPI UnRegObjectIDChangeNotify(VS_ObjectIDChangeNotifyProc ChangeNotifyProc,VS_ULONG Para) = 0;
    virtual VS_BOOL SRPAPI RegObjectFreeNotify(VS_ObjectFreeNotifyProc FreeNotifyProc,VS_ULONG Para) = 0;
    virtual void SRPAPI UnRegObjectFreeNotify(VS_ObjectFreeNotifyProc FreeNotifyProc,VS_ULONG Para) = 0;

    virtual void SRPAPI DupVString(VS_VSTRING *InVString,VS_VSTRING *OutVString) = 0;
    virtual VS_ULONG SRPAPI GetVStringBufSize(VS_VSTRING *VString) = 0;
    virtual void SRPAPI ExpandVStringBufSize(VS_VSTRING *VString,VS_ULONG Size) = 0;

    virtual VS_ULONG SRPAPI GetAllocType(void *Object) = 0;

//----user manager
    virtual VS_BOOL SRPAPI CreateUser(VS_CHAR *In_UserName,VS_CHAR *UserPass,VS_UINT8 ReadWriteOrExecute) = 0;
    virtual void SRPAPI DeleteUser(VS_CHAR *In_UserName) = 0;
    virtual VS_CHAR *SRPAPI QueryFirstUser(VS_QUERYRECORD *QueryRecord,VS_UINT8 *ReadWriteOrExecute ) = 0;
    virtual VS_CHAR *SRPAPI QueryNextUser(VS_QUERYRECORD *QueryRecord,VS_UINT8 *ReadWriteOrExecute ) = 0;

//----Corresponding class of object
    virtual void SRPAPI SetAppClass(void *Object,VS_ULONG ClassLayer,class ClassOfSRPObject *SRPObjectClass) = 0;
    virtual class ClassOfSRPObject *SRPAPI GetAppClass(void *Object,VS_ULONG ClassLayer = 0xFFFFFFFF) = 0;

//----Object properties, functions and events
    virtual VS_INT32 SRPAPI GetAttributeNumber(void *Object) = 0;
    virtual VS_INT32 SRPAPI GetAttributeSize(void *Object) = 0;
    virtual VS_CHAR *SRPAPI GetAttributeName(void *Object,OBJECTATTRIBUTEINDEX AttributeIndex) = 0;
    virtual VS_BOOL SRPAPI GetAttributeInfo(void *Object,OBJECTATTRIBUTEINDEX AttributeIndex,VS_ATTRIBUTEINFO *AttributeInfo) = 0;
    virtual VS_BOOL SRPAPI GetAttributeInfoEx(void *Object,VS_CHAR *AttributeName,VS_ATTRIBUTEINFO *AttributeInfo) = 0;
    virtual VS_INT32 SRPAPI GetAttachAttributeNumber(void *Object) = 0;
    virtual VS_INT32 SRPAPI GetAttachAttributeSize(void *Object) = 0;    
    virtual VS_BOOL SRPAPI GetAttachAttributeInfo(void *Object,OBJECTATTRIBUTEINDEX AttributeIndex,VS_ATTRIBUTEINFO *AttributeInfo) = 0;
    virtual VS_BOOL SRPAPI GetComboBoxItem(VS_UINT8 *ComboBoxID, VS_COMBOBOXITEM *ComboBoxItemBuf) = 0;

    virtual VS_INT32 SRPAPI GetFunctionNumber(void *Object) = 0;
    virtual VS_BOOL SRPAPI GetFunctionInfo(void *Object,VS_INT32 FunctionIndex,VS_FUNCTIONINFO *FunctionInfo) = 0;

    virtual VS_INT32 SRPAPI GetOutEventNumber(void *Object) = 0;
    virtual VS_BOOL SRPAPI GetOutEventInfo(void *Object,VS_INT32 OutEventIndex,VS_OUTEVENTINFO *OutEventInfo) = 0;

//----index related
    virtual void *SRPAPI CreateIndex_Nor(VS_INT32 KeyNumber,VS_UINT16 HashTableBits) = 0;  // KeyNumber == 1 ==2 ==3
    virtual void *SRPAPI CreateIndexCmp_Nor(VS_INT32 KeyNumber,VS_UINT16 HashTableBits,VS_IndexCompareProc CompareProc) = 0;  // KeyNumber == 1 ==2 ==3
    virtual void *SRPAPI CreateIDIndex_Nor(VS_UINT16 HashTableBits) = 0;
    virtual void *SRPAPI CreateIDIndexEx_Nor(VS_UINT16 HashTableBits) = 0;     //---UUID + DWORD
    virtual void *SRPAPI CreateIndex_Dbg(VS_INT32 KeyNumber,VS_UINT16 HashTableBits,VS_CHAR *FileName,VS_INT32 LineNumber) = 0;  // KeyNumber == 1 ==2 ==3
    virtual void *SRPAPI CreateIndexCmp_Dbg(VS_INT32 KeyNumber,VS_UINT16 HashTableBits,VS_IndexCompareProc CompareProc,VS_CHAR *FileName,VS_INT32 LineNumber) = 0;  // KeyNumber == 1 ==2 ==3
    virtual void *SRPAPI CreateIDIndex_Dbg(VS_UINT16 HashTableBits,VS_CHAR *FileName,VS_INT32 LineNumber) = 0;
    virtual void *SRPAPI CreateIDIndexEx_Dbg(VS_UINT16 HashTableBits,VS_CHAR *FileName,VS_INT32 LineNumber) = 0;     //---UUID + DWORD
    virtual void SRPAPI InsertOneKey(void *IndexContext,VS_ULONG MainKey,VS_INT8 *Buf) = 0;
    virtual VS_INT8 *SRPAPI FindOneKey(void *IndexContext,VS_ULONG MainKey) = 0;
    virtual VS_INT8 *SRPAPI DelOneKey(void *IndexContext,VS_ULONG MainKey) = 0;
    virtual VS_INT8 *SRPAPI QueryFirstOneKey(void *IndexContext,VS_QUERYRECORD *QueryRecord,VS_ULONG *MainKey) = 0;
    virtual VS_INT8 *SRPAPI QueryNextOneKey(void *IndexContext,VS_QUERYRECORD *QueryRecord,VS_ULONG *MainKey) = 0;
    virtual VS_INT8 *SRPAPI QueryFirstOneKeyA(void *IndexContext,VS_QUERYRECORD *QueryRecord,VS_ULONG *MainKey) = 0;
    virtual VS_INT8 *SRPAPI QueryNextOneKeyA(void *IndexContext,VS_QUERYRECORD *QueryRecord,VS_ULONG *MainKey) = 0;
    virtual void SRPAPI InsertTwoKey(void *IndexContext,VS_ULONG MainKey,VS_ULONG SecondKey,VS_INT8 *Buf) = 0;
    virtual VS_INT8 *SRPAPI FindTwoKey(void *IndexContext,VS_ULONG MainKey,VS_ULONG SecondKey) = 0;
    virtual VS_INT8 *SRPAPI DelTwoKey(void *IndexContext,VS_ULONG MainKey,VS_ULONG SecondKey) = 0;
    virtual VS_INT8 *SRPAPI QueryFirstTwoKey(void *IndexContext,VS_QUERYRECORD *QueryRecord,VS_ULONG *MainKey,VS_ULONG *SecondKey) = 0;
    virtual VS_INT8 *SRPAPI QueryNextTwoKey(void *IndexContext,VS_QUERYRECORD *QueryRecord,VS_ULONG *MainKey,VS_ULONG *SecondKey) = 0;
    virtual VS_INT8 *SRPAPI QueryFirstTwoKeyA(void *IndexContext,VS_QUERYRECORD *QueryRecord,VS_ULONG *MainKey,VS_ULONG *SecondKey) = 0;
    virtual VS_INT8 *SRPAPI QueryNextTwoKeyA(void *IndexContext,VS_QUERYRECORD *QueryRecord,VS_ULONG *MainKey,VS_ULONG *SecondKey) = 0;
    virtual VS_INT8 *SRPAPI QueryFirstTwoKey_F(void *IndexContext,VS_QUERYRECORD *QueryRecord,VS_ULONG MainKey,VS_ULONG *SecondKey) = 0;
    virtual VS_INT8 *SRPAPI QueryNextTwoKey_F(void *IndexContext,VS_QUERYRECORD *QueryRecord,VS_ULONG MainKey,VS_ULONG *SecondKey) = 0;
    virtual VS_INT8 *SRPAPI QueryFirstTwoKeyA_F(void *IndexContext,VS_QUERYRECORD *QueryRecord,VS_ULONG MainKey,VS_ULONG *SecondKey) = 0;
    virtual VS_INT8 *SRPAPI QueryNextTwoKeyA_F(void *IndexContext,VS_QUERYRECORD *QueryRecord,VS_ULONG MainKey,VS_ULONG *SecondKey) = 0;
    virtual void SRPAPI InsertThreeKey(void *IndexContext,VS_ULONG MainKey,VS_ULONG SecondKey,VS_ULONG ThirdKey,VS_INT8 *Buf) = 0;
    virtual VS_INT8 *SRPAPI FindThreeKey(void *IndexContext,VS_ULONG MainKey,VS_ULONG SecondKey,VS_ULONG ThirdKey) = 0;
    virtual VS_INT8 *SRPAPI DelThreeKey(void *IndexContext,VS_ULONG MainKey,VS_ULONG SecondKey,VS_ULONG ThirdKey) = 0;
    virtual VS_INT8 *SRPAPI QueryFirstThreeKey(void *IndexContext,VS_QUERYRECORD *QueryRecord,VS_ULONG *MainKey,VS_ULONG *SecondKey,VS_ULONG *ThirdKey) = 0;
    virtual VS_INT8 *SRPAPI QueryNextThreeKey(void *IndexContext,VS_QUERYRECORD *QueryRecord,VS_ULONG *MainKey,VS_ULONG *SecondKey,VS_ULONG *ThirdKey) = 0;
    virtual VS_INT8 *SRPAPI QueryFirstThreeKeyA(void *IndexContext,VS_QUERYRECORD *QueryRecord,VS_ULONG *MainKey,VS_ULONG *SecondKey,VS_ULONG *ThirdKey) = 0;
    virtual VS_INT8 *SRPAPI QueryNextThreeKeyA(void *IndexContext,VS_QUERYRECORD *QueryRecord,VS_ULONG *MainKey,VS_ULONG *SecondKey,VS_ULONG *ThirdKey) = 0;
    virtual VS_INT8 *SRPAPI QueryFirstThreeKey_F(void *IndexContext,VS_QUERYRECORD *QueryRecord,VS_ULONG MainKey,VS_ULONG *SecondKey,VS_ULONG *ThirdKey) = 0;
    virtual VS_INT8 *SRPAPI QueryNextThreeKey_F(void *IndexContext,VS_QUERYRECORD *QueryRecord,VS_ULONG MainKey,VS_ULONG *SecondKey,VS_ULONG *ThirdKey) = 0;
    virtual VS_INT8 *SRPAPI QueryFirstThreeKeyA_F(void *IndexContext,VS_QUERYRECORD *QueryRecord,VS_ULONG MainKey,VS_ULONG *SecondKey,VS_ULONG *ThirdKey) = 0;
    virtual VS_INT8 *SRPAPI QueryNextThreeKeyA_F(void *IndexContext,VS_QUERYRECORD *QueryRecord,VS_ULONG MainKey,VS_ULONG *SecondKey,VS_ULONG *ThirdKey) = 0;
    virtual VS_INT8 *SRPAPI QueryFirstThreeKey_S(void *IndexContext,VS_QUERYRECORD *QueryRecord,VS_ULONG MainKey,VS_ULONG SecondKey,VS_ULONG *ThirdKey) = 0;
    virtual VS_INT8 *SRPAPI QueryNextThreeKey_S(void *IndexContext,VS_QUERYRECORD *QueryRecord,VS_ULONG MainKey,VS_ULONG SecondKey,VS_ULONG *ThirdKey) = 0;
    virtual VS_INT8 *SRPAPI QueryFirstThreeKeyA_S(void *IndexContext,VS_QUERYRECORD *QueryRecord,VS_ULONG MainKey,VS_ULONG SecondKey,VS_ULONG *ThirdKey) = 0;
    virtual VS_INT8 *SRPAPI QueryNextThreeKeyA_S(void *IndexContext,VS_QUERYRECORD *QueryRecord,VS_ULONG MainKey,VS_ULONG SecondKey,VS_ULONG *ThirdKey) = 0;
    virtual void SRPAPI InsertIDKey(void *IndexContext,VS_UUID *UuidKey,VS_INT8 *Buf ) = 0;
    virtual VS_INT8 *SRPAPI FindIDKey(void *IndexContext,VS_UUID *UuidKey) = 0;
    virtual VS_INT8 *SRPAPI DelIDKey(void *IndexContext,VS_UUID *UuidKey) = 0;
    virtual VS_INT8 *SRPAPI QueryFirstIDKey(void *IndexContext,VS_QUERYRECORD *QueryRecord,VS_UUID *UuidKey) = 0;
    virtual VS_INT8 *SRPAPI QueryNextIDKey(void *IndexContext,VS_QUERYRECORD *QueryRecord,VS_UUID *UuidKey) = 0;
    virtual VS_INT8 *SRPAPI QueryFirstIDKeyA(void *IndexContext,VS_QUERYRECORD *QueryRecord,VS_UUID *UuidKey) = 0;
    virtual VS_INT8 *SRPAPI QueryNextIDKeyA(void *IndexContext,VS_QUERYRECORD *QueryRecord,VS_UUID *UuidKey) = 0;
    virtual void SRPAPI InsertIDKeyEx(void *IndexContext,VS_UUID *UuidKey,VS_ULONG ExKey,VS_INT8 *Buf) = 0;
    virtual VS_INT8 *SRPAPI FindIDKeyEx(void *IndexContext,VS_UUID *UuidKey,VS_ULONG ExKey) = 0;
    virtual VS_INT8 *SRPAPI DelIDKeyEx(void *IndexContext,VS_UUID *UuidKey,VS_ULONG ExKey) = 0;
    virtual VS_INT8 *SRPAPI QueryFirstIDKeyEx(void *IndexContext,VS_QUERYRECORD *QueryRecord,VS_UUID *UuidKey,VS_ULONG *ExKey) = 0;
    virtual VS_INT8 *SRPAPI QueryNextIDKeyEx(void *IndexContext,VS_QUERYRECORD *QueryRecord,VS_UUID *UuidKey,VS_ULONG *ExKey) = 0;
    virtual VS_INT8 *SRPAPI QueryFirstIDKeyEx_F(void *IndexContext,VS_QUERYRECORD *QueryRecord,VS_UUID *UuidKey,VS_ULONG *ExKey) = 0;
    virtual VS_INT8 *SRPAPI QueryNextIDKeyEx_F(void *IndexContext,VS_QUERYRECORD *QueryRecord,VS_UUID *UuidKey,VS_ULONG *ExKey) = 0;
    virtual VS_INT8 *SRPAPI QueryFirstIDKeyExA(void *IndexContext,VS_QUERYRECORD *QueryRecord,VS_UUID *UuidKey,VS_ULONG *ExKey) = 0;
    virtual VS_INT8 *SRPAPI QueryNextIDKeyExA(void *IndexContext,VS_QUERYRECORD *QueryRecord,VS_UUID *UuidKey,VS_ULONG *ExKey) = 0;
    virtual VS_INT32  SRPAPI GetKeyNumber(void *IndexContext) = 0;
    virtual void SRPAPI DelAllKey(void *IndexContext) = 0;
    virtual void SRPAPI DestoryIndex(void *IndexContext) = 0;
    virtual VS_ULONG SRPAPI GetHashValue(void *Key,VS_ULONG Length,VS_ULONG InitValue) = 0;

//----memory management related
    virtual void *SRPAPI CreateMemory_Nor(VS_INT32 ItemSize) = 0;
    virtual void *SRPAPI CreateMemory_Dbg(VS_INT32 ItemSize,VS_CHAR *FileName,VS_INT32 LineNumber) = 0;
    virtual void *SRPAPI GetMemoryPtr_Nor(void *MemoryContext) = 0;
    virtual void *SRPAPI GetMemoryPtr_Dbg(void *MemoryContext,VS_CHAR *FileName,VS_INT32 LineNumber) = 0;
    virtual void *SRPAPI QueryFirstMemoryPtr(void *MemoryContext,VS_QUERYRECORD *QueryRecord) = 0;
    virtual void *SRPAPI QueryNextMemoryPtr(void *MemoryContext,VS_QUERYRECORD *QueryRecord) = 0;
    virtual void SRPAPI FreeMemoryPtr(void *MemoryContext,void *Ptr) = 0;
    virtual void SRPAPI ClearMemory(void *MemoryContext) = 0;
    virtual void SRPAPI DestoryMemory(void *MemoryContext) = 0;
    virtual void *SRPAPI Malloc_Nor(VS_INT32 MemorySize) = 0;
    virtual void *SRPAPI Malloc_Dbg(VS_INT32 MemorySize,VS_CHAR *FileName,VS_INT32 LineNumber) = 0;
    virtual void SRPAPI Free(void *MemoryPtr) = 0;
    virtual void SRPAPI GetMemoryUsed( VS_ULONG *KernelAllocSize,VS_ULONG *DataAllocSize,VS_ULONG *AppAllocSize, VS_ULONG *ScriptMemoryUsed ) = 0;

//-----event management related functions
    virtual void SRPAPI InjectSysEventFunction(VS_ULONG SysEventID,VSSystemEvent_EventProc ProcessFunctionProc) = 0;
    virtual void SRPAPI RejectSysEventFunction(VS_ULONG SysEventID,VSSystemEvent_EventProc ProcessFunctionProc) = 0;
    virtual VS_EVENTPARAM_RUNPARAM *SRPAPI GetResponseBuf() = 0;
    virtual VS_EVENTPARAM_RUNPARAM *SRPAPI GetRequestBuf() = 0;
    virtual VS_ULONG SRPAPI GetSysEventID(VS_EVENTPARAM *EventParam) = 0;  //---Returns 0 is not a system event
    virtual void SRPAPI FreeResponseBuf(VS_EVENTPARAM_RUNPARAM *ResponseParam) = 0;
    virtual void SRPAPI FreeRequestBuf(VS_EVENTPARAM_RUNPARAM *RequestParam) = 0;
    virtual void SRPAPI AttachResponseBuf(VS_EVENTPARAM *EventParam,VS_EVENTPARAM_RUNPARAM *ResponseParam) = 0;
    virtual VS_EVENTPARAM_RUNPARAM *SRPAPI ProcessEvent_Nor(VS_UUID *EventID,void *SrcObject,void *DesObject,VS_EVENTPARAM_RUNPARAM *RequestParam) = 0;
    virtual VS_EVENTPARAM_RUNPARAM *SRPAPI ProcessEvent_Dbg(VS_UUID *EventID,void *SrcObject,void *DesObject,VS_EVENTPARAM_RUNPARAM *RequestParam) = 0;
    virtual VS_EVENTPARAM_RUNPARAM *SRPAPI PostProcessEvent_Nor(VS_UUID *EventID,void *SrcObject,void *DesObject,VS_EVENTPARAM_RUNPARAM *RequestParam) = 0;
    virtual VS_EVENTPARAM_RUNPARAM *SRPAPI PostProcessEvent_Dbg(VS_UUID *EventID,void *SrcObject,void *DesObject,VS_EVENTPARAM_RUNPARAM *RequestParam) = 0;
    virtual VS_EVENTPARAM_RUNPARAM *SRPAPI ProcessParentEvent(VS_EVENTPARAM *EventParam) = 0;
    virtual void SRPAPI SetEventAttachBuf(VS_EVENTPARAM_RUNPARAM *RunParam) = 0;

//---Dynamic event registration
    virtual VS_BOOL SRPAPI RegEventFunction(void *SrcObject,VS_UUID *EventID, void *Object, void *FuncAddr,VS_ULONG Para) = 0;
    virtual void SRPAPI UnRegEventFunction(void *SrcObject,VS_UUID *EventID, void *Object, void *FuncAddr,VS_ULONG Para ) = 0;
    virtual VS_BOOL SRPAPI RegSysEventFunction(void *Object,VS_ULONG SysEventID, void *FuncAddr,VS_ULONG Para ) = 0;
    virtual void SRPAPI UnRegSysEventFunction(void *Object,VS_ULONG SysEventID, void *FuncAddr,VS_ULONG Para ) = 0;

//-----about object activation
    virtual VS_BOOL SRPAPI ActiveCmd(void *Object,VS_UINT8 ActiveCmd) = 0;
    virtual VS_UINT8 SRPAPI GetActiveCmd(void *Object) = 0; 
    virtual VS_BOOL SRPAPI ActiveClient(VS_ULONG ClientID,void *Object) = 0; //--Server-side calls, client object activation
    virtual void SRPAPI DeactiveClient(VS_ULONG ClientID,void *Object) = 0; //--Server-side calls, client object deactivation
    virtual VS_BOOL SRPAPI Active(void *Object) = 0;
    virtual void SRPAPI Deactive(void *Object) = 0; 
    virtual void SRPAPI DeactiveAll() = 0;
    virtual VS_BOOL SRPAPI IsActive(void *Object) = 0;
    virtual void *SRPAPI QueryFirstActiveInst(VS_QUERYRECORD *QueryRecord,VS_UUID *ObjectClassID) = 0;
    virtual void *SRPAPI QueryNextActiveInst(VS_QUERYRECORD *QueryRecord,VS_UUID *ObjectClassID) = 0;

//-----client representative object
    virtual VS_BOOL SRPAPI SetClientObject(VS_ULONG ClientID,void *Object) = 0; //--Server-side call, set the client's representative objects
    virtual void *SRPAPI GetClientObject() = 0; //--Client calls, get representative objects

//-----serviceiem management, and activeset
    virtual VS_BOOL SRPAPI CreateSysRootItem( VS_CHAR *SystemRootItemName, VS_CHAR *DependSysRootItem, VS_UUID *SystemRootItemID, VS_UUID *SystemRootItemIDEx ) = 0;
    virtual void SRPAPI ActiveAllSysRootItem( ) = 0;
    virtual void SRPAPI ActiveSysRootItem( VS_CHAR *SystemRootItemName ) = 0;
    virtual void SRPAPI DeactiveSysRootItem(VS_CHAR *SystemRootItemName) = 0;
    virtual void SRPAPI ActiveCSysRootItem( VS_ULONG ClientID,VS_CHAR *SystemRootItemName ) = 0;
    virtual void SRPAPI DeactiveCSysRootItem( VS_ULONG ClientID,VS_CHAR *SystemRootItemName) = 0;
    virtual void *SRPAPI GetSysRootItem(VS_CHAR *SystemRootItemName) = 0;
    virtual void *SRPAPI GetSysRootItemEx(VS_UUID *SystemRootItemID) = 0;
    virtual VS_CHAR *SRPAPI GetSysRootItemName(void  *SystemRootItem) = 0;
    virtual void SRPAPI GetSysRootItemActiveSet(void  *SystemRootItem,VS_ACTIVESETITEM *ActiveSetPtr) = 0;
    virtual void SRPAPI SetSysRootItemActiveSet(void  *SystemRootItem,VS_ACTIVESETITEM *ActiveSetItem) = 0;
    virtual void SRPAPI SetCSysRootItemActiveSet(VS_ULONG ClientID,void  *SystemRootItem,VS_ACTIVESETITEM *ActiveSetItem) = 0;
    virtual VS_CHAR *SRPAPI QueryFirstSysRootItem( ) = 0;
    virtual VS_CHAR *SRPAPI QueryNextSysRootItem( ) = 0;
    virtual void *SRPAPI QueryFirstGroupObject(void *SystemRootItem,VS_SYNCGROUP GroupIndex,VS_ULONG *QueryGroupContext) = 0;
    virtual void *SRPAPI QueryNextGroupObject(VS_ULONG *QueryGroupContext) = 0;
    virtual void *SRPAPI QueryFirstSysRootItemChild( void  *SystemRootItem ) = 0;

    virtual VS_BOOL SRPAPI RegClientSysRootItemToSyncFunc(void  *SystemRootItem,VS_ClientSysRootItemChangeToSyncProc FuncPtr, VS_ULONG Para) = 0;
    virtual void SRPAPI UnRegClientSysRootItemToSyncFunc(void  *SystemRootItem,VS_ClientSysRootItemChangeToSyncProc FuncPtr, VS_ULONG Para) = 0;

//-----edit functions,used by srpdebug
    virtual void SRPAPI InitEdit(void *ClassObject,void *Object) = 0;
    virtual void SRPAPI TermEdit(void *ClassObject,void *Object) = 0;
    virtual VS_BOOL SRPAPI GetEditMode(void *Object) = 0;
    virtual void SRPAPI SetEditMode(void *Object,VS_BOOL EditFlag) = 0;
//------------------------------------------------------------------------
    virtual void SRPAPI EditCommit() = 0;  //--sync change to server
    virtual void SRPAPI EditSelect(void *Object) = 0;  //--select object
    virtual void SRPAPI EditChange(void *Object,OBJECTATTRIBUTEINDEX AttributeIndex,VS_INT8 *NewValue) = 0;
    virtual void SRPAPI EditMarkChange(void *Object,OBJECTATTRIBUTEINDEX AttributeIndex) = 0;
    virtual void SRPAPI EditSetSaveFlag(void *Object,VS_UINT8 SaveFlag) = 0;
    virtual void SRPAPI EditSetName(void *Object,VS_CHAR *Name) = 0;
    virtual void SRPAPI FillAttachBuf(VS_UUID *ObjectClassID,VS_INT8 *AttachBuf,VS_INT32 AttachBufSize) = 0;

    virtual VS_BOOL SRPAPI IsEditProcExist(void *Object) = 0;
    virtual void SRPAPI TriggerEditProc(void *ClassObject,void *Object,VS_ULONG AppCode,VS_ULONG AppCode1) = 0;

//-----edit create or delete object
    virtual void SRPAPI EditDelete(void *Object) = 0;
    virtual void *SRPAPI EditCreate(VS_UUID *ObjectClassID,VS_UUID *ParentObjectID,OBJECTATTRIBUTEINDEX AttributeIndex,VS_ULONG OrderIndex,VS_UINT8 ObjectSaveFlag,VS_INT32 AttachBufSize,void *AttachBuf) = 0;
    virtual void *SRPAPI EditCreateEx(VS_UUID *ObjectClassID,VS_UUID *ObjectID,VS_UUID *ParentObjectID,OBJECTATTRIBUTEINDEX AttributeIndex,VS_ULONG OrderIndex,VS_UINT8 ObjectSaveFlag,VS_INT32 AttachBufSize,void *AttachBuf) = 0;
    virtual void SRPAPI EditChangeParent(void *Object,void *ParentObject,OBJECTATTRIBUTEINDEX AttributeIndex) = 0;
    virtual void SRPAPI EditChangeSyncGroup(void *Object,VS_SYNCGROUP GroupIndex) = 0;
    virtual void SRPAPI EditGetClassID(VS_UUID *ObjectID,OBJECTATTRIBUTEINDEX AttributeIndex,VS_UUID *UuidPtr) = 0;
    virtual void SRPAPI EditGetInstID(VS_UUID *ObjectClassID,VS_UUID *UuidPtr) = 0;
    virtual void SRPAPI EditSetWndStatus(VS_BOOL Normal) = 0;

    virtual VS_BOOL SRPAPI EditUpdateObjectScript( void *Object, VS_CHAR *ScriptName,VS_CHAR *ScriptBuf ) = 0;  //---
    virtual VS_BOOL SRPAPI EditUpdateObjectScriptEx( void *Object, VS_CHAR *ScriptName,VS_CHAR *FileName ) = 0;  //---

//------------------------------------------------------------------------------
//-----client operation call back
    virtual VS_BOOL SRPAPI RegClientOpFunction(VS_ClientOperationCallBackProc ClientOperationCallBackProc,VS_ULONG Para) = 0;
    virtual void SRPAPI UnRegClientOpFunction(VS_ClientOperationCallBackProc ClientOperationCallBackProc,VS_ULONG Para) = 0;

//------------------------------------------------------------------------------
//-----service redirect function, client connect to other servers
    virtual void SRPAPI Redirect( VS_ULONG ClientID,VS_CHAR *DesServerInterface,VS_CHAR *DesServerName,VS_UINT16 DesServerPortNumber, class ClassOfSRPParaPackageInterface *ParaPkg,VS_RedirectCallBackProc RedirectCallBackProc,VS_ULONG Para ) = 0;

//------------------------------------------------------------------------------
//-----state machine functions
    virtual void *SRPAPI GetServiceMachine() = 0;

    virtual void SRPAPI DelMachine(void *Machine) = 0;
    virtual VS_INT8 *SRPAPI GetMachineAttachBuf(void *Machine) = 0;
    virtual void SRPAPI SetMachineAttachBuf(void *Machine,VS_INT8 *AppBuf) = 0;
    virtual VS_ULONG SRPAPI GetMachineID(void *Machine) = 0;
    virtual void *SRPAPI FindMachine(VS_ULONG MachineID) = 0;

//---client manager, valid at server
    virtual VS_BOOL SRPAPI RegClientMachineProcess(void *CallBackPtr,void *Object,VS_ULONG Para) = 0;
    virtual void SRPAPI UnRegClientMachineProcess(void *CallBackPtr,void *Object,VS_ULONG Para) = 0;
    virtual void SRPAPI ClientInitError(void *Machine) = 0;
    virtual void SRPAPI ClientInitOk(void *Machine,VS_BOOL ReSyncFlag,VS_CHAR *TermOldScript,VS_CHAR *InitNewScript,VS_ULONG ClientPrivateTag,VS_ULONG ClientOPPermission,VS_ULONG ClientUploadMaxSize) = 0;
    virtual void SRPAPI DeleteClient(void *Machine) = 0;
    virtual void SRPAPI GetClientInfo(void *Machine,VS_CLIENTINFO *ClientInfo) = 0;
    virtual VS_BOOL SRPAPI QueryFirstClientInfo(VS_CLIENTINFO *ClientInfo) = 0;  //---only valid at server
    virtual VS_BOOL SRPAPI QueryNextClientInfo(VS_CLIENTINFO *ClientInfo) = 0;  //---only valid at server
    virtual VS_INT32  SRPAPI GetClientNumber() = 0;  //---only valid at server

//---client Qos Manager
    virtual void SRPAPI GetClientQos(void *Machine,VS_CLIENTQOS *QosBuf) = 0;  //--get client Qos
    virtual void SRPAPI SetClientQos(void *Machine,VS_CLIENTQOS *QosBuf) = 0;  //--set client Qos
    virtual void SRPAPI GetServiceQos(VS_CLIENTQOS *QosBuf) = 0; //--get service qos

//---file upload ot download
    virtual VS_BOOL SRPAPI DownLoad( void *AttachObject,VS_CHAR *ServerPath,VS_CHAR *ClientPath,VS_CHAR *FileName, VS_FileUpDownLoadCallBackProc CallBackProc, void *Object, VS_ULONG Para ) = 0;      //---client or debug side
    virtual VS_BOOL SRPAPI UpLoad( void *AttachObject,VS_CHAR *ServerPath,VS_CHAR *ClientPath,VS_CHAR *FileName, VS_FileUpDownLoadCallBackProc CallBackProc, void *Object, VS_ULONG Para ) = 0;        //---client or debug side
    virtual void SRPAPI GetFileInfo( VS_UPDOWNFILEINFO *InfoPtr ) = 0;   //---client or debug side
    virtual VS_BOOL SRPAPI RegFileCallBack( VS_FileUpDownLoadCallBackProc CallBackProc, void *Object, VS_ULONG Para ) = 0;
    virtual void SRPAPI UnRegFileCallBack( VS_FileUpDownLoadCallBackProc CallBackProc, void *Object, VS_ULONG Para ) = 0;

    virtual VS_INT32  SRPAPI GetFileStatus( VS_CHAR *FileName, VS_UPDOWNFILEMSG *FileInfo ) = 0;
    virtual VS_INT32  SRPAPI GetDataStatus( void *Object,VS_ULONG UniqueDataUnitID, VS_UPDOWNFILEMSG *FileInfo ) = 0;
    virtual VS_BOOL SRPAPI QueryFirstDown( VS_QUERYRECORD *QueryRecord, VS_UPDOWNFILEMSG *FileInfo ) = 0;
    virtual VS_BOOL SRPAPI QueryNextDown( VS_QUERYRECORD *QueryRecord, VS_UPDOWNFILEMSG *FileInfo ) = 0;
    virtual VS_BOOL SRPAPI QueryFirstUp( VS_QUERYRECORD *QueryRecord, VS_UPDOWNFILEMSG *FileInfo ) = 0;
    virtual VS_BOOL SRPAPI QueryNextUp( VS_QUERYRECORD *QueryRecord, VS_UPDOWNFILEMSG *FileInfo ) = 0;

//---object static data functions
    virtual VS_INT8 *SRPAPI GetStaticData( void *Object,VS_ULONG UniqueDataUnitID,VS_STATICID *DataVersion,VS_ULONG *DataSize, VS_BOOL AutoDownLoad) = 0; //DataVersion and DataSize may be NULL，otherwise, DataVersion represent version required, and return is current version. input DataSize is ignore,and return data size
    virtual VS_INT8 *SRPAPI GetStaticDataEx( void *Object,VS_ULONG UniqueDataUnitID,VS_STATICID *DataVersion,VS_ULONG *DataSize, VS_BOOL AutoDownLoad,VS_CHAR *Token) = 0;  //DataVersion and DataSize may be NULL，otherwise, DataVersion represent version required, and return is current version. input DataSize is ignore,and return data size
    virtual VS_BOOL SRPAPI CanSetStaticData(void *Object,VS_ULONG DataSize) = 0;
    virtual VS_BOOL SRPAPI SetStaticData( void *Object,VS_ULONG UniqueDataUnitID,VS_ULONG DataSize,VS_INT8 *DataBuf,VS_STATICID *RetDataVersion) = 0;
    virtual VS_BOOL SRPAPI SetStaticDataEx( void *Object,VS_ULONG UniqueDataUnitID,VS_ULONG *DataSize,VS_ULONG Offset,VS_CHAR *FileName,VS_STATICID *RetDataVersion) = 0;
    virtual VS_ULONG SRPAPI GetStaticAppCode( void *Object,OBJECTATTRIBUTEINDEX StaticPersistentAttributeIndex) = 0;  //---If an error occurs, returns 0
    virtual VS_BOOL SRPAPI WaitGetStaticData( void *Object,VS_ULONG UniqueDataUnitID,VS_FileUpDownLoadCallBackProc CallBackProc,VS_ULONG Para,VS_BOOL WaitFlag) = 0;
    virtual VS_BOOL SRPAPI WaitSetStaticData( void *Object,VS_ULONG UniqueDataUnitID,VS_FileUpDownLoadCallBackProc CallBackProc,VS_ULONG Para,VS_BOOL WaitFlag) = 0;
    virtual void SRPAPI PackStaticData() = 0;
    virtual void SRPAPI ForceToSaveStatic() = 0;
    virtual void SRPAPI ClearStatic(VS_UINT16 BeforeDays) = 0;
    virtual void SRPAPI GetStaticVersion( VS_ULONG DataSize,VS_INT8 *DataBuf,VS_STATICID *RetDataVersion) = 0;

//---Check the type of the current program: server, debug the server, the client
    virtual VS_UINT16 SRPAPI GetProgramType( ) = 0;
    virtual VS_BOOL SRPAPI IsDefaultServer() = 0;  //---If is the server, then is default or not
    virtual VS_BOOL SRPAPI IsWindowVisible() = 0;  //---Management window is visible
    virtual void SRPAPI HideWindow() = 0;       //---hide Management window
    virtual void SRPAPI ShowWindow() = 0;       //---show Management window
    virtual void SRPAPI SetWindowCaption(VS_CHAR *Caption) = 0;   //---Set the display window caption
    virtual void SRPAPI ExitVSSystem(VS_CHAR *ErrorInfo) = 0;            //---Exit the program
    virtual VS_BOOL SRPAPI IsAppActive() = 0;       //---Program is active
    virtual void SRPAPI SetIdleActive(VS_BOOL CreateFlag) = 0;    //---Is produced in a non-active state IDLE event
    virtual void SRPAPI GetVersion(VS_UINT8 *MainVersion,VS_UINT8 *SubVersion,VS_UINT16 *BuildVersion) = 0;
    virtual void SRPAPI GetVersionInfo(VS_CHAR *InfoBuf,VS_INT32 InfoBufSize) = 0;
    virtual VS_HWND SRPAPI GetWindowHandle(  ) = 0;
    virtual void SRPAPI GetWindowSize( VS_INT32 *Width, VS_INT32 *Height ) = 0;
    virtual void SRPAPI SetColor( VS_COLOR Text,VS_COLOR Explane,VS_COLOR ObjName,VS_COLOR AttrType,VS_COLOR Number,VS_COLOR Error ) = 0;
    virtual void SRPAPI SetBkColor( VS_COLOR BkColor ) = 0;
    virtual void SRPAPI ShowStatusMenu( VS_BOOL MenuShowFlag, VS_BOOL StatusShowFlag ) = 0;

//-----Client window function (to provide a simple output)
    virtual VS_HWND SRPAPI GetClientWndHandle(  ) = 0;
    virtual void SRPAPI GetClientWndSize( VS_INT32 *Width, VS_INT32 *Height ) = 0;
    virtual void SRPAPI SetClientWndSize( VS_INT32 Width, VS_INT32 Height ) = 0;  //---Set client window
    virtual void SRPAPI SetClientWndFocus( VS_HWND hWnd, VS_BOOL NeedAction ) = 0;
    virtual void SRPAPI ClearClientWnd( ) = 0;
    virtual void SRPAPI HideClientWnd( ) = 0;
    virtual void SRPAPI ShowClientWnd( ) = 0;
    virtual void SRPAPI SetClientBkColor( VS_COLOR BkColor ) = 0;

//-----Operating environment information (for complex output)
    //---object process runenv request/response
    virtual void SRPAPI SetRunEnv_FromChildCallBack( void *Object,VS_RunEnvCallBackProc CallBack,VS_ULONG Para) = 0;
    virtual void SRPAPI SetRunEnv_FromParentCallBack( void *Object,VS_RunEnvCallBackProc CallBack,VS_ULONG Para) = 0;
    //---object capture runenv event
    virtual void SRPAPI RegRunEnv_FromParentCallBack( void *Object, void *ParentObject, VS_RunEnvCallBackProc CallBack,VS_ULONG Para) = 0;
    virtual void SRPAPI UnRegRunEnv_FromParentCallBack( void *Object, void *ParentObject, VS_RunEnvCallBackProc CallBack,VS_ULONG Para) = 0;
    //---real function
    virtual VS_BOOL SRPAPI RunEnvToChild(void *Object, void *DesObject, struct StructOfVSRunEnv *RunEnvInfo) = 0; //if DesObject == NULL, then send to register object
    virtual VS_BOOL SRPAPI RunEnvToParent(void *Object, struct StructOfVSRunEnv *RunEnvInfo) = 0;   //first send to parent, if its not handle, then send env

//-----Message processing function hook
    virtual void SRPAPI SetMessageHook(VS_SRPMessageProcessHookProc HookProc) = 0;
    virtual VS_SRPMessageProcessHookProc SRPAPI GetMessageHook( ) = 0;

//-----Dynamic registration, the Lua function of the object
    virtual VS_BOOL SRPAPI RegLuaFunc( void *Object, VS_CHAR *FuncName, void *FuncAddress, VS_ULONG Para) = 0;
    virtual void SRPAPI UnRegLuaFunc( void *Object, VS_CHAR *FuncName, void *FuncAddress, VS_ULONG Para ) = 0;
    virtual void SRPAPI UnRegLuaFuncEx( void *Object, void *FuncAddress, VS_ULONG Para ) = 0;
    virtual void SRPAPI ValidRegLuaFunc( void *Object, VS_CHAR *FuncName, void *FuncAddress, VS_ULONG Para ) = 0;
    virtual void SRPAPI InValidRegLuaFunc( void *Object, VS_CHAR *FuncName, void *FuncAddress, VS_ULONG Para ) = 0;

//-----set or get property
    virtual VS_BOOL SRPAPI RegLuaGetValueFunc( void *Object, VS_LuaGetValueProc GetValueProc, VS_ULONG Para ) = 0;
    virtual VS_BOOL SRPAPI RegLuaSetValueFunc( void *Object, VS_LuaSetValueProc SetValueProc, VS_ULONG Para ) = 0;
    virtual VS_BOOL SRPAPI UnRegLuaGetValueFunc( void *Object, VS_LuaGetValueProc GetValueProc, VS_ULONG Para ) = 0;
    virtual VS_BOOL SRPAPI UnRegLuaSetValueFunc( void *Object, VS_LuaSetValueProc SetValueProc, VS_ULONG Para ) = 0;
    virtual void SRPAPI ValidLuaGetValueFunc( void *Object, VS_LuaGetValueProc GetValueProc, VS_ULONG Para ) = 0;
    virtual void SRPAPI ValidLuaSetValueFunc( void *Object, VS_LuaSetValueProc SetValueProc, VS_ULONG Para ) = 0;
    virtual void SRPAPI InValidLuaGetValueFunc( void *Object, VS_LuaGetValueProc GetValueProc, VS_ULONG Para ) = 0;
    virtual void SRPAPI InValidLuaSetValueFunc( void *Object, VS_LuaSetValueProc SetValueProc, VS_ULONG Para ) = 0;

//-----lua script function, related functions
    virtual void *SRPAPI GetLua( ) = 0;
    virtual VS_BOOL SRPAPI DoBuffer( VS_CHAR *ScriptInterface,VS_INT8 *ScriptBuf,VS_INT32 ScriptBufSize, VS_CHAR *Name, VS_CHAR **ErrorInfo, VS_CHAR *WorkDirectory, VS_BOOL IsUTF8 ) = 0; //ScriptInterfaces length < 16
    virtual VS_BOOL SRPAPI DoFile( VS_CHAR *ScriptInterface,VS_CHAR *FileName, VS_CHAR **ErrorInfo, VS_CHAR *WorkDirectory, VS_BOOL IsUTF8 ) = 0;                                          //ScriptInterfaces length < 16

    //--user data
    virtual void SRPAPI LuaNewUserData( VS_INT32 Size ) = 0;
    virtual void SRPAPI LuaSetUserDataGC( VS_LuaUserDataGCProc GCProc ) = 0;
    virtual void *SRPAPI LuaToUserData( VS_INT32 Index ) = 0;
    //-----
    virtual void SRPAPI LuaNewTable( ) = 0;
    virtual VS_INT32  SRPAPI LuaGetTop( ) = 0;
    virtual VS_INT32  SRPAPI LuaNext( VS_INT32 Index ) = 0;
    virtual void SRPAPI LuaPop( VS_INT32 Index ) = 0;
    virtual void SRPAPI LuaPushBool( VS_BOOL Value ) = 0;
    virtual void SRPAPI LuaPushString( VS_CHAR *Value ) = 0;
    virtual void SRPAPI LuaPushNumber( VS_DOUBLE Value ) = 0;
    virtual void SRPAPI LuaPushInt( VS_INT32 Value ) = 0;
    virtual void SRPAPI LuaPushNil( ) = 0;
    virtual VS_BOOL SRPAPI LuaPushObject( void *Object ) = 0;
    virtual VS_BOOL SRPAPI LuaPushParaPkg( class ClassOfSRPParaPackageInterface *ParaPkg, VS_BOOL AutoRelease ) = 0;
    virtual VS_BOOL SRPAPI LuaPushQueryRecord( VS_QUERYRECORD *QueryRecord, VS_BOOL AutoRelease ) = 0;
    virtual VS_BOOL SRPAPI LuaPushRect( VS_RECT *rEct ) = 0;
    virtual VS_BOOL SRPAPI LuaPushFont( VS_FONT *hFont ) = 0;
    virtual VS_BOOL SRPAPI LuaPushTime( VS_TIME *hTime ) = 0;
    virtual void SRPAPI LuaPushFunction( void *FunctionAddr ) = 0;
    virtual void SRPAPI LuaPushClosure( void *FunctionAddr, VS_INT32 n ) = 0;
    virtual VS_INT32 SRPAPI LuaUpValueIndex( VS_INT32 Index ) = 0;
    virtual void SRPAPI LuaPushValue( VS_INT32 Index ) = 0;
    virtual void SRPAPI LuaInsert( VS_INT32 Index ) = 0;
    virtual void SRPAPI LuaRemove( VS_INT32 Index ) = 0;
    virtual VS_BOOL SRPAPI LuaPCall( VS_INT32 nargs, VS_INT32 nresults ) = 0;

//---lua remotecall
    virtual void SRPAPI LuaRCall(VS_ULONG ClientID,void *Object, VS_CHAR *ScriptName, VS_INT32 nArgs) = 0;
    virtual void SRPAPI LuaRCallEx(VS_ULONG ExcludeClientID,void *Object, VS_CHAR *ScriptName, VS_INT32 nArgs) = 0;
    virtual VS_BOOL SRPAPI LuaSRCall(VS_ULONG WaitTime,VS_ULONG ClientID,void *Object, VS_CHAR *ScriptName, VS_INT32 nArgs, VS_INT32 *OutArgs) = 0;
    virtual VS_BOOL SRPAPI LuaARCall(VS_ULONG WaitTime,VS_ULONG ClientID,void *Object, void *CallBackProc, VS_ULONG Para, VS_CHAR *ScriptName, VS_INT32 nArgs) = 0;
//---lua dynamic event register
    virtual VS_INT32 SRPAPI LuaRegEvent(void *SrcObject,VS_UUID *EventID, void *Object, void *FuncAddr) = 0;
    virtual void SRPAPI LuaUnRegEvent(void *SrcObject,VS_UUID *EventID, void *Object, VS_INT32 FuncRefValue ) = 0;
    virtual VS_BOOL SRPAPI LuaProcessEvent(void *Object,VS_UUID *EventID, VS_INT32 nArgs, VS_INT32 *OutArgs ) = 0;
    virtual VS_BOOL SRPAPI LuaPostProcessEvent(void *Object,VS_UUID *EventID, VS_INT32 nArgs, VS_INT32 *OutArgs ) = 0;

//---lua execute script of object
    virtual VS_BOOL SRPAPI LuaCall( void *Object, VS_CHAR *ScriptName, VS_INT32 nArgs,VS_INT32 nOutArgs ) = 0;
    virtual void SRPAPI LuaRegHook( void *FuncAddr ) = 0;
    virtual void SRPAPI LuaUnRegHook( void *FuncAddr ) = 0;

    virtual VS_INT32 SRPAPI LuaType( VS_INT32 Index ) = 0;
    virtual VS_BOOL SRPAPI LuaToBool( VS_INT32 Index ) = 0;
    virtual VS_CHAR *SRPAPI LuaToString( VS_INT32 Index ) = 0;
    virtual VS_DOUBLE SRPAPI LuaToNumber( VS_INT32 Index ) = 0;
    virtual VS_INT32 SRPAPI LuaToInt( VS_INT32 Index ) = 0;
    virtual void *SRPAPI LuaToObject( VS_INT32 Index ) = 0;
    virtual class ClassOfSRPParaPackageInterface *SRPAPI LuaToParaPkg( VS_INT32 Index ) = 0;
    virtual VS_QUERYRECORD *SRPAPI LuaToQueryRecord( VS_INT32 Index ) = 0;
    virtual VS_BOOL SRPAPI LuaToRect( VS_INT32 Index, VS_RECT *rEct ) = 0;
    virtual VS_BOOL SRPAPI LuaToFont( VS_INT32 Index, VS_FONT *hFont ) = 0;
    virtual VS_BOOL SRPAPI LuaToTime( VS_INT32 Index, VS_TIME *hTime ) = 0;

    virtual VS_BOOL SRPAPI LuaIsBool( VS_INT32 Index ) = 0;
    virtual VS_BOOL SRPAPI LuaIsString( VS_INT32 Index ) = 0;
    virtual VS_BOOL SRPAPI LuaIsNumber( VS_INT32 Index ) = 0;
    virtual VS_BOOL SRPAPI LuaIsTable( VS_INT32 Index ) = 0;
    virtual VS_BOOL SRPAPI LuaIsNil( VS_INT32 Index ) = 0;
    virtual VS_BOOL SRPAPI LuaIsObject( VS_INT32 Index ) = 0;
    virtual VS_BOOL SRPAPI LuaIsParaPkg( VS_INT32 Index ) = 0;
    virtual VS_BOOL SRPAPI LuaIsQueryRecord( VS_INT32 Index ) = 0;
    virtual VS_BOOL SRPAPI LuaIsCFunction( VS_INT32 Index ) = 0;
    virtual VS_BOOL SRPAPI LuaIsFunction( VS_INT32 Index ) = 0;
    virtual VS_BOOL SRPAPI LuaIsFont( VS_INT32 Index ) = 0;
    virtual VS_BOOL SRPAPI LuaIsRect( VS_INT32 Index ) = 0;
    virtual VS_BOOL SRPAPI LuaIsTime( VS_INT32 Index ) = 0;

    virtual VS_BOOL SRPAPI LuaSetTable( VS_INT32 Index ) = 0;
    virtual VS_BOOL SRPAPI LuaGetTable( VS_INT32 Index ) = 0;
    virtual void SRPAPI LuaSetGlobal( VS_CHAR *Name ) = 0;
    virtual void SRPAPI LuaGetGlobal( VS_CHAR *Name ) = 0;

    virtual VS_INT32 SRPAPI LuaSetRef( void *Object, VS_INT32 Index) = 0;
    virtual void SRPAPI LuaClearRef( void *Object, VS_INT32 LuaRefValue ) = 0;
    virtual void SRPAPI LuaGetRef( void *Object,VS_INT32 LuaRefValue ) = 0;

    virtual VS_INT32 SRPAPI LuaInsertTable2(VS_INT32 TableIndex) = 0;
    virtual void SRPAPI LuaRemoveTable2(VS_INT32 TableIndex,VS_INT32 Pos) = 0;
    virtual void SRPAPI LuaPushLString( VS_CHAR *Value, VS_ULONG Len ) = 0;
    virtual VS_BOOL SRPAPI LuaObjectNewTempTable( void *Object, VS_CHAR *Name ) = 0;
    virtual VS_BOOL SRPAPI LuaObjectIsLock( void *Object ) = 0;
    virtual void SRPAPI LuaObjectLock( void *Object ) = 0;
    virtual void SRPAPI LuaObjectUnLock( void *Object ) = 0;
    //Get Value From LuaString
    virtual VS_BOOL SRPAPI GetValueFromLua(VS_CHAR *String) = 0;

    virtual VS_BOOL SRPAPI LuaPushBinBuf( class ClassOfSRPBinBufInterface *BinBuf, VS_BOOL AutoRelease ) = 0;
    virtual class ClassOfSRPBinBufInterface *SRPAPI LuaToBinBuf( VS_INT32 Index ) = 0;
    virtual VS_BOOL SRPAPI LuaIsBinBuf( VS_INT32 Index ) = 0;
    virtual VS_BOOL SRPAPI DefLuaFunction( void *Object, VS_CHAR *ScriptName ) = 0;
    virtual VS_BOOL SRPAPI SaveToLuaFunc( void *Object, VS_CHAR *LuaFileName, VS_CHAR *FuncName ) = 0;

    virtual VS_BOOL SRPAPI LuaPushSXml( class ClassOfSRPSXMLInterface *SXml, VS_BOOL AutoRelease ) = 0;
    virtual class ClassOfSRPSXMLInterface *SRPAPI LuaToSXml( VS_INT32 Index ) = 0;
    virtual VS_BOOL SRPAPI LuaIsSXml( VS_INT32 Index ) = 0;

    virtual VS_BOOL SRPAPI LuaPushFunctionPara( class ClassOfSRPFunctionParaInterface *FunctionPara, VS_BOOL AutoRelease ) = 0;
    virtual class ClassOfSRPFunctionParaInterface *SRPAPI LuaToFunctionPara( VS_INT32 Index ) = 0;
    virtual VS_BOOL SRPAPI LuaIsFunctionPara( VS_INT32 Index ) = 0;

    virtual VS_BOOL SRPAPI LuaPushCommInterface( class ClassOfSRPCommInterface *CommInterface, VS_BOOL AutoRelease ) = 0;
    virtual class ClassOfSRPCommInterface *SRPAPI LuaToCommInterface( VS_INT32 Index ) = 0;
    virtual VS_BOOL SRPAPI LuaIsCommInterface( VS_INT32 Index ) = 0;

    virtual void SRPAPI LuaInsertTable(VS_INT32 TableIndex,VS_INT32 Pos) = 0;
    virtual void SRPAPI LuaRemoveTable(VS_INT32 TableIndex,VS_INT32 Pos) = 0;
    virtual VS_INT32 SRPAPI LuaObjLen(VS_INT32 TableIndex) = 0;
    virtual void SRPAPI LuaGetTablei(VS_INT32 TableIndex,VS_INT32 Pos) = 0;
    virtual void SRPAPI LuaSetTablei(VS_INT32 TableIndex,VS_INT32 Pos) = 0;

    virtual void SRPAPI LuaInitObject(void *Object,VS_CHAR *InitScript) = 0;
    virtual VS_CHAR *SRPAPI GetAttributeLuaString(void *Object) = 0;
    virtual VS_BOOL SRPAPI LockGC(void *Object) = 0;
    virtual VS_BOOL SRPAPI UnLockGC(void *Object) = 0;
    //Get Object From LuaString
    virtual void *SRPAPI GetObjectFromLua(VS_CHAR *String) = 0;
    virtual void SRPAPI GCCollect() = 0;

//---------registry function
    virtual VS_CHAR *SRPAPI GetRegStr(VS_CHAR *SubKey,VS_CHAR *ValueName,VS_CHAR *DefaultValue) = 0;
    virtual VS_ULONG SRPAPI GetRegInt(VS_CHAR *SubKey,VS_CHAR *ValueName,VS_ULONG DefaultValue) = 0;

//---------time call back function
    virtual VS_ULONG SRPAPI SetupTimer(VS_INT32 Ticket,VS_TimerProc FunctionAddr,void *Object,VS_ULONG Para1,VS_ULONG Para2,VS_ULONG Para3,VS_ULONG Para4 ) = 0;
    virtual void SRPAPI KillTimer(VS_ULONG TimerID) = 0;
    virtual VS_ULONG SRPAPI GetTickCount() = 0;

//---------
    virtual VS_INT8 *SRPAPI GetMD5(VS_INT8 *Buf,VS_INT32 BufSize) = 0;
    virtual VS_BOOL SRPAPI MD5ToUuid(VS_INT8 *String,VS_UUID *Uuid) = 0;
    virtual VS_INT8 *SRPAPI UuidToMD5(VS_UUID *Uuid) = 0;
    virtual void SRPAPI CreateUuid(VS_UUID *UuidPtr) = 0;
    virtual void SRPAPI GetSRPTempPath(VS_ULONG BufSize,VS_CHAR *Buf) = 0;
    virtual VS_INT8 *SRPAPI StringToUtf8(VS_INT8 *String) = 0;
    virtual VS_INT8 *SRPAPI Utf8ToString(VS_INT8 *String) = 0;

//---------Set exception handler
    virtual void SRPAPI SetExceptHandler(VS_ExceptHandlerProc ExceptHandlerProc) = 0;

//--------global SRP lock
    virtual void SRPAPI SRPLock( ) = 0;
    virtual void SRPAPI SRPUnLock( ) = 0;

//---------Compression decompression function, use the ZLIB
    virtual VS_BOOL SRPAPI Compress(VS_UINT8 *dest,VS_ULONG *destLen,VS_UINT8 *source,VS_ULONG sourceLen ) = 0;
    virtual VS_BOOL SRPAPI UnCompress(VS_UINT8 *dest,VS_ULONG *destLen,VS_UINT8 *source,VS_ULONG sourceLen ) = 0;

//---------
    virtual VS_BOOL SRPAPI IsSysRootItemSync(void  *SystemRootItem) = 0;
    virtual VS_BOOL SRPAPI WaitSysRootItemSync(void  *SystemRootItem) = 0;

//---------atomic functions
    virtual void *SRPAPI GetAtomicService() = 0;
    virtual void *SRPAPI CreateAtomicSysRootItem( VS_CHAR *SysRootItemName, VS_CHAR *DependSysRootItem,VS_UUID *SystemRootItemID, VS_UUID *SystemRootItemIDEx ) = 0;
    virtual void *SRPAPI GetAtomicSysRootItem( VS_CHAR *SysRootItemName ) = 0;
    virtual void *SRPAPI GetAtomicObject( VS_UUID *UuidPtr ) = 0;              //--can be used for all the atomic objects
    virtual void *SRPAPI GetAtomicObjectEx( void *ParentAtomicObject, VS_CHAR *ObjectName ) = 0;
    virtual void *SRPAPI GetAtomicClass(void *AtomicObject) = 0;
    virtual void SRPAPI GetAtomicID(void *AtomicObject,VS_UUID *UuidPtr) = 0;  //--can be used for all the atomic objects
    virtual VS_CHAR *SRPAPI GetAtomicName(void *AtomicObject) = 0;             //--can be used for all the atomic objects
    virtual void *SRPAPI AtomicToObject(void *AtomicObject) = 0;
    virtual void *SRPAPI ObjectToAtomic(void *Object) = 0;
    virtual void SRPAPI FreeAtomicObject(void *Object) = 0;

    //------Between attributes to; separated
    virtual void *SRPAPI CreateAtomicObjectSimple( VS_CHAR *SysRootItemName,VS_CHAR *ObjectName,VS_CHAR *Attribute, VS_UUID *ObjectID, VS_CHAR **ErrorInfo ) = 0;
    virtual void *SRPAPI CreateAtomicObjectAttributeSimple( void *AtomicObject,VS_CHAR *Attribute, VS_CHAR **ErrorInfo ) = 0;
    virtual void *SRPAPI CreateAtomicStructSimple( VS_CHAR *StructName,VS_CHAR *Attribute, VS_UUID *ObjectID, VS_CHAR **ErrorInfo ) = 0;
    virtual void *SRPAPI CreateAtomicFunctionSimple( void *AtomicObject, VS_CHAR *FunctionName,VS_CHAR *Attribute, VS_UUID *ObjectID, VS_CHAR **ErrorInfo, VS_BOOL StdCallFlag,VS_BOOL GlobalFunctionFlag ) = 0;
    virtual void SRPAPI SetAtomicFunction(void *AtomicFunction,void *FuncAddress) = 0;

    //--macro
    virtual void *SRPAPI CreateAtomicMacro( VS_CHAR *MacroName, VS_UINT8 MacroType ) = 0;
    virtual void *SRPAPI CreateAtomicMacroItem( void *MacroObject,VS_CHAR *MacroItemName, VS_CHAR *MacroItemValue ) = 0;
    //--Module
    virtual void *SRPAPI CreateAtomicModule( VS_CHAR *ModuleName,VS_UINT16 ModuleType,VS_UUID *ModuleID ) = 0;
    virtual void *SRPAPI CreateAtomicEditModule( VS_CHAR *ModuleName,VS_UUID *ModuleID ) = 0;
    //--struct
    virtual void *SRPAPI CreateAtomicStruct( VS_CHAR *StructName,VS_CHAR *StructCaption,VS_UUID *StructID ) = 0;
    //----
    virtual void *SRPAPI CreateAtomicObject( void *AtomicObject, OBJECTATTRIBUTEINDEX AtomicAttributeIndex, void *AtomicClassObject, VS_CHAR *ObjectName, VS_UUID *ObjectID ) = 0;
    virtual void *SRPAPI CreateAtomicAttachAttribute( void *AtomicObject, VS_CHAR *AttributeName, VS_CHAR *Caption, VS_UINT8 Type, VS_ULONG StaticID, VS_UINT8 SyncFlag,VS_UINT8 CreateFlag,VS_UINT8 NotifyFlag,VS_UINT8 EditType,VS_UINT8 EditControl,VS_UINT8 EditReadOnly,VS_CHAR *Default,VS_CHAR *Desc) = 0;
    virtual void *SRPAPI CreateAtomicAttribute( void *AtomicObject, VS_CHAR *AttributeName, VS_CHAR *Caption, VS_UINT8 Type, VS_ULONG StaticID, VS_UINT8 SyncFlag,VS_UINT8 CreateFlag,VS_UINT8 NotifyFlag,VS_UINT8 EditType,VS_UINT8 EditControl,VS_UINT8 EditReadOnly,VS_CHAR *Default,VS_CHAR *Desc ) = 0;
    virtual void *SRPAPI CreateAtomicFuncRetAttribute( void *AtomicObject, VS_UINT8 Type,VS_CHAR *Desc  ) = 0;
    virtual void *SRPAPI CreateAtomicFuncParaAttribute( void *AtomicObject, VS_CHAR *AttributeName, VS_CHAR *AttributeCaption,VS_UINT8 Type,VS_CHAR *Desc ) = 0;
    virtual void *SRPAPI CreateAtomicStructAttribute( void *AtomicObject, VS_CHAR *AttributeName, VS_CHAR *Caption, VS_UINT8 Type,VS_CHAR *Desc ) = 0;

    virtual VS_BOOL SRPAPI SetAtomicAttributeLength( void *AtomicObject, VS_INT32 Length ) = 0;
    virtual VS_BOOL SRPAPI SetAtomicAttributeStruct( void *AtomicObject, void *AtomicStruct ) = 0;
    virtual VS_BOOL SRPAPI SetAtomicAttributeCombobox( void *AtomicObject, VS_CHAR *MacroName ) = 0;
    virtual VS_BOOL SRPAPI SetAtomicAttributeSyncFlag( void *AtomicObject, VS_UINT8 SyncFlag ) = 0;

    virtual void *SRPAPI CreateAtomicScript( void *AtomicObject, VS_CHAR *ScriptName, VS_UUID *ScriptID, VS_CHAR *Desc, VS_UINT8 *ScriptBuf ) = 0;

    virtual void *SRPAPI CreateAtomicFunction( void *AtomicObject, VS_CHAR *FunctionName, VS_UUID *FunctionID, VS_CHAR *Desc, VS_BOOL CantOvl, VS_BOOL CallBack, VS_BOOL StdCallFlag,VS_BOOL GlobalFunctionFlag ) = 0;
    virtual void *SRPAPI CreateAtomicLuaFunction( void *AtomicObject, VS_CHAR *LuaFunctionName, VS_UUID *LuaFunctionID, VS_CHAR *Desc ) = 0;
    virtual void *SRPAPI CreateAtomicOvlFunction( void *AtomicObject, VS_CHAR *FunctionName, VS_CHAR *OriginFunctionName, VS_UUID *OvlFunctionID, VS_CHAR *Desc, VS_BOOL CantOvl ) = 0;
    virtual void *SRPAPI CreateAtomicFunctionEx( void *AtomicObject, VS_CHAR *FunctionName, VS_UUID *FunctionID, VS_CHAR *Desc, VS_BOOL CantOvl, VS_BOOL CallBack, VS_CHAR *Type,VS_CHAR **ErrorInfo, VS_BOOL StdCallFlag,VS_BOOL GlobalFunctionFlag ) = 0;

    virtual void *SRPAPI CreateAtomicInEvent( void *AtomicObject, VS_CHAR *InEventName, VS_UUID *InEventID,VS_CHAR *OutEventName ) = 0;
    virtual void *SRPAPI CreateAtomicOutEvent( void *AtomicObject, VS_CHAR *OutEventName, VS_UUID *OutEventID, VS_CHAR *Desc, VS_BOOL DynamicFlag ) = 0;

    virtual void *SRPAPI GetAtomicFunction(VS_UUID *FunctionID) = 0;
    virtual void *SRPAPI GetAtomicFunctionEx(void *AtomicObject,VS_UUID *FunctionID) = 0;
    virtual void *SRPAPI GetAtomicFunctionByName( void *AtomicObject, VS_CHAR *FunctionName ) = 0;
    virtual void *SRPAPI GetAtomicScript( void *AtomicObject, VS_CHAR *ScriptName ) = 0;
    virtual void *SRPAPI GetAtomicOutEvent( void *AtomicObject, VS_CHAR *OutEventName ) = 0;
    virtual VS_BOOL SRPAPI GetAtomicInfo( void *Atomic, VS_ULONG *AtomicType,VS_ULONG *Para1, VS_ULONG *Para2, VS_ULONG *Para3, VS_ULONG *Para4, VS_ULONG *Para5, VS_ULONG *Para6, VS_ULONG *Para7 ) = 0;

    virtual VS_BOOL SRPAPI GetAtomicAttributeInfo(void *AtomicObject,VS_INT32 AttributeIndexNumber,OBJECTATTRIBUTEINDEX *AttributeIndex,VS_CHAR *AttributeName,VS_ATTRIBUTEINFO *AttributeInfo) = 0;
    virtual VS_BOOL SRPAPI GetAtomicAttributeInfoEx(void *AtomicObject,VS_INT32 AttributeIndexNumber,OBJECTATTRIBUTEINDEX *AttributeIndex,OBJECTATTRIBUTEINDEX ThisAtomicAttributeIndex,VS_ATTRIBUTEINFO *AttributeInfo) = 0;
    virtual VS_INT32 SRPAPI GetAtomicAttachAttributeNumber(void *AtomicObject) = 0;
    virtual VS_INT32 SRPAPI GetAtomicAttachAttributeSize(void *AtomicObject) = 0;
    virtual VS_BOOL SRPAPI GetAtomicAttachAttributeInfoEx(void *AtomicObject,OBJECTATTRIBUTEINDEX AttachAttributeIndex,VS_ATTRIBUTEINFO *AttributeInfo) = 0;
    virtual VS_BOOL SRPAPI GetAtomicAttributeLength( void *AtomicObject, VS_INT32 *Length ) = 0;
    virtual void *SRPAPI GetAtomicAttributeStruct( void *AtomicObject ) = 0;
    virtual void *SRPAPI GetAtomicAttributeCombobox( void *AtomicObject ) = 0;
    virtual VS_UINT8 SRPAPI GetAtomicAttributeSyncFlag( void *AtomicObject ) = 0;
    virtual OBJECTATTRIBUTEINDEX SRPAPI ToAttributeIndex(void *AtomicObject,OBJECTATTRIBUTEINDEX AtomicAttributeIndex) = 0;
    virtual OBJECTATTRIBUTEINDEX SRPAPI ToAtomicAttributeIndex(void *AtomicObject,OBJECTATTRIBUTEINDEX AttributeIndex) = 0;

    virtual VS_INT32 SRPAPI GetAtomicStructAttributeNumber(void *AtomicObject) = 0;
    virtual VS_INT32 SRPAPI GetAtomicStructAttributeSize(void *AtomicObject) = 0;
    virtual VS_BOOL SRPAPI GetAtomicStructAttributeInfo(void *AtomicObject,VS_CHAR *AttributeName,VS_ATTRIBUTEINFO *AttributeInfo) = 0;
    virtual VS_BOOL SRPAPI GetAtomicStructAttributeInfoEx(void *AtomicObject,OBJECTATTRIBUTEINDEX ThisAtomicAttributeIndex,VS_ATTRIBUTEINFO *AttributeInfo) = 0;

    virtual VS_INT32 SRPAPI GetAtomicFuncRetAttributeNumber(void *AtomicObject) = 0;
    virtual VS_INT32 SRPAPI GetAtomicFuncParaAttributeNumber(void *AtomicObject) = 0;

    virtual void *SRPAPI QueryFirstAtomicMacro( VS_ULONG *QueryContext, VS_UUID *ServiceID, VS_CHAR **MacroName, VS_UINT8 *Type ) = 0;
    virtual void *SRPAPI QueryNextAtomicMacro( VS_ULONG *QueryContext, VS_UUID *ServiceID, VS_CHAR **MacroName, VS_UINT8 *Type ) = 0;

    virtual void *SRPAPI QueryFirstAtomicInfo( VS_ULONG *QueryContext, VS_UINT8 AtomicType, VS_ULONG *Para1, VS_ULONG *Para2, VS_ULONG *Para3, VS_ULONG *Para4, VS_ULONG *Para5 ) = 0;
    virtual void *SRPAPI QueryNextAtomicInfo( VS_ULONG *QueryContext, VS_UINT8 AtomicType, VS_ULONG *Para1, VS_ULONG *Para2, VS_ULONG *Para3, VS_ULONG *Para4, VS_ULONG *Para5 ) = 0;
    virtual VS_BOOL SRPAPI AtomicObjectCanOutput( void *AtomicObject, VS_INT32 HasOutputNumber, void **HasOutputAtomicObject ) = 0;
    virtual VS_BOOL SRPAPI AtomicObjectAttributeCanOutput( void *AtomicObject, OBJECTATTRIBUTEINDEX AtomicAttributeIndex,VS_INT32 HasOutputNumber, void **HasOutputAtomicObject ) = 0;

    virtual VS_BOOL SRPAPI SetAtomicAttribute(void *AtomicObject,VS_INT32 AttributeIndexNumber,OBJECTATTRIBUTEINDEX *AttributeIndex,OBJECTATTRIBUTEINDEX ThisAtomicAttributeIndex,VS_INT8 *NewValue) = 0;
    virtual void *SRPAPI GetAtomicAttribute( void *AtomicObject, VS_INT32 AttributeIndexNumber,OBJECTATTRIBUTEINDEX *AttributeIndex, OBJECTATTRIBUTEINDEX ThisAtomicAttributeIndex ) = 0;
    virtual void *SRPAPI GetAtomicAttributeDefault( void *AtomicObject, VS_INT32 AttributeIndexNumber,OBJECTATTRIBUTEINDEX *AttributeIndex, OBJECTATTRIBUTEINDEX ThisAtomicAttributeIndex ) = 0;

    virtual VS_SYNCGROUP SRPAPI GetAtomicObjectSyncGroup( void *AtomicObject ) = 0;
    virtual VS_BOOL SRPAPI SetAtomicObjectSyncGroup( void *AtomicObject, VS_SYNCGROUP SyncGroup ) = 0;

    virtual VS_BOOL SRPAPI GetAtomicObjectAttribute( void *AtomicObject,VS_BOOL *SysEvent,VS_UINT8 *SpecialEvent,VS_UINT8 *ActiveCmd,VS_UINT8 *SaveFlag) = 0;
    virtual VS_BOOL SRPAPI SetAtomicObjectAttribute( void *AtomicObject,VS_BOOL SysEvent,VS_UINT8 SpecialEvent,VS_UINT8 ActiveCmd,VS_UINT8 SaveFlag ) = 0;
    virtual void *SRPAPI CreateAtomicDepend( VS_CHAR *DependServiceName, VS_UUID *DependID) = 0;

    virtual VS_BOOL SRPAPI IsValid() = 0;
    virtual VS_BOOL SRPAPI ProgramRestart( ) = 0;

    virtual VS_BOOL SRPAPI HttpDownLoad( VS_UUID *AttachObjectID, VS_CHAR *ServerUrl,VS_CHAR *ClientPath,VS_CHAR *FileName, VS_FileUpDownLoadCallBackProc CallBackProc, VS_UUID *ObjectID, VS_ULONG Para, VS_BOOL SaveFileFlag ) = 0;
    virtual void SRPAPI HttpDownLoadAbort(  ) = 0;
    virtual void SRPAPI RegWebDownFunction(VS_WebDownInfoProc CallBackProc,VS_ULONG Para) = 0;
    virtual void SRPAPI UnRegWebDownFunction(VS_WebDownInfoProc CallBackProc,VS_ULONG Para) = 0;
    virtual void SRPAPI WebDownPrint(VS_ULONG uMes, VS_CHAR *FileName, VS_ULONG MaxLength, VS_ULONG CurLength) = 0;

    virtual void SRPAPI RegDllCallBack( VS_MsgCallBackProc MsgCallBackProc, VS_ULONG MsgCallBackPara ) = 0;
    virtual void SRPAPI UnRegDllCallBack( VS_MsgCallBackProc MsgCallBackProc, VS_ULONG MsgCallBackPara ) = 0;

    virtual OBJECTATTRIBUTEINDEX SRPAPI AllocQueue(void *ParentObject,void *Object) = 0;
    virtual OBJECTATTRIBUTEINDEX SRPAPI AllocQueueEx(void *ParentObject,VS_UUID *ClassID) = 0;

    virtual VS_BOOL SRPAPI GetPeerIP(VS_ULONG ClientID,VSSOCKADDR_IN *ClientIP) = 0;
    virtual VS_ULONG SRPAPI GetServerID() = 0;
    //---Client/Send Message
    virtual VS_BOOL SRPAPI RemoteSend( void *Object, VS_ULONG ClientID, class ClassOfSRPParaPackageInterface *ParaPkg ) = 0;
    //---------------------------------------------------
    //---SRPDoc接口
    virtual void *SRPAPI GetSysDocClass() = 0;
    virtual void *SRPAPI FirstDoc(VS_QUERYRECORD *QueryRecord,VS_CHAR **DocName) = 0;
    virtual void *SRPAPI NextDoc(VS_QUERYRECORD *QueryRecord,VS_CHAR **DocName) = 0;
    virtual void SRPAPI RegisterDoc(void *DocObject,VS_CHAR *DocName) = 0;
    virtual void SRPAPI UnRegisterDoc(void *DocObject) = 0;
    virtual VS_EVENTPARAM_RUNPARAM *SRPAPI ProcessSysDocEvent(VS_UUID *DocObjectID,VS_UUID *EventID,VS_EVENTPARAM_RUNPARAM *RequestParam) = 0;
    virtual VS_BOOL SRPAPI RegDocEventFunction(VS_UUID *DocObjectID,VS_UUID *EventID, void *FuncAddr,VS_ULONG Para) = 0;
    virtual void SRPAPI UnRegDocEventFunction(VS_UUID *DocObjectID,VS_UUID *EventID, void *FuncAddr,VS_ULONG Para ) = 0;
    virtual void SRPAPI GetActiveServiceID(VS_UUID *UuidPtr) = 0;
    //---------------------------------------------------
    //Get Value From LuaString
    virtual void SRPAPI RegisterAttachClass(void *OriginClass,void *AttachClass) = 0;
    virtual void SRPAPI UnRegisterAttachClass(void *OriginClass,void *AttachClass) = 0;
    //Wait Event
    virtual VS_BOOL SRPAPI WaitEvent(void *SrcObject,VS_UUID *EventID, void *Object, void *FuncAddr,VS_ULONG Para,VS_BOOL AutoDelete) = 0;
    virtual void SRPAPI UnWaitEvent(void *SrcObject,VS_UUID *EventID, void *Object, void *FuncAddr,VS_ULONG Para) = 0;
    virtual VS_INT32 SRPAPI LuaPushEventPara(void *EventParam) = 0;

    virtual VS_BOOL SRPAPI RegChangeCallBackEx(void *Object,VS_ObjectChangeNotifyExProc ObjectChangeNotifyProc,void *DesObject,VS_ULONG Para,VS_BOOL ChildNotify) = 0;
    virtual void SRPAPI UnRegChangeCallBackEx(void *Object,VS_ObjectChangeNotifyExProc ObjectChangeNotifyProc,void *DesObject,VS_ULONG Para) = 0;

    //---CopyToClipBoard
    virtual void SRPAPI ToClipBoard(VS_CHAR *Info) = 0;
    virtual VS_CHAR *SRPAPI FromClipBoard() = 0;

    virtual VS_BOOL SRPAPI IsWindowlessSite( ) = 0;
    virtual void SRPAPI RegWindowlessSiteCallBack(struct VSWindowlessSiteCallBackInfo *CallBackInfo, VS_UUID *ObjectID, VS_ULONG Para ) = 0;
    virtual void SRPAPI UnRegWindowlessSiteCallBack(struct VSWindowlessSiteCallBackInfo *CallBackInfo, VS_UUID *ObjectID, VS_ULONG Para ) = 0;

    virtual VS_BOOL SRPAPI IsWindowlessTransparent( ) = 0;
    virtual void SRPAPI Windowless_Redraw( VS_BOOL fErase ) = 0;
    virtual void SRPAPI Windowless_GetDC( void **hDC,VS_RECT *rEct ) = 0;
    virtual void SRPAPI Windowless_ReleaseDC( void *hDC ) = 0;
    virtual void SRPAPI KillClientWndFocus( VS_HWND hWnd, VS_BOOL NeedAction ) = 0;

    virtual VS_BOOL SRPAPI XmlToSysRootItem(class ClassOfSRPSXMLInterface *SXMLInterface,VS_CHAR *DataPath,VS_CHAR *SegmentName,SRPParse_PrintProc PrintProc,VS_ULONG Para) = 0;
    virtual VS_BOOL SRPAPI XmlToObject(class ClassOfSRPSXMLInterface *SXMLInterface,void *ParentObject,OBJECTATTRIBUTEINDEX AttributeIndex,VS_CHAR *DataPath,VS_CHAR *SegmentName,SRPParse_PrintProc PrintProc,VS_ULONG Para) = 0;
    virtual VS_BOOL SRPAPI ServiceToXml(class ClassOfSRPSXMLInterface *SXMLInterface,VS_CHAR *PassWord,VS_CHAR *DataPath,VS_BOOL CFunctionFlag,VS_BOOL OutputObjectID,SRPParse_PrintProc PrintProc,VS_ULONG Para) = 0;
    virtual VS_BOOL SRPAPI SysRootItemToXml(class ClassOfSRPSXMLInterface *SXMLInterface,VS_CHAR *SysRootItemName,VS_CHAR *DataPath,VS_BOOL CFunctionFlag,VS_BOOL OutputObjectID,SRPParse_PrintProc PrintProc,VS_ULONG Para) = 0;
    virtual VS_BOOL SRPAPI ObjectToXml(class ClassOfSRPSXMLInterface *SXMLInterface,void *Object,VS_CHAR *DataPath,VS_BOOL CFunctionFlag,VS_BOOL OutputObjectID,SRPParse_PrintProc PrintProc,VS_ULONG Para) = 0;

    virtual VS_UUID *SRPAPI GetVSObjectID(VS_INT32 Which) = 0;
    
//---------get interfaces
    virtual class ClassOfBasicSRPInterface *SRPAPI GetBasicInterface() = 0;
    virtual class ClassOfSRPSXMLInterface *SRPAPI GetSXMLInterface() = 0;
    virtual class ClassOfSRPFunctionParaInterface *SRPAPI GetFunctionParaInterface() = 0;
    virtual class ClassOfSRPLockInterface *SRPAPI GetSRPLockInterface(void) = 0;
    virtual class ClassOfSRPBinBufInterface *SRPAPI GetSRPBinBufInterface(void) = 0;
    virtual class ClassOfSRPParaPackageInterface *SRPAPI GetParaPkgInterface() = 0;
    virtual class ClassOfSRPMemoryFileInterface *SRPAPI GetEnvMemoryFile() = 0;
	virtual class ClassOfSRPCommInterface *SRPAPI GetCommInterface() = 0;
    virtual class ClassOfSRPFileDiskInterface *SRPAPI GetFileDiskInterface() = 0;

    virtual void SRPAPI GetSRPConfigPath(VS_ULONG BufSize,VS_CHAR *Buf) = 0;
    //temp file
    virtual VS_BOOL SRPAPI RegTempFile(VS_CHAR *TempFileName,VS_CHAR *OriFileName) = 0;     /*clear when process not exist*/
    virtual VS_CHAR *SRPAPI GetRegTempFile(VS_CHAR *OriFileName,VS_CHAR *Buf,VS_INT32 BufSize) = 0;   /*get temp filename of other process*/
    virtual void SRPAPI UnRegTempFile(VS_CHAR *TempFileName) = 0;   /*clear when process not exist,temp file will be delete by starcore*/

    virtual void *SRPAPI QueryInterface( VS_UUID *InterfaceID ) = 0;

    virtual VS_BOOL SRPAPI LockLuaTable( ) = 0;
    virtual VS_BOOL SRPAPI UnLockLuaTable( ) = 0;

    virtual VS_UUID *SRPAPI GetIDEx(void *Object) = 0;
    virtual VS_BOOL SRPAPI IsRootService() = 0;

    virtual VS_BOOL SRPAPI LuaGetObjectValue(void *Object,VS_CHAR *Name) = 0;
    virtual VS_BOOL SRPAPI LuaSetObjectValue(void *Object,VS_CHAR *Name) = 0;

    virtual class ClassOfSRPInterface *SRPAPI GetSRPInterface(void *Object) = 0;
    virtual class ClassOfSRPInterface *SRPAPI GetSRPInterfaceEx(VS_UUID *ObjectID) = 0;
    virtual VS_BOOL SRPAPI IsThisServiceEx(VS_UUID *ObjectID) = 0;

    virtual void SRPAPI AddRef() = 0;
    virtual VS_ULONG SRPAPI GetRef() = 0;

    //---edit log, bin file
    virtual void SRPAPI SetLog(void *Object,VS_BOOL Flag) = 0;
    virtual void SRPAPI SetLogFile(VS_CHAR *FileName) = 0;
    virtual VS_CHAR *SRPAPI GetLogFile() = 0;
    virtual void SRPAPI ClearLog() = 0;
    virtual VS_BOOL SRPAPI ApplyLog() = 0;

    //----local common call to c/c++ defined attribute or functions
    virtual VS_ULONG SRPAPI Call(void *Object,VS_CHAR *FunctionName,...) = 0;
    virtual VS_ULONG SRPAPI CallVar(void *Object,VS_CHAR *FunctionName,va_list argList) = 0;
    virtual VS_FLOAT SRPAPI FCall(void *Object,VS_CHAR *FunctionName,...) = 0;
    virtual VS_FLOAT SRPAPI FCallVar(void *Object,VS_CHAR *FunctionName,va_list argList) = 0;

    virtual VS_BOOL SRPAPI Set(void *Object,VS_CHAR *AttributeName,...) = 0;
    virtual VS_BOOL SRPAPI SetEx(void *Object,OBJECTATTRIBUTEINDEX AttributeIndex,...) = 0;
    virtual VS_BOOL SRPAPI SetVar(void *Object,VS_CHAR *AttributeName,va_list argList) = 0;
    virtual VS_BOOL SRPAPI SetVarEx(void *Object,OBJECTATTRIBUTEINDEX AttributeIndex,va_list argList) = 0;
    virtual VS_ULONG SRPAPI Get(void *Object,VS_CHAR *AttributeName) = 0;
    virtual VS_ULONG SRPAPI GetEx(void *Object,OBJECTATTRIBUTEINDEX AttributeIndex) = 0;
    virtual VS_FLOAT SRPAPI FGet(void *Object,VS_CHAR *AttributeName) = 0;
    virtual VS_FLOAT SRPAPI FGetEx(void *Object,OBJECTATTRIBUTEINDEX AttributeIndex) = 0;

    virtual VS_CHAR *SRPAPI LuaToLString(VS_INT32 index,VS_ULONG *len) = 0;
    virtual VS_BOOL SRPAPI SetNameBoolValue( void *Object,VS_CHAR *Name, VS_BOOL Value, VS_BOOL LocalChange  ) = 0;
    virtual VS_BOOL SRPAPI GetNameBoolValue( void *Object,VS_CHAR *Name, VS_BOOL *Value, VS_BOOL DefaultValue  ) = 0;
    virtual VS_BOOL SRPAPI LuaIsInt( VS_INT32 Index ) = 0;

    virtual VS_BOOL SRPAPI AtomicAttach( void *AtomicObject,VS_CHAR *ShareLibName ) = 0;   //called after atomic function created finish

    virtual VS_BOOL SRPAPI IsGlobalFunction(VS_UUID *FunctionID) = 0;
    virtual VS_BOOL SRPAPI IsGlobalFunctionEx(void *Object,VS_UUID *FunctionID) = 0;  //---Can be a parent class definition, the object is overloaded

    virtual void SRPAPI LuaGetDefinedClass(void *Object,VS_UUID *ObjectID) = 0;    //after use LuaGetTable，then can use this function to judge which object define the attribute
    virtual VS_BOOL SRPAPI LuaGetDefinedClassEx(void *Object,VS_CHAR *AttributeName,VS_UUID *ObjectID) = 0;

    //----common call to script/c/c++ defined attribute or functions
    //TypeSet format:  "(XXXX)X"  
    virtual VS_ULONG SRPAPI ScriptCall(void *Object,VS_ULONG *RetCode,VS_CHAR *FunctionName,VS_CHAR *TypeSet,...) = 0;
    virtual VS_ULONG SRPAPI ScriptCallVar(void *Object,VS_ULONG *RetCode,VS_CHAR *FunctionName,VS_CHAR *TypeSet,va_list argList) = 0;
    virtual VS_FLOAT SRPAPI ScriptFCall(void *Object,VS_ULONG *RetCode,VS_CHAR *FunctionName,VS_CHAR *TypeSet,...) = 0;
    virtual VS_FLOAT SRPAPI ScriptFCallVar(void *Object,VS_ULONG *RetCode,VS_CHAR *FunctionName,VS_CHAR *TypeSet,va_list argList) = 0;

    virtual VS_BOOL SRPAPI ScriptRCall(VS_ULONG ClientID,void *Object, VS_CHAR *ScriptName,VS_CHAR *TypeSet,...) = 0;
    virtual VS_BOOL SRPAPI ScriptRCallVar(VS_ULONG ClientID,void *Object, VS_CHAR *ScriptName,VS_CHAR *TypeSet,va_list argList) = 0;
    virtual VS_BOOL SRPAPI ScriptRCallEx(VS_ULONG ExcludeClientID,void *Object, VS_CHAR *ScriptName,VS_CHAR *TypeSet,...) = 0;
    virtual VS_BOOL SRPAPI ScriptRCallExVar(VS_ULONG ExcludeClientID,void *Object, VS_CHAR *ScriptName,VS_CHAR *TypeSet,va_list argList) = 0;
    virtual VS_ULONG SRPAPI ScriptSRCall(VS_ULONG WaitTime,VS_ULONG ClientID,VS_ULONG *RetCode,void *Object, VS_CHAR *ScriptName,VS_CHAR *TypeSet,...) = 0;
    virtual VS_ULONG SRPAPI ScriptSRCallVar(VS_ULONG WaitTime,VS_ULONG ClientID,VS_ULONG *RetCode,void *Object, VS_CHAR *ScriptName,VS_CHAR *TypeSet,va_list argList) = 0;
    virtual VS_FLOAT SRPAPI ScriptFSRCall(VS_ULONG WaitTime,VS_ULONG ClientID,VS_ULONG *RetCode,void *Object, VS_CHAR *ScriptName,VS_CHAR *TypeSet,...) = 0;
    virtual VS_FLOAT SRPAPI ScriptFSRCallVar(VS_ULONG WaitTime,VS_ULONG ClientID,VS_ULONG *RetCode,void *Object, VS_CHAR *ScriptName,VS_CHAR *TypeSet,va_list argList) = 0;

    //----get object attributes
    virtual VS_BOOL SRPAPI ScriptSetBool(void *Object,VS_CHAR *AttributeName,VS_BOOL Value) = 0;
    virtual VS_BOOL SRPAPI ScriptSetInt(void *Object,VS_CHAR *AttributeName,VS_INT32 Value) = 0;
    virtual VS_BOOL SRPAPI ScriptSetNumber(void *Object,VS_CHAR *AttributeName,VS_FLOAT Value) = 0;
    virtual VS_BOOL SRPAPI ScriptSetStr(void *Object,VS_CHAR *AttributeName,VS_CHAR *Value) = 0;
    virtual VS_BOOL SRPAPI ScriptSetObject(void *Object,VS_CHAR *AttributeName,VS_UINT8 Type,VS_ULONG Value) = 0;

    virtual VS_BOOL SRPAPI ScriptGetBool(void *Object,VS_CHAR *AttributeName) = 0;
    virtual VS_INT32 SRPAPI ScriptGetInt(void *Object,VS_CHAR *AttributeName) = 0;
    virtual VS_FLOAT SRPAPI ScriptGetNumber(void *Object,VS_CHAR *AttributeName) = 0;
    virtual VS_CHAR *SRPAPI ScriptGetStr(void *Object,VS_CHAR *AttributeName) = 0;
    virtual VS_ULONG SRPAPI ScriptGetObject(void *Object,VS_CHAR *AttributeName,VS_UINT8 *RetType) = 0;

    virtual VS_BOOL SRPAPI IsRegistered() = 0;

    virtual void SRPAPI SetVString(VS_VSTRING *Buf,VS_CHAR *Str) = 0;
    virtual VS_VSTRING *SRPAPI ToVString(VS_CHAR *Str) = 0;

    virtual void SRPAPI CheckPassword(VS_BOOL CheckFlag) = 0;  /* For GetSRPInterface, whether to check password of root user or not, default is true*/
};


#ifndef _VSOPENRESDBG
    #define CreateIndex(X,T)    CreateIndex_Nor(X,T)
    #define CreateIndexCmp(X,T,Y)    CreateIndexCmp_Nor(X,T,Y)
    #define CreateIDIndex(X)   CreateIDIndex_Nor(X)
    #define CreateIDIndexEx(X) CreateIDIndexEx_Nor(X)
    #define CreateMemory(X)   CreateMemory_Nor(X)
    #define GetMemoryPtr(X)   GetMemoryPtr_Nor(X)
    #define Malloc(X)         Malloc_Nor(X)
    #define ProcessEvent      ProcessEvent_Nor
    #define PostProcessEvent  PostProcessEvent_Nor
#else
    #define CreateIndex(X,T)    CreateIndex_Dbg(X,T,__FILE__,__LINE__)
    #define CreateIndexCmp(X,T,Y)    CreateIndexCmp_Dbg(X,T,Y,__FILE__,__LINE__)
    #define CreateIDIndex(X)   CreateIDIndex_Dbg(X,__FILE__,__LINE__)
    #define CreateIDIndexEx(X) CreateIDIndexEx_Dbg(X,__FILE__,__LINE__)
    #define CreateMemory(X)   CreateMemory_Dbg(X,__FILE__,__LINE__)
    #define GetMemoryPtr(X)   GetMemoryPtr_Dbg(X,__FILE__,__LINE__)
    #define Malloc(X)         Malloc_Dbg(X,__FILE__,__LINE__)
    #define ProcessEvent      ProcessEvent_Dbg
    #define PostProcessEvent  PostProcessEvent_Dbg
#endif

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
#define SRPCALL_RET1( SRP_Interface, SRP_Func,SRP_Ret,SRP_Default,SRP_Obj)\
    { \
        SRP_Func##Proc LocalProc; \
        LocalProc = (SRP_Func##Proc)SRP_Interface -> GetFunction( &VSFUNCID_##SRP_Func ); \
        if( LocalProc == NULL ) \
            SRP_Ret = SRP_Default; \
        else    \
            SRP_Ret = LocalProc( SRP_Obj );  \
    }
#define SRPCALL_RET2( SRP_Interface, SRP_Func,SRP_Ret,SRP_Default,SRP_Obj,SRP_Arg2)\
    { \
        SRP_Func##Proc LocalProc; \
        LocalProc = (SRP_Func##Proc)SRP_Interface -> GetFunction( &VSFUNCID_##SRP_Func ); \
        if( LocalProc == NULL ) \
            SRP_Ret = SRP_Default; \
        else    \
            SRP_Ret = LocalProc( SRP_Obj, SRP_Arg2 );   \
    }
#define SRPCALL_RET3( SRP_Interface, SRP_Func,SRP_Ret,SRP_Default,SRP_Obj,SRP_Arg2,SRP_Arg3)\
    { \
        SRP_Func##Proc LocalProc; \
        LocalProc = (SRP_Func##Proc)SRP_Interface -> GetFunction( &VSFUNCID_##SRP_Func ); \
        if( LocalProc == NULL ) \
            SRP_Ret = SRP_Default; \
        else    \
            SRP_Ret = LocalProc( SRP_Obj, SRP_Arg2, SRP_Arg3 );   \
    }
#define SRPCALL_RET4( SRP_Interface, SRP_Func,SRP_Ret,SRP_Default,SRP_Obj,SRP_Arg2,SRP_Arg3,SRP_Arg4)\
    { \
        SRP_Func##Proc LocalProc; \
        LocalProc = (SRP_Func##Proc)SRP_Interface -> GetFunction( &VSFUNCID_##SRP_Func ); \
        if( LocalProc == NULL ) \
            SRP_Ret = SRP_Default; \
        else    \
            SRP_Ret = LocalProc( SRP_Obj, SRP_Arg2, SRP_Arg3, SRP_Arg4 );   \
    }
#define SRPCALL_RET5( SRP_Interface, SRP_Func,SRP_Ret,SRP_Default,SRP_Obj,SRP_Arg2,SRP_Arg3,SRP_Arg4,SRP_Arg5)\
    { \
        SRP_Func##Proc LocalProc; \
        LocalProc = (SRP_Func##Proc)SRP_Interface -> GetFunction( &VSFUNCID_##SRP_Func ); \
        if( LocalProc == NULL ) \
            SRP_Ret = SRP_Default; \
        else    \
            SRP_Ret = LocalProc( SRP_Obj, SRP_Arg2, SRP_Arg3, SRP_Arg4, SRP_Arg5 );  \
    }
#define SRPCALL_RET6( SRP_Interface, SRP_Func,SRP_Ret,SRP_Default,SRP_Obj,SRP_Arg2,SRP_Arg3,SRP_Arg4,SRP_Arg5,SRP_Arg6)\
    { \
        SRP_Func##Proc LocalProc; \
        LocalProc = (SRP_Func##Proc)SRP_Interface -> GetFunction( &VSFUNCID_##SRP_Func ); \
        if( LocalProc == NULL ) \
            SRP_Ret = SRP_Default; \
        else    \
            SRP_Ret = LocalProc( SRP_Obj, SRP_Arg2, SRP_Arg3, SRP_Arg4, SRP_Arg5, SRP_Arg6 );  \
    }
#define SRPCALL_RET7( SRP_Interface, SRP_Func,SRP_Ret,SRP_Default,SRP_Obj,SRP_Arg2,SRP_Arg3,SRP_Arg4,SRP_Arg5,SRP_Arg6,SRP_Arg7)\
    { \
        SRP_Func##Proc LocalProc; \
        LocalProc = (SRP_Func##Proc)SRP_Interface -> GetFunction( &VSFUNCID_##SRP_Func ); \
        if( LocalProc == NULL ) \
            SRP_Ret = SRP_Default; \
        else    \
            SRP_Ret = LocalProc( SRP_Obj, SRP_Arg2, SRP_Arg3, SRP_Arg4, SRP_Arg5, SRP_Arg6, SRP_Arg7 ); \
    }
#define SRPCALL_RET8( SRP_Interface, SRP_Func,SRP_Ret,SRP_Default,SRP_Obj,SRP_Arg2,SRP_Arg3,SRP_Arg4,SRP_Arg5,SRP_Arg6,SRP_Arg7,SRP_Arg8)\
    { \
        SRP_Func##Proc LocalProc; \
        LocalProc = (SRP_Func##Proc)SRP_Interface -> GetFunction( &VSFUNCID_##SRP_Func ); \
        if( LocalProc == NULL ) \
            SRP_Ret = SRP_Default; \
        else    \
            SRP_Ret = LocalProc( SRP_Obj, SRP_Arg2, SRP_Arg3, SRP_Arg4, SRP_Arg5, SRP_Arg6, SRP_Arg7, SRP_Arg8 );  \
    }
#define SRPCALL_RET9( SRP_Interface, SRP_Func,SRP_Ret,SRP_Default,SRP_Obj,SRP_Arg2,SRP_Arg3,SRP_Arg4,SRP_Arg5,SRP_Arg6,SRP_Arg7,SRP_Arg8,SRP_Arg9) \
    { \
        SRP_Func##Proc LocalProc; \
        LocalProc = (SRP_Func##Proc)SRP_Interface -> GetFunction( &VSFUNCID_##SRP_Func ); \
        if( LocalProc == NULL ) \
            SRP_Ret = SRP_Default; \
        else    \
            SRP_Ret = LocalProc( SRP_Obj, SRP_Arg2, SRP_Arg3, SRP_Arg4, SRP_Arg5, SRP_Arg6, SRP_Arg7, SRP_Arg8, SRP_Arg9 );  \
    }
#define SRPCALL_RET10( SRP_Interface, SRP_Func,SRP_Ret,SRP_Default,SRP_Obj,SRP_Arg2,SRP_Arg3,SRP_Arg4,SRP_Arg5,SRP_Arg6,SRP_Arg7,SRP_Arg8,SRP_Arg9,SRP_Arg10) \
    { \
        SRP_Func##Proc LocalProc; \
        LocalProc = (SRP_Func##Proc)SRP_Interface -> GetFunction( &VSFUNCID_##SRP_Func ); \
        if( LocalProc == NULL ) \
            SRP_Ret = SRP_Default; \
        else    \
            SRP_Ret = LocalProc( SRP_Obj, SRP_Arg2, SRP_Arg3, SRP_Arg4, SRP_Arg5, SRP_Arg6, SRP_Arg7, SRP_Arg8, SRP_Arg9, SRP_Arg10 ); \
    }

#define SRPCALL_RET11( SRP_Interface, SRP_Func,SRP_Ret,SRP_Default,SRP_Obj,SRP_Arg2,SRP_Arg3,SRP_Arg4,SRP_Arg5,SRP_Arg6,SRP_Arg7,SRP_Arg8,SRP_Arg9,SRP_Arg10,SRP_Arg11) \
    { \
        SRP_Func##Proc LocalProc; \
        LocalProc = (SRP_Func##Proc)SRP_Interface -> GetFunction( &VSFUNCID_##SRP_Func ); \
        if( LocalProc == NULL ) \
            SRP_Ret = SRP_Default; \
        else    \
            SRP_Ret = LocalProc( SRP_Obj, SRP_Arg2, SRP_Arg3, SRP_Arg4, SRP_Arg5, SRP_Arg6, SRP_Arg7, SRP_Arg8, SRP_Arg9, SRP_Arg10, SRP_Arg11 );  \
    }
#define SRPCALL_RET12( SRP_Interface, SRP_Func,SRP_Ret,SRP_Default,SRP_Obj,SRP_Arg2,SRP_Arg3,SRP_Arg4,SRP_Arg5,SRP_Arg6,SRP_Arg7,SRP_Arg8,SRP_Arg9,SRP_Arg10,SRP_Arg11,SRP_Arg12) \
    { \
        SRP_Func##Proc LocalProc; \
        LocalProc = (SRP_Func##Proc)SRP_Interface -> GetFunction( &VSFUNCID_##SRP_Func ); \
        if( LocalProc == NULL ) \
            SRP_Ret = SRP_Default; \
        else    \
            SRP_Ret = LocalProc( SRP_Obj, SRP_Arg2, SRP_Arg3, SRP_Arg4, SRP_Arg5, SRP_Arg6, SRP_Arg7, SRP_Arg8, SRP_Arg9, SRP_Arg10, SRP_Arg11,SRP_Arg12 );  \
    }
#define SRPCALL_RET13( SRP_Interface, SRP_Func,SRP_Ret,SRP_Default,SRP_Obj,SRP_Arg2,SRP_Arg3,SRP_Arg4,SRP_Arg5,SRP_Arg6,SRP_Arg7,SRP_Arg8,SRP_Arg9,SRP_Arg10,SRP_Arg11,SRP_Arg12,SRP_Arg13) \
    { \
        SRP_Func##Proc LocalProc; \
        LocalProc = (SRP_Func##Proc)SRP_Interface -> GetFunction( &VSFUNCID_##SRP_Func ); \
        if( LocalProc == NULL ) \
            SRP_Ret = SRP_Default; \
        else    \
            SRP_Ret = LocalProc( SRP_Obj, SRP_Arg2, SRP_Arg3, SRP_Arg4, SRP_Arg5, SRP_Arg6, SRP_Arg7, SRP_Arg8, SRP_Arg9, SRP_Arg10, SRP_Arg11,SRP_Arg12,SRP_Arg13 );  \
    }
#define SRPCALL_RET14( SRP_Interface, SRP_Func,SRP_Ret,SRP_Default,SRP_Obj,SRP_Arg2,SRP_Arg3,SRP_Arg4,SRP_Arg5,SRP_Arg6,SRP_Arg7,SRP_Arg8,SRP_Arg9,SRP_Arg10,SRP_Arg11,SRP_Arg12,SRP_Arg13,SRP_Arg14) \
    { \
        SRP_Func##Proc LocalProc; \
        LocalProc = (SRP_Func##Proc)SRP_Interface -> GetFunction( &VSFUNCID_##SRP_Func ); \
        if( LocalProc == NULL ) \
            SRP_Ret = SRP_Default; \
        else    \
            SRP_Ret = LocalProc( SRP_Obj, SRP_Arg2, SRP_Arg3, SRP_Arg4, SRP_Arg5, SRP_Arg6, SRP_Arg7, SRP_Arg8, SRP_Arg9, SRP_Arg10, SRP_Arg11,SRP_Arg12,SRP_Arg13,SRP_Arg14 );  \
    }
#define SRPCALL_RET15( SRP_Interface, SRP_Func,SRP_Ret,SRP_Default,SRP_Obj,SRP_Arg2,SRP_Arg3,SRP_Arg4,SRP_Arg5,SRP_Arg6,SRP_Arg7,SRP_Arg8,SRP_Arg9,SRP_Arg10,SRP_Arg11,SRP_Arg12,SRP_Arg13,SRP_Arg14,SRP_Arg15) \
    { \
        SRP_Func##Proc LocalProc; \
        LocalProc = (SRP_Func##Proc)SRP_Interface -> GetFunction( &VSFUNCID_##SRP_Func ); \
        if( LocalProc == NULL ) \
            SRP_Ret = SRP_Default; \
        else    \
            SRP_Ret = LocalProc( SRP_Obj, SRP_Arg2, SRP_Arg3, SRP_Arg4, SRP_Arg5, SRP_Arg6, SRP_Arg7, SRP_Arg8, SRP_Arg9, SRP_Arg10, SRP_Arg11,SRP_Arg12,SRP_Arg13,SRP_Arg14,SRP_Arg15);  \
    }
#define SRPCALL_RET16( SRP_Interface, SRP_Func,SRP_Ret,SRP_Default,SRP_Obj,SRP_Arg2,SRP_Arg3,SRP_Arg4,SRP_Arg5,SRP_Arg6,SRP_Arg7,SRP_Arg8,SRP_Arg9,SRP_Arg10,SRP_Arg11,SRP_Arg12,SRP_Arg13,SRP_Arg14,SRP_Arg15,SRP_Arg16) \
    { \
        SRP_Func##Proc LocalProc; \
        LocalProc = (SRP_Func##Proc)SRP_Interface -> GetFunction( &VSFUNCID_##SRP_Func ); \
        if( LocalProc == NULL ) \
            SRP_Ret = SRP_Default; \
        else    \
            SRP_Ret = LocalProc( SRP_Obj, SRP_Arg2, SRP_Arg3, SRP_Arg4, SRP_Arg5, SRP_Arg6, SRP_Arg7, SRP_Arg8, SRP_Arg9, SRP_Arg10, SRP_Arg11,SRP_Arg12,SRP_Arg13,SRP_Arg14,SRP_Arg15,SRP_Arg16);  \
    }

#define SRPCALL_RET17( SRP_Interface, SRP_Func,SRP_Ret,SRP_Default,SRP_Obj,SRP_Arg2,SRP_Arg3,SRP_Arg4,SRP_Arg5,SRP_Arg6,SRP_Arg7,SRP_Arg8,SRP_Arg9,SRP_Arg10,SRP_Arg11,SRP_Arg12,SRP_Arg13,SRP_Arg14,SRP_Arg15,SRP_Arg16,SRP_Arg17) \
    { \
        SRP_Func##Proc LocalProc; \
        LocalProc = (SRP_Func##Proc)SRP_Interface -> GetFunction( &VSFUNCID_##SRP_Func ); \
        if( LocalProc == NULL ) \
            SRP_Ret = SRP_Default; \
        else    \
            SRP_Ret = LocalProc( SRP_Obj, SRP_Arg2, SRP_Arg3, SRP_Arg4, SRP_Arg5, SRP_Arg6, SRP_Arg7, SRP_Arg8, SRP_Arg9, SRP_Arg10, SRP_Arg11,SRP_Arg12,SRP_Arg13,SRP_Arg14,SRP_Arg15,SRP_Arg16,SRP_Arg17);  \
    }

#define SRPCALL_RET18( SRP_Interface, SRP_Func,SRP_Ret,SRP_Default,SRP_Obj,SRP_Arg2,SRP_Arg3,SRP_Arg4,SRP_Arg5,SRP_Arg6,SRP_Arg7,SRP_Arg8,SRP_Arg9,SRP_Arg10,SRP_Arg11,SRP_Arg12,SRP_Arg13,SRP_Arg14,SRP_Arg15,SRP_Arg16,SRP_Arg17,SRP_Arg18) \
    { \
        SRP_Func##Proc LocalProc; \
        LocalProc = (SRP_Func##Proc)SRP_Interface -> GetFunction( &VSFUNCID_##SRP_Func ); \
        if( LocalProc == NULL ) \
            SRP_Ret = SRP_Default; \
        else    \
            SRP_Ret = LocalProc( SRP_Obj, SRP_Arg2, SRP_Arg3, SRP_Arg4, SRP_Arg5, SRP_Arg6, SRP_Arg7, SRP_Arg8, SRP_Arg9, SRP_Arg10, SRP_Arg11,SRP_Arg12,SRP_Arg13,SRP_Arg14,SRP_Arg15,SRP_Arg16,SRP_Arg17,SRP_Arg18);  \
    }

#define SRPCALL_RET19( SRP_Interface, SRP_Func,SRP_Ret,SRP_Default,SRP_Obj,SRP_Arg2,SRP_Arg3,SRP_Arg4,SRP_Arg5,SRP_Arg6,SRP_Arg7,SRP_Arg8,SRP_Arg9,SRP_Arg10,SRP_Arg11,SRP_Arg12,SRP_Arg13,SRP_Arg14,SRP_Arg15,SRP_Arg16,SRP_Arg17,SRP_Arg18,SRP_Arg19) \
    { \
        SRP_Func##Proc LocalProc; \
        LocalProc = (SRP_Func##Proc)SRP_Interface -> GetFunction( &VSFUNCID_##SRP_Func ); \
        if( LocalProc == NULL ) \
            SRP_Ret = SRP_Default; \
        else    \
            SRP_Ret = LocalProc( SRP_Obj, SRP_Arg2, SRP_Arg3, SRP_Arg4, SRP_Arg5, SRP_Arg6, SRP_Arg7, SRP_Arg8, SRP_Arg9, SRP_Arg10, SRP_Arg11,SRP_Arg12,SRP_Arg13,SRP_Arg14,SRP_Arg15,SRP_Arg16,SRP_Arg17,SRP_Arg18,SRP_Arg19);  \
    }

#define SRPCALL_RET20( SRP_Interface, SRP_Func,SRP_Ret,SRP_Default,SRP_Obj,SRP_Arg2,SRP_Arg3,SRP_Arg4,SRP_Arg5,SRP_Arg6,SRP_Arg7,SRP_Arg8,SRP_Arg9,SRP_Arg10,SRP_Arg11,SRP_Arg12,SRP_Arg13,SRP_Arg14,SRP_Arg15,SRP_Arg16,SRP_Arg17,SRP_Arg18,SRP_Arg19,SRP_Arg20) \
    { \
        SRP_Func##Proc LocalProc; \
        LocalProc = (SRP_Func##Proc)SRP_Interface -> GetFunction( &VSFUNCID_##SRP_Func ); \
        if( LocalProc == NULL ) \
            SRP_Ret = SRP_Default; \
        else    \
            SRP_Ret = LocalProc( SRP_Obj, SRP_Arg2, SRP_Arg3, SRP_Arg4, SRP_Arg5, SRP_Arg6, SRP_Arg7, SRP_Arg8, SRP_Arg9, SRP_Arg10, SRP_Arg11,SRP_Arg12,SRP_Arg13,SRP_Arg14,SRP_Arg15,SRP_Arg16,SRP_Arg17,SRP_Arg18,SRP_Arg19,SRP_Arg20);  \
    }

//+++++++++++++++++++++++++++++++++++++++++++
#define SRPCALL_RETEX1( SRP_Interface, SRP_Func,SRP_Ret,SRP_Default,SRP_Obj)\
    { \
        SRP_Func##Proc LocalProc; \
        if( SRP_Obj != NULL ){ \
            LocalProc = (SRP_Func##Proc)SRP_Interface -> GetFunctionEx( SRP_Obj, &VSFUNCID_##SRP_Func ); \
            if( LocalProc == NULL ) \
                SRP_Ret = SRP_Default; \
            else    \
                SRP_Ret = LocalProc( SRP_Obj );  \
        }\
    }
#define SRPCALL_RETEX2( SRP_Interface, SRP_Func,SRP_Ret,SRP_Default,SRP_Obj,SRP_Arg2)\
    { \
        SRP_Func##Proc LocalProc; \
        if( SRP_Obj != NULL ){ \
            LocalProc = (SRP_Func##Proc)SRP_Interface -> GetFunctionEx( SRP_Obj, &VSFUNCID_##SRP_Func ); \
            if( LocalProc == NULL ) \
                SRP_Ret = SRP_Default; \
            else    \
                SRP_Ret = LocalProc( SRP_Obj, SRP_Arg2 );   \
        }\
    }
#define SRPCALL_RETEX3( SRP_Interface, SRP_Func,SRP_Ret,SRP_Default,SRP_Obj,SRP_Arg2,SRP_Arg3)\
    { \
        SRP_Func##Proc LocalProc; \
        if( SRP_Obj != NULL ){ \
            LocalProc = (SRP_Func##Proc)SRP_Interface -> GetFunctionEx( SRP_Obj, &VSFUNCID_##SRP_Func ); \
            if( LocalProc == NULL ) \
                SRP_Ret = SRP_Default; \
            else    \
                SRP_Ret = LocalProc( SRP_Obj, SRP_Arg2, SRP_Arg3 );   \
        }\
    }
#define SRPCALL_RETEX4( SRP_Interface, SRP_Func,SRP_Ret,SRP_Default,SRP_Obj,SRP_Arg2,SRP_Arg3,SRP_Arg4)\
    { \
        SRP_Func##Proc LocalProc; \
        if( SRP_Obj != NULL ){ \
           LocalProc = (SRP_Func##Proc)SRP_Interface -> GetFunctionEx( SRP_Obj, &VSFUNCID_##SRP_Func ); \
            if( LocalProc == NULL ) \
                SRP_Ret = SRP_Default; \
            else    \
                SRP_Ret = LocalProc( SRP_Obj, SRP_Arg2, SRP_Arg3, SRP_Arg4 );   \
        }\
    }
#define SRPCALL_RETEX5( SRP_Interface, SRP_Func,SRP_Ret,SRP_Default,SRP_Obj,SRP_Arg2,SRP_Arg3,SRP_Arg4,SRP_Arg5)\
    { \
        SRP_Func##Proc LocalProc; \
        if( SRP_Obj != NULL ){ \
            LocalProc = (SRP_Func##Proc)SRP_Interface -> GetFunctionEx( SRP_Obj, &VSFUNCID_##SRP_Func ); \
            if( LocalProc == NULL ) \
                SRP_Ret = SRP_Default; \
            else    \
                SRP_Ret = LocalProc( SRP_Obj, SRP_Arg2, SRP_Arg3, SRP_Arg4, SRP_Arg5 );  \
        }\
    }
#define SRPCALL_RETEX6( SRP_Interface, SRP_Func,SRP_Ret,SRP_Default,SRP_Obj,SRP_Arg2,SRP_Arg3,SRP_Arg4,SRP_Arg5,SRP_Arg6)\
    { \
        SRP_Func##Proc LocalProc; \
        if( SRP_Obj != NULL ){ \
            LocalProc = (SRP_Func##Proc)SRP_Interface -> GetFunctionEx( SRP_Obj, &VSFUNCID_##SRP_Func ); \
            if( LocalProc == NULL ) \
                SRP_Ret = SRP_Default; \
            else    \
                SRP_Ret = LocalProc( SRP_Obj, SRP_Arg2, SRP_Arg3, SRP_Arg4, SRP_Arg5, SRP_Arg6 );  \
        }\
    }
#define SRPCALL_RETEX7( SRP_Interface, SRP_Func,SRP_Ret,SRP_Default,SRP_Obj,SRP_Arg2,SRP_Arg3,SRP_Arg4,SRP_Arg5,SRP_Arg6,SRP_Arg7)\
    { \
        SRP_Func##Proc LocalProc; \
        if( SRP_Obj != NULL ){ \
            LocalProc = (SRP_Func##Proc)SRP_Interface -> GetFunctionEx( SRP_Obj, &VSFUNCID_##SRP_Func ); \
            if( LocalProc == NULL ) \
                SRP_Ret = SRP_Default; \
            else    \
                SRP_Ret = LocalProc( SRP_Obj, SRP_Arg2, SRP_Arg3, SRP_Arg4, SRP_Arg5, SRP_Arg6, SRP_Arg7 ); \
        }\
    }
#define SRPCALL_RETEX8( SRP_Interface, SRP_Func,SRP_Ret,SRP_Default,SRP_Obj,SRP_Arg2,SRP_Arg3,SRP_Arg4,SRP_Arg5,SRP_Arg6,SRP_Arg7,SRP_Arg8)\
    { \
        SRP_Func##Proc LocalProc; \
        if( SRP_Obj != NULL ){ \
            LocalProc = (SRP_Func##Proc)SRP_Interface -> GetFunctionEx( SRP_Obj, &VSFUNCID_##SRP_Func ); \
            if( LocalProc == NULL ) \
                SRP_Ret = SRP_Default; \
            else    \
                SRP_Ret = LocalProc( SRP_Obj, SRP_Arg2, SRP_Arg3, SRP_Arg4, SRP_Arg5, SRP_Arg6, SRP_Arg7, SRP_Arg8 );  \
        }\
    }
#define SRPCALL_RETEX9( SRP_Interface, SRP_Func,SRP_Ret,SRP_Default,SRP_Obj,SRP_Arg2,SRP_Arg3,SRP_Arg4,SRP_Arg5,SRP_Arg6,SRP_Arg7,SRP_Arg8,SRP_Arg9) \
    { \
        SRP_Func##Proc LocalProc; \
        if( SRP_Obj != NULL ){ \
            LocalProc = (SRP_Func##Proc)SRP_Interface -> GetFunctionEx( SRP_Obj, &VSFUNCID_##SRP_Func ); \
            if( LocalProc == NULL ) \
                SRP_Ret = SRP_Default; \
            else    \
                SRP_Ret = LocalProc( SRP_Obj, SRP_Arg2, SRP_Arg3, SRP_Arg4, SRP_Arg5, SRP_Arg6, SRP_Arg7, SRP_Arg8, SRP_Arg9 );  \
        }\
    }
#define SRPCALL_RETEX10( SRP_Interface, SRP_Func,SRP_Ret,SRP_Default,SRP_Obj,SRP_Arg2,SRP_Arg3,SRP_Arg4,SRP_Arg5,SRP_Arg6,SRP_Arg7,SRP_Arg8,SRP_Arg9,SRP_Arg10) \
    { \
        SRP_Func##Proc LocalProc; \
        if( SRP_Obj != NULL ){ \
            LocalProc = (SRP_Func##Proc)SRP_Interface -> GetFunctionEx( SRP_Obj, &VSFUNCID_##SRP_Func ); \
            if( LocalProc == NULL ) \
                SRP_Ret = SRP_Default; \
            else    \
                SRP_Ret = LocalProc( SRP_Obj, SRP_Arg2, SRP_Arg3, SRP_Arg4, SRP_Arg5, SRP_Arg6, SRP_Arg7, SRP_Arg8, SRP_Arg9, SRP_Arg10 ); \
        }\
    }

#define SRPCALL_RETEX11( SRP_Interface, SRP_Func,SRP_Ret,SRP_Default,SRP_Obj,SRP_Arg2,SRP_Arg3,SRP_Arg4,SRP_Arg5,SRP_Arg6,SRP_Arg7,SRP_Arg8,SRP_Arg9,SRP_Arg10,SRP_Arg11) \
    { \
        SRP_Func##Proc LocalProc; \
        if( SRP_Obj != NULL ){ \
            LocalProc = (SRP_Func##Proc)SRP_Interface -> GetFunctionEx( SRP_Obj, &VSFUNCID_##SRP_Func ); \
            if( LocalProc == NULL ) \
                SRP_Ret = SRP_Default; \
            else    \
                SRP_Ret = LocalProc( SRP_Obj, SRP_Arg2, SRP_Arg3, SRP_Arg4, SRP_Arg5, SRP_Arg6, SRP_Arg7, SRP_Arg8, SRP_Arg9, SRP_Arg10, SRP_Arg11 );  \
        }\
    }
#define SRPCALL_RETEX12( SRP_Interface, SRP_Func,SRP_Ret,SRP_Default,SRP_Obj,SRP_Arg2,SRP_Arg3,SRP_Arg4,SRP_Arg5,SRP_Arg6,SRP_Arg7,SRP_Arg8,SRP_Arg9,SRP_Arg10,SRP_Arg11,SRP_Arg12) \
    { \
        SRP_Func##Proc LocalProc; \
        if( SRP_Obj != NULL ){ \
            LocalProc = (SRP_Func##Proc)SRP_Interface -> GetFunctionEx( SRP_Obj, &VSFUNCID_##SRP_Func ); \
            if( LocalProc == NULL ) \
                SRP_Ret = SRP_Default; \
            else    \
                SRP_Ret = LocalProc( SRP_Obj, SRP_Arg2, SRP_Arg3, SRP_Arg4, SRP_Arg5, SRP_Arg6, SRP_Arg7, SRP_Arg8, SRP_Arg9, SRP_Arg10, SRP_Arg11,SRP_Arg12 );  \
        }\
    }
#define SRPCALL_RETEX13( SRP_Interface, SRP_Func,SRP_Ret,SRP_Default,SRP_Obj,SRP_Arg2,SRP_Arg3,SRP_Arg4,SRP_Arg5,SRP_Arg6,SRP_Arg7,SRP_Arg8,SRP_Arg9,SRP_Arg10,SRP_Arg11,SRP_Arg12,SRP_Arg13) \
    { \
        SRP_Func##Proc LocalProc; \
        if( SRP_Obj != NULL ){ \
            LocalProc = (SRP_Func##Proc)SRP_Interface -> GetFunctionEx( SRP_Obj, &VSFUNCID_##SRP_Func ); \
            if( LocalProc == NULL ) \
                SRP_Ret = SRP_Default; \
            else    \
                SRP_Ret = LocalProc( SRP_Obj, SRP_Arg2, SRP_Arg3, SRP_Arg4, SRP_Arg5, SRP_Arg6, SRP_Arg7, SRP_Arg8, SRP_Arg9, SRP_Arg10, SRP_Arg11,SRP_Arg12,SRP_Arg13 );  \
        }\
    }
#define SRPCALL_RETEX14( SRP_Interface, SRP_Func,SRP_Ret,SRP_Default,SRP_Obj,SRP_Arg2,SRP_Arg3,SRP_Arg4,SRP_Arg5,SRP_Arg6,SRP_Arg7,SRP_Arg8,SRP_Arg9,SRP_Arg10,SRP_Arg11,SRP_Arg12,SRP_Arg13,SRP_Arg14) \
    { \
        SRP_Func##Proc LocalProc; \
        if( SRP_Obj != NULL ){ \
            LocalProc = (SRP_Func##Proc)SRP_Interface -> GetFunctionEx( SRP_Obj, &VSFUNCID_##SRP_Func ); \
            if( LocalProc == NULL ) \
                SRP_Ret = SRP_Default; \
            else    \
                SRP_Ret = LocalProc( SRP_Obj, SRP_Arg2, SRP_Arg3, SRP_Arg4, SRP_Arg5, SRP_Arg6, SRP_Arg7, SRP_Arg8, SRP_Arg9, SRP_Arg10, SRP_Arg11,SRP_Arg12,SRP_Arg13,SRP_Arg14 );  \
        }\
    }
#define SRPCALL_RETEX15( SRP_Interface, SRP_Func,SRP_Ret,SRP_Default,SRP_Obj,SRP_Arg2,SRP_Arg3,SRP_Arg4,SRP_Arg5,SRP_Arg6,SRP_Arg7,SRP_Arg8,SRP_Arg9,SRP_Arg10,SRP_Arg11,SRP_Arg12,SRP_Arg13,SRP_Arg14,SRP_Arg15) \
    { \
        SRP_Func##Proc LocalProc; \
        if( SRP_Obj != NULL ){ \
            LocalProc = (SRP_Func##Proc)SRP_Interface -> GetFunctionEx( SRP_Obj, &VSFUNCID_##SRP_Func ); \
            if( LocalProc == NULL ) \
                SRP_Ret = SRP_Default; \
            else    \
                SRP_Ret = LocalProc( SRP_Obj, SRP_Arg2, SRP_Arg3, SRP_Arg4, SRP_Arg5, SRP_Arg6, SRP_Arg7, SRP_Arg8, SRP_Arg9, SRP_Arg10, SRP_Arg11,SRP_Arg12,SRP_Arg13,SRP_Arg14,SRP_Arg15);  \
        }\
    }
#define SRPCALL_RETEX16( SRP_Interface, SRP_Func,SRP_Ret,SRP_Default,SRP_Obj,SRP_Arg2,SRP_Arg3,SRP_Arg4,SRP_Arg5,SRP_Arg6,SRP_Arg7,SRP_Arg8,SRP_Arg9,SRP_Arg10,SRP_Arg11,SRP_Arg12,SRP_Arg13,SRP_Arg14,SRP_Arg15,SRP_Arg16) \
    { \
        SRP_Func##Proc LocalProc; \
        if( SRP_Obj != NULL ){ \
            LocalProc = (SRP_Func##Proc)SRP_Interface -> GetFunctionEx( SRP_Obj, &VSFUNCID_##SRP_Func ); \
            if( LocalProc == NULL ) \
                SRP_Ret = SRP_Default; \
            else    \
                SRP_Ret = LocalProc( SRP_Obj, SRP_Arg2, SRP_Arg3, SRP_Arg4, SRP_Arg5, SRP_Arg6, SRP_Arg7, SRP_Arg8, SRP_Arg9, SRP_Arg10, SRP_Arg11,SRP_Arg12,SRP_Arg13,SRP_Arg14,SRP_Arg15,SRP_Arg16);  \
        }\
    }

#define SRPCALL_RETEX17( SRP_Interface, SRP_Func,SRP_Ret,SRP_Default,SRP_Obj,SRP_Arg2,SRP_Arg3,SRP_Arg4,SRP_Arg5,SRP_Arg6,SRP_Arg7,SRP_Arg8,SRP_Arg9,SRP_Arg10,SRP_Arg11,SRP_Arg12,SRP_Arg13,SRP_Arg14,SRP_Arg15,SRP_Arg16,SRP_Arg17) \
    { \
        SRP_Func##Proc LocalProc; \
        if( SRP_Obj != NULL ){ \
            LocalProc = (SRP_Func##Proc)SRP_Interface -> GetFunctionEx( SRP_Obj, &VSFUNCID_##SRP_Func ); \
            if( LocalProc == NULL ) \
                SRP_Ret = SRP_Default; \
            else    \
                SRP_Ret = LocalProc( SRP_Obj, SRP_Arg2, SRP_Arg3, SRP_Arg4, SRP_Arg5, SRP_Arg6, SRP_Arg7, SRP_Arg8, SRP_Arg9, SRP_Arg10, SRP_Arg11,SRP_Arg12,SRP_Arg13,SRP_Arg14,SRP_Arg15,SRP_Arg16,SRP_Arg17);  \
        }\
    }

#define SRPCALL_RETEX18( SRP_Interface, SRP_Func,SRP_Ret,SRP_Default,SRP_Obj,SRP_Arg2,SRP_Arg3,SRP_Arg4,SRP_Arg5,SRP_Arg6,SRP_Arg7,SRP_Arg8,SRP_Arg9,SRP_Arg10,SRP_Arg11,SRP_Arg12,SRP_Arg13,SRP_Arg14,SRP_Arg15,SRP_Arg16,SRP_Arg17,SRP_Arg18) \
    { \
        SRP_Func##Proc LocalProc; \
        if( SRP_Obj != NULL ){ \
            LocalProc = (SRP_Func##Proc)SRP_Interface -> GetFunctionEx( SRP_Obj, &VSFUNCID_##SRP_Func ); \
            if( LocalProc == NULL ) \
                SRP_Ret = SRP_Default; \
            else    \
                SRP_Ret = LocalProc( SRP_Obj, SRP_Arg2, SRP_Arg3, SRP_Arg4, SRP_Arg5, SRP_Arg6, SRP_Arg7, SRP_Arg8, SRP_Arg9, SRP_Arg10, SRP_Arg11,SRP_Arg12,SRP_Arg13,SRP_Arg14,SRP_Arg15,SRP_Arg16,SRP_Arg17,SRP_Arg18);  \
        }\
    }

#define SRPCALL_RETEX19( SRP_Interface, SRP_Func,SRP_Ret,SRP_Default,SRP_Obj,SRP_Arg2,SRP_Arg3,SRP_Arg4,SRP_Arg5,SRP_Arg6,SRP_Arg7,SRP_Arg8,SRP_Arg9,SRP_Arg10,SRP_Arg11,SRP_Arg12,SRP_Arg13,SRP_Arg14,SRP_Arg15,SRP_Arg16,SRP_Arg17,SRP_Arg18,SRP_Arg19) \
    { \
        SRP_Func##Proc LocalProc; \
        if( SRP_Obj != NULL ){ \
            LocalProc = (SRP_Func##Proc)SRP_Interface -> GetFunctionEx( SRP_Obj, &VSFUNCID_##SRP_Func ); \
            if( LocalProc == NULL ) \
                SRP_Ret = SRP_Default; \
            else    \
                SRP_Ret = LocalProc( SRP_Obj, SRP_Arg2, SRP_Arg3, SRP_Arg4, SRP_Arg5, SRP_Arg6, SRP_Arg7, SRP_Arg8, SRP_Arg9, SRP_Arg10, SRP_Arg11,SRP_Arg12,SRP_Arg13,SRP_Arg14,SRP_Arg15,SRP_Arg16,SRP_Arg17,SRP_Arg18,SRP_Arg19);  \
        }\
    }

#define SRPCALL_RETEX20( SRP_Interface, SRP_Func,SRP_Ret,SRP_Default,SRP_Obj,SRP_Arg2,SRP_Arg3,SRP_Arg4,SRP_Arg5,SRP_Arg6,SRP_Arg7,SRP_Arg8,SRP_Arg9,SRP_Arg10,SRP_Arg11,SRP_Arg12,SRP_Arg13,SRP_Arg14,SRP_Arg15,SRP_Arg16,SRP_Arg17,SRP_Arg18,SRP_Arg19,SRP_Arg20) \
    { \
        SRP_Func##Proc LocalProc; \
        if( SRP_Obj != NULL ){ \
            LocalProc = (SRP_Func##Proc)SRP_Interface -> GetFunctionEx( SRP_Obj, &VSFUNCID_##SRP_Func ); \
            if( LocalProc == NULL ) \
                SRP_Ret = SRP_Default; \
            else    \
                SRP_Ret = LocalProc( SRP_Obj, SRP_Arg2, SRP_Arg3, SRP_Arg4, SRP_Arg5, SRP_Arg6, SRP_Arg7, SRP_Arg8, SRP_Arg9, SRP_Arg10, SRP_Arg11,SRP_Arg12,SRP_Arg13,SRP_Arg14,SRP_Arg15,SRP_Arg16,SRP_Arg17,SRP_Arg18,SRP_Arg19,SRP_Arg20);  \
        }\
    }

//-------------No args
#define SRPCALL1( SRP_Interface, SRP_Func,SRP_Obj)\
    { \
        SRP_Func##Proc LocalProc; \
        LocalProc = (SRP_Func##Proc)SRP_Interface -> GetFunction( &VSFUNCID_##SRP_Func ); \
        if( LocalProc != NULL ) \
            LocalProc( SRP_Obj );  \
    }
#define SRPCALL2( SRP_Interface, SRP_Func,SRP_Obj,SRP_Arg2)\
    { \
        SRP_Func##Proc LocalProc; \
        LocalProc = (SRP_Func##Proc)SRP_Interface -> GetFunction( &VSFUNCID_##SRP_Func ); \
        if( LocalProc != NULL ) \
            LocalProc( SRP_Obj, SRP_Arg2 );  \
    }
#define SRPCALL3( SRP_Interface, SRP_Func,SRP_Obj,SRP_Arg2,SRP_Arg3) \
    { \
        SRP_Func##Proc LocalProc; \
        LocalProc = (SRP_Func##Proc)SRP_Interface -> GetFunction( &VSFUNCID_##SRP_Func ); \
        if( LocalProc != NULL ) \
            LocalProc( SRP_Obj, SRP_Arg2, SRP_Arg3 );  \
    }
#define SRPCALL4( SRP_Interface, SRP_Func,SRP_Obj,SRP_Arg2,SRP_Arg3,SRP_Arg4) \
    { \
        SRP_Func##Proc LocalProc; \
        LocalProc = (SRP_Func##Proc)SRP_Interface -> GetFunction( &VSFUNCID_##SRP_Func ); \
        if( LocalProc != NULL ) \
            LocalProc( SRP_Obj, SRP_Arg2, SRP_Arg3, SRP_Arg4 );  \
    }
#define SRPCALL5( SRP_Interface, SRP_Func,SRP_Obj,SRP_Arg2,SRP_Arg3,SRP_Arg4,SRP_Arg5) \
    { \
        SRP_Func##Proc LocalProc; \
        LocalProc = (SRP_Func##Proc)SRP_Interface -> GetFunction( &VSFUNCID_##SRP_Func ); \
        if( LocalProc != NULL ) \
            LocalProc( SRP_Obj, SRP_Arg2, SRP_Arg3, SRP_Arg4, SRP_Arg5 );  \
    }
#define SRPCALL6( SRP_Interface, SRP_Func,SRP_Obj,SRP_Arg2,SRP_Arg3,SRP_Arg4,SRP_Arg5,SRP_Arg6) \
    { \
        SRP_Func##Proc LocalProc; \
        LocalProc = (SRP_Func##Proc)SRP_Interface -> GetFunction( &VSFUNCID_##SRP_Func ); \
        if( LocalProc != NULL ) \
            LocalProc( SRP_Obj, SRP_Arg2, SRP_Arg3, SRP_Arg4, SRP_Arg5, SRP_Arg6 );  \
    }
#define SRPCALL7( SRP_Interface, SRP_Func,SRP_Obj,SRP_Arg2,SRP_Arg3,SRP_Arg4,SRP_Arg5,SRP_Arg6,SRP_Arg7) \
    { \
        SRP_Func##Proc LocalProc; \
        LocalProc = (SRP_Func##Proc)SRP_Interface -> GetFunction( &VSFUNCID_##SRP_Func ); \
        if( LocalProc != NULL ) \
            LocalProc( SRP_Obj, SRP_Arg2, SRP_Arg3, SRP_Arg4, SRP_Arg5, SRP_Arg6, SRP_Arg7 );  \
    }
#define SRPCALL8( SRP_Interface, SRP_Func,SRP_Obj,SRP_Arg2,SRP_Arg3,SRP_Arg4,SRP_Arg5,SRP_Arg6,SRP_Arg7,SRP_Arg8) \
    { \
        SRP_Func##Proc LocalProc; \
        LocalProc = (SRP_Func##Proc)SRP_Interface -> GetFunction( &VSFUNCID_##SRP_Func ); \
        if( LocalProc != NULL ) \
            LocalProc( SRP_Obj, SRP_Arg2, SRP_Arg3, SRP_Arg4, SRP_Arg5, SRP_Arg6, SRP_Arg7, SRP_Arg8 );  \
    }
#define SRPCALL9( SRP_Interface, SRP_Func,SRP_Obj,SRP_Arg2,SRP_Arg3,SRP_Arg4,SRP_Arg5,SRP_Arg6,SRP_Arg7,SRP_Arg8,SRP_Arg9) \
    { \
        SRP_Func##Proc LocalProc; \
        LocalProc = (SRP_Func##Proc)SRP_Interface -> GetFunction( &VSFUNCID_##SRP_Func ); \
        if( LocalProc != NULL ) \
            LocalProc( SRP_Obj, SRP_Arg2, SRP_Arg3, SRP_Arg4, SRP_Arg5, SRP_Arg6, SRP_Arg7, SRP_Arg8, SRP_Arg9 );   \
    }
#define SRPCALL10( SRP_Interface, SRP_Func,SRP_Obj,SRP_Arg2,SRP_Arg3,SRP_Arg4,SRP_Arg5,SRP_Arg6,SRP_Arg7,SRP_Arg8,SRP_Arg9,SRP_Arg10) \
    { \
        SRP_Func##Proc LocalProc; \
        LocalProc = (SRP_Func##Proc)SRP_Interface -> GetFunction( &VSFUNCID_##SRP_Func ); \
        if( LocalProc != NULL ) \
            LocalProc( SRP_Obj, SRP_Arg2, SRP_Arg3, SRP_Arg4, SRP_Arg5, SRP_Arg6, SRP_Arg7, SRP_Arg8, SRP_Arg9, SRP_Arg10 );   \
    }
#define SRPCALL11( SRP_Interface, SRP_Func,SRP_Obj,SRP_Arg2,SRP_Arg3,SRP_Arg4,SRP_Arg5,SRP_Arg6,SRP_Arg7,SRP_Arg8,SRP_Arg9,SRP_Arg10,SRP_Arg11) \
    { \
        SRP_Func##Proc LocalProc; \
        LocalProc = (SRP_Func##Proc)SRP_Interface -> GetFunction( &VSFUNCID_##SRP_Func ); \
        if( LocalProc != NULL ) \
            LocalProc( SRP_Obj, SRP_Arg2, SRP_Arg3, SRP_Arg4, SRP_Arg5, SRP_Arg6, SRP_Arg7, SRP_Arg8, SRP_Arg9, SRP_Arg10,SRP_Arg11 );   \
    }
#define SRPCALL12( SRP_Interface, SRP_Func,SRP_Obj,SRP_Arg2,SRP_Arg3,SRP_Arg4,SRP_Arg5,SRP_Arg6,SRP_Arg7,SRP_Arg8,SRP_Arg9,SRP_Arg10,SRP_Arg11,SRP_Arg12) \
    { \
        SRP_Func##Proc LocalProc; \
        LocalProc = (SRP_Func##Proc)SRP_Interface -> GetFunction( &VSFUNCID_##SRP_Func ); \
        if( LocalProc != NULL ) \
            LocalProc( SRP_Obj, SRP_Arg2, SRP_Arg3, SRP_Arg4, SRP_Arg5, SRP_Arg6, SRP_Arg7, SRP_Arg8, SRP_Arg9, SRP_Arg10,SRP_Arg11,SRP_Arg12 );   \
    }
#define SRPCALL13( SRP_Interface, SRP_Func,SRP_Obj,SRP_Arg2,SRP_Arg3,SRP_Arg4,SRP_Arg5,SRP_Arg6,SRP_Arg7,SRP_Arg8,SRP_Arg9,SRP_Arg10,SRP_Arg11,SRP_Arg12,SRP_Arg13) \
    { \
        SRP_Func##Proc LocalProc; \
        LocalProc = (SRP_Func##Proc)SRP_Interface -> GetFunction( &VSFUNCID_##SRP_Func ); \
        if( LocalProc != NULL ) \
            LocalProc( SRP_Obj, SRP_Arg2, SRP_Arg3, SRP_Arg4, SRP_Arg5, SRP_Arg6, SRP_Arg7, SRP_Arg8, SRP_Arg9, SRP_Arg10,SRP_Arg11,SRP_Arg12,SRP_Arg13 );   \
    }
#define SRPCALL14( SRP_Interface, SRP_Func,SRP_Obj,SRP_Arg2,SRP_Arg3,SRP_Arg4,SRP_Arg5,SRP_Arg6,SRP_Arg7,SRP_Arg8,SRP_Arg9,SRP_Arg10,SRP_Arg11,SRP_Arg12,SRP_Arg13,SRP_Arg14) \
    { \
        SRP_Func##Proc LocalProc; \
        LocalProc = (SRP_Func##Proc)SRP_Interface -> GetFunction( &VSFUNCID_##SRP_Func ); \
        if( LocalProc != NULL ) \
            LocalProc( SRP_Obj, SRP_Arg2, SRP_Arg3, SRP_Arg4, SRP_Arg5, SRP_Arg6, SRP_Arg7, SRP_Arg8, SRP_Arg9, SRP_Arg10,SRP_Arg11,SRP_Arg12,SRP_Arg13,SRP_Arg14 );   \
    }
#define SRPCALL15( SRP_Interface, SRP_Func,SRP_Obj,SRP_Arg2,SRP_Arg3,SRP_Arg4,SRP_Arg5,SRP_Arg6,SRP_Arg7,SRP_Arg8,SRP_Arg9,SRP_Arg10,SRP_Arg11,SRP_Arg12,SRP_Arg13,SRP_Arg14,SRP_Arg15) \
    { \
        SRP_Func##Proc LocalProc; \
        LocalProc = (SRP_Func##Proc)SRP_Interface -> GetFunction( &VSFUNCID_##SRP_Func ); \
        if( LocalProc != NULL ) \
            LocalProc( SRP_Obj, SRP_Arg2, SRP_Arg3, SRP_Arg4, SRP_Arg5, SRP_Arg6, SRP_Arg7, SRP_Arg8, SRP_Arg9, SRP_Arg10,SRP_Arg11,SRP_Arg12,SRP_Arg13,SRP_Arg14,SRP_Arg15 );   \
    }
#define SRPCALL16( SRP_Interface, SRP_Func,SRP_Obj,SRP_Arg2,SRP_Arg3,SRP_Arg4,SRP_Arg5,SRP_Arg6,SRP_Arg7,SRP_Arg8,SRP_Arg9,SRP_Arg10,SRP_Arg11,SRP_Arg12,SRP_Arg13,SRP_Arg14,SRP_Arg15,SRP_Arg16) \
    { \
        SRP_Func##Proc LocalProc; \
        LocalProc = (SRP_Func##Proc)SRP_Interface -> GetFunction( &VSFUNCID_##SRP_Func ); \
        if( LocalProc != NULL ) \
            LocalProc( SRP_Obj, SRP_Arg2, SRP_Arg3, SRP_Arg4, SRP_Arg5, SRP_Arg6, SRP_Arg7, SRP_Arg8, SRP_Arg9, SRP_Arg10,SRP_Arg11,SRP_Arg12,SRP_Arg13,SRP_Arg14,SRP_Arg15,SRP_Arg16 );   \
    }

#define SRPCALL17( SRP_Interface, SRP_Func,SRP_Obj,SRP_Arg2,SRP_Arg3,SRP_Arg4,SRP_Arg5,SRP_Arg6,SRP_Arg7,SRP_Arg8,SRP_Arg9,SRP_Arg10,SRP_Arg11,SRP_Arg12,SRP_Arg13,SRP_Arg14,SRP_Arg15,SRP_Arg16,SRP_Arg17) \
    { \
        SRP_Func##Proc LocalProc; \
        LocalProc = (SRP_Func##Proc)SRP_Interface -> GetFunction( &VSFUNCID_##SRP_Func ); \
        if( LocalProc != NULL ) \
            LocalProc( SRP_Obj, SRP_Arg2, SRP_Arg3, SRP_Arg4, SRP_Arg5, SRP_Arg6, SRP_Arg7, SRP_Arg8, SRP_Arg9, SRP_Arg10,SRP_Arg11,SRP_Arg12,SRP_Arg13,SRP_Arg14,SRP_Arg15,SRP_Arg16,SRP_Arg17 );   \
    }
#define SRPCALL18( SRP_Interface, SRP_Func,SRP_Obj,SRP_Arg2,SRP_Arg3,SRP_Arg4,SRP_Arg5,SRP_Arg6,SRP_Arg7,SRP_Arg8,SRP_Arg9,SRP_Arg10,SRP_Arg11,SRP_Arg12,SRP_Arg13,SRP_Arg14,SRP_Arg15,SRP_Arg16,SRP_Arg17,SRP_Arg18) \
    { \
        SRP_Func##Proc LocalProc; \
        LocalProc = (SRP_Func##Proc)SRP_Interface -> GetFunction( &VSFUNCID_##SRP_Func ); \
        if( LocalProc != NULL ) \
            LocalProc( SRP_Obj, SRP_Arg2, SRP_Arg3, SRP_Arg4, SRP_Arg5, SRP_Arg6, SRP_Arg7, SRP_Arg8, SRP_Arg9, SRP_Arg10,SRP_Arg11,SRP_Arg12,SRP_Arg13,SRP_Arg14,SRP_Arg15,SRP_Arg16,SRP_Arg17,SRP_Arg18 );   \
    }
#define SRPCALL19( SRP_Interface, SRP_Func,SRP_Obj,SRP_Arg2,SRP_Arg3,SRP_Arg4,SRP_Arg5,SRP_Arg6,SRP_Arg7,SRP_Arg8,SRP_Arg9,SRP_Arg10,SRP_Arg11,SRP_Arg12,SRP_Arg13,SRP_Arg14,SRP_Arg15,SRP_Arg16,SRP_Arg17,SRP_Arg18,SRP_Arg19) \
    { \
        SRP_Func##Proc LocalProc; \
        LocalProc = (SRP_Func##Proc)SRP_Interface -> GetFunction( &VSFUNCID_##SRP_Func ); \
        if( LocalProc != NULL ) \
            LocalProc( SRP_Obj, SRP_Arg2, SRP_Arg3, SRP_Arg4, SRP_Arg5, SRP_Arg6, SRP_Arg7, SRP_Arg8, SRP_Arg9, SRP_Arg10,SRP_Arg11,SRP_Arg12,SRP_Arg13,SRP_Arg14,SRP_Arg15,SRP_Arg16,SRP_Arg17,SRP_Arg18,SRP_Arg19 );   \
    }
#define SRPCALL20( SRP_Interface, SRP_Func,SRP_Obj,SRP_Arg2,SRP_Arg3,SRP_Arg4,SRP_Arg5,SRP_Arg6,SRP_Arg7,SRP_Arg8,SRP_Arg9,SRP_Arg10,SRP_Arg11,SRP_Arg12,SRP_Arg13,SRP_Arg14,SRP_Arg15,SRP_Arg16,SRP_Arg17,SRP_Arg18,SRP_Arg19,SRP_Arg20) \
    { \
        SRP_Func##Proc LocalProc; \
        LocalProc = (SRP_Func##Proc)SRP_Interface -> GetFunction( &VSFUNCID_##SRP_Func ); \
        if( LocalProc != NULL ) \
            LocalProc( SRP_Obj, SRP_Arg2, SRP_Arg3, SRP_Arg4, SRP_Arg5, SRP_Arg6, SRP_Arg7, SRP_Arg8, SRP_Arg9, SRP_Arg10,SRP_Arg11,SRP_Arg12,SRP_Arg13,SRP_Arg14,SRP_Arg15,SRP_Arg16,SRP_Arg17,SRP_Arg18,SRP_Arg19,SRP_Arg20 );   \
    }

//+++++++++++++++
#define SRPCALLEX1( SRP_Interface, SRP_Func,SRP_Obj)\
    { \
        SRP_Func##Proc LocalProc; \
        if( SRP_Obj != NULL ){ \
            LocalProc = (SRP_Func##Proc)SRP_Interface -> GetFunctionEx( SRP_Obj, &VSFUNCID_##SRP_Func ); \
            if( LocalProc != NULL ) \
                LocalProc( SRP_Obj );  \
        }\
    }
#define SRPCALLEX2( SRP_Interface, SRP_Func,SRP_Obj,SRP_Arg2)\
    { \
        SRP_Func##Proc LocalProc; \
        if( SRP_Obj != NULL ){ \
            LocalProc = (SRP_Func##Proc)SRP_Interface -> GetFunctionEx( SRP_Obj, &VSFUNCID_##SRP_Func ); \
            if( LocalProc != NULL ) \
                LocalProc( SRP_Obj, SRP_Arg2 );  \
        }\
    }
#define SRPCALLEX3( SRP_Interface, SRP_Func,SRP_Obj,SRP_Arg2,SRP_Arg3) \
    { \
        SRP_Func##Proc LocalProc; \
        if( SRP_Obj != NULL ){ \
            LocalProc = (SRP_Func##Proc)SRP_Interface -> GetFunctionEx( SRP_Obj, &VSFUNCID_##SRP_Func ); \
            if( LocalProc != NULL ) \
                LocalProc( SRP_Obj, SRP_Arg2, SRP_Arg3 );  \
        }\
    }
#define SRPCALLEX4( SRP_Interface, SRP_Func,SRP_Obj,SRP_Arg2,SRP_Arg3,SRP_Arg4) \
    { \
        SRP_Func##Proc LocalProc; \
        if( SRP_Obj != NULL ){ \
            LocalProc = (SRP_Func##Proc)SRP_Interface -> GetFunctionEx( SRP_Obj, &VSFUNCID_##SRP_Func ); \
            if( LocalProc != NULL ) \
                LocalProc( SRP_Obj, SRP_Arg2, SRP_Arg3, SRP_Arg4 );  \
        }\
    }
#define SRPCALLEX5( SRP_Interface, SRP_Func,SRP_Obj,SRP_Arg2,SRP_Arg3,SRP_Arg4,SRP_Arg5) \
    { \
        SRP_Func##Proc LocalProc; \
        if( SRP_Obj != NULL ){ \
            LocalProc = (SRP_Func##Proc)SRP_Interface -> GetFunctionEx( SRP_Obj, &VSFUNCID_##SRP_Func ); \
            if( LocalProc != NULL ) \
                LocalProc( SRP_Obj, SRP_Arg2, SRP_Arg3, SRP_Arg4, SRP_Arg5 );  \
        }\
    }
#define SRPCALLEX6( SRP_Interface, SRP_Func,SRP_Obj,SRP_Arg2,SRP_Arg3,SRP_Arg4,SRP_Arg5,SRP_Arg6) \
    { \
        SRP_Func##Proc LocalProc; \
        if( SRP_Obj != NULL ){ \
            LocalProc = (SRP_Func##Proc)SRP_Interface -> GetFunctionEx( SRP_Obj, &VSFUNCID_##SRP_Func ); \
            if( LocalProc != NULL ) \
                LocalProc( SRP_Obj, SRP_Arg2, SRP_Arg3, SRP_Arg4, SRP_Arg5, SRP_Arg6 );  \
        }\
    }
#define SRPCALLEX7( SRP_Interface, SRP_Func,SRP_Obj,SRP_Arg2,SRP_Arg3,SRP_Arg4,SRP_Arg5,SRP_Arg6,SRP_Arg7) \
    { \
        SRP_Func##Proc LocalProc; \
        if( SRP_Obj != NULL ){ \
            LocalProc = (SRP_Func##Proc)SRP_Interface -> GetFunctionEx( SRP_Obj, &VSFUNCID_##SRP_Func ); \
            if( LocalProc != NULL ) \
                LocalProc( SRP_Obj, SRP_Arg2, SRP_Arg3, SRP_Arg4, SRP_Arg5, SRP_Arg6, SRP_Arg7 );  \
        }\
    }
#define SRPCALLEX8( SRP_Interface, SRP_Func,SRP_Obj,SRP_Arg2,SRP_Arg3,SRP_Arg4,SRP_Arg5,SRP_Arg6,SRP_Arg7,SRP_Arg8) \
    { \
        SRP_Func##Proc LocalProc; \
        if( SRP_Obj != NULL ){ \
            LocalProc = (SRP_Func##Proc)SRP_Interface -> GetFunctionEx( SRP_Obj, &VSFUNCID_##SRP_Func ); \
            if( LocalProc != NULL ) \
                LocalProc( SRP_Obj, SRP_Arg2, SRP_Arg3, SRP_Arg4, SRP_Arg5, SRP_Arg6, SRP_Arg7, SRP_Arg8 );  \
        }\
    }
#define SRPCALLEX9( SRP_Interface, SRP_Func,SRP_Obj,SRP_Arg2,SRP_Arg3,SRP_Arg4,SRP_Arg5,SRP_Arg6,SRP_Arg7,SRP_Arg8,SRP_Arg9) \
    { \
        SRP_Func##Proc LocalProc; \
        if( SRP_Obj != NULL ){ \
            LocalProc = (SRP_Func##Proc)SRP_Interface -> GetFunctionEx( SRP_Obj, &VSFUNCID_##SRP_Func ); \
            if( LocalProc != NULL ) \
                LocalProc( SRP_Obj, SRP_Arg2, SRP_Arg3, SRP_Arg4, SRP_Arg5, SRP_Arg6, SRP_Arg7, SRP_Arg8, SRP_Arg9 );   \
        }\
    }
#define SRPCALLEX10( SRP_Interface, SRP_Func,SRP_Obj,SRP_Arg2,SRP_Arg3,SRP_Arg4,SRP_Arg5,SRP_Arg6,SRP_Arg7,SRP_Arg8,SRP_Arg9,SRP_Arg10) \
    { \
        SRP_Func##Proc LocalProc; \
        if( SRP_Obj != NULL ){ \
            LocalProc = (SRP_Func##Proc)SRP_Interface -> GetFunctionEx( SRP_Obj, &VSFUNCID_##SRP_Func ); \
            if( LocalProc != NULL ) \
                LocalProc( SRP_Obj, SRP_Arg2, SRP_Arg3, SRP_Arg4, SRP_Arg5, SRP_Arg6, SRP_Arg7, SRP_Arg8, SRP_Arg9, SRP_Arg10 );   \
        }\
    }
#define SRPCALLEX11( SRP_Interface, SRP_Func,SRP_Obj,SRP_Arg2,SRP_Arg3,SRP_Arg4,SRP_Arg5,SRP_Arg6,SRP_Arg7,SRP_Arg8,SRP_Arg9,SRP_Arg10,SRP_Arg11) \
    { \
        SRP_Func##Proc LocalProc; \
        if( SRP_Obj != NULL ){ \
            LocalProc = (SRP_Func##Proc)SRP_Interface -> GetFunctionEx( SRP_Obj, &VSFUNCID_##SRP_Func ); \
            if( LocalProc != NULL ) \
                LocalProc( SRP_Obj, SRP_Arg2, SRP_Arg3, SRP_Arg4, SRP_Arg5, SRP_Arg6, SRP_Arg7, SRP_Arg8, SRP_Arg9, SRP_Arg10,SRP_Arg11 );   \
        }\
    }
#define SRPCALLEX12( SRP_Interface, SRP_Func,SRP_Obj,SRP_Arg2,SRP_Arg3,SRP_Arg4,SRP_Arg5,SRP_Arg6,SRP_Arg7,SRP_Arg8,SRP_Arg9,SRP_Arg10,SRP_Arg11,SRP_Arg12) \
    { \
        SRP_Func##Proc LocalProc; \
        if( SRP_Obj != NULL ){ \
            LocalProc = (SRP_Func##Proc)SRP_Interface -> GetFunctionEx( SRP_Obj, &VSFUNCID_##SRP_Func ); \
            if( LocalProc != NULL ) \
                LocalProc( SRP_Obj, SRP_Arg2, SRP_Arg3, SRP_Arg4, SRP_Arg5, SRP_Arg6, SRP_Arg7, SRP_Arg8, SRP_Arg9, SRP_Arg10,SRP_Arg11,SRP_Arg12 );   \
        }\
    }
#define SRPCALLEX13( SRP_Interface, SRP_Func,SRP_Obj,SRP_Arg2,SRP_Arg3,SRP_Arg4,SRP_Arg5,SRP_Arg6,SRP_Arg7,SRP_Arg8,SRP_Arg9,SRP_Arg10,SRP_Arg11,SRP_Arg12,SRP_Arg13) \
    { \
        SRP_Func##Proc LocalProc; \
        if( SRP_Obj != NULL ){ \
            LocalProc = (SRP_Func##Proc)SRP_Interface -> GetFunctionEx( SRP_Obj, &VSFUNCID_##SRP_Func ); \
            if( LocalProc != NULL ) \
                LocalProc( SRP_Obj, SRP_Arg2, SRP_Arg3, SRP_Arg4, SRP_Arg5, SRP_Arg6, SRP_Arg7, SRP_Arg8, SRP_Arg9, SRP_Arg10,SRP_Arg11,SRP_Arg12,SRP_Arg13 );   \
        }\
    }
#define SRPCALLEX14( SRP_Interface, SRP_Func,SRP_Obj,SRP_Arg2,SRP_Arg3,SRP_Arg4,SRP_Arg5,SRP_Arg6,SRP_Arg7,SRP_Arg8,SRP_Arg9,SRP_Arg10,SRP_Arg11,SRP_Arg12,SRP_Arg13,SRP_Arg14) \
    { \
        SRP_Func##Proc LocalProc; \
        if( SRP_Obj != NULL ){ \
            LocalProc = (SRP_Func##Proc)SRP_Interface -> GetFunctionEx( SRP_Obj, &VSFUNCID_##SRP_Func ); \
            if( LocalProc != NULL ) \
                LocalProc( SRP_Obj, SRP_Arg2, SRP_Arg3, SRP_Arg4, SRP_Arg5, SRP_Arg6, SRP_Arg7, SRP_Arg8, SRP_Arg9, SRP_Arg10,SRP_Arg11,SRP_Arg12,SRP_Arg13,SRP_Arg14 );   \
        }\
    }
#define SRPCALLEX15( SRP_Interface, SRP_Func,SRP_Obj,SRP_Arg2,SRP_Arg3,SRP_Arg4,SRP_Arg5,SRP_Arg6,SRP_Arg7,SRP_Arg8,SRP_Arg9,SRP_Arg10,SRP_Arg11,SRP_Arg12,SRP_Arg13,SRP_Arg14,SRP_Arg15) \
    { \
        SRP_Func##Proc LocalProc; \
        if( SRP_Obj != NULL ){ \
            LocalProc = (SRP_Func##Proc)SRP_Interface -> GetFunctionEx( SRP_Obj, &VSFUNCID_##SRP_Func ); \
            if( LocalProc != NULL ) \
                LocalProc( SRP_Obj, SRP_Arg2, SRP_Arg3, SRP_Arg4, SRP_Arg5, SRP_Arg6, SRP_Arg7, SRP_Arg8, SRP_Arg9, SRP_Arg10,SRP_Arg11,SRP_Arg12,SRP_Arg13,SRP_Arg14,SRP_Arg15 );   \
        }\
    }
#define SRPCALLEX16( SRP_Interface, SRP_Func,SRP_Obj,SRP_Arg2,SRP_Arg3,SRP_Arg4,SRP_Arg5,SRP_Arg6,SRP_Arg7,SRP_Arg8,SRP_Arg9,SRP_Arg10,SRP_Arg11,SRP_Arg12,SRP_Arg13,SRP_Arg14,SRP_Arg15,SRP_Arg16) \
    { \
        SRP_Func##Proc LocalProc; \
        if( SRP_Obj != NULL ){ \
            LocalProc = (SRP_Func##Proc)SRP_Interface -> GetFunctionEx( SRP_Obj, &VSFUNCID_##SRP_Func ); \
            if( LocalProc != NULL ) \
                LocalProc( SRP_Obj, SRP_Arg2, SRP_Arg3, SRP_Arg4, SRP_Arg5, SRP_Arg6, SRP_Arg7, SRP_Arg8, SRP_Arg9, SRP_Arg10,SRP_Arg11,SRP_Arg12,SRP_Arg13,SRP_Arg14,SRP_Arg15,SRP_Arg16 );   \
        }\
    }
#define SRPCALLEX17( SRP_Interface, SRP_Func,SRP_Obj,SRP_Arg2,SRP_Arg3,SRP_Arg4,SRP_Arg5,SRP_Arg6,SRP_Arg7,SRP_Arg8,SRP_Arg9,SRP_Arg10,SRP_Arg11,SRP_Arg12,SRP_Arg13,SRP_Arg14,SRP_Arg15,SRP_Arg16,SRP_Arg17) \
    { \
        SRP_Func##Proc LocalProc; \
        if( SRP_Obj != NULL ){ \
            LocalProc = (SRP_Func##Proc)SRP_Interface -> GetFunctionEx( SRP_Obj, &VSFUNCID_##SRP_Func ); \
            if( LocalProc != NULL ) \
                LocalProc( SRP_Obj, SRP_Arg2, SRP_Arg3, SRP_Arg4, SRP_Arg5, SRP_Arg6, SRP_Arg7, SRP_Arg8, SRP_Arg9, SRP_Arg10,SRP_Arg11,SRP_Arg12,SRP_Arg13,SRP_Arg14,SRP_Arg15,SRP_Arg16,SRP_Arg17 );   \
        }\
    }
#define SRPCALLEX18( SRP_Interface, SRP_Func,SRP_Obj,SRP_Arg2,SRP_Arg3,SRP_Arg4,SRP_Arg5,SRP_Arg6,SRP_Arg7,SRP_Arg8,SRP_Arg9,SRP_Arg10,SRP_Arg11,SRP_Arg12,SRP_Arg13,SRP_Arg14,SRP_Arg15,SRP_Arg16,SRP_Arg17,SRP_Arg18) \
    { \
        SRP_Func##Proc LocalProc; \
        if( SRP_Obj != NULL ){ \
            LocalProc = (SRP_Func##Proc)SRP_Interface -> GetFunctionEx( SRP_Obj, &VSFUNCID_##SRP_Func ); \
            if( LocalProc != NULL ) \
                LocalProc( SRP_Obj, SRP_Arg2, SRP_Arg3, SRP_Arg4, SRP_Arg5, SRP_Arg6, SRP_Arg7, SRP_Arg8, SRP_Arg9, SRP_Arg10,SRP_Arg11,SRP_Arg12,SRP_Arg13,SRP_Arg14,SRP_Arg15,SRP_Arg16,SRP_Arg17,SRP_Arg18 );   \
        }\
    }
#define SRPCALLEX19( SRP_Interface, SRP_Func,SRP_Obj,SRP_Arg2,SRP_Arg3,SRP_Arg4,SRP_Arg5,SRP_Arg6,SRP_Arg7,SRP_Arg8,SRP_Arg9,SRP_Arg10,SRP_Arg11,SRP_Arg12,SRP_Arg13,SRP_Arg14,SRP_Arg15,SRP_Arg16,SRP_Arg17,SRP_Arg18,SRP_Arg19) \
    { \
        SRP_Func##Proc LocalProc; \
        if( SRP_Obj != NULL ){ \
            LocalProc = (SRP_Func##Proc)SRP_Interface -> GetFunctionEx( SRP_Obj, &VSFUNCID_##SRP_Func ); \
            if( LocalProc != NULL ) \
                LocalProc( SRP_Obj, SRP_Arg2, SRP_Arg3, SRP_Arg4, SRP_Arg5, SRP_Arg6, SRP_Arg7, SRP_Arg8, SRP_Arg9, SRP_Arg10,SRP_Arg11,SRP_Arg12,SRP_Arg13,SRP_Arg14,SRP_Arg15,SRP_Arg16,SRP_Arg17,SRP_Arg18,SRP_Arg19 );   \
        }\
    }
#define SRPCALLEX20( SRP_Interface, SRP_Func,SRP_Obj,SRP_Arg2,SRP_Arg3,SRP_Arg4,SRP_Arg5,SRP_Arg6,SRP_Arg7,SRP_Arg8,SRP_Arg9,SRP_Arg10,SRP_Arg11,SRP_Arg12,SRP_Arg13,SRP_Arg14,SRP_Arg15,SRP_Arg16,SRP_Arg17,SRP_Arg18,SRP_Arg19,SRP_Arg20) \
    { \
        SRP_Func##Proc LocalProc; \
        if( SRP_Obj != NULL ){ \
            LocalProc = (SRP_Func##Proc)SRP_Interface -> GetFunctionEx( SRP_Obj, &VSFUNCID_##SRP_Func ); \
            if( LocalProc != NULL ) \
                LocalProc( SRP_Obj, SRP_Arg2, SRP_Arg3, SRP_Arg4, SRP_Arg5, SRP_Arg6, SRP_Arg7, SRP_Arg8, SRP_Arg9, SRP_Arg10,SRP_Arg11,SRP_Arg12,SRP_Arg13,SRP_Arg14,SRP_Arg15,SRP_Arg16,SRP_Arg17,SRP_Arg18,SRP_Arg19,SRP_Arg20 );   \
        }\
    }

//------------------------------------------------------------------------------
#define SRPCHANGE_BOOL( SRP_Interface, Object, SRP_VarIndex, SRP_Value ) \
    {   \
        VS_BOOL SRP_ValueTemp; \
        SRP_ValueTemp = (VS_BOOL)SRP_Value;\
        SRP_Interface -> ChangeObject(Object,SRP_VarIndex,(VS_INT8 *)&SRP_ValueTemp);   \
    }
#define SRPCHANGE_CHAR( SRP_Interface, Object, SRP_VarIndex, SRP_Value ) \
    {   \
        VS_INT8 SRP_ValueTemp; \
        SRP_ValueTemp = (VS_INT8)SRP_Value;\
        SRP_Interface -> ChangeObject(Object,SRP_VarIndex,(VS_INT8 *)&SRP_ValueTemp);  \
    }
#define SRPCHANGE_SHORT( SRP_Interface, Object, SRP_VarIndex, SRP_Value ) \
    {   \
        VS_INT16 SRP_ValueTemp; \
        SRP_ValueTemp = (VS_INT16)SRP_Value;\
        SRP_Interface -> ChangeObject(Object,SRP_VarIndex,(VS_INT8 *)&SRP_ValueTemp);  \
    }
#define SRPCHANGE_INT( SRP_Interface, Object, SRP_VarIndex, SRP_Value ) \
    {   \
        VS_INT32 SRP_ValueTemp; \
        SRP_ValueTemp = (VS_INT32)SRP_Value;\
        SRP_Interface -> ChangeObject(Object,SRP_VarIndex,(VS_INT8 *)&SRP_ValueTemp);  \
    }
#define SRPCHANGE_FLOAT( SRP_Interface, Object, SRP_VarIndex, SRP_Value ) \
    {   \
        VS_FLOAT SRP_ValueTemp; \
        SRP_ValueTemp = (VS_FLOAT)SRP_Value;\
        SRP_Interface -> ChangeObject(Object,SRP_VarIndex,(VS_INT8 *)&SRP_ValueTemp);  \
    }
//----------------
#define SRPCHANGEEX_BOOL( SRP_Interface, Object, SRP_VarIndex, SRP_Value ) \
    {   \
        VS_BOOL SRP_ValueTemp; \
        SRP_ValueTemp = (VS_BOOL)SRP_Value;\
        SRP_Interface -> ChangeGlobal(Object,SRP_VarIndex,(VS_INT8 *)&SRP_ValueTemp);    \
    }
#define SRPCHANGEEX_CHAR( SRP_Interface, Object, SRP_VarIndex, SRP_Value ) \
    {   \
        VS_INT8 SRP_ValueTemp; \
        SRP_ValueTemp = (VS_INT8)SRP_Value;\
        SRP_Interface -> ChangeGlobal(Object,SRP_VarIndex,(VS_INT8 *)&SRP_ValueTemp);   \
    }
#define SRPCHANGEEX_SHORT( SRP_Interface, Object, SRP_VarIndex, SRP_Value ) \
    {   \
        VS_INT16 SRP_ValueTemp; \
        SRP_ValueTemp = (VS_INT16)SRP_Value;\
        SRP_Interface -> ChangeGlobal(Object,SRP_VarIndex,(VS_INT8 *)&SRP_ValueTemp);   \
    }
#define SRPCHANGEEX_INT( SRP_Interface, Object, SRP_VarIndex, SRP_Value ) \
    {   \
        VS_INT32 SRP_ValueTemp; \
        SRP_ValueTemp = (VS_INT32)SRP_Value;\
        SRP_Interface -> ChangeGlobal(Object,SRP_VarIndex,(VS_INT8 *)&SRP_ValueTemp);  \
    }
#define SRPCHANGEEX_FLOAT( SRP_Interface, Object, SRP_VarIndex, SRP_Value ) \
    {   \
        VS_FLOAT SRP_ValueTemp; \
        SRP_ValueTemp = (VS_FLOAT)SRP_Value;\
        SRP_Interface -> ChangeGlobal(Object,SRP_VarIndex,(VS_INT8 *)&SRP_ValueTemp);  \
    }

//==============================================================================
#ifdef _INCLUDE_STARLIB
class SRPDLLEXPORT ClassOfSRPObject{
private:
    VS_BOOL WrapObjectFlag;
    VS_ULONG ThisSRPClassLayer;
    void ReleaseAttachClass( void *SRPObject,VS_ULONG ClassLayer);
    class ClassOfSRPObject *SRPObjectWrap(void *SROObject);

public:
    void *ThisSRPObject;
    class ClassOfSRPInterface *ThisSRPInterface;
    VS_BOOL AutoReleaseObject;
    VS_BOOL AutoReleaseThis;

public:
    //--SysEvent Get/Put Function Define
    VSSystemEvent_EventProc Get_E_OnFirstCreate();
    void Put_E_OnFirstCreate(VSSystemEvent_EventProc In_Value);

    VSSystemEvent_EventProc Get_E_OnMalloc();
    void Put_E_OnMalloc(VSSystemEvent_EventProc In_Value);

    VSSystemEvent_EventProc Get_E_OnFree();
    void Put_E_OnFree(VSSystemEvent_EventProc In_Value);

    VSSystemEvent_EventProc Get_E_OnCreate();
    void Put_E_OnCreate(VSSystemEvent_EventProc In_Value);

    VSSystemEvent_EventProc Get_E_OnDestory();
    void Put_E_OnDestory(VSSystemEvent_EventProc In_Value);

    VSSystemEvent_EventProc Get_E_OnBeforeFirstCreate();
    void Put_E_OnBeforeFirstCreate(VSSystemEvent_EventProc In_Value);

    VSSystemEvent_EventProc Get_E_OnCreateChild();
    void Put_E_OnCreateChild(VSSystemEvent_EventProc In_Value);

    VSSystemEvent_EventProc Get_E_OnDestoryChild();
    void Put_E_OnDestoryChild(VSSystemEvent_EventProc In_Value);

    VSSystemEvent_EventProc Get_E_OnActivating();
    void Put_E_OnActivating(VSSystemEvent_EventProc In_Value);

    VSSystemEvent_EventProc Get_E_OnDeactivating();
    void Put_E_OnDeactivating(VSSystemEvent_EventProc In_Value);

    VSSystemEvent_EventProc Get_E_OnActivate();
    void Put_E_OnActivate(VSSystemEvent_EventProc In_Value);

    VSSystemEvent_EventProc Get_E_OnDeactivate();
    void Put_E_OnDeactivate(VSSystemEvent_EventProc In_Value);

    VSSystemEvent_EventProc Get_E_OnActivateChild();
    void Put_E_OnActivateChild(VSSystemEvent_EventProc In_Value);

    VSSystemEvent_EventProc Get_E_OnDeactivateChild();
    void Put_E_OnDeactivateChild(VSSystemEvent_EventProc In_Value);

    VSSystemEvent_EventProc Get_E_OnAttributeBeforeChange();
    void Put_E_OnAttributeBeforeChange(VSSystemEvent_EventProc In_Value);

    VSSystemEvent_EventProc Get_E_OnAttributeChange();
    void Put_E_OnAttributeChange(VSSystemEvent_EventProc In_Value);

    VSSystemEvent_EventProc Get_E_OnParentBeforeChange();
    void Put_E_OnParentBeforeChange(VSSystemEvent_EventProc In_Value);

    VSSystemEvent_EventProc Get_E_OnParentChange();
    void Put_E_OnParentChange(VSSystemEvent_EventProc In_Value);

    VSSystemEvent_EventProc Get_E_OnStaticChange();
    void Put_E_OnStaticChange(VSSystemEvent_EventProc In_Value);

    VSSystemEvent_EventProc Get_E_OnScriptChange();
    void Put_E_OnScriptChange(VSSystemEvent_EventProc In_Value);

    VSSystemEvent_EventProc Get_E_OnBecomeSync();
    void Put_E_OnBecomeSync(VSSystemEvent_EventProc In_Value);

    VSSystemEvent_EventProc Get_E_OnSyncGroupChange();
    void Put_E_OnSyncGroupChange(VSSystemEvent_EventProc In_Value);

    VSSystemEvent_EventProc Get_E_OnActiveSetChange();
    void Put_E_OnActiveSetChange(VSSystemEvent_EventProc In_Value);

    VSSystemEvent_EventProc Get_E_OnChildSyncGroupChange();
    void Put_E_OnChildSyncGroupChange(VSSystemEvent_EventProc In_Value);

    VSSystemEvent_EventProc Get_E_OnLoadMask();
    void Put_E_OnLoadMask(VSSystemEvent_EventProc In_Value);

    VSSystemEvent_EventProc Get_E_OnLoadFinish();
    void Put_E_OnLoadFinish(VSSystemEvent_EventProc In_Value);

    VSSystemEvent_EventProc Get_E_OnRemoteSend();
    void Put_E_OnRemoteSend(VSSystemEvent_EventProc In_Value);

    VSSystemEvent_EventProc Get_E_OnCall();
    void Put_E_OnCall(VSSystemEvent_EventProc In_Value);

#if( VS_OS_TYPE == VS_OS_WINDOW )
public:
    __declspec(property(get=Get_E_OnFirstCreate, put=Put_E_OnFirstCreate)) VSSystemEvent_EventProc E_OnFirstCreate;
    __declspec(property(get=Get_E_OnMalloc, put=Put_E_OnMalloc)) VSSystemEvent_EventProc E_OnOnMalloc;
    __declspec(property(get=Get_E_OnFree, put=Put_E_OnFree)) VSSystemEvent_EventProc E_OnFree;
    __declspec(property(get=Get_E_OnCreate, put=Put_E_OnCreate)) VSSystemEvent_EventProc E_OnCreate;
    __declspec(property(get=Get_E_OnDestory, put=Put_E_OnDestory)) VSSystemEvent_EventProc E_OnDestory;
    __declspec(property(get=Get_E_OnBeforeFirstCreate, put=Put_E_OnBeforeFirstCreate)) VSSystemEvent_EventProc E_OnBeforeFirstCreate;
    __declspec(property(get=Get_E_OnCreateChild, put=Put_E_OnCreateChild)) VSSystemEvent_EventProc E_OnCreateChild;
    __declspec(property(get=Get_E_OnDestoryChild, put=Put_E_OnDestoryChild)) VSSystemEvent_EventProc E_OnDestoryChild;
    __declspec(property(get=Get_E_OnActivating, put=Put_E_OnActivating)) VSSystemEvent_EventProc E_OnActivating;
    __declspec(property(get=Get_E_OnDeactivating, put=Put_E_OnDeactivating)) VSSystemEvent_EventProc E_OnDeactivating;
    __declspec(property(get=Get_E_OnActivate, put=Put_E_OnActivate)) VSSystemEvent_EventProc E_OnActivate;
    __declspec(property(get=Get_E_OnDeactivate, put=Put_E_OnDeactivate)) VSSystemEvent_EventProc E_OnDeactivate;
    __declspec(property(get=Get_E_OnActivateChild, put=Put_E_OnActivateChild)) VSSystemEvent_EventProc E_OnActivateChild;
    __declspec(property(get=Get_E_OnDeactivateChild, put=Put_E_OnDeactivateChild)) VSSystemEvent_EventProc E_OnDeactivateChild;
    __declspec(property(get=Get_E_OnAttributeBeforeChange, put=Put_E_OnAttributeBeforeChange)) VSSystemEvent_EventProc E_OnAttributeBeforeChange;
    __declspec(property(get=Get_E_OnAttributeChange, put=Put_E_OnAttributeChange)) VSSystemEvent_EventProc E_OnAttributeChange;
    __declspec(property(get=Get_E_OnParentBeforeChange, put=Put_E_OnParentBeforeChange)) VSSystemEvent_EventProc E_OnParentBeforeChange;
    __declspec(property(get=Get_E_OnParentChange, put=Put_E_OnParentChange)) VSSystemEvent_EventProc E_OnParentChange;
    __declspec(property(get=Get_E_OnStaticChange, put=Put_E_OnStaticChange)) VSSystemEvent_EventProc E_OnStaticChange;
    __declspec(property(get=Get_E_OnScriptChange, put=Put_E_OnScriptChange)) VSSystemEvent_EventProc E_OnScriptChange;
    __declspec(property(get=Get_E_OnBecomeSync, put=Put_E_OnBecomeSync)) VSSystemEvent_EventProc E_OnBecomeSync;
    __declspec(property(get=Get_E_OnSyncGroupChange, put=Put_E_OnSyncGroupChange)) VSSystemEvent_EventProc E_OnSyncGroupChange;
    __declspec(property(get=Get_E_OnActiveSetChange, put=Put_E_OnActiveSetChange)) VSSystemEvent_EventProc E_OnActiveSetChange;
    __declspec(property(get=Get_E_OnChildSyncGroupChange, put=Put_E_OnChildSyncGroupChange)) VSSystemEvent_EventProc E_OnChildSyncGroupChange;
    __declspec(property(get=Get_E_OnLoadMask, put=Put_E_OnLoadMask)) VSSystemEvent_EventProc E_OnLoadMask;
    __declspec(property(get=Get_E_OnLoadFinish, put=Put_E_OnLoadFinish)) VSSystemEvent_EventProc E_OnLoadFinish;
    __declspec(property(get=Get_E_OnRemoteSend, put=Put_E_OnRemoteSend)) VSSystemEvent_EventProc E_RemoteSend;
    __declspec(property(get=Get_E_OnCall, put=Put_E_OnCall)) VSSystemEvent_EventProc E_Call;
#endif

public:
    ClassOfSRPObject();
    virtual ~ClassOfSRPObject();
    virtual void SRPAPI Release();  //--The subclass inherits the function, you need to override this function to release resources
    virtual VS_CHAR *SRPAPI GetSelfName();

    /*-------------------------------------------------------------------------
      WrapObject bind the class object, you can get bound by GetAppClass function
      Since objects may be released by other means, if you set In_AutoReleaseThis = true, then release the this instance
      At this point if there is need to release additional resources need to reload the Release function,to release resources
      if In_AutoReleaseObject ==true，Automatic release object when release instance; otherwise object does not release
    ---------------------------------------------------------------------------*/
    void SRPAPI WrapObject(class ClassOfSRPInterface *In_SRPInterface,VS_BOOL In_AutoReleaseObject,VS_BOOL In_AutoReleaseThis,void *SRPObject,VS_ULONG In_ClassLayer = 0xFFFFFFFF);
	void SRPAPI UnWrapObject();    // Clear correspondence with the Object, then the class can not operate on the object
    void SRPAPI SetAutoReleaseSelf(VS_BOOL In_AutoReleaseThis){AutoReleaseThis = In_AutoReleaseThis;};
    void SRPAPI SetAutoReleaseObject(VS_BOOL In_AutoReleaseObject){AutoReleaseObject = In_AutoReleaseObject;};
    VS_BOOL SRPAPI GetAutoReleaseSelf(){return AutoReleaseThis;};
    VS_BOOL SRPAPI GetAutoReleaseObject(){return AutoReleaseObject;};
    /*--------------------------------------------------------------------------
      The function and WrapObject different. just set the corresponding object class. does not bind the object and class
      Only temporarily effective. Can not get set by GetAppClass function
      If such has binds a object, the cancellation binding relations.
    ---------------------------------------------------------------------------*/
    void SRPAPI ProcessObject( class ClassOfSRPInterface *In_SRPInterface,void *SRPObject );

    void *SRPAPI GetSRPObject(){return ThisSRPObject;};
    class ClassOfSRPInterface *SRPAPI GetSRPInterface(){return ThisSRPInterface;};

    static class ClassOfSRPObject *SRPAPI GetSRPWrap( class ClassOfSRPInterface *In_SRPInterface,void *SRPObject,VS_ULONG In_ClassLayer = 0xFFFFFFFF);

//------------------------------------------------------------------------------
     VS_BOOL SRPAPI StringToUuid(VS_INT8 *String,VS_UUID *Uuid);
     VS_INT8 *SRPAPI UuidToString(VS_UUID *Uuid);

    class ClassOfSRPObject *SRPAPI GetParent();
    class ClassOfSRPObject *SRPAPI GetClass();
    void SRPAPI GetClassID(VS_UUID *UuidPtr);
    void SRPAPI GetID(VS_UUID *UuidPtr);
    VS_UUID *SRPAPI GetIDEx();

    class ClassOfSRPObject *SRPAPI GetPrevEx();
    class ClassOfSRPObject *SRPAPI GetNextEx();
    class ClassOfSRPObject *SRPAPI QueryFirst(void *VSObject);
    class ClassOfSRPObject *SRPAPI QueryNext();
    class ClassOfSRPObject *SRPAPI QueryPrev();
    class ClassOfSRPObject *SRPAPI QueryFirstActiveChild(VS_ULONG *Context);
    class ClassOfSRPObject *SRPAPI QueryNextActiveChild(VS_ULONG *Context);

    VS_BOOL SRPAPI IsObjectInActiveSet();
    VS_BOOL SRPAPI GetWebServiceFlag();
    VS_BOOL SRPAPI SetWebServiceFlag(VS_BOOL WebServiceFlag);

    class ClassOfSRPObject *SRPAPI QueryFirstInst(VS_QUERYRECORD *QueryRecord);
    class ClassOfSRPObject *SRPAPI QueryNextInst(VS_QUERYRECORD *QueryRecord);
    void SRPAPI QueryInstClose(VS_QUERYRECORD *QueryRecord);

    VS_CHAR *SRPAPI GetName();
    void SRPAPI SetName(VS_CHAR *Name);
    VS_BOOL SRPAPI IsInst(VS_UUID *ObjectClassID);
    VS_BOOL SRPAPI IsDirectInst(VS_UUID *ObjectClassID);
    VS_BOOL SRPAPI IsChild(class ClassOfSRPObject *ParentObject);

    void SRPAPI GetObjectSysRootItemID(VS_UUID *UuidPtr);
    void *SRPAPI GetObjectSysRootItem();
    VS_BOOL SRPAPI IsThisService();
    VS_BOOL SRPAPI IsActiveServiceObject();

    VS_BOOL SRPAPI IsThisClient();
    VS_ULONG SRPAPI GetClientID();
    VS_BOOL SRPAPI ExportObjectHeader( VS_CHAR *FileName );

    void SRPAPI SetSaveFlag(VS_UINT8 SaveFlag);
    VS_UINT8 SRPAPI GetSaveFlag();
    VS_INT8 *SRPAPI SaveToBuf( VS_INT32 *ObjectSize, VS_CHAR *Password, VS_UINT8 SaveFlag, VS_BOOL SaveNameValue );
    VS_BOOL SRPAPI SaveToFile( VS_CHAR *FileName, VS_CHAR *Password, VS_UINT8 SaveFlag, VS_BOOL SaveNameValue );
    VS_BOOL SRPAPI LoadFromBuf( VS_INT8 *Buf, VS_INT32 BufSize, VS_CHAR *Password, VS_BOOL LoadAsLocal,VS_BOOL LoadNameValue, VS_BOOL UpdateFlag );
    void SRPAPI DeferLoadFromBuf( VS_INT8 *Buf, VS_INT32 BufSize, VS_CHAR *Password, VS_BOOL LoadAsLocal,VS_BOOL LoadNameValue, VS_BOOL UpdateFlag );
    VS_BOOL SRPAPI LoadFromFile( VS_CHAR *FileName, VS_CHAR *Password, VS_BOOL LoadAsLocal,VS_BOOL LoadNameValue, VS_BOOL UpdateFlag, VS_BOOL StaticDataUseFile );
    void SRPAPI DeferLoadFromFile( VS_CHAR *FileName, VS_CHAR *Password, VS_BOOL LoadAsLocal,VS_BOOL LoadNameValue, VS_BOOL UpdateFlag, VS_BOOL StaticDataUseFile );
    void SRPAPI ResetLoad( );

    VS_BOOL SRPAPI IsLocalControl( );
    VS_BOOL SRPAPI IsRemoteCreate( );
    void SRPAPI SetIDInParent( VS_UINT16 IDInParent );
    VS_UINT16 SRPAPI GetIDInParent(  );
    class ClassOfSRPObject *SRPAPI GetChildByID( OBJECTATTRIBUTEINDEX AttributeIndex,VS_UINT16 IDInParent );

    VS_BOOL SRPAPI GetFunctionID(VS_CHAR *FuntionName,VS_UUID *FunctionID);
    VS_BOOL SRPAPI GetInEventID(VS_CHAR *InEventName,VS_UUID *InEventID);
    VS_BOOL SRPAPI GetOutEventID(VS_CHAR *OutEventName,VS_UUID *OutEventID);
    VS_CHAR *SRPAPI GetOutEventName(VS_UUID *OutEventID);

    VS_BOOL SRPAPI CreateOVLFunction(VS_UUID *OriginFunctionID,void *FuncAddress);
    VS_BOOL SRPAPI CreateOVLInEvent(VS_UUID *OutEventID,void *InEventAddress);

    void *SRPAPI GetFunction(VS_UUID *FunctionID,VS_BOOL *GlobalFunctionFlag = NULL);
    void *SRPAPI GetFunctionEx(VS_UUID *FunctionID,VS_BOOL *GlobalFunctionFlag = NULL);
    void *SRPAPI GetOriFunction(VS_UUID *FunctionID,VS_BOOL *GlobalFunctionFlag = NULL);
    void SRPAPI SetFunction(VS_UUID *FunctionID,void *FuncAddress);
    void SRPAPI SetInEvent(VS_UUID *InEventID,void *InEventAddress);
    void *SRPAPI GetSysEvent(VS_ULONG *Para);
    void SRPAPI SetSysEvent(void *SysEventAddress,VS_ULONG Para);

    void SRPAPI SetSysEventMask(VS_ULONG EventMask, VSSystemEvent_EventProc EventProc );
    VS_ULONG SRPAPI GetSysEventMask(VSSystemEvent_EventProc EventProc );
    void SRPAPI SetChildEventMask(VS_ULONG ClassLayer,VS_ULONG EventMask );
    VS_ULONG SRPAPI GetChildEventMask(VS_ULONG ClassLayer );

    VS_ULONG SRPAPI EventIDToDWORD(VS_UUID *OutEventID);
    VS_ULONG SRPAPI EventNameToDWORD(VS_UUID *OutEventID);

    VS_BOOL SRPAPI CreateNameScript( VS_CHAR *ScriptName,VS_CHAR *ScriptBuf );  //---
    VS_BOOL SRPAPI CreateNameScriptEx( VS_CHAR *ScriptName,VS_CHAR *FileName );  //---
    void SRPAPI DeleteNameScript( VS_CHAR *ScriptName );  //---
    void SRPAPI ChangeScriptName( VS_CHAR *ScriptName, VS_CHAR *NewScriptName);
    VS_CHAR *SRPAPI GetNameScript( VS_CHAR *ScriptName);
    VS_CHAR *SRPAPI QueryFirstNameScript( VS_CHAR **ScriptPtr );
    VS_CHAR *SRPAPI QueryNextNameScript( VS_CHAR **ScriptPtr );
    VS_BOOL SRPAPI ExecNameScript( VS_CHAR *ScriptName, VS_INT32 nArgs,VS_INT32 nOutArgs );
    VS_BOOL SRPAPI ExecNameScriptEx( VS_CHAR *ScriptName, VS_INT32 nArgs,VS_INT32 nOutArgs );
    VS_BOOL SRPAPI ExecScript(VS_CHAR *FuncName,VS_INT8 *ScriptBuf,VS_INT32 ScriptBufSize,VS_INT32 nArgs,VS_INT32 nOutArgs);
    VS_BOOL SRPAPI ExecScriptEx(VS_CHAR *FuncName,VS_CHAR *FileName,VS_INT32 nArgs,VS_INT32 nOutArgs);

    void SRPAPI RemoteCall(VS_ULONG ClientID,VS_UUID *FunctionID,...);
    void SRPAPI RemoteCallVar(VS_ULONG ClientID,VS_UUID *FunctionID,va_list argList);
    void SRPAPI RemoteCallEx(VS_ULONG ExcludeClientID,VS_UUID *FunctionID,...);
    void SRPAPI RemoteCallExVar(VS_ULONG ExcludeClientID,VS_UUID *FunctionID,va_list argList);
    VS_ULONG SRPAPI GetRemoteID();
    VS_UINT16 SRPAPI GetRemoteSourceTag();
    VS_BOOL SRPAPI IsRemoteCall();
    VS_ULONG SRPAPI GetRemotePrivateTag();
    void *SRPAPI GetRemoteAttach();
    VS_CHAR *SRPAPI GetRemoteCallName();

    VS_ULONG SRPAPI SRemoteCall(VS_ULONG WaitTime,VS_ULONG ClientID,VS_ULONG *RetCode,VS_UUID *FunctionID,...);
    VS_ULONG SRPAPI SRemoteCallVar(VS_ULONG WaitTime,VS_ULONG ClientID,VS_ULONG *RetCode,VS_UUID *FunctionID,va_list argList);
    VS_BOOL SRPAPI ARemoteCall(VS_ULONG WaitTime,VS_ULONG ClientID,VS_RemoteCallResponseProc CallBackProc,VS_ULONG Para,VS_UUID *FunctionID,...);
    VS_BOOL SRPAPI ARemoteCallVar(VS_ULONG WaitTime,VS_ULONG ClientID,VS_RemoteCallResponseProc CallBackProc,VS_ULONG Para,VS_UUID *FunctionID,va_list argList);
    VS_ULONG SRPAPI GetRemoteCallID();
    void SRPAPI SetDeferRspFlag();
    void SRPAPI SetRetCode(VS_ULONG RemoteRetCode);
    void SRPAPI SetRemoteRspAttach(void *RemoteAttach);
    void SRPAPI RemoteCallRsp(VS_ULONG ClientID,VS_ULONG RemoteCallID,VS_CHAR *RemoteCallName,VS_UINT16 RemoteSourceTag,VS_ULONG RetCode,VS_UINT8 RetType,VS_ULONG RetValue, void *RemoteAttach);

    VS_ULONG SRPAPI GetLayer( );
    void SRPAPI SetPrivateValue( VS_ULONG ClassLayer,VS_ULONG Index, VS_ULONG Value  );
    VS_BOOL SRPAPI GetPrivateValue( VS_ULONG ClassLayer,VS_ULONG Index, VS_ULONG *Value, VS_ULONG DefaultValue  );
    void *SRPAPI MallocPrivateBuf( VS_ULONG ClassLayer,VS_ULONG Index, VS_INT32 BufSize  );
    void *SRPAPI GetPrivateBuf( VS_ULONG ClassLayer,VS_ULONG Index, VS_INT32 *BufSize );
    void SRPAPI FreePrivate( VS_ULONG ClassLayer,VS_ULONG Index );

    VS_BOOL SRPAPI SetNameBoolValue( VS_CHAR *Name, VS_BOOL Value, VS_BOOL LocalChange = VS_FALSE );
    VS_BOOL SRPAPI SetNameIntValue( VS_CHAR *Name, VS_INT32 Value, VS_BOOL LocalChange = VS_FALSE );
    VS_BOOL SRPAPI SetNameFloatValue( VS_CHAR *Name, VS_DOUBLE Value, VS_BOOL LocalChange = VS_FALSE );
    VS_BOOL SRPAPI SetNameBinValue( VS_CHAR *Name, VS_INT8 *Value, VS_UINT16 ValueSize, VS_BOOL LocalChange = VS_FALSE  );
    VS_BOOL SRPAPI SetNameStringValue( VS_CHAR *Name, VS_CHAR *Value, VS_BOOL LocalChange = VS_FALSE );

    VS_BOOL SRPAPI GetNameBoolValue( VS_CHAR *Name, VS_BOOL DefaultValue );
    VS_INT32 SRPAPI GetNameIntValue( VS_CHAR *Name, VS_INT32 DefaultValue = 0 );
    VS_DOUBLE SRPAPI GetNameFloatValue( VS_CHAR *Name, VS_DOUBLE DefaultValue = 0 );
    VS_INT8 *SRPAPI GetNameBinValue( VS_CHAR *Name, VS_UINT16 *ValueSize);
    VS_CHAR *SRPAPI GetNameStringValue( VS_CHAR *Name, VS_CHAR *DefaultValue = NULL );

    VS_BOOL SRPAPI RegNameValueChangeCallBack(VS_ObjectNameValueChangeNotifyProc ObjectNameValueChangeNotifyProc,VS_ULONG Para);
    void SRPAPI UnRegNameValueChangeCallBack(VS_ObjectNameValueChangeNotifyProc ObjectNameValueChangeNotifyProc,VS_ULONG Para);

    VS_UINT8 SRPAPI GetSyncStatus( );
    VS_UINT8 SRPAPI GetSyncGroupStatus( VS_UUID *SysRootItemID,VS_SYNCGROUP GroupIndex );
    VS_UINT8 SRPAPI GetSyncGroupStatusEx( VS_CHAR *SysRootItemName,VS_SYNCGROUP GroupIndex );
    void SRPAPI SetSyncGroup(VS_SYNCGROUP GroupIndex);
    void SRPAPI GetSyncGroup(VS_SYNCGROUP *GroupIndex);

    void SRPAPI FreeObject();
    void SRPAPI DeferFreeObject();
    VS_BOOL SRPAPI IsObjectInFree();

    void SRPAPI ChangeLocal(OBJECTATTRIBUTEINDEX AttributeIndex,VS_INT8 *NewValue);
    VS_BOOL SRPAPI ChangeParent(class ClassOfSRPObject *ParentObject);
    void SRPAPI ChangeParent(class ClassOfSRPObject *ParentObject,OBJECTATTRIBUTEINDEX AttributeIndex);
    void SRPAPI MarkChange(OBJECTATTRIBUTEINDEX AttributeIndex);

    VS_BOOL SRPAPI RegBeforeChangeCallBack(VS_ObjectBeforeChangeNotifyProc ObjectBeforeChangeNotifyProc,VS_ULONG Para,VS_BOOL ChildNotify);
    VS_BOOL SRPAPI RegChangeCallBack(VS_ObjectChangeNotifyProc ObjectChangeNotifyProc,VS_ULONG Para,VS_BOOL ChildNotify);
    VS_BOOL SRPAPI RegReMallocCallBack(VS_ObjectReMallocNotifyProc ObjectReMallocNotifyProc,VS_ULONG Para);
    void SRPAPI UnRegBeforeChangeCallBack(VS_ObjectBeforeChangeNotifyProc ObjectBeforeChangeNotifyProc,VS_ULONG Para);
    void SRPAPI UnRegChangeCallBack(VS_ObjectChangeNotifyProc ObjectChangeNotifyProc,VS_ULONG Para);
    void SRPAPI UnRegReMallocCallBack(VS_ObjectReMallocNotifyProc ObjectReMallocNotifyProc,VS_ULONG Para);

    VS_BOOL SRPAPI RegObjectIDChangeNotify(VS_ObjectIDChangeNotifyProc ChangeNotifyProc,VS_ULONG Para);
    void SRPAPI UnRegObjectIDChangeNotify(VS_ObjectIDChangeNotifyProc ChangeNotifyProc,VS_ULONG Para);
    VS_BOOL SRPAPI RegObjectFreeNotify(VS_ObjectFreeNotifyProc FreeNotifyProc,VS_ULONG Para);
    void SRPAPI UnRegObjectFreeNotify(VS_ObjectFreeNotifyProc FreeNotifyProc,VS_ULONG Para);

    void SRPAPI DupVString(VS_VSTRING *InVString,VS_VSTRING *OutVString);
    VS_ULONG SRPAPI GetVStringBufSize(VS_VSTRING *VString);
    void SRPAPI ExpandVStringBufSize(VS_VSTRING *VString,VS_ULONG Size);

    VS_ULONG SRPAPI GetAllocType();

    VS_EVENTPARAM_RUNPARAM *SRPAPI GetResponseBuf();
    void SRPAPI FreeResponseBuf(VS_EVENTPARAM_RUNPARAM *ResponseParam);
    void SRPAPI AttachResponseBuf(VS_EVENTPARAM *EventParam,VS_EVENTPARAM_RUNPARAM *ResponseParam);

    VS_BOOL SRPAPI RegEventFunction(VS_UUID *EventID, void *FuncAddr);
    void SRPAPI UnRegEventFunction(VS_UUID *EventID, void *FuncAddr);
    VS_BOOL SRPAPI RegSysEventFunction(VS_ULONG SysEventID, void *FuncAddr);
    void SRPAPI UnRegSysEventFunction(VS_ULONG SysEventID, void *FuncAddr);

    VS_BOOL SRPAPI RegEventFunctionEx(class ClassOfSRPObject *SrcObject,VS_UUID *EventID, void *FuncAddr,VS_ULONG Para);
    void SRPAPI UnRegEventFunctionEx(class ClassOfSRPObject *SrcObject,VS_UUID *EventID, void *FuncAddr,VS_ULONG Para );
    VS_BOOL SRPAPI RegSysEventFunctionEx(VS_ULONG SysEventID, void *FuncAddr,VS_ULONG Para);
    void SRPAPI UnRegSysEventFunctionEx(VS_ULONG SysEventID, void *FuncAddr,VS_ULONG Para );

    VS_BOOL SRPAPI ActiveCmd(VS_UINT8 ActiveCmd);
    VS_UINT8 SRPAPI GetActiveCmd();
    VS_BOOL SRPAPI ActiveClient(VS_ULONG ClientID);
    void SRPAPI DeactiveClient(VS_ULONG ClientID);
    VS_BOOL SRPAPI Active();
    void SRPAPI Deactive();
    VS_BOOL SRPAPI IsActive();

    VS_BOOL SRPAPI SetClientObject(VS_ULONG ClientID);
    class ClassOfSRPObject *SRPAPI GetClientObject();

    VS_INT32  SRPAPI GetDataStatus( VS_ULONG UniqueDataUnitID, VS_UPDOWNFILEMSG *FileInfo );
    VS_INT8 *SRPAPI GetStaticData( VS_ULONG UniqueDataUnitID,VS_STATICID *DataVersion,VS_ULONG *DataSize, VS_BOOL AutoDownLoad);
    VS_INT8 *SRPAPI GetStaticDataEx( VS_ULONG UniqueDataUnitID,VS_STATICID *DataVersion,VS_ULONG *DataSize, VS_BOOL AutoDownLoad,VS_CHAR *Token);
    VS_BOOL SRPAPI SetStaticData( VS_ULONG UniqueDataUnitID,VS_ULONG DataSize,VS_INT8 *DataBuf,VS_STATICID *RetDataVersion);
    VS_BOOL SRPAPI SetStaticDataEx( VS_ULONG UniqueDataUnitID,VS_ULONG *DataSize,VS_ULONG Offset,VS_CHAR *FileName,VS_STATICID *RetDataVersion);
    VS_ULONG SRPAPI GetStaticAppCode( OBJECTATTRIBUTEINDEX StaticPersistentAttributeIndex);

    VS_ULONG SRPAPI SetupTimer(VS_INT32 Ticket,VS_TimerProc FunctionAddr,VS_ULONG Para1,VS_ULONG Para2,VS_ULONG Para3,VS_ULONG Para4 );
    void SRPAPI KillTimer(VS_ULONG TimerID);
    VS_ULONG SRPAPI GetTickCount();

    void SRPAPI SetRunEnv_FromChildCallBack( VS_RunEnvCallBackProc CallBack,VS_ULONG Para);
    void SRPAPI SetRunEnv_FromParentCallBack( VS_RunEnvCallBackProc CallBack,VS_ULONG Para);
    //---object capture runenv event
    void SRPAPI RegRunEnv_FromParentCallBack( void *ParentObject, VS_RunEnvCallBackProc CallBack,VS_ULONG Para);
    void SRPAPI UnRegRunEnv_FromParentCallBack( void *ParentObject, VS_RunEnvCallBackProc CallBack,VS_ULONG Para);
    //---real function
    VS_BOOL SRPAPI RunEnvToChild(void *DesObject, struct StructOfVSRunEnv *RunEnvInfo); //if DesObject == NULL, then send to register object
    VS_BOOL SRPAPI RunEnvToParent(struct StructOfVSRunEnv *RunEnvInfo);   //first send to parent, if its not handle, then send env

//-----Dynamic registered object Lua function
     VS_BOOL SRPAPI RegLuaFunc( VS_CHAR *FuncName, void *FuncAddress, VS_ULONG Para);
     void SRPAPI UnRegLuaFunc( VS_CHAR *FuncName, void *FuncAddress, VS_ULONG Para );
     void SRPAPI UnRegLuaFuncEx( void *FuncAddress, VS_ULONG Para );
     void SRPAPI ValidRegLuaFunc( VS_CHAR *FuncName, void *FuncAddress, VS_ULONG Para );
     void SRPAPI InValidRegLuaFunc( VS_CHAR *FuncName, void *FuncAddress, VS_ULONG Para );

//-----set and get attributes
     VS_BOOL SRPAPI RegLuaGetValueFunc( VS_LuaGetValueProc GetValueProc, VS_ULONG Para );
     VS_BOOL SRPAPI RegLuaSetValueFunc( VS_LuaSetValueProc SetValueProc, VS_ULONG Para );
     VS_BOOL SRPAPI UnRegLuaGetValueFunc( VS_LuaGetValueProc GetValueProc, VS_ULONG Para );
     VS_BOOL SRPAPI UnRegLuaSetValueFunc( VS_LuaSetValueProc SetValueProc, VS_ULONG Para );
     void SRPAPI ValidLuaGetValueFunc( VS_LuaGetValueProc GetValueProc, VS_ULONG Para );
     void SRPAPI ValidLuaSetValueFunc( VS_LuaSetValueProc SetValueProc, VS_ULONG Para );
     void SRPAPI InValidLuaGetValueFunc( VS_LuaGetValueProc GetValueProc, VS_ULONG Para );
     void SRPAPI InValidLuaSetValueFunc( VS_LuaSetValueProc SetValueProc, VS_ULONG Para );

    void *SRPAPI GetLua( );
    VS_BOOL SRPAPI DoBuffer( VS_CHAR *ScriptInterface,VS_INT8 *ScriptBuf,VS_INT32 ScriptBufSize, VS_CHAR *Name, VS_CHAR **ErrorInfo, VS_CHAR *WorkDirectory,VS_BOOL IsUTF8 );  //ScriptInterfaces length < 16
    VS_BOOL SRPAPI DoFile( VS_CHAR *ScriptInterface,VS_CHAR *FileName, VS_CHAR **ErrorInfo, VS_CHAR *WorkDirectory,VS_BOOL IsUTF8 );                                           //ScriptInterfaces length < 16

    VS_BOOL SRPAPI LuaPushObject( );
    class ClassOfSRPObject *SRPAPI LuaToObject( VS_INT32 Index );

    void SRPAPI LuaRCall(VS_ULONG ClientID,VS_CHAR *ScriptName, VS_INT32 nArgs);
    void SRPAPI LuaRCallEx(VS_ULONG ExcludeClientID,VS_CHAR *ScriptName, VS_INT32 nArgs);
    VS_BOOL SRPAPI LuaSRCall(VS_ULONG WaitTime,VS_ULONG ClientID,VS_CHAR *ScriptName, VS_INT32 nArgs, VS_INT32 *OutArgs);
    VS_BOOL SRPAPI LuaARCall(VS_ULONG WaitTime,VS_ULONG ClientID,void *CallBackProc, VS_ULONG Para, VS_CHAR *ScriptName, VS_INT32 nArgs);

    VS_BOOL SRPAPI DefLuaFunction( VS_CHAR *ScriptName );
    VS_BOOL SRPAPI SaveToLuaFunc( VS_CHAR *LuaFileName, VS_CHAR *FuncName );

    void SRPAPI LuaInitObject(VS_CHAR *InitScript);
    VS_CHAR *SRPAPI GetAttributeLuaString();

    VS_BOOL SRPAPI LuaGetObjectValue(VS_CHAR *Name);
    VS_BOOL SRPAPI LuaSetObjectValue(VS_CHAR *Name);

    VS_BOOL SRPAPI LockGC();
    VS_BOOL SRPAPI UnLockGC();

    //---Client/Send Message
    VS_BOOL SRPAPI RemoteSend( VS_ULONG ClientID, class ClassOfSRPParaPackageInterface *ParaPkg );

    void SRPAPI SetLog(VS_BOOL Flag);
};
#endif

//==============================================================================
typedef VS_BOOL (SRPAPI *VS_SRPMsgLoopExitProc)();
//---management configuration information
typedef struct StructOfStarCoreConfigEx{
    VS_CHAR InitConfigFile[512];   //XML File
    VS_CHAR ManagerServerUrl[512];
    VS_CHAR ControlServerUrl[512];
    VS_CHAR ManagerPath[256];      // "/XXXXX"
    VS_CHAR ControlPath[256];      // "/XXXXX"
    VS_UUID EndPointID;
    VS_CHAR EndPointAlias[256];
    VS_UINT8 EndPointType;
    VS_INT8 Reserved[3];
    VS_CHAR EnvTag[64];          //EnvTag predefined: "" "noloop" "activex",
    VS_ULONG PID;                //Parent Process ID
    VS_INT8 CheckBuf[128];       //application register string, reserved, should clear to 0
    VS_INT8 CheckCode[256];      //application register code, reserved, should clear to 0
    VS_INT8 Reserved1[188];
}VS_STARCONFIGEX;

/*if EndPointType is not VSMANAGER_ENDPOINTTYPE_ENDPOINT
/then the following url is process by hookmanagerproc
http://XXXXX/ManagerPath/alloccooperator
http://XXXXX/ManagerPath/freecooperator
http://XXXXX/ManagerPath/getcooperator
http://XXXXX/ManagerPath/setconfig
http://XXXXX/ManagerPath/getconfig
http://XXXXX/ManagerPath/statistic

http://ManagerServerUrl/ManagerPath/echo
http://ManagerServerUrl/ManagerPath/serverurl
http://ManagerServerUrl/ManagerPath/serviceurl
http://ManagerServerUrl/ManagerPath/registerserver
http://ManagerServerUrl/ManagerPath/getconfig
http://ManagerServerUrl/ManagerPath/alloccooperator
http://ManagerServerUrl/ManagerPath/freecooperator

other url can be process by RegWebServerMsgProc
*/

#define VSMANAGER_ENDPOINTTYPE_ENDPOINT ((VS_UINT8)0)
#define VSMANAGER_ENDPOINTTYPE_CONTROL  ((VS_UINT8)1)
#define VSMANAGER_ENDPOINTTYPE_MANAGER  ((VS_UINT8)2)
#define VSMANAGER_ENDPOINTTYPE_UNKNOWN  ((VS_UINT8)255)

#define VSMANAGER_ENDPOINTNAME_ENDPOINT "endpoint"
#define VSMANAGER_ENDPOINTNAME_CONTROL  "control"
#define VSMANAGER_ENDPOINTNAME_MANAGER  "manager"

/*
inlcude control and manager server, comunicate using http protocol
source type :  endpoint, control, or manager
------------------------------------------------------------------
starcore <-> controlserver
starcore -> control  : POST
URL : http://ControlServerUrl/ControlPath/restart
Request : <?xml version="1.0" encoding="utf-8" ?>
          <routerinfo>
            <info source="endpoint" ID="id">
          </routerinfo>
Response : no
+++++++++++++++++++
control -> starcore  : POST
URL : http://ControlServerUrl/ControlPath/managerconfig
Request : <?xml version="1.0" encoding="utf-8" ?>
          <routerinfo>
            <info source="control" ID="id">
          </routerinfo>
          <body>
          ManagerServerUrl
             <info managerserverurl="XXX" managerpath="XXX"/>    //host may be changed by router
          </body>
Response : no
------------------------------------------------------------------
starcore <-> manager

|-------|     |-------|     |--------|
|manager|---->|router |---->|endpoint|
|-------|     |-------|     |--------|

+++++++++++++++++++
starcore -> manager  : POST
URL : http://ManagerServerUrl/ManagerPath/echo
Request : <?xml version="1.0" encoding="utf-8" ?>
          <routerinfo>
            <info source="endpoint" ID="id"/>
          </routerinfo>
          <body>
             <info pushflag="true/false" alias="XXX" host="XXX" osalias="XXX cooperatornum="XXX" servicename="XXX" serviceid="XXXX"/>    //host may be changed by router
          </body>
Response : no
+++++++++++++++++++
starcore -> manager  : POST
URL : http://ManagerServerUrl/ManagerPath/getconfig
Request : <?xml version="1.0" encoding="utf-8" ?>
          <routerinfo>
            <info source="endpoint" ID="id">
          </routerinfo>
Response: <?xml version="1.0" encoding="utf-8" ?>
          <StarCoreConfig DynamicConfig=0/1>
          ..
          </StarCoreConfig>
+++++++++++++++++++
starcore -> manager  : POST
URL : http://ManagerServerUrl/ManagerPath/serverurl
Request : <?xml version="1.0" encoding="utf-8" ?>
          <routerinfo>
            <info source="endpoint" ID="id">
          </routerinfo>
          <body>
             <info servicename="XXX" osalias="XXX"/>
          </body>
Response: <?xml version="1.0" encoding="utf-8" ?>
          <body>
             <info interface="XXX" host="XXX" port="XXX"/>
          </body>
+++++++++++++++++++
starcore -> manager  : POST
URL : http://ManagerServerUrl/ManagerPath/serviceurl   --used at import service
Request : <?xml version="1.0" encoding="utf-8" ?>
          <routerinfo>
            <info source="endpoint" ID="id">
          </routerinfo>
          <body>
             <info servicename="XXX"  osalias="XXX"/>
          </body>
Response: <?xml version="1.0" encoding="utf-8" ?>
          <body>
             <info host="XXX"/>    //http://www.XXX.XXX/XXX
          </body>
+++++++++++++++++++
starcore -> manager  : POST
URL : http://ManagerServerUrl/ManagerPath/registerserver
Request : <?xml version="1.0" encoding="utf-8" ?>
          <routerinfo>
            <info source="endpoint" ID="id">
          </routerinfo>
          <body>
             <info servicename="XXX" host="XXX" interface="XXX" port="XXX"/>
          </body>
Response: no
+++++++++++++++++++
starcore -> manager  : POST
URL : http://ManagerServerUrl/ManagerPath/alloccooperator
Request : <?xml version="1.0" encoding="utf-8" ?>
          <routerinfo>
            <info source="endpoint" ID="id">
          </routerinfo>
          <body>
             <info servicename="XXX" host="XXX" interface="XXX" port="XXX"/>
          </body>
Response: no
+++++++++++++++++++
starcore -> manager  : POST
URL : http://ManagerServerUrl/ManagerPath/freecooperator
Request : <?xml version="1.0" encoding="utf-8" ?>
          <routerinfo>
            <info source="endpoint" ID="id">
          </routerinfo>
          <body>
             <info servicename="XXX" host="XXX" interface="XXX" port="XXX"/>
          </body>
Response: no
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
manager -> starcore : POST
URL : http://XXXXX/ManagerPath/alloccooperator
Request : <?xml version="1.0" encoding="utf-8" ?>
          <routerinfo>
            <info source="manager" ID="id">
          </routerinfo>
          <body>
             <info host="XXX" interface="XXX" port="XXX"/>
          </body>
Response: no
+++++++++++++++++++
manager -> starcore : POST
URL : http://XXXXX/ManagerPath/freecooperator
Request : <?xml version="1.0" encoding="utf-8" ?>
          <routerinfo>
            <info source="manager" ID="id">
          </routerinfo>
          <body>
             <info host="XXX" interface="XXX" port="XXX"/>
          </body>
Response: no
+++++++++++++++++++
manager -> starcore : POST
URL : http://XXXXX/ManagerPath/getcooperator
Request : <?xml version="1.0" encoding="utf-8" ?>
          <routerinfo>
            <info source="manager" ID="id">
          </routerinfo>
Response: <?xml version="1.0" encoding="utf-8" ?>
          <body>
             <cooperator host="XXX" interface="XXX" port="XXX" connect="true/false"/>
          </body>
+++++++++++++++++++
manager -> starcore : POST
URL : http://XXXXX/ManagerPath/setconfig
Request : <?xml version="1.0" encoding="utf-8" ?>
          <routerinfo>
            <info source="manager" ID="id">
          </routerinfo>
          <StarCoreConfig DynamicConfig=0/1  Host="127.0.0.1">
          ..
          </StarCoreConfig>
Response: <?xml version="1.0" encoding="utf-8" ?>
          <body>
             <DebugCfgResult>true/false</DebugCfgResult>
             <DirectClientCfgResult>true/false</DirectClientCfgResult>
             <TelnetCfgResult>true/false</TelnetCfgResult>
             <WebServerCfgResult>true/false</WebServerCfgResult>
          </body>
+++++++++++++++++++
manager -> starcore : POST
URL : http://XXXXX/ManagerPath/getconfig
Request : <?xml version="1.0" encoding="utf-8" ?>
          <routerinfo>
            <info source="manager" ID="id">
          </routerinfo>
Response: <?xml version="1.0" encoding="utf-8" ?>
          <StarCoreConfig DynamicConfig=0/1  Host="127.0.0.1">
          ..
          </StarCoreConfig>
+++++++++++++++++++
manager -> starcore : POST
URL : http://XXXXX/ManagerPath/statistic
Request : <?xml version="1.0" encoding="utf-8" ?>
          <routerinfo>
            <info source="manager" ID="id">
          </routerinfo>
Response: <?xml version="1.0" encoding="utf-8" ?>
          <starcore>
          ..
          </starcore>
--------------------------------------------------------------------------------
--------------------------------------------------------------------------------
--------------------------------------------------------------------------------
control <-> manager / user

ability: cooperator="true/false" :  alloc free and get
         config="true/false"     :  setconfig getconfig
         control="true/false"    :  create delete restart get (service process)

+++++++++++++++++++
control -> manager  : POST
URL : http://ManagerServerUrl/ManagerPath/echo
Request : <?xml version="1.0" encoding="utf-8" ?>
          <routerinfo>
            <info source="control" ID="id">
          </routerinfo>
          <body>
             <info pushflag="true/false" alias="XXX" host="XXX" osalias="XXX/>    //host may be changed by router
             <router>
                 <endpoint ID="XXX" Alias="XXX" osalias="XXX source="endpoint"/>
                 <endpoint ID="XXX" Alias="XXX" osalias="XXX source="control"/>
             </router>
             <ability enableconfig="true/false" enablecontrol="true/false"/>
          </body>
Response :  <?xml version="1.0" encoding="utf-8" ?>
          <body>
             <router>
                 <endpoint ID="XXX" userid="XXX"/>
                 <endpoint ID="XXX" userid="XXX"/>
             </router>
          </body>
+++++++++++++++++++
user -> control  : POST
URL : http://ControlServerUrl/userid/setoutport
Request : <?xml version="1.0" encoding="utf-8" ?>
          <body>
             <output host="XXX" port="XXX"/>  // if host not apperance, use source ip address as host
          </body>
Response : no
user -> control  : POST
URL : http://ControlServerUrl/userid/getcfginfo
Request : <?xml version="1.0" encoding="utf-8" ?>
          <body/>
Response : no
+++++++++++++++++++
manager -> control  : POST
URL : http://ControlServerUrl/ManagerPath/createservice
Request : <?xml version="1.0" encoding="utf-8" ?>
          <routerinfo>
            <info source="manager" ID="id">
          </routerinfo>
          <body>
             <service ID="XXX" cmd="XXX"/>
          </body>
Response : no
user -> control  : POST
URL : http://ControlServerUrl/userid/createservice
Request : <?xml version="1.0" encoding="utf-8" ?>
          <body>
             <service cmd="XXX" local="true/false">
          </body>
Response : no
+++++++++++++++++++
manager -> control  : POST
URL : http://ControlServerUrl/ManagerPath/restartservice
Request : <?xml version="1.0" encoding="utf-8" ?>
          <routerinfo>
            <info source="manager" ID="id">
          </routerinfo>
          <body>
             <service ID="XXX"/>
          </body>
Response : no
user -> control  : POST
URL : http://ControlServerUrl/userid/restartservice
Request : <?xml version="1.0" encoding="utf-8" ?>
          <body>
             <service local="true/false">
          </body>
Response : no
+++++++++++++++++++
manager -> control  : POST
URL : http://ControlServerUrl/ManagerPath/stopservice
Request : <?xml version="1.0" encoding="utf-8" ?>
          <routerinfo>
            <info source="manager" ID="id">
          </routerinfo>
          <body>
             <service ID="XXX"/>
          </body>
Response : no
user -> control  : POST
URL : http://ControlServerUrl/userid/stopservice
Request : <?xml version="1.0" encoding="utf-8" ?>
          <body/>
Response : no
+++++++++++++++++++
manager -> control  : POST
URL : http://ControlServerUrl/ManagerPath/startservice
Request : <?xml version="1.0" encoding="utf-8" ?>
          <routerinfo>
            <info source="manager" ID="id">
          </routerinfo>
          <body>
             <service ID="XXX"/>
          </body>
Response : no
user -> control  : POST
URL : http://ControlServerUrl/userid/startservice
Request : <?xml version="1.0" encoding="utf-8" ?>
          <body>
             <service local="true/false">
          </body>
Response : no
+++++++++++++++++++
manager -> control  : POST
URL : http://ControlServerUrl/ManagerPath/getservice
Request : <?xml version="1.0" encoding="utf-8" ?>
          <routerinfo>
            <info source="manager" ID="id">
          </routerinfo>
Response: <?xml version="1.0" encoding="utf-8" ?>
          <body>
             <service ID="XXX" dynaflag="true/false" extern="true/false" maxcooperatornum="XXX" curcooperatornum="XXX" cmd="XXX" status="run/fail/stop" servicename="XXX" serviceid="XXXX" userid="XXX"/>
             <service ID="XXX" dynaflag="true/false" extern="true/false" maxcooperatornum="XXX" curcooperatornum="XXX" cmd="XXX" status="run/fail/stop" servicename="XXX" serviceid="XXXX" userid="XXX"/>
             <service ID="XXX" dynaflag="true/false" extern="true/false" maxcooperatornum="XXX" curcooperatornum="XXX" cmd="XXX" status="run/fail/stop" servicename="XXX" serviceid="XXXX" userid="XXX"/>
             ...
          </body>
dynaflag=true : canbe create service by manager or user, else can't

user -> control  : POST
URL : http://ControlServerUrl/userid/getservice
Request : <?xml version="1.0" encoding="utf-8" ?>
          <body/>
Response: <?xml version="1.0" encoding="utf-8" ?>
          <body>
             <service cmd="XXX" status="run/fail/stop" servicename="XXX" serviceid="XXXX"/>
          </body>
+++++++++++++++++++
user -> manager  : POST
URL : http://ManagerServerUrl/userid/userregister
Request : <?xml version="1.0" encoding="utf-8" ?>
          <body/>
Response: <?xml version="1.0" encoding="utf-8" ?>
          <body>
             <service osalias="XXX host=""/>
          </body>

------------------------------------------------------------------
public -> starcore
URL : http://XXX/statistic        : GET
URL : http://XXX/__SSS/statistic    : GET
URL : http://XXX/__SrvGroupIndex/statistic    : GET
URL : http://XXX/wsdl             : GET
URL : http://XXX/__SSS/wsdl         : GET
URL : http://XXX/__SrvGroupIndex/wsdl : GET
URL : http://XXX/webservice/XXXX  : POST
URL : http://XXX/__SSS/webservice/XXXX : POST
URL : http://XXX/__SrvGroupIndex/webservice/XXXX : POST

__SSS:represent service name
__SrvGroupIndex : is number 

*/
#define VSINIT_OK    0
#define VSINIT_HAS   1
#define VSINIT_ERROR -1

#include "vsopencoredll.h"

class ClassOfStarCore{
public:
    //--server as client
    ClassOfStarCore(){};
    virtual ~ClassOfStarCore(){};
    virtual void SRPAPI Release() = 0;

    virtual VS_INT32 SRPAPI GetInitResult() = 0;
    virtual void SRPAPI SetWnd(VS_HWND In_hWnd) = 0;
    virtual void SRPAPI SetWndActive( VS_BOOL In_ActiveFlag ) = 0;

    virtual class ClassOfSRPInterface * SRPAPI CreateService(VS_CHAR *ServiceName,VSImportServiceDef ImportService[]) = 0;
    virtual class ClassOfSRPInterface * SRPAPI CreateService1(VS_CHAR *ServicePath,VS_CHAR *ServiceName,VS_UUID *ServiceID,VS_CHAR *RootPass,VSImportServiceDef ImportService[]) = 0;
    virtual class ClassOfSRPInterface * SRPAPI CreateService2(VS_CHAR *ServicePath,VS_CHAR *ServiceName,VS_UUID *ServiceID,VS_CHAR *RootPass,VS_INT32 FrameInterval,VS_INT32 NetPkgSize,VS_INT32 UploadPkgSize,VS_INT32 DownloadPkgSize,VS_INT32 DataUpPkgSize,VS_INT32 DataDownPkgSize,VSImportServiceDef ImportService[]) = 0;

    virtual void SRPAPI Flush(VS_BOOL WaitFlag) = 0;
    virtual void SRPAPI MsgLoop(VS_SRPMsgLoopExitProc SRPMsgLoopExitProc) = 0;

    virtual class ClassOfSRPControlInterface * SRPAPI GetControlInterface() = 0;  //--do not call release
    virtual class ClassOfBasicSRPInterface * SRPAPI GetBasicInterface() = 0;      //--do not call release
};

//ConfigEx maybe NULL
extern void SRPAPI StarCore_Init();   //must be call first, if want to use the following functions
extern void SRPAPI StarCore_Term();   //must be call last, if want to use the following functions

//--Init AS VS_SERVER_USER
extern class ClassOfStarCore *SRPAPI StarCore_Init(VS_STARCONFIGEX *ConfigEx);
extern class ClassOfStarCore *SRPAPI StarCore_Init(VS_BOOL SRPPrintFlag, VS_MsgCallBackProc MsgCallBackProc, VS_ULONG MsgCallBackPara,VS_STARCONFIGEX *ConfigEx);
//--Init AS VS_SERVER_SERVER
extern class ClassOfStarCore *SRPAPI StarCore_Init(VS_CHAR *DebugInterface, VS_UINT16 PortNumberForDebug, VS_CHAR *ClientInterface, VS_UINT16 PortNumberForDirectClient,VS_STARCONFIGEX *ConfigEx);
extern class ClassOfStarCore *SRPAPI StarCore_Init(VS_BOOL SRPPrintFlag, VS_CHAR *DebugInterface, VS_UINT16 PortNumberForDebug, VS_CHAR *ClientInterface, VS_UINT16 PortNumberForDirectClient, VS_MsgCallBackProc MsgCallBackProc, VS_ULONG MsgCallBackPara,VS_STARCONFIGEX *ConfigEx);
//--Init AS BY Exist Interface
extern class ClassOfStarCore *SRPAPI StarCore_Init( class ClassOfSRPControlInterface *In_VSControlInterface,class ClassOfBasicSRPInterface *In_BasicSRPInterface);

//--interface for service dll
#define VSCORE_STARCORESERVICE_INIT_NAME "StarCoreService_Init"
typedef VS_BOOL (SRPAPI *StarCoreService_InitProc)( class ClassOfStarCore *StarCore );
extern "C" SRPDLLEXPORT VS_BOOL SRPAPI StarCoreService_Init(class ClassOfStarCore *StarCore);  //--do not save StarCore,it maybe invalid after function return
#define VSCORE_STARCORESERVICE_TERM_NAME "StarCoreService_Term"
typedef void (SRPAPI *StarCoreService_TermProc)( class ClassOfStarCore *StarCore );
extern "C" SRPDLLEXPORT void SRPAPI StarCoreService_Term(class ClassOfStarCore *StarCore);     //--do not save StarCore,it maybe invalid after function return

extern void starlib_uuidtostring(VS_UUID *ObjectUUID,VS_INT8 *Buf);
extern VS_BOOL starlib_stringtouuid(VS_INT8 *Buf,VS_UUID *ObjectUUID);
extern VS_UUID *starlib_stringtouuidptr(VS_INT8 *Buf);  //should not use twice without buffered the result

extern VS_HANDLE starlib_dll_open_starcore(VS_CHAR *TempFileName,VS_INT32 TempFileNameSize); //load starcore.dll/so, if has load, then copy the dll file to tempfile, load tempfile
extern VS_HANDLE starlib_dll_open(class ClassOfSRPControlInterface *ControlInterface,const VS_CHAR *ModuleName);  //if has load, then copy the dll file to tempfile, load tempfile

#define _UUIDPTR(X) (starlib_stringtouuidptr(X))

//---simple interface
typedef struct StructOfCoreSimpleContext{
    VS_HANDLE hVSDllInstance;
    class ClassOfSRPControlInterface *VSControlInterface;
    class ClassOfBasicSRPInterface *VSBasicSRPInterface;

    VSCore_RegisterCallBackInfoProc    VSRegisterCallBackInfo;
    VSCore_InitProc                    VSInit;
    VSCore_TermExProc                  VSTermEx;
    VSCore_QueryControlInterfaceProc   VSQueryControlInterface;
}VS_CORESIMPLECONTEXT;

class ClassOfSRPInterface *VSCore_InitSimple(VS_CORESIMPLECONTEXT *Context,VS_CHAR *ServiceName,VS_CHAR *RootPass,VS_UINT16 ClientPort,VS_UINT16 WebPort,VS_MsgCallBackProc MsgCallBackProc, VS_ULONG MsgCallBackPara,...); // depend service name,... NULL as end; MsgCallBackProc maybe NULL
class ClassOfSRPInterface *VSCore_InitSimple1(VS_CORESIMPLECONTEXT *Context,VS_CHAR *ServiceName,VS_UUID *ServiceID,VS_CHAR *RootPass,VS_UINT16 ClientPort,VS_UINT16 WebPort,VS_MsgCallBackProc MsgCallBackProc, VS_ULONG MsgCallBackPara,...); // depend service name,... NULL as end; MsgCallBackProc maybe NULL
class ClassOfBasicSRPInterface *VSCore_InitSimpleEx(VS_CORESIMPLECONTEXT *Context,VS_UINT16 ClientPort,VS_UINT16 WebPort,VS_MsgCallBackProc MsgCallBackProc, VS_ULONG MsgCallBackPara,...);    //--load sharelib, but not create service, depend service name ... NULL as end; MsgCallBackProc maybe NULL
class ClassOfSRPInterface *VSCore_InitService(class ClassOfBasicSRPInterface *BasicSRPInterface,VS_CHAR *ServiceName,VS_UUID *ServiceID,VS_CHAR *RootPass,VS_INT32 FrameInterval,VS_INT32 NetPkgSize,VS_INT32 UploadPkgSize,VS_INT32 DownloadPkgSize,VS_INT32 DataUpPkgSize,VS_INT32 DataDownPkgSize,...);    //--create service, ... and sysrootitem, NULL as end;
void VSCore_TermSimple(VS_CORESIMPLECONTEXT *Context);
void VSCore_RunSimple(VS_CORESIMPLECONTEXT *Context,VS_SRPMsgLoopExitProc SRPMsgLoopExitProc);  //msg loop, SRPMsgLoopExitProc maybe NULL

//------------------------------------------------------------------------------
//-------------extern script interface
#define VSCORE_STARCORESCRIPT_INIT_NAME "StarCoreScript_Init"
typedef VS_BOOL (SRPAPI *StarCoreScript_InitProc)( VS_CHAR *ScriptName,VS_CHAR *Para,VSCore_RegisterCallBackInfoProc RegisterCallBackInfoProc,VSCore_UnRegisterCallBackInfoProc UnRegisterCallBackInfoProc,VSCore_InitProc InitProc,VSCore_TermProc TermProc,VSCore_TermExProc TermExProc,VSCore_HasInitProc HasInitProc,VSCore_QueryControlInterfaceProc QueryControlInterfaceProc,void *VirtualMachine);
extern "C" SRPDLLEXPORT VS_BOOL SRPAPI StarCoreScript_Init(VS_CHAR *ScriptName,VS_CHAR *Para,VSCore_RegisterCallBackInfoProc RegisterCallBackInfoProc,VSCore_UnRegisterCallBackInfoProc UnRegisterCallBackInfoProc,VSCore_InitProc InitProc,VSCore_TermProc TermProc,VSCore_TermExProc TermExProc,VSCore_HasInitProc HasInitProc,VSCore_QueryControlInterfaceProc QueryControlInterfaceProc,void *VirtualMachine);
//typedef void (SRPAPI *StarCoreScript_TermProc)( );

//------------------------------------------------------------------------------
//-------------define virtual key , valid on windows
#define VS_LBUTTON  	0x01
#define VS_RBUTTON  	0x02
#define VS_MBUTTON  	0x04

#define VS_ESCAPE		0x1B
#define VS_BACKSPACE	0x08
#define VS_TAB	    	0x09
#define VS_ENTER		0x0D
#define VS_SPACE		0x20

#define VS_SHIFT		0x10
#define VS_CTRL	    	0x11
#define VS_ALT	    	0x12

#define VS_LWIN	    	0x5B
#define VS_RWIN	    	0x5C
#define VS_APPS	    	0x5D

#define VS_PAUSE		0x13
#define VS_CAPSLOCK 	0x14
#define VS_NUMLOCK  	0x90
#define VS_SCROLLLOCK	0x91

#define VS_PGUP	    	0x21
#define VS_PGDN	    	0x22
#define VS_HOME	    	0x24
#define VS_END	    	0x23
#define VS_INSERT		0x2D
#define VS_DELETE		0x2E

#define VS_LEFT	    	0x25
#define VS_UP			0x26
#define VS_RIGHT		0x27
#define VS_DOWN	    	0x28

#define VS_0			0x30
#define VS_1			0x31
#define VS_2			0x32
#define VS_3			0x33
#define VS_4			0x34
#define VS_5			0x35
#define VS_6			0x36
#define VS_7			0x37
#define VS_8			0x38
#define VS_9			0x39

#define VS_A			0x41
#define VS_B			0x42
#define VS_C			0x43
#define VS_D			0x44
#define VS_E			0x45
#define VS_F			0x46
#define VS_G			0x47
#define VS_H			0x48
#define VS_I			0x49
#define VS_J			0x4A
#define VS_K			0x4B
#define VS_L			0x4C
#define VS_M			0x4D
#define VS_N			0x4E
#define VS_O			0x4F
#define VS_P			0x50
#define VS_Q			0x51
#define VS_R			0x52
#define VS_S			0x53
#define VS_T			0x54
#define VS_U			0x55
#define VS_V			0x56
#define VS_W			0x57
#define VS_X			0x58
#define VS_Y			0x59
#define VS_Z			0x5A

#define VS_GRAVE		0xC0
#define VS_MINUS		0xBD
#define VS_EQUALS		0xBB
#define VS_BACKSLASH	0xDC
#define VS_LBRACKET	    0xDB
#define VS_RBRACKET   	0xDD
#define VS_SEMICOLON	0xBA
#define VS_APOSTROPHE	0xDE
#define VS_COMMA		0xBC
#define VS_PERIOD		0xBE
#define VS_SLASH		0xBF

#define VS_NUMPAD0	    0x60
#define VS_NUMPAD1	    0x61
#define VS_NUMPAD2	    0x62
#define VS_NUMPAD3  	0x63
#define VS_NUMPAD4  	0x64
#define VS_NUMPAD5  	0x65
#define VS_NUMPAD6  	0x66
#define VS_NUMPAD7  	0x67
#define VS_NUMPAD8  	0x68
#define VS_NUMPAD9  	0x69

#define VS_MULTIPLY	    0x6A
#define VS_DIVIDE		0x6F
#define VS_ADD		    0x6B
#define VS_SUBTRACT	    0x6D
#define VS_DECIMAL  	0x6E

#define VS_F1			0x70
#define VS_F2			0x71
#define VS_F3			0x72
#define VS_F4			0x73
#define VS_F5			0x74
#define VS_F6			0x75
#define VS_F7			0x76
#define VS_F8			0x77
#define VS_F9			0x78
#define VS_F10	    	0x79
#define VS_F11	    	0x7A
#define VS_F12	    	0x7B

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
struct StructOfVSOpenInfoColor{
    VS_COLOR NormalTextColor;
    VS_COLOR ExplaneColor;
    VS_COLOR ObjectNameColor;
    VS_COLOR AttributeTypeColor;
    VS_COLOR NumberColor;
    VS_COLOR ErrorColor;
};

#define MSG_VSDISPMSG     0x00000001  //--wParam *str lParam 0    display info,VSFAULT_DISP types of information
#define MSG_VSDISPLUAMSG  0x00000002  //--wParam *str lParam 0    display info,VSFAULT_DISP types of information
#define MSG_DISPMSG       0x00000003  //--wParam *str lParam (int)Level    display info,extern app info，or other types of information
#define MSG_DISPLUAMSG    0x00000004  //--wParam *str lParam (int)Level    display info,extern app info，or other types of information
#define MSG_MESSAGEBOX    0x00000005  //--wParam *caption lParam *Info    show dialog
#define MSG_EXIT          0x00000006  //--wParam *ErrorInfo lParam 0       exit the program
#define MSG_GETWNDHANDLE  0x00000007  //--wParam (VS_HWND *hWnd) get window handle
#define MSG_SETWNDSIZE    0x00000008  //--wParam Width lParam Height  set client window size
#define MSG_GETWNDSIZE    0x00000009  //--wParam (VS_INT32 *Width) lParam (VS_INT32 *Height) get client window size
#define MSG_CLEARWND     0x0000000A  //--wParam 0 lParam 0          clear client window
#define MSG_HIDEWND      0x0000000B  //--wParam 0 lParam 0           hide client window
#define MSG_SHOWWND      0x0000000C  //--wParam 0 lParam 0           show client window
#define MSG_SETWNDBK     0x0000000D  //--wParam (VS_ULONG)Color lParam 0   set background color
#define MSG_SETFOCUS     0x0000000E   //--wParam VS_HWND lParam 0/1         set focus,0 represent notify,and 1 should be process
#define MSG_ISAPPACTIVE  0x0000000F  //--wParam 0 lParam 0      return:0(VS_FALSE),1(VS_TRUE)
#define MSG_SETIDLEACTIVE 0x00000010  //--wParam (VS_BOOL)ActiveFlag lParam 0   In no msg to be process, then set to VS_TRUE, will waste CPU resources

#define MSG_SETINFOCOLOR 0x00000011  //--wParam (VS_ULONG)(struct StructOfVSOpenInfoColor *) lParam 0   set information color
#define MSG_SETINFOBK     0x00000012  //--wParam (VS_ULONG)Color lParam 0   set information backgound color
#define MSG_KILLFOCUS     0x00000013   //--wParam VS_HWND lParam 0/1        cancel current focus,0 represent notify,and 1 should be process

#define MSG_RUNENV        0x00000014   //--wParam (VS_UUID *ObjectID) lParam(StructOfVSRunEnv *)        environment interaction

#define MSG_ONBEFORESTOPSERVICE 0x000000020  //--wParam (VS_UUID *ServiceID) lParam 0
#define MSG_ONSTOPSERVICE 0x000000021  //--wParam (VS_UUID *ServiceID) lParam 0
#define MSG_ONACTIVESERVICE  0x00000022  //--wParam (VS_UUID *)ServiceID lParam 0   current active service change
#define MSG_SAVESERVICE   0x000000023  //--wParam 0 lParam 0      return:0(VS_FALSE),1(VS_TRUE)    //---Returns whether to allow the storage service

#define MSG_SETMSGHOOK  0x00000024  //--wParam (VS_SRPMessageProcessHookProc Proc) lParam 0
#define MSG_GETMSGHOOK  0x00000025  //--wParam 0 lParam 0  return:irtualSociety_GlobalFuncDefine_SRPMessageProcessHookProc

#define MSG_HYPERLINK   0x00000026  //--wParam *caption lParam CreateNewWindowFlag    trigger hyperlink

#define MSG_SERVERTERM  0x00000027  //--wParam 0 lParam 0   server shutdown   [note:reserved, not use]
#define MSG_APPEVENT    0x00000028  //--wParam EventID lParam VS_CHAR *EventInfo    trigger app defined event

//------管理窗口的消息
#define MSG_ISMANAGERVISIBLE  0x00000030  //--wParam 0 lParam 0  return :0(InVisible),1(Visible)
#define MSG_HIDEMANAGER       0x00000031  //--wParam 0 lParam 0
#define MSG_SHOWMANAGER       0x00000032  //--wParam 0 lParam 0
#define MSG_SETMANAGERCAPTION 0x00000033  //--wParam *str lParam 0
#define MSG_GETMANAGERSIZE    0x00000034  //--wParam *Width lParam *Heiht
#define MSG_GETMANAGERHANDLE  0x00000035  //--wParam *(VS_HWND) lParam 0
#define MSG_SHOWMANAGERSTATUSMENU   0x00000036  //--wParam VS_BOOL MenuShowFlag lParam VS_BOOL StatusShowFlag  show or hide menu,status
#define MSG_SETMANAGERSTYLE    0x00000037  //--wParam VSWINDOWSTYLE *Style lParam 0
#define MSG_MOVEMANAGER        0x00000038  //--wParam VSWINDOWPOSITION *Position lParam RepaintFlag
#define MSG_GETMANAGERPOS      0x00000039  //--wParam VSWINDOWPOSITION *Position lParam 0
#define MSG_SETMANAGERSTATUS   0x0000003A  //--wParam Status lParam 0         //--0 normal  1  minimize  other maximize

//------service redirect
#define MSG_REDIRECTTOURLREQUEST  0x00000040  //--wParam *Url lParam *WorkDirectory  return:0(Allowed), 1 (forbid)
#define MSG_REDIRECTTOURLINFO  0x00000041  //--wParam *Url lParam (class ClassOfSRPParaPackageInterface *ParaPkg)

//------get current URL
#define MSG_GETURLREQUEST      0x00000050  //--wParam *Buf lParam BufSize

//-----set program type
#define MSG_SETPROGRAMTYPE     0x00000060  //--wParam Type[VS_SERVER/VS_SERVERCLIENT], lParam 0, return:0(Allowed),1(forbid)

//------Determine whether the current is windowless
#define MSG_ISWINDOWLESSSITE   0x00000070  //--wParam 0 lParam 0,return: 0(yes),1(no)
//------windowless Request to re-draw
#define MSG_WINDOWLESSREDRAW   0x00000071  //--wParam 0 not refresh background  1 refresh  lParam 0
#define MSG_ISWINDOWLESSTRANSPARENT   0x00000072  //--wParam 0 lParam 0,return: 0(not transparent),1(transparent)
#define MSG_GETWINDOWLESSDC           0x00000073  //--wParam (*HDC) lParam (VS_RECT*)
#define MSG_RELEASEWINDOWLESSDC       0x00000074  //--wParam (*HDC)

//------------------------------------------------------------------------------
//------Service and Server config info
//------get current Url
struct StructOfVSServerUrlInfo{
    VS_CHAR ServerInterface[512];
    VS_CHAR ServerName[512];
    VS_UINT16 ServerPortNumber;
};
#define MSG_GETSERVERURL       0x00000080  //--wParam (*ServiceName)  LParam (*StructOfVSServerUrlInfo)  return: 0(success),1(fail)
//-------when using import service or load depend service
#define MSG_GETSERVICEURL      0x00000081  //--wParam (*ServiceName)  LParam (*UrlBuf)  return: 0(success),1(fail)
#define MSG_GETCONFIG          0x00000082  //--wParam (ClassOfSRPSXMLInterface *)  LParam 0  return: 0(success),1(fail)
#define MSG_REGISTERSERVER     0x00000083  //--wParam (*ServiceName)  LParam (*StructOfVSServerUrlInfo)
#define MSG_ALLOCCOOPERATOR    0x00000084  //--wParam (*ServiceName)  LParam (*StructOfVSServerUrlInfo)
#define MSG_FREECOOPERATOR     0x00000085  //--wParam (*ServiceName)  LParam (*StructOfVSServerUrlInfo)

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//---Memory File Buf
class ClassOfSRPMemoryFileInterface{
public:
    ClassOfSRPMemoryFileInterface(){};
    virtual ~ClassOfSRPMemoryFileInterface(){};
    virtual void SRPAPI Release() = 0;
//-----
    virtual VS_INT32 SRPAPI GetNumber() = 0;

    virtual VS_BOOL SRPAPI InsertFile( VS_CHAR *FileName, VS_UINT8 *FileBuf, VS_ULONG FileBufSize, VS_UUID *FileID ) = 0;
    //格式: FileName,0(1B),UnCompressLength(4B),CompressLength(4B),DiskFileOffset(4B),VS_UUID(16B,reserved),SupportOSType(2B);0(1B)to end
    virtual VS_BOOL SRPAPI SetFromDisk( VS_CHAR *FileListInfo, VS_CHAR *DiskFileName, VS_ULONG FileStartOffset ) = 0;
    virtual VS_BOOL SRPAPI SetFromMemory( VS_CHAR *FileListInfo, VS_UINT8 *FileMemory, VS_ULONG FileStartOffset ) = 0;

    virtual VS_BOOL SRPAPI IsExist( VS_CHAR *FileName ) = 0;
    virtual VS_ULONG SRPAPI GetSize( VS_CHAR *FileName  ) = 0;
    virtual VS_ULONG SRPAPI Read( VS_CHAR *FileName, VS_UINT8 *ReadBuf  ) = 0;

    virtual void SRPAPI AddRef() = 0;
    virtual VS_ULONG SRPAPI GetRef() = 0;
};

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//---FileDisk
#define VS_FILEDISK_FILEHEADERSIZE    256
#define VS_FILEDISK_SECTORDATAHDRSIZE 40
#define VS_FILEDISK_SECTORDATASIZE    4096
#define VS_FILEDISK_INVALIDSECTOR 0xFFFFFFFF
#define VS_FILEDISK_INVALIDSIZE 0xFFFFFFFF

class ClassOfSRPFileDiskInterface{
public:
    ClassOfSRPFileDiskInterface(){};  //SectorNumberPerCluster max is 256
    virtual ~ClassOfSRPFileDiskInterface(){};
    virtual void SRPAPI Release() = 0;
//-----
    virtual VS_BOOL SRPAPI Open(VS_ULONG SectorNumberPerCluster,VS_CHAR *FileName,VS_BOOL CreateFlag) = 0;
    virtual VS_ULONG SRPAPI GetEmptySector() = 0;
    virtual void SRPAPI ClearSectorList( VS_INT32 SectorNumber, VS_ULONG *SectorIndex ) = 0;
    virtual void SRPAPI FlushDirtySector() = 0;
    virtual VS_BOOL SRPAPI IsSectorEmpty( VS_ULONG SectorIndex ) = 0;
    virtual VS_BOOL SRPAPI SetSectorInUse( VS_ULONG SectorIndex ) = 0;

    virtual VS_ULONG SRPAPI WriteSector(VS_ULONG SectorIndex,VS_UINT8 *DataHeader,VS_ULONG NextSector,VS_ULONG DataOffset,VS_ULONG DataSize,VS_INT8 *DataBuf) = 0;  //---return size, DataSize is total data size
    virtual VS_ULONG SRPAPI ReadSector(VS_ULONG SectorIndex,VS_UINT8 *DataHeader,VS_ULONG *NextSector,VS_ULONG DataOffset,VS_ULONG DataSize,VS_INT8 *DataBuf) = 0;  //---return size, if DataSize equals to 0，then return sector size

    virtual VS_BOOL SRPAPI ReadFileHeader(VS_UINT8 *Buf) = 0;
    virtual VS_BOOL SRPAPI SaveFileHeader(VS_UINT8 *Buf) = 0;

    //---if DataHeader is filled, then will compare when read next sector, the parameter may be  NULL
    virtual VS_BOOL SRPAPI GetNextSectorIndex( VS_ULONG SectorIndex,VS_UINT8 *DataHeader, VS_ULONG *NextSector ) = 0;

    virtual VS_ULONG SRPAPI GetSize(VS_ULONG *DiskSize) = 0;  //--return current data suze,DiskSize maybe NULL
    virtual void SRPAPI Clear() = 0;  //---clear all data
    virtual void SRPAPI Close() = 0;

    virtual void SRPAPI AddRef() = 0;
    virtual VS_ULONG SRPAPI GetRef() = 0;
};

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//---Pack to single exefile Tail, 256 Bytes
#define VSEXEFILEATTACHDATA_TAG "VSAttachData"
struct StructOfVSExeFileAttachData{
    VS_CHAR TAG[16];
    VS_ULONG AttachDataSize;          //--if AttachDataSize is 0，then AttachDataCaption represents starup file
    VS_ULONG AttachDataOffset;
    VS_ULONG Reserved[10];
    VS_CHAR AttachDataCaption[128];
    VS_ULONG Reserved1[16];
};


#endif
