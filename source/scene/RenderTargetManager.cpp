#include "RenderTargetManager.h"
#include "utils/Logger.h"
#include "stream/StreamManager.h"

using namespace v3d;
using namespace scene;
using namespace stream;
using namespace renderer;

CRenderTargetManager::CRenderTargetManager()
{
}

CRenderTargetManager::~CRenderTargetManager()
{
    CRenderTargetManager::unloadAll();
}

void CRenderTargetManager::add(const renderer::RenderTargetPtr& target)
{
    std::string name = target->getName();
    m_renderTargets.insert(std::map<std::string, renderer::RenderTargetPtr>::value_type(name, target));
}

const RenderTargetPtr CRenderTargetManager::get(const std::string& name)
{
    auto it = m_renderTargets.find(name);
    if (it != m_renderTargets.end())
    {
        return it->second;
    }

    return nullptr;
}

void CRenderTargetManager::unload(const std::string& name)
{
    auto it = m_renderTargets.find(name);
    if (it != m_renderTargets.end())
    {
        m_renderTargets.erase(it);
    }
}

void CRenderTargetManager::unload(const renderer::RenderTargetPtr& target)
{
    auto predDelete = [target](const std::pair<std::string, renderer::RenderTargetPtr>& pair) -> bool
    {
        return pair.second == target;
    };

    auto it = std::find_if(m_renderTargets.begin(), m_renderTargets.end(), predDelete);
    if (it != m_renderTargets.end())
    {
        m_renderTargets.erase(it);
    }
}

void CRenderTargetManager::unloadAll()
{
    m_renderTargets.clear();
}

RenderTargetIter CRenderTargetManager::begin()
{
    return m_renderTargets.begin();
}

RenderTargetCIter CRenderTargetManager::begin() const
{
    return m_renderTargets.cbegin();
}

RenderTargetIter CRenderTargetManager::end()
{
    return m_renderTargets.end();
}

RenderTargetCIter CRenderTargetManager::end() const
{
    return m_renderTargets.end();
}
