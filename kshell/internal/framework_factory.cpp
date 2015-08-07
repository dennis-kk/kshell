#include "framework_factory.h"
#include "framework_impl.h"

Framework* FrameworkFactory::newFramework() {
    static FrameworkImpl framework;
    return dynamic_cast<Framework*>(&framework);
}
