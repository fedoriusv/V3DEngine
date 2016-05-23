#ifndef _V3D_TEXTURE_MANAGER_H_
#define _V3D_TEXTURE_MANAGER_H_

#include "common.h"
#include "utils/Singleton.h"
#include "renderer/Texture.h"
#include "stream/ResourceLoader.h"

namespace v3d
{

namespace resources
{
    class CImage;
} //namespace resources

namespace scene
{
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////

    /**
    * Texture manager. Provide access to all registered textures in application.
    */
    class CTextureManager : public utils::TSingleton<CTextureManager>, public stream::TResourceLoader<renderer::TexturePtr>
    {
    private:

        friend utils::TSingleton<CTextureManager>;

        CTextureManager();
        ~CTextureManager();

    public:

        renderer::TexturePtr         load(const std::string& file, const std::string& alias = "") override;
        renderer::TexturePtr         load(const std::string files[6], const std::string& alias = "");

        renderer::TexturePtr         createTextureFromImage(const resources::CImage* image);
        renderer::TexturePtr         createCubeTextureFromImages(const resources::CImage* image[6]);

    private:

        std::string                 getFileExtension(const std::string& fullFileName);

        const bool                  k_useTextureBuffer = false;
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace scene
} //namespace v3d

#endif //_V3D_TEXTURE_MANAGER_H_
