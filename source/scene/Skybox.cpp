#include "Skybox.h"
#include "Engine.h"
#include "TextureManager.h"
#include "utils/Logger.h"
#include "renderer/Material.h"

using namespace v3d;
using namespace scene;
using namespace renderer;

CSkybox::CSkybox()
{
    m_nodeType = ENodeType::eSkyBox;
    LOG_INFO("Create node type: %s", getNodeNameByType(m_nodeType).c_str());

    CRenderable::setMaterial(new CMaterial());
}

CSkybox::~CSkybox()
{
    CRenderable::getGeometry()->free();
    m_textures.clear();
}

void CSkybox::load(const std::string files[6])
{
    m_textures.resize(k_countSize);
    for (u32 i = 0; i < k_countSize; ++i)
    {
        const CTexture* texure = CTextureManager::getInstance()->load(files[i]);
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
    f32 s = k_extend;
    SVertexData& data = CRenderable::getGeometry()->getData();

    data._vertices =
    {
        { -s, s, -s }, { -s, -s, -s }, { s, s, -s  }, { -s, -s, -s}, { s, s, -s  }, { s, -s, -s},
        { -s, s, s  }, { -s, -s, s  }, { s, s, s   }, { -s, -s, s }, { s, s, s   }, { s, -s, s },
        { -s, s, -s }, { -s, s, s   }, { -s, -s, -s}, { -s, s, s  }, { -s, -s, -s}, { -s, -s, s},
        { s, s, -s  }, { s, s, s    }, { s, -s, -s }, { s, s, s   }, { s, -s, -s }, { s, -s, s },
        { -s, s, s  }, { -s, s, -s  }, { s, s, s   }, { -s, s, -s }, { s, s, s   }, { s, s, -s },
        { -s, -s, s }, { -s, -s, -s }, { s, -s, s  }, { -s, -s, -s}, { s, -s, s  }, { s, -s, -s}
    };

    data._normals =
    {
        { 0.0f, 0.0f, 1.0f  }, { 0.0f, 0.0f, 1.0f  }, { 0.0f, 0.0f, 1.0f  }, { 0.0f, 0.0f, 1.0f  }, { 0.0f, 0.0f, 1.0f  }, { 0.0f, 0.0f, 1.0f  },
        { 0.0f, 0.0f, -1.0f }, { 0.0f, 0.0f, -1.0f }, { 0.0f, 0.0f, -1.0f }, { 0.0f, 0.0f, -1.0f }, { 0.0f, 0.0f, -1.0f }, { 0.0f, 0.0f, -1.0f },
        { 1.0f, 0.0f, 0.0f  }, { 1.0f, 0.0f, 0.0f  }, { 1.0f, 0.0f, 0.0f  }, { 1.0f, 0.0f, 0.0f  }, { 1.0f, 0.0f, 0.0f  }, { 1.0f, 0.0f, 0.0f  },
        { -1.0f, 0.0f, 0.0f }, { -1.0f, 0.0f, 0.0f }, { -1.0f, 0.0f, 0.0f }, { -1.0f, 0.0f, 0.0f }, { -1.0f, 0.0f, 0.0f }, { -1.0f, 0.0f, 0.0f },
        { 0.0f, -1.0f, 0.0f }, { 0.0f, -1.0f, 0.0f }, { 0.0f, -1.0f, 0.0f }, { 0.0f, -1.0f, 0.0f }, { 0.0f, -1.0f, 0.0f }, { 0.0f, -1.0f, 0.0f },
        { 0.0f, 1.0f, 0.0f  }, { 0.0f, 1.0f, 0.0f  }, { 0.0f, 1.0f, 0.0f  }, { 0.0f, 1.0f, 0.0f  }, { 0.0f, 1.0f, 0.0f  }, { 0.0f, 1.0f, 0.0f  },
    };

    data._texCoords.resize(1);
    data._texCoords[0] =
    {
        { 0.0f, 0.0f }, { 0.0f, 1.0f }, { 1.0f, 0.0f }, { 0.0f, 1.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f },
        { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 0.0f }, { 0.0f, 1.0f },
        { 1.0f, 0.0f }, { 0.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f },
        { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 0.0f, 1.0f }, { 1.0f, 0.0f }, { 0.0f, 1.0f }, { 1.0f, 1.0f },
        { 0.0f, 1.0f }, { 1.0f, 1.0f }, { 0.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 0.0f }, { 1.0f, 0.0f },
        { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 0.0f, 1.0f }, { 1.0f, 0.0f }, { 0.0f, 1.0f }, { 1.0f, 1.0f },
    };
}

void CSkybox::init()
{
    if (m_initialiazed)
    {
        return;
    }

    const CRenderTechnique* technique = CRenderable::getMaterial()->getRenderTechique();
    if (!technique)
    {
        LOG_ERROR("CSkybox: Do not exist RenderTechique");
        ASSERT(false && "CSkybox: Do not exist RenderTechique");
        return;
    }

    CRenderable::setGeometry(RENDERER->makeSharedGeometry(technique));
    CRenderable::setRenderJob(std::make_shared<CRenderJob>(CRenderable::getMaterial(), CRenderable::getGeometry(), CNode::getAbsTransform()));

    CSkybox::build();
    CRenderable::getGeometry()->setDrawMode(CGeometry::eTriangles);

    CRenderable::getGeometry()->init();
    m_initialiazed = true;
}

void CSkybox::update(s32 dt)
{
    if (!CNode::isVisible() || !m_initialiazed)
    {
        return;
    }

    CNode::update(dt);
    CRenderable::getRenderJob()->setTransform(CNode::getAbsTransform());
}

void CSkybox::render()
{
    if (!CNode::isVisible() || !m_initialiazed)
    {
        return;
    }

    for (u32 i = 0; i < k_countSize; ++i)
    {
        CRenderable::getGeometry()->setInterval((i * 6), 6);
        const MaterialPtr& material = CRenderable::getMaterial();
        material->setTexture(0, m_textures[i]);

        CRenderable::render();
    }
}