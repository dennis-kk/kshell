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

#include <algorithm>
#include <iostream>

#include "resolver.h"

ResolverNode::ResolverNode() {
    _parent = NULL;
    _symbolicName = "";
}

ResolverNode::ResolverNode(const std::string& symbolicName) {
    _parent = NULL;
    _symbolicName = symbolicName;
}

ResolverNode::~ResolverNode() {
    std::list<ResolverNode*>::iterator iterator = _children.begin();
    for (; iterator != _children.end(); iterator++) {
        delete *iterator;
    }
    _children.clear();
}

void ResolverNode::setSymbolicName(const std::string& symbolicName) {
    _symbolicName = symbolicName;
}

std::string ResolverNode::getSymbolicName() {
    return _symbolicName;
}

void ResolverNode::addChild(ResolverNode* child) {
    _children.push_back(child);
    child->setParent(this);
}

void ResolverNode::removeChild(ResolverNode* child) {
    std::list<ResolverNode*>::iterator iterator = std::find(
        _children.begin(), _children.end(), child);
    if (iterator != _children.end()) {
        delete *iterator;
        _children.erase(iterator);
    }
}

void ResolverNode::setParent(ResolverNode* parent) {
    _parent = parent;
}

ResolverNode* ResolverNode::getParent() {
    return _parent;
}

std::list<ResolverNode*>* ResolverNode::getChildren() {
    return &_children;
}

int ResolverNode::size() {
    return (int)_children.size();
}

Resolver::Resolver() {
    _root = new ResolverNode("root");
}

Resolver::~Resolver() {
    delete _root;
}

void Resolver::addDependence(const std::string& father, std::string child) {
    ResolverNode* childNode = NULL;
    if (!child.empty()) {
        childNode = new ResolverNode(child);
    }
    ResolverNode* node = find(father);
    if (NULL == node) {
        ResolverNode* fatherNode = new ResolverNode(father);
        if (NULL != childNode) {
            fatherNode->addChild(childNode);
        }
        _root->addChild(fatherNode);
    } else {
        if (NULL != childNode) {
            node->addChild(childNode);
        }
    }
}

ResolverNode* Resolver::getRoot() {
    return _root;
}

ResolverNode* Resolver::find(const std::string& symbolicName) {
    return find(_root, symbolicName);
}

ResolverNode* Resolver::check() {    
    std::list<ResolverNode*>* children = _root->getChildren();
    std::list<ResolverNode*>::iterator iterator = children->begin();
    for (; iterator != children->end(); iterator++) {
        ResolverNode* node = check(_root, *iterator, _root->getSymbolicName());
        if (NULL != node) {
            return node;
        }
    }
    return NULL;
}

ResolverNode* Resolver::check(ResolverNode* father, ResolverNode* child, std::string symbolicName) {
    if (father == child) {
        return NULL;
    }
    if (child->getSymbolicName() == symbolicName) {
        return child;
    }
    std::list<ResolverNode*>* children = child->getChildren();
    std::list<ResolverNode*>::iterator iterator = children->begin();
    for (; iterator != children->end(); iterator++) {
        if ((*iterator)->getSymbolicName() == symbolicName) {
            return *iterator;
        }
        ResolverNode* node = check(child, *iterator, symbolicName);
        if (NULL != node) {
            return node;
        } else {
            node = check(child, *iterator, child->getSymbolicName());
            if (NULL != node) {
                return node;
            }
        }
    }
    return NULL;
}

std::vector<std::string> Resolver::resolve() {
    std::vector<std::string> nodes;
    ResolverNode* node = check();
    if (NULL != node) {
        return nodes;
    }
    resolve(_root, nodes);

    std::vector<std::string> resolvedNodes;
    for (size_t i = 0; i < nodes.size(); i++) {
        std::vector<std::string>::iterator iterator = std::find(
            resolvedNodes.begin(), resolvedNodes.end(), nodes[i]);
        if (iterator == resolvedNodes.end()) {
            resolvedNodes.push_back(nodes[i]);
        }
    }

    return resolvedNodes;
}

void Resolver::resolve(ResolverNode* child, std::vector<std::string>& nodes) {
    if (child != _root) {
        if (0 == child->size()) {
            nodes.push_back(child->getSymbolicName());
            return;
        }
    }
    std::list<ResolverNode*>* children = child->getChildren();
    std::list<ResolverNode*>::iterator iterator = children->begin();
    for (; iterator != children->end(); iterator++) {
        resolve(*iterator, nodes);
        if (0 != (*iterator)->size()) {
            nodes.push_back((*iterator)->getSymbolicName());
        }
    }
}

ResolverNode* Resolver::find(ResolverNode* node, std::string symbolicName) {
    if (node->getSymbolicName() == symbolicName) {
        return node;
    }
    if (0 == node->size()) {
        return NULL;
    }
    std::list<ResolverNode*>* children = node->getChildren();
    std::list<ResolverNode*>::iterator iterator = children->begin();
    for (; iterator != children->end(); iterator++) {
        ResolverNode* node = find(*iterator, symbolicName);
        if (NULL != node) {
            return node;
        }
    }
    return NULL;
}

void Resolver::print(ResolverNode* child) {
    std::string tag = child->getSymbolicName();
    ResolverNode* parent = NULL;
    while (NULL != (parent = child->getParent())) {
        if (parent->getSymbolicName() == child->getSymbolicName()) {
            break;
        }
        child = parent;
    }
    print(child, "", tag);
}

void Resolver::print() {
    print(_root, "", "");
}

void Resolver::print(ResolverNode* node, std::string tab, std::string tag) {
    if (tag == node->getSymbolicName()) {
        std::cout << tab << "(" << node->getSymbolicName() << ")";
    } else {
        std::cout << tab << node->getSymbolicName();
    }
    std::list<ResolverNode*>* children = node->getChildren();
    if (0 < children->size()) {
        std::cout << "->";
    }
    std::cout << std::endl;
    std::list<ResolverNode*>::iterator iterator = children->begin();
    for (; iterator != children->end(); iterator++) {
        print(*iterator, tab + "\t", tag);
    }
}
