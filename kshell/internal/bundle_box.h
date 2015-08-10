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

#ifndef BUNDLE_BOX_H
#define BUNDLE_BOX_H

#include "pragma.h"

#include <list>
#include <map>
#include <string>

#include "scope_lock.h"
#include "condition.h"
#include "bundle_exception.h"
#include "bundle_event.h"
#include "resolver.h"
#include "bundle_box_loader.h"
#include "ignore.h"

class BundleContextImpl;

/**
 * @brief bundle container, manages the life-cycle of bundles
 */
class BundleBox {
public:
    /**
     * @brief ctor
     */
    BundleBox();

    /**
     * @brief dtor
     */
    ~BundleBox();

    /**
     * @brief start bundles locates in 'root'
     * @param root bundles' location
     */
    void start(const std::string& root = "") throw(BundleException);

    /**
     * @brief stop bundles
     */
    void stop() throw(BundleException);

    /**
     * @brief wait for stopping notification and return
     */
    bool waitForStop();

    /**
     * @brief get root directory
     */
    std::string getRoot();

private:
    std::list<std::string> _directories;
    std::map<std::string, BundleContextImpl*> _contexts;
    std::string _root;
    Resolver _resolver;
    Ignore _ignore;
    BundleBoxLoader _boxLoader;
    RecursiveLock _lock;
    volatile bool _quit;
    size_t _maxMargin;
};

#endif // BUNDLE_BOX_H
