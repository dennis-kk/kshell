#ifndef FRAMEWORK_H
#define FRAMEWORK_H

#include <string>

#include "framework_exception.h"

class Controller;
class Monitor;

/**
 * @brief osgi framework
 */
class Framework {
public:
    virtual ~Framework() {}
    /**
     * @brief start framework
     */
    virtual void start() throw(FrameworkException) = 0;

    /**
     * @brief stop framework
     */
    virtual void stop() throw(FrameworkException) = 0;

    /**
     * @brief get framework binary directory
     */
    virtual const std::string& getLocation() = 0;
};

#endif // FRAMEWORK_H
