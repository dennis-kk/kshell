#ifndef BUNDLE_GUID_H
#define BUNDLE_GUID_H

#include "pragma.h"

class BundleGuid {
public:
    BundleGuid() {
        _id = 0;
    }

    BundleGuid(int id) {
        _id = id;
    }

    BundleGuid(const BundleGuid& guid) {
        operator=(guid);
    }

    ~BundleGuid() {
        _id = 0;
    }

    const BundleGuid& operator=(const BundleGuid& guid) {
        _id = guid._id;
        return *this;
    }

    int get() {
        return _id;
    }

    bool operator<(const BundleGuid& right) const {
        return (_id < right._id);
    }

private:
    int _id;
};

#endif // BUNDLE_GUID_H
