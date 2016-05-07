#ifndef _V3D_RENDEREBLE_H_
#define _V3D_RENDEREBLE_H_

#include "Geometry.h"
#include "RenderJob.h"
#include "Material.h"

namespace v3d
{
namespace renderer
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    class CRenderTechnique;

    /**
    * Renderable property class
    */
    class Renderable
    {
    public:

        Renderable();
        Renderable(const Renderable&) = delete;
        Renderable& operator=(const Renderable&) = delete;

        virtual                 ~Renderable();

        virtual void            render() = 0;

        void                    setMaterial(const MaterialPtr& material);
        const MaterialPtr&      getMaterial() const;
        MaterialPtr&            getMaterial();

        const RenderJobPtr&     getRenderJob() const;
        const GeometryPtr&      getGeometry() const;

        bool                    setRenderTechnique(const std::string& file);
        bool                    setRenderTechnique(const stream::IStreamPtr& stream);

        void                    setRenderTechnique(const CRenderTechnique* technique);
        const CRenderTechnique* getRenderTechique() const;
        CRenderTechnique*       getRenderTechique();

    protected:

        void                    setRenderJob(const RenderJobPtr& job);
        void                    setGeometry(const GeometryPtr& geometry);

    private:

        const CRenderTechnique* m_renderTechnique;
        MaterialPtr             m_material;
        GeometryPtr             m_geometry;
        RenderJobPtr            m_renderJob;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace renderer
} //namespace v3d

#endif //_V3D_RENDEREBLE_H_
