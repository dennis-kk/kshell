/*
 * Copyright (c) 2013-2015, dennis wang
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef BUNDLE_MANAGER_H
#define BUNDLE_MANAGER_H

#include "pragma.h"

#include <map>
#include <string>
#include <vector>

#include "scope_lock.h"
#include "bundle_guid.h"
#include "bundle_reference_impl.h"

class BundleImpl;
class BundleReferenceImpl;
class BundleContextImpl;

class BundleManager {
public:
    static BundleManager* getInstance();
    static void destory();

    BundleGuid add(BundleImpl* bundleImpl, BundleContextImpl* bundleContextImpl);
    void remove(BundleGuid id);

    bool has(BundleGuid id);
    bool has(const std::string& name);

    BundleReferenceImpl* get(BundleGuid id);
    BundleReferenceImpl* get(const std::string& name);
    std::vector<BundleReferenceImpl*> getBundles();

protected:
    BundleManager() {}
    BundleManager(const BundleManager&) {}
    ~BundleManager() {}

private:
    std::map<BundleGuid, BundleReferenceImpl*> _bundles;
    std::map<std::string, BundleGuid> _bundleNames;
    static BundleManager* _instance;
    static RecursiveLock _lock;
    static int _bundleId;
    static BundleReferenceImpl _invalidBundleReference;
};

#endif // BUNDLE_MANAGER_H
