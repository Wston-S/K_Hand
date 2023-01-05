#include "ResourceManager.h"
#include <algorithm>
#include <string.h>

ResourceManager::ResourceManager()
{
    DefaultPtr = nullptr;
}

ResourceManager::~ResourceManager()
{
}

bool ResourceManager::SearchNode(const char *name, ResourceNode_t *node)
{
    for (auto iter : NodePool)
    {
        if (strcmp(name, iter.name) == 0)
        {
            *node = iter;
            return true;
        }
    }

    return false;
}

bool ResourceManager::AddResource(const char *name, void *ptr)
{
    ResourceNode_t node;

    if(SearchNode(name, &node))
    {
        return false;
    }

    node.name = name;
    node.ptr = ptr;
    NodePool.push_back(node);

    return true;
}
bool ResourceManager::RemoveResource(const char *name)
{
    ResourceNode_t node;
    if(!SearchNode(name, &node))
    {
        return false;
    }

    auto iter =std::find(NodePool.begin(), NodePool.end(), node);
    if(iter == NodePool.end())
    {
        return false;
    }

    NodePool.erase(iter);

    return true;

}
void *ResourceManager::GetResource(const char *name)
{
    ResourceNode_t node;

    if(!SearchNode(name, &node))
    {
        return DefaultPtr;
    }

    return node.ptr;
}

void ResourceManager::SetDefault(void *ptr)
{
    DefaultPtr = ptr;
}
