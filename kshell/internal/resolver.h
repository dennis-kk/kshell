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

#ifndef RESOLVER_H
#define RESOLVER_H

#include <string>
#include <list>
#include <vector>

class ResolverNode {
public:
    ResolverNode();
    ResolverNode(const std::string& symbolicName);
    ~ResolverNode();

    void setSymbolicName(const std::string& symbolicName);
    std::string getSymbolicName();

    void addChild(ResolverNode* child);
    void removeChild(ResolverNode* child);

    void setParent(ResolverNode* parent);
    ResolverNode* getParent();

    std::list<ResolverNode*>* getChildren();
    int size();

private:
    std::list<ResolverNode*> _children;
    ResolverNode* _parent;
    std::string _symbolicName;
};

class Resolver {
public:
    Resolver();
    ~Resolver();

    void addDependence(const std::string& father, std::string child);
    ResolverNode* getRoot();
    ResolverNode* find(const std::string& symbolicName);
    ResolverNode* check();
    std::vector<std::string> resolve();
    void print();
    void print(ResolverNode* child);

private:
    ResolverNode* find(ResolverNode* node, std::string symbolicName);
    ResolverNode* check(ResolverNode* father, ResolverNode* child, std::string symbolicName);
    void resolve(ResolverNode* child, std::vector<std::string>& nodes);
    void print(ResolverNode* node, std::string tab, std::string tag = "");

private:
    ResolverNode* _root;
};

#endif // RESOLVER_H
