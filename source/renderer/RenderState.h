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

    enum ECompareFunc
    {
        eCmpLess,
        eCmpLequal,
        eCmpEqual,
        eCmpGequal,
        eCmpGreater,
        eCmpNotequal,
        eCmpAlways,
        eCmpNever,

        eCompareCount
    };

    enum EPolygonMode
    {
        ePolyModeFill,
        ePolyModeLine,
        ePolyModePoint,

        ePolygonModeCount
    };

    enum ECullMode
    {
        eCullNone,
        eFaceFront,
        eFaceBack,
        eFaceFrontAndBack, 

        eCullModeCount
    };

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
        eBlendSrcAplhaSaturated,

        eBlendCount
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
        
        virtual                   ~RenderState();

        ECullMode                 getCullface() const;
        bool                      isCulling() const;
        bool                      isBlend() const;
        EWinding                  getWinding() const;
        EPolygonMode              getPolygonMode() const;
        EBlendFactor              getBlendFactorSrc() const;
        EBlendFactor              getBlendFactorDst() const;
        bool                      getDepthWrite() const;
        bool                      getDepthTest() const;
        ECompareFunc              getDepthfunc() const;
        bool                      isRasterizerEnable() const;
                              
        void                      setCullface(ECullMode mode);
        void                      setCulling(bool enable);
        void                      setBlend(bool enable);
        void                      setWinding(EWinding type);
        void                      setPolygonMode(EPolygonMode type);
        void                      setBlendFactors(EBlendFactor dst, EBlendFactor src);
        void                      setDepthWrite(bool enable);
        void                      setDepthTest(bool enable);
        void                      setDepthFunc(ECompareFunc func);
        void                      setRasterizerEnable(bool enable);

        virtual void              bind() = 0;

        static EPolygonMode       getPolygonModeByName(const std::string& name);
        static EBlendFactor       getBlendFactorByName(const std::string& name);
        static ECullMode          getCullFaceByName(const std::string& name);
        static ECompareFunc       getCompareFuncByName(const std::string& name);

    protected:

        bool                      m_culling;
        ECullMode                 m_cullface;
        EWinding                  m_winding;
        EPolygonMode              m_polygonMode;

        EBlendFactor              m_blendSrc;
        EBlendFactor              m_blendDst;
        bool                      m_blend;

        bool                      m_depthWrite;
        bool                      m_depthTest;
        ECompareFunc              m_depthFunc;

        bool                      m_isChanged;

        bool                      m_isRasterizerEnable;

    private:

        friend                    CRenderPass;

        bool                      parse(const tinyxml2::XMLElement* root);

        static const std::string  s_renderPolygonMode[EPolygonMode::ePolygonModeCount];
        static const std::string  s_blendFactor[EBlendFactor::eBlendCount];
        static const std::string  s_cullface[ECullMode::eCullModeCount];
        static const std::string  s_comparefunc[ECompareFunc::eCompareCount];
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<RenderState> RenderStatePtr;

    /////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace renderer
} //namespace v3d

#endif //_V3D_RENDER_STATE_H_
