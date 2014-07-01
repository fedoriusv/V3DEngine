#ifndef _V3D_TEXTURE_MANAGER_H_
#define _V3D_TEXTURE_MANAGER_H_

#include "common.h"
#include "Singleton.h"
#include "renderer/Texture.h"
#include "stream/IStream.h"
#include "ResourceDecoder.h"
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
        virtual                 ~CTextureManager();

        renderer::TexturePtr    get(const std::string& name);
        renderer::TexturePtr    load(const std::string& name);
        renderer::TexturePtr    load(const std::string* files[6]);

        void	                unload( const std::string& name );
        void	                unload(const renderer::TexturePtr& texture);

        void	                unloadAll();

        void	                registerPath( const std::string& path );
        void	                registerDecoder( DecoderPtr decoder );

        void	                unregisterPath( const std::string& path );
        void	                unregisterDecoder( DecoderPtr decoder );

    private:

        renderer::TextureMap    m_textures;
        std::vector<DecoderPtr> m_decoders;
        std::vector<std::string>m_pathes;
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_TEXTURE_MANAGER_H_