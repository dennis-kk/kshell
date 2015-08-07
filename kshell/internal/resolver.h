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
