#ifndef _V3D_TEXTURE_MANAGER_H_
#define _V3D_TEXTURE_MANAGER_H_

#include "common.h"
#include "Singleton.h"
#include "renderer/Texture.h"
#include "stream/IStream.h"
#include "decoders/ResourceDecoder.h"
#include "Singleton.h"

namespace v3d
{
namespace scene
{
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////

    class CTextureManager : public Singleton<CTextureManager>
    {
    public:

        CTextureManager();
        virtual                     ~CTextureManager();

        const renderer::TexturePtr  get(const std::string& name);
        const renderer::TexturePtr  load(const std::string& file, const std::string& alias = "");
        const renderer::TexturePtr  load(const std::string* files[6]);


        renderer::TexturePtr        createTexture2DFromData(const Dimension2D& size, renderer::EImageFormat format, renderer::EImageType type, void* data);

        void                        copyToTexture2D(const renderer::TexturePtr& texture, const Dimension2D& offset, const Dimension2D& size, renderer::EImageFormat format, void* data);

        void                        unload(const std::string& name);
        void                        unload(const renderer::TexturePtr& texture);

        void                        unloadAll();

        void                        registerPath(const std::string& path);
        void                        unregisterPath(const std::string& path);

        void                        registerDecoder(decoders::DecoderPtr decoder);
        void                        unregisterDecoder(decoders::DecoderPtr& decoder);

    private:

        renderer::TextureMap        m_textures;
        decoders::DecoderList       m_decoders;
        std::vector<std::string>    m_pathes;
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_TEXTURE_MANAGER_H_
