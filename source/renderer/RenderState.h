#ifndef _V3D_RENDER_STATE_H_
#define _V3D_RENDER_STATE_H_

#include "common.h"

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

    enum EPolygonMode
    {
        ePolyModeFill,
        ePolyModeLine,
        ePolyModePoint,

        eModeCount
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

    class CRenderState
    {
    public:

        CRenderState();
        virtual                   ~CRenderState();

        bool                      getCullFace()       const;
        bool                      getBlend()          const;
        EWinding                  getWinding()        const;
        EPolygonMode              getPolygonMode()    const;
        EBlendFactor              getBlendFactorSrc() const;
        EBlendFactor              getBlendFactorDst() const;
                                  
        void                      setCullFace(bool use);
        void                      setBlend(bool use);
        void                      setWinding(EWinding type);
        void                      setPolygonMode(EPolygonMode type);
        void                      setBlendFactors(EBlendFactor dst, EBlendFactor src);

        virtual void              bind() = 0;

        static EPolygonMode       getPolygonModeByName(const std::string& name);
        static EBlendFactor       getBlendFactorByName(const std::string& name);

    protected:

        bool                      m_cullFace;
        EWinding                  m_winding;
        EPolygonMode              m_polygonMode;

        EBlendFactor              m_blendSrc;
        EBlendFactor              m_blendDst;
        bool                      m_blend;

    private:

        static const std::string  s_renderPolygonMode[EPolygonMode::eModeCount];
        static const std::string  s_blendFactor[EBlendFactor::eBlendCount];

    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<CRenderState> RenderStatePtr;

    /////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_RENDER_STATE_H_
