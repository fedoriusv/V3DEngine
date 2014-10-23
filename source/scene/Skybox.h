#ifndef _V3D_SKYBOX_H_
#define _V3D_SKYBOX_H_

#include "Node.h"
#include "renderer/Texture.h"
#include "renderer/Geometry.h"
#include "renderer/RenderJob.h"
#include "renderer/Material.h"

namespace v3d
{
namespace scene
{
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////

    class CSkybox : public CNode
    {
    public:

        CSkybox();
        virtual                         ~CSkybox();

        void	                        load(const std::string files[6]);

        void	                        load(const std::string& front, const std::string& back,
                                            const std::string& left, const std::string& right, 
                                            const std::string& up, const std::string& down);

        void                            setMaterial(const renderer::MaterialPtr& material);
        const renderer::MaterialPtr&    getMaterial() const;

        void                            init()             override;
        void                            update(f64 time)   override;
        void                            render()           override;

    private:

        const u32                       k_countSize = 6U;

        void                            build();

        renderer::TextureList           m_textures;

        renderer::MaterialPtr           m_material;
        renderer::GeometryPtr           m_geometry;
        renderer::RenderJobPtr          m_renderJob;

    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_SKYBOX_H_