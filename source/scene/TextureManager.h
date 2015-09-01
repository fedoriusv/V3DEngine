#ifndef _V3D_TEXTURE_MANAGER_H_
#define _V3D_TEXTURE_MANAGER_H_

#include "common.h"
#include "Singleton.h"
#include "renderer/Texture.h"
#include "stream/IStream.h"
#include "decoders/ResourceDecoder.h"
#include "stream/ResourceLoader.h"
#include "Singleton.h"

namespace v3d
{
namespace scene
{
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////

    class CTextureManager : public Singleton<CTextureManager>, public stream::TResourceLoader<renderer::TexturePtr>
    {
    public:

        CTextureManager();
        virtual                     ~CTextureManager();

        void                        add(const renderer::TexturePtr& texture);
        const renderer::TexturePtr  load(const std::string& file, const std::string& alias = "") override;
        const renderer::TexturePtr  load(const std::string* files[6]);

        renderer::TexturePtr        createTexture2DFromData(const Dimension2D& size, renderer::EImageFormat format, renderer::EImageType type, void* data);
        renderer::TexturePtr        createTexture2DMSAA(const Dimension2D& size, renderer::EImageFormat format, renderer::EImageType type);

        void                        copyToTexture2D(const renderer::TexturePtr& texture, const Dimension2D& offset, const Dimension2D& size, renderer::EImageFormat format, void* data);
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_TEXTURE_MANAGER_H_
