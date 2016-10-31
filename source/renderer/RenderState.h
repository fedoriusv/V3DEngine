#ifndef _V3D_RENDER_STATE_H_
#define _V3D_RENDER_STATE_H_

#include "common.h"

namespace tinyxml2
{
    class XMLElement;
}

namespace v3d
{
namespace renderer
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    enum EWinding
    {
        eWindingCW,
        eWindingCCW
    };

    //////////////////////////////////////////////////////////////////////////////////////////////////////

    enum ECompareFunc
    {
        eCmpNever,
        eCmpLess,
        eCmpLequal,
        eCmpEqual,
        eCmpGequal,
        eCmpGreater,
        eCmpNotEqual,
        eCmpAlways,

        eCompareCount
    };

    //////////////////////////////////////////////////////////////////////////////////////////////////////

    enum EPolygonMode
    {
        ePolyModeFill,
        ePolyModeLine,
        ePolyModePoint,

        ePolygonModeCount
    };

    //////////////////////////////////////////////////////////////////////////////////////////////////////

    enum ECullMode
    {
        eCullNone,
        eFaceFront,
        eFaceBack,
        eFaceFrontAndBack, 

        eCullModeCount
    };

    //////////////////////////////////////////////////////////////////////////////////////////////////////

    enum EBlendEquation
    {
        eFuncAdd,
        eFuncSubtract,
        eFuncReverseSubtract,
        eFuncMin,
        eFuncMax,

        eFuncBlendEquationCount
    };

    //////////////////////////////////////////////////////////////////////////////////////////////////////

    enum EBlendFactor
    {
        eBlendZero,
        eBlendOne,
        eBlendSrcColor,
        eBlendInvSrcColor,
        eBlendDstColor,
        eBlendInvDstColor,
        eBlendSrcAlpha,
        eBlendInvSrcAlpha,
        eBlendDstAlpha,
        eBlendInvDstAlpha,
        eBlendConstantColor,
        eBlendConstantInvColor,
        eBlendConstantAlpha,
        eBlendConstantInvAlpha,
        eBlendSrcAplhaSaturated,

        eBlendFactorCount
    };

    //////////////////////////////////////////////////////////////////////////////////////////////////////

    enum EPrimitivesTopology
    {
        eTriangles,
        eTriangleStrip,
        eTriangleFan,
        eLines,
        eLinesStrip,
        eLinesLoop,
        ePoints,
        ePatches,

        ePrimitivesTopologyCount
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    class CRenderPass;


    /**
    * RenderState class
    */
    class RenderState
    {
    public:

        RenderState();
        RenderState(const RenderState& state);
        RenderState& operator=(const RenderState& state);
        
        virtual                     ~RenderState();

        ECullMode                   getCullface() const;
        bool                        isCulling() const;
        bool                        isBlending() const;
        EWinding                    getWinding() const;
        EPolygonMode                getPolygonMode() const;
        EBlendFactor                getBlendColorFactorSrc() const;
        EBlendFactor                getBlendColorFactorDst() const;
        EBlendFactor                getBlendAlphaFactorSrc() const;
        EBlendFactor                getBlendAlphaFactorDst() const;
        EBlendEquation              getBlendColorEquation() const;
        EBlendEquation              getBlendAlphaEquation() const;
        bool                        getDepthWrite() const;
        bool                        getDepthTest() const;
        ECompareFunc                getDepthfunc() const;
        bool                        getStencilTest() const;
        bool                        isRasterizerEnable() const;
                              
        void                        setCullface(ECullMode mode);
        void                        setCulling(bool enable);
        void                        setBlending(bool enable);
        void                        setWinding(EWinding type);
        void                        setPolygonMode(EPolygonMode type);
        void                        setBlendColorFactors(EBlendFactor src, EBlendFactor dst);
        void                        setBlendAlphaFactors(EBlendFactor src, EBlendFactor dst);
        void                        setBlendColorEquation(EBlendEquation type);
        void                        setBlendAlphaEquation(EBlendEquation type);
        void                        setDepthWrite(bool enable);
        void                        setDepthTest(bool enable);
        void                        setDepthFunc(ECompareFunc func);
        void                        setStencilTest(bool enable);
        void                        setRasterizerEnable(bool enable);

        virtual void                bind() = 0;

        static EPolygonMode         getPolygonModeByName(const std::string& name);
        static EBlendFactor         getBlendFactorByName(const std::string& name);
        static ECullMode            getCullFaceByName(const std::string& name);
        static ECompareFunc         getCompareFuncByName(const std::string& name);
        static EBlendEquation       getCompareBlendEquationByName(const std::string& name);
        static const std::string&   getStringByPrimitivesTopology(EPrimitivesTopology type);
        static EPrimitivesTopology  getPrimitivesTopologyByString(const std::string& name);

    protected:

        bool                        m_culling;
        ECullMode                   m_cullface;
        EWinding                    m_winding;
        EPolygonMode                m_polygonMode;

        EBlendFactor                m_blendColorSrc;
        EBlendFactor                m_blendColorDst;
        EBlendEquation              m_blendColorEquation;

        EBlendFactor                m_blendAlphaSrc;
        EBlendFactor                m_blendAlphaDst;
        EBlendEquation              m_blendAlphaEquation;

        bool                        m_blend;

        bool                        m_depthWrite;
        bool                        m_depthTest;
        ECompareFunc                m_depthFunc;

        bool                        m_stencilTest;

        u32                         m_rasterizationSamples;
        bool                        m_isRasterizerEnable;

        EPrimitivesTopology         m_topology;
        u32                         m_instanced;

        bool                        m_isChanged;

    private:

        friend                      CRenderPass;

        bool                        parse(const tinyxml2::XMLElement* root);

        static const std::string    s_typeName[EPrimitivesTopology::ePrimitivesTopologyCount];
        static const std::string    s_renderPolygonMode[EPolygonMode::ePolygonModeCount];
        static const std::string    s_blendFactor[EBlendFactor::eBlendFactorCount];
        static const std::string    s_cullface[ECullMode::eCullModeCount];
        static const std::string    s_comparefunc[ECompareFunc::eCompareCount];
        static const std::string    s_blendEquation[EBlendEquation::eFuncBlendEquationCount];
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<RenderState> RenderStatePtr;

    /////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace renderer
} //namespace v3d

#endif //_V3D_RENDER_STATE_H_
