﻿// This code is part of Toolkit(DNSPing)
// DNSPing, a useful and powerful toolkit
// Copyright (C) 2014-2019 Chengr28
// 
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.


#ifndef TOOLKIT_DNSPING_PLATFORM_H
#define TOOLKIT_DNSPING_PLATFORM_H

//////////////////////////////////////////////////
// Operating system selection
// 
/* This code is from Qt source, which in /src/corelib/global/qsystemdetection.h header file, please visit https://www.qt.io/developers.

   The operating system, must be one of: (PLATFOEM_x)

     DARWIN   - Any Darwin system (macOS, iOS, watchOS, tvOS)
     MACOS    - macOS
     IOS      - iOS
     WATCHOS  - watchOS
     TVOS     - tvOS
     MSDOS    - MS-DOS and Windows
     OS2      - OS/2
     OS2EMX   - XFree86 on OS/2 (not PM)
     WIN32    - Win32 (Windows 2000/XP/Vista/7 and Windows Server 2003/2008)
     WINRT    - WinRT (Windows 8 Runtime)
     CYGWIN   - Cygwin
     SOLARIS  - Sun Solaris
     HPUX     - HP-UX
     ULTRIX   - DEC Ultrix
     LINUX    - Linux [has variants]
     FREEBSD  - FreeBSD [has variants]
     NETBSD   - NetBSD
     OPENBSD  - OpenBSD
     BSDI     - BSD/OS
     INTERIX  - Interix
     IRIX     - SGI Irix
     OSF      - HP Tru64 UNIX
     SCO      - SCO OpenServer 5
     UNIXWARE - UnixWare 7, Open UNIX 8
     AIX      - AIX
     HURD     - GNU Hurd
     DGUX     - DG/UX
     RELIANT  - Reliant UNIX
     DYNIX    - DYNIX/ptx
     QNX      - QNX [has variants]
     QNX6     - QNX RTP 6.1
     LYNX     - LynxOS
     BSD4     - Any BSD 4.4 system
     UNIX     - Any UNIX BSD/SYSV system
     ANDROID  - Android platform
     HAIKU    - Haiku

   The following operating systems have variants:
     LINUX    - both PLATFORM_LINUX and PLATFORM_ANDROID are defined when building for Android
              - only PLATFORM_LINUX is defined if building for other Linux systems
     FREEBSD  - PLATFORM_FREEBSD is defined only when building for FreeBSD with a BSD userland
              - PLATFORM_FREEBSD_KERNEL is always defined on FreeBSD, even if the userland is from GNU
*/

#if defined(__APPLE__) && (defined(__GNUC__) || defined(__xlC__) || defined(__xlc__))
#  include <TargetConditionals.h>
#  if defined(TARGET_OS_MAC) && TARGET_OS_MAC
#    define PLATFORM_DARWIN
#    define PLATFORM_BSD4
#    ifdef __LP64__
#      define PLATFORM_DARWIN64
#    else
#      define PLATFORM_DARWIN32
#    endif
#    if defined(TARGET_OS_IPHONE) && TARGET_OS_IPHONE
#      if defined(TARGET_OS_WATCH) && TARGET_OS_WATCH
#        define PLATFORM_WATCHOS
#      else
#        define QT_PLATFORM_UIKIT
#        if defined(TARGET_OS_TV) && TARGET_OS_TV
#          define PLATFORM_TVOS
#        else
#          // TARGET_OS_IOS is only available in newer SDKs, 
#          // so assume any other iOS-based platform is iOS for now
#          define PLATFORM_IOS
#        endif
#      endif
#    else
#      // TARGET_OS_OSX is only available in newer SDKs, 
#      // so assume any non iOS-based platform is macOS for now
#      define PLATFORM_MACOS
#    endif
#  else
#    error "Qt has not been ported to this Apple platform - see https://www.qt.io/developers"
#  endif
#elif defined(__ANDROID__) || defined(ANDROID)
#  define PLATFORM_ANDROID
#  define PLATFORM_LINUX
#elif defined(__CYGWIN__)
#  define PLATFORM_CYGWIN
#elif !defined(SAG_COM) && (!defined(WINAPI_FAMILY) || WINAPI_FAMILY==WINAPI_FAMILY_DESKTOP_APP) && (defined(WIN64) || defined(_WIN64) || defined(__WIN64__))
#  define PLATFORM_WIN32
#  define PLATFORM_WIN64
#elif !defined(SAG_COM) && (defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__))
#  if defined(WINAPI_FAMILY)
#    ifndef WINAPI_FAMILY_PC_APP
#      define WINAPI_FAMILY_PC_APP WINAPI_FAMILY_APP
#    endif
#    if defined(WINAPI_FAMILY_PHONE_APP) && WINAPI_FAMILY==WINAPI_FAMILY_PHONE_APP
#      define PLATFORM_WINPHONE
#      define PLATFORM_WINRT
#    elif WINAPI_FAMILY==WINAPI_FAMILY_PC_APP
#      define PLATFORM_WINRT
#    else
#      define PLATFORM_WIN32
#    endif
#  else
#    define PLATFORM_WIN32
#  endif
#elif defined(__sun) || defined(sun)
#  define PLATFORM_SOLARIS
#elif defined(hpux) || defined(__hpux)
#  define PLATFORM_HPUX
#elif defined(__ultrix) || defined(ultrix)
#  define PLATFORM_ULTRIX
#elif defined(sinix)
#  define PLATFORM_RELIANT
#elif defined(__native_client__)
#  define PLATFORM_NACL
#elif defined(__linux__) || defined(__linux)
#  define PLATFORM_LINUX
#elif defined(__FreeBSD__) || defined(__DragonFly__) || defined(__FreeBSD_kernel__)
#  ifndef __FreeBSD_kernel__
#    define PLATFORM_FREEBSD
#  endif
#  define PLATFORM_FREEBSD_KERNEL
#  define PLATFORM_BSD4
#elif defined(__NetBSD__)
#  define PLATFORM_NETBSD
#  define PLATFORM_BSD4
#elif defined(__OpenBSD__)
#  define PLATFORM_OPENBSD
#  define PLATFORM_BSD4
#elif defined(__bsdi__)
#  define PLATFORM_BSDI
#  define PLATFORM_BSD4
#elif defined(__INTERIX)
#  define PLATFORM_INTERIX
#  define PLATFORM_BSD4
#elif defined(__sgi)
#  define PLATFORM_IRIX
#elif defined(__osf__)
#  define PLATFORM_OSF
#elif defined(_AIX)
#  define PLATFORM_AIX
#elif defined(__Lynx__)
#  define PLATFORM_LYNX
#elif defined(__GNU__)
#  define PLATFORM_HURD
#elif defined(__DGUX__)
#  define PLATFORM_DGUX
#elif defined(__QNXNTO__)
#  define PLATFORM_QNX
#elif defined(_SEQUENT_)
#  define PLATFORM_DYNIX
#elif defined(_SCO_DS) /* SCO OpenServer 5 + GCC */
#  define PLATFORM_SCO
#elif defined(__USLC__) /* all SCO platforms + UDK or OUDK */
#  define PLATFORM_UNIXWARE
#elif defined(__svr4__) && defined(i386) /* Open UNIX 8 + GCC */
#  define PLATFORM_UNIXWARE
#elif defined(__INTEGRITY)
#  define PLATFORM_INTEGRITY
#elif defined(VXWORKS) /* there is no "real" VxWorks define - this has to be set in the mkspec! */
#  define PLATFORM_VXWORKS
#elif defined(__HAIKU__)
#  define PLATFORM_HAIKU
#elif defined(__MAKEDEPEND__)
#else
#  error "Qt has not been ported to this OS - see https://www.qt-project.org/"
#endif

#if defined(PLATFORM_WIN32) || defined(PLATFORM_WIN64) || defined(PLATFORM_WINRT)
#  define PLATFORM_WIN
#endif

#if defined(PLATFORM_WIN)
#  undef PLATFORM_UNIX
#elif !defined(PLATFORM_UNIX)
#  define PLATFORM_UNIX
#endif

//Compatibility synonyms
#ifdef PLATFORM_DARWIN
#define PLATFORM_MAC
#endif
#ifdef PLATFORM_DARWIN32
#define PLATFORM_MAC32
#endif
#ifdef PLATFORM_DARWIN64
#define PLATFORM_MAC64
#endif
#ifdef PLATFORM_MACOS
#define PLATFORM_MACOS
#define PLATFORM_OSX
#endif

#ifdef PLATFORM_DARWIN
#  include <Availability.h>
#  include <AvailabilityMacros.h>
# 
#  ifdef PLATFORM_MACOS
#    if !defined(__MAC_OS_X_VERSION_MIN_REQUIRED) || __MAC_OS_X_VERSION_MIN_REQUIRED < __MAC_10_6
#       undef __MAC_OS_X_VERSION_MIN_REQUIRED
#       define __MAC_OS_X_VERSION_MIN_REQUIRED __MAC_10_6
#    endif
#    if !defined(MAC_OS_X_VERSION_MIN_REQUIRED) || MAC_OS_X_VERSION_MIN_REQUIRED < MAC_OS_X_VERSION_10_6
#       undef MAC_OS_X_VERSION_MIN_REQUIRED
#       define MAC_OS_X_VERSION_MIN_REQUIRED MAC_OS_X_VERSION_10_6
#    endif
#  endif
# 
#  // Numerical checks are preferred to named checks, but to be safe
#  // we define the missing version names in case Qt uses them.
# 
#  if !defined(__MAC_10_7)
#       define __MAC_10_7 1070
#  endif
#  if !defined(__MAC_10_8)
#       define __MAC_10_8 1080
#  endif
#  if !defined(__MAC_10_9)
#       define __MAC_10_9 1090
#  endif
#  if !defined(__MAC_10_10)
#       define __MAC_10_10 101000
#  endif
#  if !defined(__MAC_10_11)
#       define __MAC_10_11 101100
#  endif
#  if !defined(__MAC_10_12)
#       define __MAC_10_12 101200
#  endif
#  if !defined(MAC_OS_X_VERSION_10_7)
#       define MAC_OS_X_VERSION_10_7 1070
#  endif
#  if !defined(MAC_OS_X_VERSION_10_8)
#       define MAC_OS_X_VERSION_10_8 1080
#  endif
#  if !defined(MAC_OS_X_VERSION_10_9)
#       define MAC_OS_X_VERSION_10_9 1090
#  endif
#  if !defined(MAC_OS_X_VERSION_10_10)
#       define MAC_OS_X_VERSION_10_10 101000
#  endif
#  if !defined(MAC_OS_X_VERSION_10_11)
#       define MAC_OS_X_VERSION_10_11 101100
#  endif
#  if !defined(MAC_OS_X_VERSION_10_12)
#       define MAC_OS_X_VERSION_10_12 101200
#  endif
# 
#  if !defined(__IPHONE_4_3)
#       define __IPHONE_4_3 40300
#  endif
#  if !defined(__IPHONE_5_0)
#       define __IPHONE_5_0 50000
#  endif
#  if !defined(__IPHONE_5_1)
#       define __IPHONE_5_1 50100
#  endif
#  if !defined(__IPHONE_6_0)
#       define __IPHONE_6_0 60000
#  endif
#  if !defined(__IPHONE_6_1)
#       define __IPHONE_6_1 60100
#  endif
#  if !defined(__IPHONE_7_0)
#       define __IPHONE_7_0 70000
#  endif
#  if !defined(__IPHONE_7_1)
#       define __IPHONE_7_1 70100
#  endif
#  if !defined(__IPHONE_8_0)
#       define __IPHONE_8_0 80000
#  endif
#  if !defined(__IPHONE_8_1)
#       define __IPHONE_8_1 80100
#  endif
#  if !defined(__IPHONE_8_2)
#       define __IPHONE_8_2 80200
#  endif
#  if !defined(__IPHONE_8_3)
#       define __IPHONE_8_3 80300
#  endif
#  if !defined(__IPHONE_8_4)
#       define __IPHONE_8_4 80400
#  endif
#  if !defined(__IPHONE_9_0)
#       define __IPHONE_9_0 90000
#  endif
#  if !defined(__IPHONE_9_1)
#       define __IPHONE_9_1 90100
#  endif
#  if !defined(__IPHONE_9_2)
#       define __IPHONE_9_2 90200
#  endif
#  if !defined(__IPHONE_9_3)
#       define __IPHONE_9_3 90300
#  endif
#  if !defined(__IPHONE_10_0)
#       define __IPHONE_10_0 100000
#  endif
#endif

#ifdef __LSB_VERSION__
#  if __LSB_VERSION__ < 40
#    error "This version of the Linux Standard Base is unsupported"
#  endif
#ifndef QT_LINUXBASE
#  define QT_LINUXBASE
#endif
#endif


//////////////////////////////////////////////////
// Platform check
//Toolkit now support FreeBSD, Linux, macOS, and Windows.
#if !(defined(PLATFORM_FREEBSD) || defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS) || defined(PLATFORM_WIN))
	#error "This platform is unsupported."
#endif


//////////////////////////////////////////////////
// Base header
// 
//C Standard Library and C++ Standard Template Library/STL Headers
#include <array>                   //Container that encapsulates fixed size arrays support
#include <cstring>                 //C-Style String support
#include <ctime>                   //Get and manipulate date and time information support
#include <memory>                  //Manage dynamic memory support
#include <string>                  //STL String support

#if defined(PLATFORM_WIN)
//Windows headers
	#include <winsock2.h>                //WinSock 2.0+ support
	#include <windns.h>                  //Windows DNS definitions and DNS API
	#include <ws2tcpip.h>                //WinSock 2.0+ Extension for TCP/IP protocols
	#include <versionhelpers.h>          //Version Helper functions

//Library linking
	#pragma comment(lib, "ws2_32.lib")   //Windows WinSock 2.0+ support

//Endian definitions
	#define __LITTLE_ENDIAN             1234                         //Little Endian
	#define __BIG_ENDIAN                4321                         //Big Endian
	#define __BYTE_ORDER                __LITTLE_ENDIAN              //x86 and x86-64/x64 is Little Endian.
	#define LITTLE_ENDIAN               __LITTLE_ENDIAN
	#define BIG_ENDIAN                  __BIG_ENDIAN
	#define BYTE_ORDER                  __BYTE_ORDER

//Winsock definitions
	#define WINSOCK_VERSION_LOW_BYTE    2                            //Low byte of Winsock version 2.2
	#define WINSOCK_VERSION_HIGH_BYTE   2                            //High byte of Winsock version 2.2

//Windows compatible definitions
	typedef SSIZE_T                     ssize_t;
#elif (defined(PLATFORM_FREEBSD) || defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS))
//Portable Operating System Interface/POSIX and Unix system header
	#include <cerrno>                                                //Error report support
	#include <climits>                                               //Limits support
	#include <cstdio>                                                //File Input/Output support
	#include <cstdlib>                                               //C Standard Library support
	#include <ctime>                                                 //Get and manipulate date and time information support
	#include <netdb.h>                                               //Network database operations support
	#include <pthread.h>                                             //Threads support
	#include <signal.h>                                              //Signals support
	#include <unistd.h>                                              //Standard library API support
	#include <arpa/inet.h>                                           //Internet operations support
	#include <sys/time.h>                                            //Date and time support
#if defined(PLATFORM_FREEBSD)
	#include <netinet/in.h>                                          //Internet Protocol family support
	#include <sys/endian.h>                                          //Endian support
	#include <sys/socket.h>                                          //Main sockets header support
#elif defined(PLATFORM_LINUX)
	#include <endian.h>                                              //Endian support
#elif defined(PLATFORM_MACOS)
	#include <mach/kern_return.h>                                    //Mach kernel return codes
	#include <mach/mach.h>                                           //Mach includes all the types that a normal user of Mach programs should need
	#include <mach/mach_time.h>                                      //Mach time structure support
	#define __LITTLE_ENDIAN             1234                         //Little Endian
	#define __BIG_ENDIAN                4321                         //Big Endian
	#define __BYTE_ORDER                __LITTLE_ENDIAN              //x86 and x86-64/x64 is Little Endian.
#endif

//Compatible definitions
	#define FALSE                                                    0
#ifndef INVALID_SOCKET
	#define INVALID_SOCKET                                           (-1)
#endif
	#define SOCKET_ERROR                                             (-1)
	#define RETURN_ERROR                                             (-1)
	typedef int                                                      SOCKET;
	#define SD_BOTH                                                  SHUT_RDWR
	#define SD_RECV                                                  SHUT_RD
	#define SD_SEND                                                  SHUT_WR

//Function definitions
	#define closesocket                                              close
	#define fwprintf_s                                               fwprintf
	#define GetLastError()                                           errno
	#define _set_errno(Value)                                        errno = (Value)
	#define strnlen_s                                                strnlen
	#define wcsnlen_s                                                wcsnlen
	#define WSAGetLastError()                                        errno
	#define localtime_s(TimeStructure, TimeValue)                    localtime_r(TimeValue, TimeStructure)
	#define memcpy_s(Dst, DstSize, Src, Size)                        memcpy(Dst, Src, Size)
	#define memmove_s(Dst, DstSize, Src, Size)                       memmove(Dst, Src, Size)
#endif
#endif
