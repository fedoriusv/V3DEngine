#ifndef _V3D_TEXTURE_MANAGER_H_
#define _V3D_TEXTURE_MANAGER_H_

#include "common.h"
#include "Singleton.h"
#include "renderer/Texture.h"
#include "stream/ResourceLoader.h"

namespace v3d
{
namespace scene
{
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////

    class CTextureManager : public TSingleton<CTextureManager>, public stream::TResourceLoader<renderer::CTexture>
    {
    private:

        friend TSingleton<CTextureManager>;

        CTextureManager();
        ~CTextureManager();

    public:

        void                        add(const renderer::CTexture* texture);
        const renderer::CTexture*   load(const std::string& file, const std::string& alias = "") override;
        const renderer::CTexture*   load(const std::string* files[6]);

        renderer::CTexture*         createTexture2DFromData(const Dimension2D& size, renderer::EImageFormat format, renderer::EImageType type, void* data);
        renderer::CTexture*         createTexture2DMSAA(const Dimension2D& size, renderer::EImageFormat format, renderer::EImageType type);

        void                        copyToTexture2D(renderer::CTexture* texture, const Dimension2D& offset, const Dimension2D& size, renderer::EImageFormat format, void* data);
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_TEXTURE_MANAGER_H_
