#ifndef BUNDLE_REFERENCE_IMPL_H
#define BUNDLE_REFERENCE_IMPL_H

#include "pragma.h"

#include "scope_lock.h"
#include "bundle_reference.h"

class Bundle;

class BundleReferenceImpl : public BundleReference {
public:
    BundleReferenceImpl();
    BundleReferenceImpl(Bundle* bundle);
    virtual ~BundleReferenceImpl();

    virtual Bundle* get();
    virtual void unget();
    virtual bool isOk();

    int getCount();
    Bundle* getNoop();

private:
    RecursiveLock _lock;
    Bundle* _bundle;
    int _reference;
};

#endif // BUNDLE_REFERENCE_IMPL_H
