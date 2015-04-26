#ifndef _V3D_SKYBOX_H_
#define _V3D_SKYBOX_H_

#include "Node.h"
#include "renderer/Texture.h"

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

        void                            init()             override;
        void                            update(s32 time)   override;
        void                            render()           override;

    private:

        const u32                       k_countSize = 6U;

        void                            build();

        renderer::TextureList           m_textures;
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_SKYBOX_H_