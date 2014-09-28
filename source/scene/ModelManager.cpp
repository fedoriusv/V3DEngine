#include "ModelManager.h"
#include "stream/FileStream.h"
#include "utils/Logger.h"
#include "resources/ModelF3DDecoder.h"

using namespace v3d;
using namespace v3d::scene;
using namespace v3d::resources;

CModelManager::CModelManager()
{
    CModelManager::registerPath("../../../../data/");
    CModelManager::registerPath("data/");

   /* const std::string ext[] = { "F3D" };
    DecoderPtr decoder = std::make_shared<CModelF3DDecoder>(ext);
    CModelManager::registerDecoder(decoder);*/
}

CModelManager::~CModelManager()
{
    m_pathes.clear();
    m_decoders.clear();
}

void CModelManager::add(const ModelPtr& model)
{
}

ModelPtr CModelManager::get(const std::string& name)
{
    return nullptr;
}

ModelPtr CModelManager::load(const std::string& name)
{
    return nullptr;
}

void CModelManager::unload(const std::string& name)
{
}

void CModelManager::unload(const ModelPtr& model)
{
}

void CModelManager::unloadAll()
{

}

void CModelManager::registerPath(const std::string& path)
{
    m_pathes.push_back(path);
}

void CModelManager::unregisterPath(const std::string& path)
{
    auto it = std::find(m_pathes.begin(), m_pathes.end(), path);
    if (it != m_pathes.end())
    {
        m_pathes.erase(std::remove(m_pathes.begin(), m_pathes.end(), *it), m_pathes.end());
    }
}

void CModelManager::registerDecoder(DecoderPtr& decoder)
{
    m_decoders.push_back(decoder);
}

void CModelManager::unregisterDecoder(DecoderPtr& decoder)
{
    auto it = std::find(m_decoders.begin(), m_decoders.end(), decoder);
    if (it != m_decoders.end())
    {
        m_decoders.erase(std::remove(m_decoders.begin(), m_decoders.end(), *it), m_decoders.end());
    }
}
