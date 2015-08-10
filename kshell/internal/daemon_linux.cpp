#include <syslog.h>
#include <unistd.h>
#include <signal.h>

#include <string>
#include <iostream>

#include "internal/framework_factory.h"
#include "internal/pid_file.h"

#define _stricmp(a,b) (std::string(a) == (b)) // helper macro
#define SZAPPNAME "kshell"                    // app name
static Framework* framework = NULL;           // global framework instance
static bool daemonized = false;                // daemon

/**
 * @brief debug start
 */
void CmdDebugService(int dwArgc, char** lpszArgv);
int CmdStartService();
int CmdStopService();

/**
 * @brief service start
 */
void ServiceStart(int dwArgc, char** lpszArgv);

/**
 * @brief service stop
 */
void ServiceStop();

/**
 * @brief CTRL-C handler
 */
void ctrlC(int);

/**
 * @brief main entry
 */
int main(int argc, char **argv) {
   // open syslog
   openlog("kshell", LOG_PID, LOG_USER);

   // signal handlers
   signal(SIGINT, ctrlC);
   signal(SIGKILL, ctrlC);
   signal(SIGTERM, ctrlC);
   signal(SIGQUIT, ctrlC);
   if ((argc > 1) && ((*argv[1] == '-') || (*argv[1] == '/'))) {
      if (_stricmp( "start", argv[1]+1)) {
         return CmdStartService();
      } else if (_stricmp( "stop", argv[1]+1)) {
         return CmdStopService();
      } else if (_stricmp( "debug", argv[1]+1)) {
         CmdDebugService(argc, argv);
      } else {
         goto dispatch;
      }
      return 0;
   }

   // if it doesn't match any of the above parameters
   // print helper string
   dispatch:
   // this is just to be friendly
   printf( "%s -start            to start the service\n", SZAPPNAME );
   printf( "%s -stop             to start the service\n", SZAPPNAME );
   printf( "%s -debug <params>   to run as a console app for debugging\n", SZAPPNAME );

   return 0;
}

void CmdDebugService(int dwArgc, char** lpszArgv) {
    try {
        framework = FrameworkFactory::newFramework();
        framework->start();
    } catch (FrameworkException exc) {
        std::cout << "Start failed: " << exc.what().c_str() << std::endl;
    }
}

void ServiceStart(int dwArgc, char** lpszArgv) {
    try {
        daemonized = true;
        syslog(LOG_CRIT, "Start kshell ...");
        framework = FrameworkFactory::newFramework();
        framework->start();
    } catch (FrameworkException exc) {
        syslog(LOG_CRIT, "%s", exc.what().c_str());
    }
}

void ServiceStop() {
    try {
        framework = FrameworkFactory::newFramework();
        framework->stop();
    } catch (FrameworkException exc) {
        syslog(LOG_CRIT, "%s", exc.what().c_str());
    }
}

int CmdStartService() {
    daemon(1, 0);
    ServiceStart(0, 0);
    return 0;
}

int CmdStopService() {
    //
    // read PID file & send signal
    //

    PidFile pf("");
    pf.kill();
    return 0;
}

void ctrlC(int) {
    if (NULL == framework) {
        return;
    }
    try {
        if (daemonized) {
            syslog(LOG_CRIT, "Stop kshell ...");
        } else {
            std::cout << "Stop kshell ..." << std::endl;
        }
        framework->stop();
        if (daemonized) {
            syslog(LOG_CRIT, "Stopped ...");
        } else {
            std::cout << "Stopped ..." << std::endl;
        }
    } catch (FrameworkException exc) {
        if (daemonized) {
            syslog(LOG_CRIT, "Stop failed: %s", exc.what().c_str());
        } else {
            std::cout << "Stop failed: " << exc.what().c_str() << std::endl;
        }        
    }
}
