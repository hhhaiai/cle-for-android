#ifndef _VSOPENDATATYPE
#define _VSOPENDATATYPE

#define  VS_OS_WINDOWS      0 
#define  VS_OS_LINUX        1 
#define  VS_OS_ANDROID      2
#define  VS_OS_ANDROIDV7A   3
#define  VS_OS_UNKOWN       255

#if !defined(ENV_WINDOWS) && !defined(ENV_LINUX) && !defined(ENV_ANDROID) && !defined(ENV_ANDROIDV7A)
    #define VS_OS_TYPE          VS_OS_WINDOW
#else
    #ifdef ENV_WINDOWS
        #define VS_OS_TYPE          VS_OS_WINDOW
    #endif
    #ifdef ENV_LINUX
        #define VS_OS_TYPE          VS_OS_LINUX
    #endif
    #ifdef ENV_ANDROID
        #define VS_OS_TYPE          VS_OS_ANDROID
    #endif
    #ifdef ENV_ANDROIDV7A
        #define VS_OS_TYPE          VS_OS_ANDROIDV7A
    #endif
#endif

#if( VS_OS_TYPE == VS_OS_WINDOW )
    #include <stdio.h>
    #include <stdlib.h>
    #include <malloc.h>
    #include <string.h>
    #include <stdarg.h>
    #include <stddef.h>
    #include <time.h>
    #include <rpc.h>
    #include <windows.h>
    #include <assert.h>
    #include <tchar.h>
    #include <sys\stat.h>
    #include <direct.h>
    #include <winbase.h>
    #include <conio.h>
    #include <ctype.h>
#ifdef WIN32_LEAN_AND_MEAN
#ifndef _WINSOCKAPI_
	#include <winsock2.h>
#endif
#endif

#endif

#if( VS_OS_TYPE == VS_OS_LINUX || VS_OS_TYPE == VS_OS_ANDROID || VS_OS_TYPE == VS_OS_ANDROIDV7A )
    #include <stdio.h>
    #include <stdlib.h>
    #include <malloc.h>
    #include <string.h>
    #include <stdarg.h>
    #include <stddef.h>
    #include <time.h>
#if( VS_OS_TYPE != VS_OS_ANDROID && VS_OS_TYPE != VS_OS_ANDROIDV7A )
    #include <uuid/uuid.h>
#endif    
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <assert.h>
    #include <wchar.h>
    #include <sys/stat.h>
    #include <unistd.h>
    #include <dlfcn.h>
    #include <dirent.h>
#if( VS_OS_TYPE != VS_OS_ANDROID && VS_OS_TYPE != VS_OS_ANDROIDV7A )
    #include <curses.h>
#endif    
    #include <termios.h>
    #include <ctype.h>
    
    #define _MULTI_THREADED
    #include <pthread.h>
    #include <sched.h>
#if( VS_OS_TYPE != VS_OS_ANDROID && VS_OS_TYPE != VS_OS_ANDROIDV7A )        
    #include <sys/sem.h>
#else
    #include <semaphore.h>
#endif    
    #include <sys/ipc.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <netdb.h>
    #include <arpa/inet.h>
    #include <errno.h>
    #include <fcntl.h>
    #include <alloca.h>
    #include <utime.h>
    #include <signal.h>
#if( VS_OS_TYPE != VS_OS_ANDROID && VS_OS_TYPE != VS_OS_ANDROIDV7A )
    #include <wait.h>
#endif    
#endif

/*--------------OS TYPE And Module Extension-----------*/
/*  Max Number Is 16  */
#define VS_OSTYPE_NUM   4
#define VSOS_WIN32      ((VS_UINT16)0x01)
#define VSOS_LINUX      ((VS_UINT16)0x02)
#define VSOS_ANDROID    ((VS_UINT16)0x04)
#define VSOS_ANDROIDV7A  ((VS_UINT16)0x08)

#define VSMODULEEXT_WIN32   ".dll"
#define VSMODULEEXT_LINUX   ".so"
#define VSMODULEEXT_ANDROID ".so"
#define VSMODULEEXT_ANDROIDV7A ".so"

#define VSSRVLIBEXT_WIN32   ".dll"
#define VSSRVLIBEXT_LINUX   ".so"
#define VSSRVLIBEXT_ANDROID "_android.so"
#define VSSRVLIBEXT_ANDROIDV7A "_androidv7a.so"

#define VSOSALIAS_WIN32     "win32"
#define VSOSALIAS_LINUX     "linux"
#define VSOSALIAS_ANDROID   "android"
#define VSOSALIAS_ANDROIDV7A   "androidv7a"

/*-----for current os----*/
#if( VS_OS_TYPE == VS_OS_WINDOW )
#define VS_OSTYPE       VSOS_WIN32
#define VS_MODULEEXT    VSMODULEEXT_WIN32
#define VS_SRVLIBEXT    VSSRVLIBEXT_WIN32
#define VS_OSALIAS      VSOSALIAS_WIN32
#define VS_COREPATH     ""
#endif
#if( VS_OS_TYPE == VS_OS_LINUX )
#define VS_OSTYPE       VSOS_LINUX
#define VS_MODULEEXT    VSMODULEEXT_LINUX
#define VS_SRVLIBEXT    VSSRVLIBEXT_LINUX
#define VS_OSALIAS      VSOSALIAS_LINUX
#define VS_COREPATH      ""
#endif
#if( VS_OS_TYPE == VS_OS_ANDROID )
#define VS_OSTYPE       VSOS_ANDROID
#define VS_MODULEEXT    VSMODULEEXT_ANDROID
#define VS_SRVLIBEXT    VSSRVLIBEXT_ANDROID
#define VS_OSALIAS      VSOSALIAS_ANDROID
#define VS_COREPATH      "/data/data/com.srplab.starcore/files/"
#endif
#if( VS_OS_TYPE == VS_OS_ANDROIDV7A )
#define VS_OSTYPE       VSOS_ANDROIDV7A
#define VS_MODULEEXT    VSMODULEEXT_ANDROIDV7A
#define VS_SRVLIBEXT    VSSRVLIBEXT_ANDROIDV7A
#define VS_OSALIAS      VSOSALIAS_ANDROIDV7A
#define VS_COREPATH      "/data/data/com.srplab.starcore/files/"
#endif

typedef char                VS_BOOL;
typedef char                VS_INT8;
typedef unsigned char       VS_UINT8;
typedef short int           VS_INT16;
typedef unsigned short int  VS_UINT16;
typedef int                 VS_INT32;
typedef unsigned int        VS_UINT32;
typedef float               VS_FLOAT;
typedef double              VS_DOUBLE;

/*#ifdef _UNICODE*/
/*typedef unsigned short int  VS_CHAR;*/
/*#else*/
typedef char                VS_CHAR;
/*#endif*/

typedef long                VS_LONG;   /*--64bits for X64  32bits for X32*/
typedef unsigned long       VS_ULONG;  /*--64bits for X64  32bits for X32*/

#if( VS_OS_TYPE == VS_OS_WINDOWS )
typedef __int64             VS_INT64;
#endif
#if( VS_OS_TYPE == VS_OS_LINUX || VS_OS_TYPE == VS_OS_ANDROID || VS_OS_TYPE == VS_OS_ANDROIDV7A )
typedef int64_t             VS_INT64;
#endif

typedef void *              VS_HWND;
#if( VS_OS_TYPE == VS_OS_WINDOWS )
typedef UUID                VS_UUID;
typedef WCHAR               VS_WCHAR;
#endif
#if( VS_OS_TYPE == VS_OS_LINUX || VS_OS_TYPE == VS_OS_ANDROID || VS_OS_TYPE == VS_OS_ANDROIDV7A )
/*typedef uuid_t              VS_UUID;*/
typedef struct{
    unsigned long Data1;
    unsigned short Data2;
    unsigned short Data3;
    unsigned char Data4[8];
}VS_UUID;
typedef wchar_t             VS_WCHAR;
#endif

typedef void *              VS_HANDLE;
#define VS_INVALID_HANDLE   ((VS_HANDLE)-1)

#if( VS_OS_TYPE == VS_OS_WINDOWS )
typedef SYSTEMTIME VS_TIME_T;
#endif
#if( VS_OS_TYPE == VS_OS_LINUX || VS_OS_TYPE == VS_OS_ANDROID || VS_OS_TYPE == VS_OS_ANDROIDV7A )
typedef struct{
    VS_UINT16 wYear; 
    VS_UINT16 wMonth; 
    VS_UINT16 wDayOfWeek; 
    VS_UINT16 wDay; 
    VS_UINT16 wHour; 
    VS_UINT16 wMinute; 
    VS_UINT16 wSecond; 
    VS_UINT16 wMilliseconds; 
}VS_TIME_T;
#endif

#if( VS_OS_TYPE == VS_OS_WINDOWS )
#define WSAEWOULDBLOCK_CONNECT  WSAEWOULDBLOCK
struct _in_addr {
        union {
                struct { VS_UINT8 s_b1,s_b2,s_b3,s_b4; } S_un_b;
                struct { VS_UINT16 s_w1,s_w2; } S_un_w;
                VS_UINT32 S_addr;
        } S_un;
};
#endif
#if( VS_OS_TYPE == VS_OS_LINUX || VS_OS_TYPE == VS_OS_ANDROID || VS_OS_TYPE == VS_OS_ANDROIDV7A )
struct _in_addr {
        union {
                struct { u_char s_b1,s_b2,s_b3,s_b4; } S_un_b;
                struct { u_short s_w1,s_w2; } S_un_w;
                u_long S_addr;
        } S_un;
};
typedef struct sockaddr SOCKADDR;
typedef struct{
    short            sin_family;
    unsigned short   sin_port;
    struct in_addr   sin_addr;
    char             sin_zero[8];
}SOCKADDR_IN;
typedef VS_INT32 SOCKET;
typedef struct hostent *LPHOSTENT;
#define INVALID_SOCKET (SOCKET)(~0)
#define SOCKET_ERROR (-1)
#define WSAEWOULDBLOCK EAGAIN
#define WSAEINVAL      EINVAL
#define WSAEWOULDBLOCK_CONNECT  EINPROGRESS
#endif

#define  VS_TRUE            ((VS_BOOL)1)
#define  VS_FALSE           ((VS_BOOL)0)

#define  VS_OK              ((VS_INT32)0)
#define  VS_FAIL            ((VS_INT32)-1)

#define  VS_MOD(x,y)        ((x)%(y))
#define  VS_DIV(x,y)        ((x)/(y))

/*-------------function call format */
#if( VS_OS_TYPE == VS_OS_WINDOWS )
#define SRPAPI __cdecl
#define SRPSTDCALL __stdcall
#define SRPCALLBACK __cdecl
#define SRPDLLEXPORT __declspec(dllexport)
#define SRPDLLIMPORT __declspec(dllimport)
#endif
#if( VS_OS_TYPE == VS_OS_LINUX || VS_OS_TYPE == VS_OS_ANDROID || VS_OS_TYPE == VS_OS_ANDROIDV7A )
#define SRPAPI
#define SRPSTDCALL
#define SRPCALLBACK
#define SRPDLLEXPORT
#define SRPDLLIMPORT
#endif

#define SRPCPLUSPLUS

#endif

