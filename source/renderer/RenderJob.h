#ifndef _V3D_RENDERJOB_H_
#define _V3D_RENDERJOB_H_

#include "common.h"
#include "Material.h"
#include "Geometry.h"

namespace v3d
{
namespace renderer
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    using PassIndexIterConst = std::vector<u32>::const_iterator;

    class Renderable;

    /**
    * Render job class
    */
    class CRenderJob
    {
    public:

        CRenderJob(const Renderable* renderable, const core::Matrix4D& transform);
        ~CRenderJob();

        const MaterialPtr&      getMaterial() const;
        MaterialPtr&            getMaterial();

        const CRenderTechnique* getRenderTechique() const;
        CRenderTechnique*       getRenderTechique();

        const GeometryPtr&      getGeometry() const;

        const core::Matrix4D&   getTransform() const;

        void                    setRenderTarget(u32 target);
        u32                     getRenderTarget() const;

        void                    addRenderPassIndex(u32 pass);
        void                    clearRenderPassIndexList();

        PassIndexIterConst      renderPassIndexBegin() const;
        PassIndexIterConst      renderPassIndexEnd() const;

        void                    setTransform(const core::Matrix4D& transform);

    private:

        const Renderable*       m_renderable;

        core::Matrix4D          m_transform;

        u32                     m_targetIndex;
        std::vector<u32>        m_passIndexList;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<CRenderJob> RenderJobPtr;

    /////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace renderer
} //namespace v3d

#endif //_V3D_RENDERJOB_H_
