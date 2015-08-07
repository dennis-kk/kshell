#ifndef BUNDLE_REFERENCE_H
#define BUNDLE_REFERENCE_H

#include "pragma.h"

class Bundle;

class BundleReference {
public:
    virtual ~BundleReference() {}
    virtual Bundle* get() = 0;
    virtual void unget() = 0;
    virtual bool isOk() = 0;
};

#endif // BUNDLE_REFERENCE_H
