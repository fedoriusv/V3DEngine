#pragma once

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
    class TextureManager : public utils::TSingleton<TextureManager>, public stream::TResourceLoader<renderer::TexturePtr>
    {
    private:

        friend utils::TSingleton<TextureManager>;

        TextureManager();
        ~TextureManager();

    public:

        renderer::TexturePtr         load(const std::string& file, const std::string& alias = "") override;
        renderer::TexturePtr         load(const std::string files[6], const std::string& alias = "");

        renderer::TexturePtr         createTextureFromImage(const resources::CImage* image);
        renderer::TexturePtr         createCubeTextureFromImages(const resources::CImage* image[6]);

        u32                          calculateMipmapDataSize(const core::Dimension3D& size, renderer::EImageFormat format, renderer::EImageType type, u32 mipCount);
        void*                        generateMipMaps(const core::Dimension3D& size, const void* data, renderer::EImageFormat format, renderer::EImageType type, u32 mipCount);

    private:

        u32                         culculateMipmapLevelSize(const core::Dimension3D& size, renderer::EImageFormat format, renderer::EImageType type, u32 level);

        std::string                 getFileExtension(const std::string& fullFileName);

        const bool                  k_useTextureBuffer = false;
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace scene
} //namespace v3d
