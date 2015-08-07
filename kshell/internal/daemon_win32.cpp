/** -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
/** Cherokee
 *
 * Authors:
 *      Alvaro Lopez Ortega <alvaro@alobbs.com>
 *
 * Copyright (C) 2001-2010 Alvaro Lopez Ortega
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of version 2 of the GNU General Public
 * License as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 */

/**---------------------------------------------------------------------------
THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED
TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.

Copyright (C) 1993 - 2000.  Microsoft Corporation.  All rights reserved.

MODULE:   service.c

PURPOSE:  Implements functions required by all Windows NT services

FUNCTIONS:
  main(int argc, char **argv);
  service_ctrl(DWORD dwCtrlCode);
  service_main(DWORD dwArgc, LPTSTR *lpszArgv);
  CmdInstallService();
  CmdRemoveService();
  CmdStartService();
  CmdDebugService(int argc, char **argv);
  ControlHandler ( DWORD dwCtrlType );
  GetLastErrorText( LPTSTR lpszBuf, DWORD dwSize );

---------------------------------------------------------------------------*/

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <process.h>
#include <tchar.h>

#include "framework_factory.h"

// internal variables
SERVICE_STATUS          ssStatus;       // current status of the service
SERVICE_STATUS_HANDLE   sshStatusHandle;
DWORD                   dwErr = 0;
BOOL                    bDebug = FALSE;
TCHAR                   szErr[256];
// globl defines
#define MSG_FLAGS_SYS_CODE 1
#define MSG_FLAGS_ERROR MSG_FLAGS_SYS_CODE
#define SZAPPNAME "kshell"
#define SZSERVICENAME "kshell"
#define SZSERVICEDISPLAYNAME "kshell"
// internal function prototypes
VOID WINAPI service_ctrl(DWORD dwCtrlCode);
VOID WINAPI service_main(DWORD dwArgc, LPTSTR *lpszArgv);
int CmdInstallService();
int CmdRemoveService();
int CmdStartService();
int CmdStopService();
VOID CmdDebugService(int argc, char **argv);
BOOL WINAPI ControlHandler ( DWORD dwCtrlType );
LPTSTR GetLastErrorText( LPTSTR lpszBuf, DWORD dwSize );
void AddToMessageLog(DWORD flags, LPCTSTR lpszMsg);
BOOL ReportStatusToSCMgr(DWORD dwCurrentState, DWORD dwWin32ExitCode, DWORD dwWaitHint);
// service start/stop function prototypes
void ServiceStart(DWORD dwArgc, LPTSTR *lpszArgv);
void ServiceStop();

//
//  FUNCTION: main
//
//  PURPOSE: entrypoint for service
//
//  PARAMETERS:
//    argc - number of command line arguments
//    argv - array of command line arguments
//
//  RETURN VALUE:
//    none
//
//  COMMENTS:
//    main() either performs the command line task, or
//    call StartServiceCtrlDispatcher to register the
//    main service thread.  When the this call returns,
//    the service has stopped, so exit.
//
int __cdecl main(int argc, char **argv)
{
   SERVICE_TABLE_ENTRY dispatchTable[] =
   {
      { TEXT(SZSERVICENAME), (LPSERVICE_MAIN_FUNCTION)service_main},
      { NULL, NULL}
   };

   if ( (argc > 1) &&
        ((*argv[1] == '-') || (*argv[1] == '/')) )
   {
      if ( _stricmp( "install", argv[1]+1 ) == 0 )
      {
         return CmdInstallService();
      }
      else if ( _stricmp( "remove", argv[1]+1 ) == 0 )
      {
         return CmdRemoveService();
      }
      else if ( _stricmp( "start", argv[1]+1 ) == 0)
      {
         return CmdStartService();
      }
      else if ( _stricmp( "stop", argv[1]+1 ) == 0)
      {
         return CmdStopService();
      }
      else if ( _stricmp( "debug", argv[1]+1 ) == 0 )
      {
         bDebug = TRUE;
         CmdDebugService(argc, argv);
      }
      else
      {
         goto dispatch;
      }
      return 0;
   }

   // if it doesn't match any of the above parameters
   // the service control manager may be starting the service
   // so we must call StartServiceCtrlDispatcher
   dispatch:
   // this is just to be friendly
   printf( "%s -install          to install the service\n", SZAPPNAME );
   printf( "%s -start            to start the service\n", SZAPPNAME );
   printf( "%s -stop             to start the service\n", SZAPPNAME );
   printf( "%s -remove           to remove the service\n", SZAPPNAME );
   printf( "%s -debug <params>   to run as a console app for debugging\n", SZAPPNAME );

   ::StartServiceCtrlDispatcherA(dispatchTable);

   return 0;
}



//
//  FUNCTION: service_main
//
//  PURPOSE: To perform actual initialization of the service
//
//  PARAMETERS:
//    dwArgc   - number of command line arguments
//    lpszArgv - array of command line arguments
//
//  RETURN VALUE:
//    none
//
//  COMMENTS:
//    This routine performs the service initialization and then calls
//    the user defined ServiceStart() routine to perform majority
//    of the work.
//
void WINAPI service_main(DWORD dwArgc, LPTSTR *lpszArgv)
{

   // register our service control handler:
   //
   sshStatusHandle = RegisterServiceCtrlHandler( TEXT(SZSERVICENAME), service_ctrl);

   if (!sshStatusHandle)
      goto cleanup;

   // SERVICE_STATUS members that don't change in example
   //
   ssStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
   ssStatus.dwServiceSpecificExitCode = 0;


   // report the status to the service control manager.
   //
   if (!ReportStatusToSCMgr(
                           SERVICE_RUNNING, // service state
                           NO_ERROR,              // exit code
                           3000))                 // wait hint
      goto cleanup;


   ServiceStart( dwArgc, lpszArgv );

   cleanup:

   // try to report the stopped status to the service control manager.
   //
   if (sshStatusHandle)
      (VOID)ReportStatusToSCMgr(
                               SERVICE_STOPPED,
                               dwErr,
                               0);

   return;
}



//
//  FUNCTION: service_ctrl
//
//  PURPOSE: This function is called by the SCM whenever
//           ControlService() is called on this service.
//
//  PARAMETERS:
//    dwCtrlCode - type of control requested
//
//  RETURN VALUE:
//    none
//
//  COMMENTS:
//
VOID WINAPI service_ctrl(DWORD dwCtrlCode)
{
   // Handle the requested control code.
   //
   switch (dwCtrlCode)
   {
   // Stop the service.
   //
   // SERVICE_STOP_PENDING should be reported before
   // setting the Stop Event - hServerStopEvent - in
   // ServiceStop().  This avoids a race condition
   // which may result in a 1053 - The Service did not respond...
   // error.
   case SERVICE_CONTROL_STOP:
      ReportStatusToSCMgr(SERVICE_STOP_PENDING, NO_ERROR, 0);
      ServiceStop();
      return;

      // Update the service status.
      //
   case SERVICE_CONTROL_INTERROGATE:
      break;

      // invalid control code
      //
   default:
      break;

   }

   ReportStatusToSCMgr(ssStatus.dwCurrentState, NO_ERROR, 0);
}



//
//  FUNCTION: ReportStatusToSCMgr()
//
//  PURPOSE: Sets the current status of the service and
//           reports it to the Service Control Manager
//
//  PARAMETERS:
//    dwCurrentState - the state of the service
//    dwWin32ExitCode - error code to report
//    dwWaitHint - worst case estimate to next checkpoint
//
//  RETURN VALUE:
//    TRUE  - success
//    FALSE - failure
//
//  COMMENTS:
//
BOOL ReportStatusToSCMgr(DWORD dwCurrentState,
                         DWORD dwWin32ExitCode,
                         DWORD dwWaitHint)
{
   static DWORD dwCheckPoint = 1;
   BOOL fResult = TRUE;


   if ( !bDebug ) // when debugging we don't report to the SCM
   {
      if (dwCurrentState == SERVICE_START_PENDING)
         ssStatus.dwControlsAccepted = 0;
      else
         ssStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP;

      ssStatus.dwCurrentState = dwCurrentState;
      ssStatus.dwWin32ExitCode = dwWin32ExitCode;
      ssStatus.dwWaitHint = dwWaitHint;

      if ( ( dwCurrentState == SERVICE_RUNNING ) ||
           ( dwCurrentState == SERVICE_STOPPED ) )
         ssStatus.dwCheckPoint = 0;
      else
         ssStatus.dwCheckPoint = dwCheckPoint++;


      // Report the status of the service to the service control manager.
      //
      if (!(fResult = SetServiceStatus( sshStatusHandle, &ssStatus)))
      {
         AddToMessageLog(MSG_FLAGS_ERROR, TEXT("SetServiceStatus"));
      }
   }
   return fResult;
}



//
//  FUNCTION: AddToMessageLog(LPCTSTR lpszMsg)
//
//  PURPOSE: Allows any thread to log an error message
//
//  PARAMETERS:
//    lpszMsg - text for message
//
//  RETURN VALUE:
//    none
//
//  COMMENTS:
//
void AddToMessageLog(DWORD flags, LPCTSTR lpszMsg)
{
   TCHAR szMsg [(sizeof(SZSERVICENAME) / sizeof(TCHAR)) + 100 ];
   HANDLE  hEventSource;
   LPCSTR  lpszStrings[2];

   if (flags & MSG_FLAGS_SYS_CODE)
      dwErr = GetLastError();
   else
      dwErr = 0;

   _stprintf(szMsg, TEXT("%s error: %d"), TEXT(SZSERVICENAME), (int)dwErr);
   lpszStrings[0] = szMsg;
   lpszStrings[1] = lpszMsg;

   if ( bDebug ) {
      printf("%s\n", szMsg);
      printf("%s\n", lpszMsg);
      return;
   }

   {
      // Use event logging to log the error.
      //
      hEventSource = RegisterEventSource(NULL, TEXT(SZSERVICENAME));

      if (hEventSource != NULL)
      {
         ReportEvent(hEventSource, // handle of event source
           // event type
                     (flags & MSG_FLAGS_ERROR)
             ? EVENTLOG_ERROR_TYPE : EVENTLOG_INFORMATION_TYPE,
                     0,                    // event category
                     0,                    // event ID
                     NULL,                 // current user's SID
                     2,                    // strings in lpszStrings
                     0,                    // no bytes of raw data
                     lpszStrings,          // array of error strings
                     NULL);                // no raw data

         (VOID) DeregisterEventSource(hEventSource);
      }
   }
}

void ResetError (void)
{
  dwErr = 0;
}

///////////////////////////////////////////////////////////////////
//
//  The following code handles service installation and removal
//


//
//  FUNCTION: CmdInstallService()
//
//  PURPOSE: Installs the service
//
//  PARAMETERS:
//    none
//
//  RETURN VALUE:
//    0 if success
//
//  COMMENTS:
//
int CmdInstallService()
{
   SC_HANDLE   schService;
   SC_HANDLE   schSCManager;

   TCHAR szPath[512];

   int ret = 0;

   if ( GetModuleFileName( NULL, szPath, 512 ) == 0 )
   {
      _tprintf(TEXT("Unable to install %s - %s\n"), TEXT(SZSERVICEDISPLAYNAME), GetLastErrorText(szErr, 256));
      return 1;
   }

   schSCManager = OpenSCManager(
                               NULL,                   // machine (NULL == local)
                               NULL,                   // database (NULL == default)
                               SC_MANAGER_ALL_ACCESS   // access required
                               );
   if ( schSCManager )
   {
      schService = CreateService(
                                schSCManager,               // SCManager database
                                TEXT(SZSERVICENAME),        // name of service
                                TEXT(SZSERVICEDISPLAYNAME), // name to display
                                SERVICE_ALL_ACCESS,         // desired access
                                SERVICE_WIN32_OWN_PROCESS,  // service type
                                SERVICE_AUTO_START,         // start type -- alternative: SERVICE_AUTO_START
                                SERVICE_ERROR_NORMAL,       // error control type
                                szPath,                     // service's binary
                                NULL,                       // no load ordering group
                                NULL,                       // no tag identifier
                                NULL, // TEXT(SZDEPENDENCIES),       // dependencies
                                NULL,                       // LocalSystem account
                                NULL);                      // no password

      if ( schService )
      {
         _tprintf(TEXT("%s installed.\n"), TEXT(SZSERVICEDISPLAYNAME) );
         CloseServiceHandle(schService);
      }
      else
      {
         _tprintf(TEXT("CreateService failed - %s\n"), GetLastErrorText(szErr, 256));
         ret = 1;
      }

      CloseServiceHandle(schSCManager);
   }
   else
     {
      _tprintf(TEXT("OpenSCManager failed - %s\n"), GetLastErrorText(szErr,256));
       ret = 1;
     }
   return ret;
}

//
//  FUNCTION: CmdStartService()
//
//  PURPOSE: Start the service
//
//  PARAMETERS:
//    none
//
//  RETURN VALUE:
//    0 if success
//
//  COMMENTS:

int CmdStartService()
{
  int ret = 0;

  SC_HANDLE schSCManager;
  SC_HANDLE schService;


    // Open a handle to the SC Manager database.
    schSCManager = OpenSCManager(
       NULL,                    // local machine
       NULL,                    // ServicesActive database
       SC_MANAGER_ALL_ACCESS);  // full access rights

    if (NULL == schSCManager) {
       _tprintf(TEXT("OpenSCManager failed - %s\n"), GetLastErrorText(szErr,256));
       ret = 1;
    }

    schService = OpenService(
        schSCManager,          // SCM database
        SZSERVICENAME,         // service name
        SERVICE_ALL_ACCESS);

    if (schService == NULL) {
      _tprintf(TEXT("OpenService failed - %s\n"), GetLastErrorText(szErr,256));
       ret = 1;
    }

    if (!StartService(
            schService,  // handle to service
            0,           // number of arguments
            NULL) )      // no arguments
    {
      _tprintf(TEXT("StartService failed - %s\n"), GetLastErrorText(szErr,256));
       ret = 1;
    }
    else
   {
      _tprintf(TEXT("Service Started\n"));
       ret = 0;
   }
    CloseServiceHandle(schService);
    CloseServiceHandle(schSCManager);
    return ret;
}

int CmdStopService()
{
  int ret = 0;

  SC_HANDLE schSCManager;
  SC_HANDLE schService;

  SERVICE_STATUS ServiceStatus;

    // Open a handle to the SC Manager database.
    schSCManager = OpenSCManager(
       NULL,                    // local machine
       NULL,                    // ServicesActive database
       SC_MANAGER_ALL_ACCESS);  // full access rights

    if (NULL == schSCManager) {
       _tprintf(TEXT("OpenSCManager failed - %s\n"), GetLastErrorText(szErr,256));
       ret = 1;
    }

    schService = OpenService(
        schSCManager,          // SCM database
        SZSERVICENAME,         // service name
        SERVICE_ALL_ACCESS);

    if (schService == NULL) {
      _tprintf(TEXT("OpenService failed - %s\n"), GetLastErrorText(szErr,256));
       ret = 1;
    }

    if (!ControlService(
            schService,  // handle to service
            SERVICE_CONTROL_STOP,           // number of arguments
            &ServiceStatus) )
    {
      _tprintf(TEXT("ControlServicefailed - %s\n"), GetLastErrorText(szErr,256));
       ret = 1;
    }
    else
   {
      _tprintf(TEXT("Service Stopped\n"));
       ret = 0;
   }
    CloseServiceHandle(schService);
    CloseServiceHandle(schSCManager);
    return ret;
}

//
//  FUNCTION: CmdRemoveService()
//
//  PURPOSE: Stops and removes the service
//
//  PARAMETERS:
//    none
//
//  RETURN VALUE:
//    0 if success
//
//  COMMENTS:
//
int CmdRemoveService()
{
   SC_HANDLE   schService;
   SC_HANDLE   schSCManager;

   int ret = 0;

   schSCManager = OpenSCManager(
                               NULL,                   // machine (NULL == local)
                               NULL,                   // database (NULL == default)
                               SC_MANAGER_CONNECT   // access required
                               );
   if ( schSCManager )
   {
      schService = OpenService(schSCManager, TEXT(SZSERVICENAME), DELETE | SERVICE_STOP | SERVICE_QUERY_STATUS);

      if (schService)
      {
         // try to stop the service
         if ( ControlService( schService, SERVICE_CONTROL_STOP, &ssStatus ) )
         {
            _tprintf(TEXT("Stopping %s."), TEXT(SZSERVICEDISPLAYNAME));
            Sleep( 1000 );

            while ( QueryServiceStatus( schService, &ssStatus ) )
            {
               if ( ssStatus.dwCurrentState == SERVICE_STOP_PENDING )
               {
                  _tprintf(TEXT("."));
                  Sleep( 1000 );
               }
               else
                  break;
            }

            if ( ssStatus.dwCurrentState == SERVICE_STOPPED )
               _tprintf(TEXT("\n%s stopped.\n"), TEXT(SZSERVICEDISPLAYNAME) );
            else
         {
               _tprintf(TEXT("\n%s failed to stop.\n"), TEXT(SZSERVICEDISPLAYNAME) );
      ret = 1;
         }

         }

         // now remove the service
         if ( DeleteService(schService) )
            _tprintf(TEXT("%s removed.\n"), TEXT(SZSERVICEDISPLAYNAME) );
         else
      {
            _tprintf(TEXT("DeleteService failed - %s\n"), GetLastErrorText(szErr,256));
        ret = 1;
      }


         CloseServiceHandle(schService);
      }
      else
   {
         _tprintf(TEXT("OpenService failed - %s\n"), GetLastErrorText(szErr,256));
     ret = 1;
   }

      CloseServiceHandle(schSCManager);
   }
   else
     {
      _tprintf(TEXT("OpenSCManager failed - %s\n"), GetLastErrorText(szErr,256));
       ret = 1;
     }
   return ret;
}




///////////////////////////////////////////////////////////////////
//
//  The following code is for running the service as a console app
//


//
//  FUNCTION: CmdDebugService(int argc, char ** argv)
//
//  PURPOSE: Runs the service as a console application
//
//  PARAMETERS:
//    argc - number of command line arguments
//    argv - array of command line arguments
//
//  RETURN VALUE:
//    none
//
//  COMMENTS:
//
void CmdDebugService(int argc, char ** argv)
{
   DWORD dwArgc;
   LPTSTR *lpszArgv;

#ifdef UNICODE
   lpszArgv = CommandLineToArgvW(GetCommandLineW(), &(dwArgc) );
   if (NULL == lpszArgv)
   {
       // CommandLineToArvW failed!!
       _tprintf(TEXT("CmdDebugService CommandLineToArgvW returned NULL\n"));
       return;
   }
#else
   dwArgc   = (DWORD) argc;
   lpszArgv = argv;
#endif

   _tprintf(TEXT("Debugging %s.\n"), TEXT(SZSERVICEDISPLAYNAME));

   SetConsoleCtrlHandler( ControlHandler, TRUE );

   ServiceStart( dwArgc, lpszArgv );

#ifdef UNICODE
// Must free memory allocated for arguments

   GlobalFree(lpszArgv);
#endif // UNICODE

}


//
//  FUNCTION: ControlHandler ( DWORD dwCtrlType )
//
//  PURPOSE: Handled console control events
//
//  PARAMETERS:
//    dwCtrlType - type of control event
//
//  RETURN VALUE:
//    True - handled
//    False - unhandled
//
//  COMMENTS:
//
BOOL WINAPI ControlHandler ( DWORD dwCtrlType )
{
   switch ( dwCtrlType )
   {
   case CTRL_BREAK_EVENT:  // use Ctrl+C or Ctrl+Break to simulate
   case CTRL_C_EVENT:      // SERVICE_CONTROL_STOP in debug mode
      _tprintf(TEXT("Stopping %s.\n"), TEXT(SZSERVICEDISPLAYNAME));
      ServiceStop();
      _tprintf(TEXT("Stopped ...\n"));
      return TRUE;
      break;

   }
   return FALSE;
}

//
//  FUNCTION: GetLastErrorText
//
//  PURPOSE: copies error message text to string
//
//  PARAMETERS:
//    lpszBuf - destination buffer
//    dwSize - size of buffer
//
//  RETURN VALUE:
//    destination buffer
//
//  COMMENTS:
//
LPTSTR GetLastErrorText( LPTSTR lpszBuf, DWORD dwSize )
{
   DWORD dwRet;
   LPTSTR lpszTemp = NULL;

   dwRet = FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |FORMAT_MESSAGE_ARGUMENT_ARRAY,
                          NULL,
                          GetLastError(),
                          LANG_NEUTRAL,
                          (LPTSTR)&lpszTemp,
                          0,
                          NULL );

   // supplied buffer is not long enough
   if ( !dwRet || ( (long)dwSize < (long)dwRet+14 ) )
      lpszBuf[0] = TEXT('\0');
   else
   {
      lpszTemp[lstrlen(lpszTemp)-2] = TEXT('\0');  //remove cr and newline character
      _stprintf( lpszBuf, TEXT("%s (0x%x)"), lpszTemp, (int)GetLastError() );
   }

   if ( lpszTemp )
      LocalFree((HLOCAL) lpszTemp );

   return lpszBuf;
}

void ServiceStart(DWORD dwArgc, LPTSTR *lpszArgv) {
    try {
        Framework* framework = FrameworkFactory::newFramework();
        framework->start();
    } catch (FrameworkException exc) {
        AddToMessageLog(MSG_FLAGS_ERROR, exc.what().c_str());
    }
}

void ServiceStop() {
    try {
        Framework* framework = FrameworkFactory::newFramework();
        framework->stop();
    } catch (FrameworkException exc) {
        AddToMessageLog(MSG_FLAGS_ERROR, exc.what().c_str());
    }
}
