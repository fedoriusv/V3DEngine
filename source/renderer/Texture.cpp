#include "Texture.h"

namespace f3d
{
namespace renderer
{
	CTexture::CTexture()
		: CResource()
		, m_textureID( -1 )
		, m_target( ETextureTarget::eTargetUnknown )
		, m_minFilter( ETextureFilter::eFilterLinear )
		, m_magFilter( ETextureFilter::eFilterLinear )
	{
		m_type = EObjectType::eTypeTexture;

		m_data.width  = 0;
		m_data.height = 0;
		m_data.depth  = 0;
		m_data.type   = EImageType::eTypeUnsignedByte;
		m_data.format = EImageFormat::eFormatRGB;
		m_data.data   = NULL;
	}


	CTexture::~CTexture()
	{
		if (m_data.data != NULL)
		{
			free(m_data.data);
			m_data.data = NULL;
		}
	}

	u32 CTexture::getTextureID() const
	{
		return m_textureID;
	}

	ETextureTarget CTexture::getTarget() const
	{
		return m_target;
	}

	ETextureFilter CTexture::getMinFiler() const
	{
		return m_minFilter;
	}
	
	ETextureFilter CTexture::getMagFiler() const
	{
		return m_magFilter;
	}

	void CTexture::setFilterType( ETextureFilter minFilter, ETextureFilter magFilter )
	{
		m_minFilter = minFilter;
		m_magFilter = magFilter;
	}
}
}