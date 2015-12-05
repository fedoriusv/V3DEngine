#ifndef _V3D_TEXTURE_MANAGER_H_
#define _V3D_TEXTURE_MANAGER_H_

#include "common.h"
#include "utils/Singleton.h"
#include "renderer/Texture.h"
#include "stream/ResourceLoader.h"

namespace v3d
{
namespace scene
{
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////

    class CTextureManager : public utils::TSingleton<CTextureManager>, public stream::TResourceLoader<renderer::CTexture>
    {
    private:

        friend utils::TSingleton<CTextureManager>;

        CTextureManager();
        ~CTextureManager();

    public:

        void                        add(renderer::CTexture* texture);
        void                        add(const renderer::CTexture* texture);
        renderer::CTexture*         load(const std::string& file, const std::string& alias = "") override;
        renderer::CTexture*         load(const std::string files[6], const std::string& alias = "");

        renderer::CTexture*         createTexture2DFromData(const core::Dimension2D& size, renderer::EImageFormat format, renderer::EImageType type, void* data);
        renderer::CTexture*         createTexture2DMSAA(const core::Dimension2D& size, renderer::EImageFormat format, renderer::EImageType type);

        void                        copyToTexture2D(renderer::CTexture* texture, const core::Dimension2D& offset, const core::Dimension2D& size, renderer::EImageFormat format, void* data);

    private:

        void                        addStreamToCubeTexture(renderer::CTexture* texture, const stream::IStreamPtr& stream);
        std::string                 getFileExtension(const std::string& fullFileName);
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_TEXTURE_MANAGER_H_
