#include "ResourceDecoder.h"

using namespace v3d;
using namespace v3d::scene;

CResourceDecoder::CResourceDecoder()
{
}

CResourceDecoder::CResourceDecoder( const std::string supportedExtensions[] )
{
	m_supportedExtensions.assign( supportedExtensions, supportedExtensions + sizeof( supportedExtensions ) / sizeof( supportedExtensions[0] ) );
}

CResourceDecoder::~CResourceDecoder()
{
    m_supportedExtensions.clear();
}

bool CResourceDecoder::isExtensionSupported( const std::string& extension )  const
{
	auto it = std::find( m_supportedExtensions.begin(), m_supportedExtensions.end(), extension );
	if( it != m_supportedExtensions.end() )
	{
		return true;
	}

	return false;
}

void CResourceDecoder::setSupportedExtensions( const std::string supportedExtensions[] )
{
	m_supportedExtensions.assign( supportedExtensions, supportedExtensions + sizeof( supportedExtensions ) / sizeof( supportedExtensions[0] ) );
}

const std::vector<std::string>& CResourceDecoder::getSupportedExtensions() const
{
	return m_supportedExtensions;
}