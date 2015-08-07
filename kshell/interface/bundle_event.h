#ifndef BUNDLE_EVENT_H
#define BUNDLE_EVENT_H

#include "pragma.h"

class BundleReference;

class BundleEvent {
public:
    BundleEvent(int type, BundleReference* bundle) {
        _type = type;
        _reference = bundle;
    }

    BundleEvent(const BundleEvent& bundleEvent) {
        operator=(bundleEvent);
    }

    const BundleEvent& operator=(const BundleEvent& bundleEvent) {
        _type = bundleEvent._type;
        _reference = bundleEvent._reference;
        return *this;
    }

    int type() {
        return _type;
    }

    BundleReference* reference() {
        return _reference;
    }

private:
     BundleEvent() {
        _type = 0;
        _reference = 0;
    }

private:
    int _type;
    BundleReference* _reference;
};

#endif // BUNDLE_EVENT_H
