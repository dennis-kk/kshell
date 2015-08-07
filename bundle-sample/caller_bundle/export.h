#ifndef EXPORT_H
#define EXPORT_H

#include "kshell.h"

/**
 * @brief fork a activator
 */
BundleFunc void* newActivator();

/**
 * @brief fork a service by name
 */
BundleFunc void* newService(const char* name);

/**
 * @brief destroy a service by name
 */
BundleFunc void* destroyService(void* service, const char* name);

#endif // EXPORT_H
