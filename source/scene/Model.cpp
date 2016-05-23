#include "Model.h"
#include "Mesh.h"
#include "Camera.h"
#include "Light.h"
#include "ModelManager.h"
#include "RenderTechniqueManager.h"
#include "utils/Logger.h"
#include "stream/StreamManager.h"
#include "renderer/Material.h"

using namespace v3d;
using namespace scene;
using namespace renderer;
using namespace stream;

CModel::CModel()
{
    m_nodeType = ENodeType::eModel;
    LOG_INFO("CModel: Create node type: %s", getNodeNameByType(m_nodeType).c_str());
}

CModel::CModel(const CModel& model)
: CNode(model)
{
    m_nodeType = model.m_nodeType;
    LOG_INFO("CModel: Clone node type: %s", getNodeNameByType(m_nodeType).c_str());
}

CModel::~CModel()
{
    LOG_INFO("Delete node type: %s", getNodeNameByType(m_nodeType).c_str());
    for (NodeList::const_iterator iter = m_nodeList.cbegin(); iter < m_nodeList.cend(); ++iter)
    {
        CNode* node = (*iter);
        
        delete node;
        node = nullptr;
    }

    m_nodeList.clear();
}

void CModel::init()
{
    if (m_initialiazed)
    {
        return;
    }

    for (NodeList::const_iterator iter = m_nodeList.cbegin(); iter < m_nodeList.cend(); ++iter)
    {
        CNode* node = (*iter);
        (*iter)->init();
    }

    m_initialiazed = true;
}

void CModel::update(s32 dt)
{
    if (!CNode::isVisible() || !m_initialiazed)
    {
        return;
    }

    CNode::update(dt);
}

void CModel::init(const stream::IStreamPtr& stream)
{
    IResource::setStream(stream);
}

bool CModel::load()
{
    const stream::IStreamPtr& stream = IResource::getStream();
    if (!stream)
    {
        LOG_ERROR("CModel: Empty Stream with name [%s] form File", IResource::getResourseName().c_str());
        return false;
    }

    if (stream->size() > 0)
    {
        stream->seekBeg(0);

        stream->read(m_id);
        stream->read(m_name);

        m_nodeList.clear();

        u32 nodesCount;
        stream->read(nodesCount);
        for (u32 index = 0; index < nodesCount; ++index)
        {
            u32 nodetype;
            stream->read(nodetype);

            u32 nodeStreamSize;
            stream->read(nodeStreamSize);

            MemoryStreamPtr nodeStream = CStreamManager::createMemoryStream(nullptr, nodeStreamSize);
            stream->read(nodeStream->getData(), sizeof(u8), nodeStreamSize);

            CNode* node = nullptr;
            switch (nodetype)
            {
                case eMesh:
                {
                    node = new CMesh();
                    static_cast<CMesh*>(node)->init(nodeStream);
                
                    std::string materialName;
                    stream->read(materialName);
                    if (!materialName.empty())
                    {
                        u32 materialStreamSize;
                        stream->read(materialStreamSize);

                        MemoryStreamPtr materialStream = CStreamManager::createMemoryStream(nullptr, materialStreamSize);
                        stream->read(materialStream->getData(), sizeof(u8), materialStreamSize);

                        const MaterialPtr& material = static_cast<CMesh*>(node)->getMaterial();
                        material->init(materialStream);
                    }
                }
                    break;

                case eLight:
                {
                    node = new CLight();
                    static_cast<CLight*>(node)->init(nodeStream);
                }
                    break;

                case eCamera:
                {
                    node = new CCamera();
                    static_cast<CCamera*>(node)->init(nodeStream);
                }
                    break;

            default:
                break;
            }

            if (node)
            {
                m_nodeList.push_back(node);
            }
        }

        u32 parentListSize;
        stream->read(parentListSize);

        std::vector<s32> parentList(parentListSize);
        stream->read(parentList.data(), sizeof(s32), parentListSize);

        //Parents
        u32 index = 0;
        for (NodeList::iterator iter = m_nodeList.begin(); iter < m_nodeList.end(); ++iter)
        {
            CNode* node = (*iter);
            s32 parentIdx = parentList[index];
            if (parentIdx >= 0)
            {
                auto predHaveParent = [parentIdx](const CNode* node) -> bool
                {
                    return parentIdx == node->getID();
                };

                auto parentIter = std::find_if(m_nodeList.begin(), m_nodeList.end(), predHaveParent);
                node->setParent(parentIter != m_nodeList.end() ? (*parentIter) : this);
            }
            else
            {
                node->setParent(this);
            }
            ++index;
        }

        return true;
    }

    return false;
}

bool CModel::setRenderTechniqueForAllMeshes(const std::string& file)
{
    const CRenderTechnique* technique = scene::CRenderTechniqueManager::getInstance()->load(file);
    if (!technique)
    {
        LOG_ERROR("CModel: Error read file [%s]", file.c_str());
        return false;
    }

    for (NodeList::const_iterator iter = m_nodeList.cbegin(); iter < m_nodeList.cend(); ++iter)
    {
        CNode* node = (*iter);
        switch (node->getNodeType())
        {
            case eMesh:
                static_cast<CMesh*>(node)->setRenderTechnique(technique);
                break;

            default:
                break;
        }
    }

    return true;
}

const NodeList& CModel::getNodeList() const
{
    return m_nodeList;
}

CModel* CModel::clone()
{
    CModel* model = new CModel(*this);
    model->init(IResource::getStream());
    if (!model->load())
    {
        LOG_ERROR("CModel: Can't load model stream");
        ASSERT(false, "CModel: Can't load model stream");

        delete model;
        model = nullptr;
    }

    return model;
}
