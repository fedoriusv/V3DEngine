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
        ePoints,
        ePatches,

        ePrimitivesTopologyCount
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    class RenderPass;


    /**
    * RenderState class
    * Client side
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

        u32                         getCountInstance() const;
        EPrimitivesTopology         getPrimitiveTopology() const;
        const core::Point2DU&       getDrawInterval()  const;
        u32                         getCountPatches()  const;
        f32                         getPatchInnerLevel() const;
        f32                         getPatchOuterLevel() const;
                              
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


        void                        setPrimitiveTopology(EPrimitivesTopology topology);
        void                        setDrawInterval(u32 begin, u32 count);
        void                        setCountInstance(u32 count);
        void                        setCountPatches(u32 patches);
        void                        setPatchLevel(f32 inner, f32 outer);

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
        core::Point2DU              m_interval;

        u32                         m_patches;
        f32                         m_patchInnerLevel;
        f32                         m_patchOuterLevel;

        bool                        m_isChanged;

    private:

        friend                      RenderPass;

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
