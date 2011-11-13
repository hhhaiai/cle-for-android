#ifndef _VSOPENREG
#define _VSOPENREG

#include "vsopenapi.h"

#if( VS_OS_TYPE == VS_OS_WINDOWS )
#define VS_CP_ACP   CP_ACP
#define VS_CP_UTF8  CP_UTF8
#endif
#if( VS_OS_TYPE == VS_OS_LINUX || VS_OS_TYPE == VS_OS_ANDROID || VS_OS_TYPE == VS_OS_ANDROIDV7A )
#define VS_CP_ACP   0
#define VS_CP_UTF8  1
#endif

#define USR_XML_REG

//------------------------------------------------------------------------------
//----Registry functions  need to shift to linux : /var/srplab/
#if( VS_OS_TYPE == VS_OS_WINDOWS ) && !defined(USR_XML_REG)
typedef HKEY vs_reg_hkey;
#define VS_REGKEY_CURRENT_USER HKEY_CURRENT_USER
#define VS_REGKEY_CLASSES_ROOT HKEY_CLASSES_ROOT
#define VS_REGKEY_LOCAL_MACHINE HKEY_LOCAL_MACHINE
#define VS_REGTYPE_SZ          REG_SZ
#define VS_REGTYPE_DWORD       REG_DWORD
#define VS_REGERROR_SUCCESS    ERROR_SUCCESS
#define VS_REGERROR_FAIL       1
#define VS_REG_CREATED_NEW_KEY REG_CREATED_NEW_KEY
#define VS_REG_OPENED_EXISTING_KEY REG_OPENED_EXISTING_KEY
#endif
#if( VS_OS_TYPE == VS_OS_LINUX || VS_OS_TYPE == VS_OS_ANDROID || VS_OS_TYPE == VS_OS_ANDROIDV7A ) || defined(USR_XML_REG)
struct StructOfVSRegKey{
    VS_CHAR Buf[512];  /*--Max Key Length--*/
};
typedef struct StructOfVSRegKey * vs_reg_hkey;
#define VS_REGKEY_CURRENT_USER ((vs_reg_hkey)0x00000001)
#define VS_REGKEY_CLASSES_ROOT ((vs_reg_hkey)0x00000002)
#define VS_REGKEY_LOCAL_MACHINE ((vs_reg_hkey)0x00000003)
#define VS_REGTYPE_SZ          0
#define VS_REGTYPE_DWORD       1
#define VS_REGERROR_SUCCESS    0
#define VS_REGERROR_FAIL       1
#define VS_REG_CREATED_NEW_KEY 1
#define VS_REG_OPENED_EXISTING_KEY 2
#endif

class ClassOfCoreShellInterface{
public:
    ClassOfCoreShellInterface(){};
    virtual ~ClassOfCoreShellInterface(){};
    virtual void SRPAPI Release() = 0;

    //if StrLength == -1 then auto calc
    //   return is null-terminated string
    virtual VS_INT8 *SRPAPI AnsiToUnicode(VS_UINT32 CodePage,VS_CHAR *ToCode,VS_INT8 *AnsiStr,VS_INT32 StrLength)= 0; //ToCode is ignore on windows, maybe UCS2 UCS4 UTF-16 UTF-32 UTF-16BE UTF-16LE UTF-32BE UTF-32LE
    virtual VS_INT8 *SRPAPI UnicodeToAnsi(VS_CHAR *FromCode,VS_UINT32 CodePage,VS_INT8 *WideStr,VS_INT32 StrLength,VS_INT32 BytesPerChar)= 0; //BytesPerChar(2/4) is ignore on windows
    virtual VS_INT8 *SRPAPI AnsiToUTF8(VS_INT8 *AnsiStr,VS_INT32 StrLength)= 0;
    virtual VS_INT8 *SRPAPI UTF8ToAnsi(VS_INT8 *UTFStr,VS_INT32 StrLength)= 0;
    virtual VS_INT8 *SRPAPI ToOSPlatString(VS_INT8 *Str,VS_INT32 StrLength)= 0; //for linux: discard \r, and reserved \n,  for win32 use \r\n
    virtual VS_INT8 *SRPAPI ToOSPlatStringEx(VS_UINT16 OSType,VS_INT8 *Str,VS_INT32 StrLength)= 0; //for linux: discard \r, and reserved \n,  for win32 use \r\n
    //the return value of above functions is free by the following function     
    virtual void SRPAPI FreeBuf(void *Buf)= 0;

    virtual void SRPAPI vs_reg_beginlock(void)=0;
    virtual void SRPAPI vs_reg_endlock(void)=0;

    virtual VS_ULONG SRPAPI vs_reg_createkeyex(vs_reg_hkey hKey,VS_CHAR *SubKey,VS_CHAR *Class,vs_reg_hkey *RetKey,VS_ULONG *DisPosition)= 0;
    virtual VS_ULONG SRPAPI vs_reg_closekey(vs_reg_hkey hKey)= 0;
    virtual VS_ULONG SRPAPI vs_reg_setvalue(vs_reg_hkey hKey,VS_CHAR *ValueName,VS_ULONG Type,VS_CHAR *Value,VS_ULONG ValueSize)= 0;
    virtual VS_ULONG SRPAPI vs_reg_openkeyex(vs_reg_hkey hKey,VS_CHAR *SubKey,vs_reg_hkey *RetKey)= 0;
    virtual VS_ULONG SRPAPI vs_reg_queryvalueex(vs_reg_hkey hKey,VS_CHAR *ValueName,VS_ULONG *Type,VS_UINT8 *DataBuf,VS_ULONG *DataBufSize)= 0;
    virtual VS_ULONG SRPAPI vs_reg_deletekeyex(vs_reg_hkey hKey,VS_CHAR *SubKey)= 0;
    virtual VS_ULONG SRPAPI vs_reg_enumkey(vs_reg_hkey hKey,VS_ULONG Index,VS_CHAR *NameBuf,VS_ULONG NameBufSize)= 0;
};

#endif
