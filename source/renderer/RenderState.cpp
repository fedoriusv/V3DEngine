#include "RenderState.h"
#include "utils/Logger.h"

#include "tinyxml2.h"

namespace v3d
{
namespace renderer
{

const std::string RenderState::s_renderPolygonMode[ePolygonModeCount] =
{
    "fill",
    "line",
    "point"
};

EPolygonMode RenderState::getPolygonModeByName(const std::string& name)
{
    for (u32 i = 0; i < ePolygonModeCount; ++i)
    {
        if (s_renderPolygonMode[i].compare(name) == 0)
        {
            return (EPolygonMode)i;
        }
    }

    return ePolyModeFill;
}

const std::string RenderState::s_blendFactor[eBlendFactorCount] =
{
    "zero",
    "one",
    "srcColor",
    "invSrcColor",
    "dstColor",
    "invDstColor",
    "srcAlpha",
    "invSrcAlpha",
    "dstAlpha",
    "invDstAlpha",
    "constColor",
    "invConstColor",
    "constAlpha",
    "invConstAlpha",
    "srcAplhaSaturated"
};

EBlendFactor RenderState::getBlendFactorByName(const std::string& name)
{
    for (u32 i = 0; i < eBlendFactorCount; ++i)
    {
        if (s_blendFactor[i].compare(name) == 0)
        {
            return (EBlendFactor)i;
        }
    }

    return eBlendZero;
}

const std::string RenderState::s_cullface[eCullModeCount] =
{
    "none",
    "front",
    "back",
    "frontAndBack"
};

ECullMode RenderState::getCullFaceByName(const std::string& name)
{
    for (u32 i = 0; i < eCullModeCount; ++i)
    {
        if (s_cullface[i].compare(name) == 0)
        {
            return (ECullMode)i;
        }
    }

    return eFaceBack;
}

const std::string RenderState::s_comparefunc[eCompareCount] =
{
    "never",
    "less",
    "lequal",
    "equal",
    "gequal",
    "greater",
    "notequal",
    "always"
};

ECompareFunc RenderState::getCompareFuncByName(const std::string& name)
{
    for (u32 i = 0; i < eCompareCount; ++i)
    {
        if (s_comparefunc[i].compare(name) == 0)
        {
            return (ECompareFunc)i;
        }
    }

    return eCmpLequal;
}

const std::string RenderState::s_blendEquation[eFuncBlendEquationCount] =
{
    "add",
    "substract",
    "reverseSubstract",
    "min",
    "max"
};

EBlendEquation RenderState::getCompareBlendEquationByName(const std::string& name)
{
    for (u32 i = 0; i < eFuncBlendEquationCount; ++i)
    {
        if (s_blendEquation[i].compare(name) == 0)
        {
            return (EBlendEquation)i;
        }
    }

    return eFuncAdd;
}

const std::string RenderState::s_typeName[EPrimitivesTopology::ePrimitivesTopologyCount] =
{
    "triangles",
    "trianglestrip",
    "trianglefan",
    "lines",
    "linesstrip",
    "linesloop",
    "points",
    "patches"
};

const std::string& RenderState::getStringByPrimitivesTopology(EPrimitivesTopology type)
{
    return s_typeName[type];
}

EPrimitivesTopology RenderState::getPrimitivesTopologyByString(const std::string& name)
{
    for (u32 i = 0; i < EPrimitivesTopology::ePrimitivesTopologyCount; ++i)
    {
        if (s_typeName[i] == name)
        {
            return (EPrimitivesTopology)i;
        }
    }

    return EPrimitivesTopology::eTriangles;
}

RenderState::RenderState()
    : m_culling(true)
    , m_cullface(eFaceBack)
    , m_winding(eWindingCCW)
    , m_polygonMode(ePolyModeFill)

    , m_blendColorSrc(eBlendSrcColor)
    , m_blendColorDst(eBlendDstColor)
    , m_blendColorEquation(eFuncAdd)
    , m_blendAlphaSrc(eBlendSrcAlpha)
    , m_blendAlphaDst(eBlendDstAlpha)
    , m_blendAlphaEquation(eFuncAdd)
    , m_blend(false)

    , m_depthWrite(true)
    , m_depthTest(true)
    , m_depthFunc(eCmpLequal)
    , m_stencilTest(false)

    , m_rasterizationSamples(1U)
    , m_isRasterizerEnable(true)

    , m_topology(eTriangles)
    , m_instanced(1U)

    , m_isChanged(true)
{
}

RenderState::RenderState(const RenderState& state)
    : m_culling(state.m_culling)
    , m_cullface(state.m_cullface)
    , m_winding(state.m_winding)
    , m_polygonMode(state.m_polygonMode)

    , m_blendColorSrc(state.m_blendColorSrc)
    , m_blendColorDst(state.m_blendColorDst)
    , m_blendColorEquation(state.m_blendColorEquation)
    , m_blendAlphaSrc(state.m_blendAlphaSrc)
    , m_blendAlphaDst(state.m_blendAlphaDst)
    , m_blendAlphaEquation(state.m_blendAlphaEquation)
    , m_blend(state.m_blend)

    , m_depthWrite(state.m_depthWrite)
    , m_depthTest(state.m_depthTest)
    , m_depthFunc(state.m_depthFunc)
    , m_stencilTest(state.m_stencilTest)

    , m_rasterizationSamples(state.m_rasterizationSamples)
    , m_isRasterizerEnable(state.m_isRasterizerEnable)

    , m_isChanged(true)
{
}

RenderState& RenderState::operator=(const RenderState& state)
{
    if (&state == this)
    {
        return *this;
    }

    m_culling = state.m_culling;
    m_cullface = state.m_cullface;
    m_winding = state.m_winding;
    m_polygonMode = state.m_polygonMode;

    m_blendColorSrc = state.m_blendColorSrc;
    m_blendColorDst = state.m_blendColorDst;
    m_blendColorEquation = state.m_blendColorEquation;
    m_blendAlphaSrc = state.m_blendAlphaSrc;
    m_blendAlphaDst = state.m_blendAlphaDst;
    m_blendAlphaEquation = state.m_blendAlphaEquation;
    m_blend = state.m_blend;

    m_depthWrite = state.m_depthWrite;
    m_depthTest = state.m_depthTest;
    m_depthFunc = state.m_depthFunc;
    m_stencilTest = state.m_stencilTest;

    m_rasterizationSamples = state.m_rasterizationSamples;
    m_isRasterizerEnable = state.m_isRasterizerEnable;

    m_isChanged = true;

    return *this;
}

RenderState::~RenderState()
{
}

bool RenderState::isCulling() const
{
    return m_culling;
}

EWinding RenderState::getWinding() const
{
    return m_winding;
}

EPolygonMode RenderState::getPolygonMode() const
{
    return m_polygonMode;
}

EBlendFactor RenderState::getBlendColorFactorSrc() const
{
    return m_blendColorSrc;
}

EBlendFactor RenderState::getBlendColorFactorDst() const
{
    return m_blendColorDst;
}

EBlendFactor RenderState::getBlendAlphaFactorSrc() const
{
    return m_blendAlphaSrc;
}

EBlendFactor RenderState::getBlendAlphaFactorDst() const
{
    return m_blendAlphaDst;
}

EBlendEquation RenderState::getBlendColorEquation() const
{
    return m_blendColorEquation;
}

EBlendEquation RenderState::getBlendAlphaEquation() const
{
    return m_blendAlphaEquation;
}

void RenderState::setCulling(bool enable)
{
    if (m_culling != enable)
    {
        m_culling = enable;
        m_isChanged = true;
    }
}

void RenderState::setWinding(EWinding type)
{
    if (m_winding != type)
    {
        m_winding = type;
        m_isChanged = true;
    }
}

void RenderState::setPolygonMode(EPolygonMode type)
{
    if (m_polygonMode != type)
    {
        m_polygonMode = type;
        m_isChanged = true;
    }
}

void RenderState::setBlending(bool enable)
{
    if (m_blend != enable)
    {
        m_blend = enable;
        m_isChanged = true;
    }
}

bool RenderState::isBlending() const
{
    return m_blend;
}

ECullMode RenderState::getCullface() const
{
    return m_cullface;
}

bool RenderState::getDepthWrite() const
{
    return m_depthWrite;
}

bool RenderState::getDepthTest() const
{
    return m_depthTest;
}

ECompareFunc RenderState::getDepthfunc() const
{
    return m_depthFunc;
}

bool RenderState::getStencilTest() const
{
    return m_stencilTest;
}

bool RenderState::isRasterizerEnable() const
{
    return m_isRasterizerEnable;
}

void RenderState::setCullface(ECullMode mode)
{
    if (m_cullface != mode)
    {
        m_cullface = mode;
        m_isChanged = true;
    }
}

void RenderState::setBlendColorFactors(EBlendFactor src, EBlendFactor dst)
{
    if (m_blendColorDst != dst || m_blendColorSrc != src)
    {
        m_blendColorDst = dst;
        m_blendColorSrc = src;
        m_isChanged = true;
    }
}

void RenderState::setBlendAlphaFactors(EBlendFactor src, EBlendFactor dst)
{
    if (m_blendAlphaDst != dst || m_blendAlphaSrc != src)
    {
        m_blendAlphaDst = dst;
        m_blendAlphaSrc = src;
        m_isChanged = true;
    }
}

void RenderState::setBlendColorEquation(EBlendEquation type)
{
    if (m_blendColorEquation != type)
    {
        m_blendColorEquation = type;
        m_isChanged = true;
    }
}

void RenderState::setBlendAlphaEquation(EBlendEquation type)
{
    if (m_blendAlphaEquation != type)
    {
        m_blendAlphaEquation = type;
        m_isChanged = true;
    }
}

void RenderState::setDepthWrite(bool enable)
{
    if (m_depthWrite != enable)
    {
        m_depthWrite = enable;
        m_isChanged = true;
    }
}

void RenderState::setDepthTest(bool enable)
{
    if (m_depthTest != enable)
    {
        m_depthTest = enable;
        m_isChanged = true;
    }
}

void RenderState::setDepthFunc(ECompareFunc func)
{
    if (m_depthFunc != func)
    {
        m_depthFunc = func;
        m_isChanged = true;
    }
}

void RenderState::setStencilTest(bool enable)
{
    if (m_stencilTest != enable)
    {
        m_stencilTest = enable;
        m_isChanged = true;
    }
}

void RenderState::setRasterizerEnable(bool enable)
{
    if (m_isRasterizerEnable != enable)
    {
        m_isRasterizerEnable = enable;
        m_isChanged = true;
    }
}

bool RenderState::parse(const tinyxml2::XMLElement* root)
{
    if (!root)
    {
        LOG_ERROR("CRenderState: Not exist xml renderstate element");
        return false;
    }

    const tinyxml2::XMLElement* polygonmodeElement = root->FirstChildElement("polygonmode");
    if (polygonmodeElement)
    {
        if (polygonmodeElement->Attribute("val"))
        {
            const std::string polygonModeStr = polygonmodeElement->Attribute("val");
            EPolygonMode polygonMode = RenderState::getPolygonModeByName(polygonModeStr);
            RenderState::setPolygonMode(polygonMode);
        }
    }


    const tinyxml2::XMLElement* windingElement = root->FirstChildElement("winding");
    if (windingElement)
    {
        if (windingElement->Attribute("val"))
        {
            const std::string windingStr = windingElement->Attribute("val");
            EWinding  winding = (windingStr == "ccw") ? eWindingCCW : eWindingCW;
            RenderState::setWinding(winding);
        }
    }

    const tinyxml2::XMLElement* cullfaceElement = root->FirstChildElement("culling");
    if (cullfaceElement)
    {
        if (cullfaceElement->Attribute("val"))
        {
            const std::string cullfaceStr = cullfaceElement->Attribute("val");
            if (cullfaceStr == "false")
            {
                RenderState::setCulling(false);
            }
            ECullMode cullface = RenderState::getCullFaceByName(cullfaceStr);
            RenderState::setCullface(cullface);
        }
    }

    const tinyxml2::XMLElement* blendElement = root->FirstChildElement("blending");
    if (blendElement)
    {
        bool blend = blendElement->BoolAttribute("val");
        RenderState::setBlending(blend);

        {
            EBlendEquation blendColorEquation = eFuncAdd;
            if (blendElement->Attribute("colorOp"))
            {
                const std::string blendStr = blendElement->Attribute("colorOp");
                blendColorEquation = RenderState::getCompareBlendEquationByName(blendStr);
            }

            RenderState::setBlendColorEquation(blendColorEquation);

            EBlendEquation blendAlphaEquation = eFuncAdd;
            if (blendElement->Attribute("alphaOp"))
            {
                const std::string blendStr = blendElement->Attribute("alphaOp");
                blendAlphaEquation = RenderState::getCompareBlendEquationByName(blendStr);
            }

            RenderState::setBlendAlphaEquation(blendAlphaEquation);
        }

        {
            EBlendFactor blendColorSrc = eBlendSrcColor;
            if (blendElement->Attribute("srcColor"))
            {
                const std::string blendSrcStr = blendElement->Attribute("srcColor");
                blendColorSrc = RenderState::getBlendFactorByName(blendSrcStr);
            }

            EBlendFactor blendColorDst = eBlendDstColor;
            if (blendElement->Attribute("dstColor"))
            {
                const std::string blendDstStr = blendElement->Attribute("dstColor");
                blendColorDst = RenderState::getBlendFactorByName(blendDstStr);
            }

            RenderState::setBlendColorFactors(blendColorSrc, blendColorDst);
        }

        {
            EBlendFactor blendAlphaSrc = eBlendSrcAlpha;
            if (blendElement->Attribute("srcAlpha"))
            {
                const std::string blendSrcStr = blendElement->Attribute("srcAlpha");
                blendAlphaSrc = RenderState::getBlendFactorByName(blendSrcStr);
            }

            EBlendFactor blendAlphaDst = eBlendDstColor;
            if (blendElement->Attribute("dstAlpha"))
            {
                const std::string blendDstStr = blendElement->Attribute("dstAlpha");
                blendAlphaDst = RenderState::getBlendFactorByName(blendDstStr);
            }

            RenderState::setBlendAlphaFactors(blendAlphaSrc, blendAlphaDst);
        }
    }

    const tinyxml2::XMLElement* depthElement = root->FirstChildElement("depthWrite");
    if (depthElement)
    {
        bool depth = depthElement->BoolAttribute("val");
        RenderState::setDepthWrite(depth);
    }

    const tinyxml2::XMLElement* depthTestElement = root->FirstChildElement("depthTest");
    if (depthTestElement)
    {
        if (depthTestElement->Attribute("val"))
        {
            const std::string depthTestStr = depthTestElement->Attribute("val");
            if (depthTestStr == "false")
            {
                RenderState::setDepthTest(false);
            }

            ECompareFunc depthFunc = RenderState::getCompareFuncByName(depthTestStr);
            RenderState::setDepthFunc(depthFunc);
        }
    }

    const tinyxml2::XMLElement* instanceElement = root->FirstChildElement("instanced");
    if (instanceElement)
    {
        u32 inctance = instanceElement->UnsignedAttribute("val");
        RenderState::setCountInstance(inctance);
    }

    const tinyxml2::XMLElement* primitiveElement = root->FirstChildElement("primitive");
    if (primitiveElement && primitiveElement->Attribute("val"))
    {
        std::string primitive = primitiveElement->Attribute("val");
        EPrimitivesMode mode = GeometryType::getPrimitivesModeByString(primitive);
        if (mode == EPrimitivesMode::ePrimitivesNone)
        {
            LOG_WARNING("CRenderAdvanced: Invalid primitive mode");
        }

        RenderState::setPrimitiveMode(mode);
    }

    const tinyxml2::XMLElement* drawintervalElement = root->FirstChildElement("drawinterval");
    if (drawintervalElement)
    {
        u32 begin = drawintervalElement->UnsignedAttribute("begin");
        u32 amount = drawintervalElement->UnsignedAttribute("amount");

        RenderState::setDrawInterval(begin, amount);
    }

    const tinyxml2::XMLElement* patchesElement = root->FirstChildElement("patches");
    if (patchesElement)
    {
        u32 patches = patchesElement->UnsignedAttribute("val");
        if (patches > 0)
        {
            if (RenderState::getPrimitiveMode() != EPrimitivesMode::ePatches)
            {
                LOG_WARNING("If used patches draw mode must be 'Patches'");
                RenderState::setPrimitiveMode(EPrimitivesMode::ePatches);
            }
        }

        RenderState::setCountPatches(patches);

        f32 inner = patchesElement->FloatAttribute("inner");
        f32 outer = patchesElement->FloatAttribute("outer");
        RenderState::setPatchLevel(inner, outer);
    }

    return true;
}

} //namespace renderer
} //namespace v3d
