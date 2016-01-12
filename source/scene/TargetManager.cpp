#include "TargetManager.h"
#include "utils/Logger.h"
#include "stream/StreamManager.h"

namespace v3d
{
namespace scene
{

using namespace stream;
using namespace renderer;

CTargetManager::CTargetManager()
{
}

CTargetManager::~CTargetManager()
{
    CTargetManager::unloadAll();
}

void CTargetManager::add(const renderer::TargetPtr& target)
{
    std::string name = target->getName();
    m_renderTargets.insert(std::map<std::string, renderer::TargetPtr>::value_type(name, target));
}

const TargetPtr CTargetManager::get(const std::string& name)
{
    auto it = m_renderTargets.find(name);
    if (it != m_renderTargets.end())
    {
        return it->second;
    }

    return nullptr;
}

void CTargetManager::unload(const std::string& name)
{
    auto it = m_renderTargets.find(name);
    if (it != m_renderTargets.end())
    {
        m_renderTargets.erase(it);
    }
}

void CTargetManager::unload(const renderer::TargetPtr& target)
{
    auto predDelete = [target](const std::pair<std::string, renderer::TargetPtr>& pair) -> bool
    {
        return pair.second == target;
    };

    auto it = std::find_if(m_renderTargets.begin(), m_renderTargets.end(), predDelete);
    if (it != m_renderTargets.end())
    {
        (*it).second->destroy();
        m_renderTargets.erase(it);
    }
}

void CTargetManager::unloadAll()
{
    for (auto& target : m_renderTargets)
    {
        target.second->destroy();
    }
    m_renderTargets.clear();
}

TargetIter CTargetManager::begin()
{
    return m_renderTargets.begin();
}

TargetCIter CTargetManager::cbegin() const
{
    return m_renderTargets.cbegin();
}

TargetIter CTargetManager::end()
{
    return m_renderTargets.end();
}

TargetCIter CTargetManager::cend() const
{
    return m_renderTargets.end();
}

} //namespace scenes
} //namespace v3d
