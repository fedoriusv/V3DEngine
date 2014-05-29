#ifndef _F3D_TEXTURE_MANAGER_H_
#define _F3D_TEXTURE_MANAGER_H_

#include "common.h"
#include "Singleton.h"
#include "renderer/Texture.h"
#include "stream/IStream.h"
#include "ResourceDecoder.h"

namespace v3d
{
namespace scene
{
	typedef std::shared_ptr<CResourceDecoder>   DecoderPtr;
	typedef std::shared_ptr<renderer::CTexture> TexturePtr;

	class CTextureManager
	{
		public:

			CTextureManager();
			~CTextureManager();

			TexturePtr get( const std::string& name ); 
			
			TexturePtr load( const std::string& name ); 

			void	   unload( const std::string& name );
			void	   unload( TexturePtr texture );

			void	   unloadAll();

			void	   registerPatch( const std::string& patch );
			void	   registerDecoder( DecoderPtr decoder );

			void	   unregisterPatch( const std::string& patch );
			void	   unregisterDecoder( DecoderPtr decoder );

		private:

			std::map<std::string, TexturePtr>   m_textures;
			std::vector<DecoderPtr>				m_decoders;
			std::vector<std::string>			m_pathes;
	};
}
}

#endif