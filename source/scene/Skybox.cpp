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
}

void CSkybox::load(const std::string files[6])
{
    CTexture* texure = CTextureManager::getInstance()->load(files);
    if (!texure)
    {
        LOG_ERROR("CSkybox: Texture didn't load");
        ASSERT(false && "CSkybox: Texture didn't load");
        return;
    }

    texure->setFilterType(eLinear, eLinear);
    texure->setWrap(eClampToEdge);

    const MaterialPtr& material = CRenderable::getMaterial();
    material->setTexture(0, texure);

}

void CSkybox::load(const std::string& front, const std::string& back, const std::string& left, const std::string& right, const std::string& up, const std::string& down)
{
    std::string list[6];

    list[0] = left;
    list[1] = right;
    list[2] = up;
    list[3] = down;
    list[4] = back;
    list[5] = front;

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
        LOG_ERROR("CSkybox: RenderTechique Doesn't exist");
        ASSERT(false && "CSkybox: RenderTechique Doesn't exist");
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

    CRenderable::render();
}