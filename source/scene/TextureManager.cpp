#include "TextureManager.h"
#include "stream/FileStream.h"
#include "renderer/Renderer.h"
#include "utils/Logger.h"
#include "Engine.h"

using namespace v3d;
using namespace v3d::scene;

CTextureManager::CTextureManager()
{
    CTextureManager::registerPath("../../../../data/");
}

CTextureManager::~CTextureManager()
{
	unloadAll();
}

renderer::TexturePtr CTextureManager::get( const std::string& name )
{
	return m_textures[name];
}

renderer::TexturePtr CTextureManager::load(const std::string* files[6])
{
    //TODO: load cubemap

    return nullptr;
}

renderer::TexturePtr CTextureManager::load(const std::string& name)
{
    std::string nameStr = name;
    std::transform(name.begin(), name.end(), nameStr.begin(), ::tolower);

    auto it = m_textures.find(nameStr);

	if( it != m_textures.end() )
	{
		return it->second;
	}
	else
	{	
		std::string fileExtension;
					
        const size_t pos = nameStr.find('.');
		if( pos != std::string::npos )
		{
            fileExtension = std::string(nameStr.begin() + pos, nameStr.end());
		}

        for (std::string& path : m_pathes)
		{
            const std::string fullName = path + nameStr;
			const bool isFileExist = stream::FileStream::isFileExist(fullName);
			if( isFileExist )
			{	
                stream::FileStream* stream = new stream::FileStream(fullName, stream::FileStream::e_in);

				if(stream->isOpen())
				{
                    renderer::TexturePtr texture = RENDERER->makeSharedTexture();

                    texture->init(stream);
                    texture->m_target = renderer::ETextureTarget::eTexture2D;
                    if (!texture->load())
                    {
                        LOG_ERROR("Streaming error read file [%s]", nameStr.c_str());
                        return nullptr;
                    }
                    
                    if (!texture->create())
                    {
                        LOG_ERROR("Error to Create Texture file [%s]", nameStr.c_str());
                        return nullptr;
                    }

                    m_textures.insert(std::map<std::string, renderer::TexturePtr>::value_type(nameStr, texture));

                    return texture;
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

void CTextureManager::unload(const renderer::TexturePtr& texture)
{
    auto predDelete = [texture](const std::pair<std::string, renderer::TexturePtr>& pair) -> bool
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

void CTextureManager::registerPath( const std::string& path )
{
	m_pathes.push_back( path );
}

void CTextureManager::registerDecoder( DecoderPtr decoder )
{
	m_decoders.push_back( decoder );
}

void CTextureManager::unregisterPath( const std::string& path )
{
	auto it = std::find( m_pathes.begin(), m_pathes.end(), path );
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
