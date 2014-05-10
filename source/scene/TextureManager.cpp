#include "TextureManager.h"
#include "stream/FileStream.h"
#include "renderer/Renderer.h"

using namespace f3d;
using namespace f3d::scene;

CTextureManager::CTextureManager()
{

}

CTextureManager::~CTextureManager()
{
	unloadAll();
}

TexturePtr CTextureManager::get( const std::string& name )
{
	return m_textures[name];
}


TexturePtr CTextureManager::load( const std::string& name )
{
	auto it = m_textures.find( name );

	if( it != m_textures.end() )
	{
		return it->second;
	}
	else
	{	
		std::string fileExtension;
					
		const size_t pos = name.find( '.' );
		if( pos != std::string::npos )
		{
			fileExtension = std::string( name.begin() + pos, name.end() );
		}

		for( std::string& path : m_pathes )
		{
			const std::string fullName = path + name;
			const bool isFileExist = stream::FileStream::isFileExist( fullName );
			if( isFileExist )
			{	
				stream::FileStream stream( fullName, stream::FileStream::e_in ); 

				if( stream.isOpen() )
				{
					auto predCanDecode = [fileExtension]( DecoderPtr decoder ) -> bool
					{
						return decoder->isExtensionSupported( fileExtension );
					};

					DecoderPtr decoder = *std::find_if( m_decoders.begin(), m_decoders.end(), predCanDecode );
				
					ResourcePtr texture = decoder->decode( stream );

					if( texture )
					{
						TexturePtr texturePtr = std::static_pointer_cast< renderer::CTexture >( texture );
						m_textures.insert( std::map<std::string, TexturePtr>::value_type( name, texturePtr ) ); 
						return texturePtr;
					}
				}
			}
		}
	}

	return nullptr;
}


void CTextureManager::unload( const std::string& name )
{
	auto it = m_textures.find( name );

	if( it != m_textures.end() )
	{
		m_textures.erase( it );
	}
}

void CTextureManager::unload( TexturePtr texture )
{
	auto predDelete = [texture]( const std::pair<std::string, TexturePtr>& pair ) -> bool
	{
		return pair.second == texture;
	};
	
	auto it = std::find_if( m_textures.begin(), m_textures.end(), predDelete );

	if( it != m_textures.end() )
	{
		m_textures.erase( it );
	}
}


void CTextureManager::unloadAll()
{
	m_textures.clear();
}


void CTextureManager::registerPatch( const std::string& patch )
{
	m_pathes.push_back( patch );
}

void CTextureManager::registerDecoder( DecoderPtr decoder )
{
	m_decoders.push_back( decoder );
}

void CTextureManager::unregisterPatch( const std::string& patch )
{
	auto it = std::find( m_pathes.begin(), m_pathes.end(), patch );
	if( it != m_pathes.end() )
	{
		m_pathes.erase( std::remove( m_pathes.begin(), m_pathes.end(), *it ), m_pathes.end() );
	}
}

void CTextureManager::unregisterDecoder( DecoderPtr decoder )
{
	auto it = std::find( m_decoders.begin(), m_decoders.end(), decoder );
	if( it != m_decoders.end() )
	{
		m_decoders.erase( std::remove( m_decoders.begin(), m_decoders.end(), *it ), m_decoders.end() );
	}
}
