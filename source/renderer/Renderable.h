#ifndef _V3D_RENDEREBLE_H_
#define _V3D_RENDEREBLE_H_

#include "Geometry.h"
#include "RenderJob.h"
#include "Material.h"
#include "RenderState.h"

namespace v3d
{
namespace renderer
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    class RenderTechnique;

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

        void                    setRenderTechnique(const RenderTechnique* technique);
        const RenderTechnique* getRenderTechique() const;
        RenderTechnique*       getRenderTechique();

    protected:

        void                    setRenderJob(const RenderJobPtr& job);
        void                    setGeometry(const GeometryPtr& geometry);

    private:

        const RenderTechnique* m_renderTechnique;
        MaterialPtr             m_material;
        GeometryPtr             m_geometry;
        RenderJobPtr            m_renderJob;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace renderer
} //namespace v3d

#endif //_V3D_RENDEREBLE_H_
