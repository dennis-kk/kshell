#ifndef FRAMEWORK_FACTORY_H
#define FRAMEWORK_FACTORY_H

#include "framework.h"


class FrameworkFactory {
public:
    static Framework* newFramework();

private:
    FrameworkFactory();
    FrameworkFactory(const FrameworkFactory&);
    const FrameworkFactory& operator=(const FrameworkFactory&);
};

#endif // FRAMEWORK_FACTORY_H
