#include "Skybox.h"
#include "Engine.h"
#include "TextureManager.h"
#include "utils/Logger.h"

using namespace v3d;
using namespace scene;
using namespace renderer;

CSkybox::CSkybox()
{
    m_nodeType = ENodeType::eSkyBox;
    LOG_INFO("Create node type: %s", getNodeNameByType(m_nodeType).c_str());

    m_material = std::make_shared<CMaterial>();
}

CSkybox::~CSkybox()
{
    m_geometry->free();
    m_textures.clear();
}

void CSkybox::load(const std::string files[6])
{
    m_textures.resize(k_countSize);
    for (u32 i = 0; i < k_countSize; ++i)
    {
        TexturePtr texure = CTextureManager::getInstance()->load(files[i]);
        if (!texure)
        {
            LOG_ERROR("CSkybox::load: Error load skybox. File %s", files[i].c_str());
            return;
        }

        m_textures[i] = texure;
    }

}

void CSkybox::load(const std::string& front, const std::string& back, const std::string& left, const std::string& right, const std::string& up, const std::string& down)
{
    std::string list[6];

    list[0] = front;
    list[1] = back;
    list[2] = left;
    list[3] = right;
    list[4] = up;
    list[5] = down;

    CSkybox::load(list);
}

void CSkybox::build()
{
    f32 s = 50.0f;
    u32 count = 36;

    f32 vertex[][3] =
    {
        { -s, s, -s }, { -s, -s, -s }, { s, s, -s  }, { -s, -s, -s}, { s, s, -s  }, { s, -s, -s},
        { -s, s, s  }, { -s, -s, s  }, { s, s, s   }, { -s, -s, s }, { s, s, s   }, { s, -s, s },
        { -s, s, -s }, { -s, s, s   }, { -s, -s, -s}, { -s, s, s  }, { -s, -s, -s}, { -s, -s, s},
        { s, s, -s  }, { s, s, s    }, { s, -s, -s }, { s, s, s   }, { s, -s, -s }, { s, -s, s },
        { -s, s, s  }, { -s, s, -s  }, { s, s, s   }, { -s, s, -s }, { s, s, s   }, { s, s, -s },
        { -s, -s, s }, { -s, -s, -s }, { s, -s, s  }, { -s, -s, -s}, { s, -s, s  }, { s, -s, -s}
    };

    f32 normals[][3] =
    {
        { 0.0f, 0.0f, 1.0f  }, { 0.0f, 0.0f, 1.0f  }, { 0.0f, 0.0f, 1.0f  }, { 0.0f, 0.0f, 1.0f  }, { 0.0f, 0.0f, 1.0f  }, { 0.0f, 0.0f, 1.0f  },
        { 0.0f, 0.0f, -1.0f }, { 0.0f, 0.0f, -1.0f }, { 0.0f, 0.0f, -1.0f }, { 0.0f, 0.0f, -1.0f }, { 0.0f, 0.0f, -1.0f }, { 0.0f, 0.0f, -1.0f },
        { 1.0f, 0.0f, 0.0f  }, { 1.0f, 0.0f, 0.0f  }, { 1.0f, 0.0f, 0.0f  }, { 1.0f, 0.0f, 0.0f  }, { 1.0f, 0.0f, 0.0f  }, { 1.0f, 0.0f, 0.0f  },
        { -1.0f, 0.0f, 0.0f }, { -1.0f, 0.0f, 0.0f }, { -1.0f, 0.0f, 0.0f }, { -1.0f, 0.0f, 0.0f }, { -1.0f, 0.0f, 0.0f }, { -1.0f, 0.0f, 0.0f },
        { 0.0f, -1.0f, 0.0f }, { 0.0f, -1.0f, 0.0f }, { 0.0f, -1.0f, 0.0f }, { 0.0f, -1.0f, 0.0f }, { 0.0f, -1.0f, 0.0f }, { 0.0f, -1.0f, 0.0f },
        { 0.0f, 1.0f, 0.0f  }, { 0.0f, 1.0f, 0.0f  }, { 0.0f, 1.0f, 0.0f  }, { 0.0f, 1.0f, 0.0f  }, { 0.0f, 1.0f, 0.0f  }, { 0.0f, 1.0f, 0.0f  },
    };

    f32 texCoord[][2] =
    {
        { 0.0f, 0.0f }, { 0.0f, 1.0f }, { 1.0f, 0.0f }, { 0.0f, 1.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f },
        { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 0.0f }, { 0.0f, 1.0f },
        { 1.0f, 0.0f }, { 0.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f },
        { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 0.0f, 1.0f }, { 1.0f, 0.0f }, { 0.0f, 1.0f }, { 1.0f, 1.0f },
        { 0.0f, 1.0f }, { 1.0f, 1.0f }, { 0.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 0.0f }, { 1.0f, 0.0f },
        { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 0.0f, 1.0f }, { 1.0f, 0.0f }, { 0.0f, 1.0f }, { 1.0f, 1.0f },
    };

    SVertexData& data = m_geometry->getData();
    data.malloc(count, 0);

    m_geometry->copyToVertices(vertex, count);
    m_geometry->copyToNormals(normals, count);
    m_geometry->copyToTexCoords(texCoord, 0, count);
}

void CSkybox::init()
{
    RenderTechniquePtr technique = m_material->getRenderTechique();
    if (!technique)
    {
        LOG_ERROR("CShape: Do not exist RenderTechique");
        ASSERT(false && "CShape: Do not exist RenderTechique");
        return;
    }

    m_geometry = RENDERER->makeSharedGeometry(technique);
    m_renderJob = std::make_shared<CRenderJob>(m_material, m_geometry, m_modelMatrix);

    CSkybox::build();
    m_geometry->setDrawMode(CGeometry::eTriangles);

    m_geometry->init();

    m_initialiazed = true;
}

void CSkybox::update(s32 time)
{
    if (!CNode::isVisible() || !m_initialiazed)
    {
        return;
    }

    CNode::updateTransform();
    m_renderJob->setTransform(CNode::getAbsTransform());
}

void CSkybox::render()
{
    if (!CNode::isVisible() || !m_initialiazed)
    {
        return;
    }

    for (u32 i = 0; i < k_countSize; ++i)
    {
        m_geometry->setInterval((i * 6), 6);
        m_renderJob->getMaterial()->setTexture(0, m_textures[i]);

        RENDERER->draw(m_renderJob);
    }

    RENDERER->draw(m_renderJob);
    
}

void CSkybox::setMaterial(const MaterialPtr& material)
{
    m_material = material;
}

const MaterialPtr& CSkybox::getMaterial() const
{
    return m_material;
}

const RenderJobPtr& CSkybox::getRenderJob() const
{
    return m_renderJob;
}
